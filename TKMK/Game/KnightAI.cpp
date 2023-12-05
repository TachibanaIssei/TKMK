#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Actor.h"
#include "WizardUlt.h"
#include "KnightUlt.h"
#include "Player.h"
#include "EnemyHpBar.h"

namespace
{
	const float RADIUS = 100.0f;
}
KnightAI::KnightAI()
{
	
}


KnightAI::~KnightAI()
{
	for (int i = 0; i < m_enemyHpBar.size(); i++)
	{
		if (m_enemyHpBar[i] != nullptr)
		{
			DeleteGO(m_enemyHpBar[i]);
		}
	}
}

bool KnightAI::Start() {

	m_status.Init("Knight");

	SetModel();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});


	//リスポーンする座標0番の取得
	GetRespawnPos();
	//リスポーンする座標のセット
	//キャラコン
	m_position = m_respawnPos[m_respawnNumber];
	m_rotation = m_respawnRotation[m_respawnNumber];
	m_charCon.SetPosition(m_position);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);

	m_modelRender.Update();

	//リスポーン時に向いている方向の前方向を取得
	ForwardSet();

	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");

	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
	{
		m_enemyHpBar[0] = NewGO<EnemyHpBar>(0, "enemyHpBar");
		m_enemyHpBar[0]->Init();
	}
	else
	{
		for (int i = 0; i < g_renderingEngine->GetGameMode(); i++)
		{
			m_enemyHpBar[i] = NewGO<EnemyHpBar>(0, "enemyHpBar");
			m_enemyHpBar[i]->Init(i);
		}
	}

	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);

	return true;
}

void KnightAI::Update()
{
	// 追尾エフェクトのリセット
	EffectNullptr();
	
	CharacterUpperHpBar();

	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameState == enPause) {
		return;
	}

	//やられたらリスポーンするまで実行する
	if (DeathToRespawnTimer(m_DeathToRespwanFlag, m_fade, false) == true)
	{
		//アニメーションの再生
		PlayAnimation();
		//ステート
		ManageState();
		m_modelRender.Update();
		return;
	}

	if (m_charState == enCharState_Death) {
		//アニメーションの再生
		PlayAnimation();
		m_modelRender.Update();
		//ステート
		ManageState();
		return;
	}

	//アニメーションの再生
	PlayAnimation();

	//無敵時間
	if (Invincible() == false) {
		//当たり判定
		Collision();
	}

	//必殺技を打った時
	if (UltimaitSkillTime() == true) {
		return;
	}

	if (m_status.GetHp() <= 0)
	{
		m_charState = enCharState_Death;
	}

	//自分の以外の必殺中は止まります
	if (m_game->GetStopFlag() == true && m_game->GetUltActor() != this)
	{
		//ステートが死亡と被ダメの時
		if (m_charState == enCharState_Death || m_charState == enCharState_Damege)
		{
			m_modelRender.Update();
		}
		//それ以外のときは待機状態
		else
		{
			m_charState = enCharState_Idle;
			m_modelRender.Update();
		}
		return;
	}

	//重力
	Move();
	//カントダウン中だったら
	if (m_game->NowGameState() == 0)
	{
		m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);

		//ステート
		ManageState();
	}
		
	//ゲームのステートがスタート,エンド、リザルトでないなら
	if (m_game->NowGameState() < 3 && m_game->NowGameState() != 0)
	{
		//今のフレームと前のフレームのレベルが違っていたら
		if (oldLv != m_lv) {
			//エフェクトを出す
			IsLevelEffect(oldLv, m_lv);
		}

		//前フレームのレベルを取得
		oldLv = m_lv;

		//リスポーンしたときしか使えない
		//飛び降りる処理
		//地上にいない間の処理
		if (GetIsGroundFlag() == false) {
			if (IsActorGroundChack() == true) {
				//地面にいる
				IsGroundFlag = true;
				SetAndPlaySoundSource(enSound_Metal_Falling);
			}
			else {
				//ジャンプ
				if (m_charCon.IsOnGround())
				{

					m_charState = enCharState_Jump;
				}
			}
		}

		//ステート
		ManageState();
		//回避
		AvoidanceSprite();
		//スキルクールタイムの処理
		CoolTime(m_skillCoolTime, SkillEndFlag, m_skillTimer);
		
		// 次のアクションを抽選 
		LotNextAction();

		//追跡
		ChaseAndEscape();

		if (m_isSkillReady == true && CantMove == false && SkillEndFlag == false)
		{
			m_charState = enCharState_Skill;
			//スキルを使うときのスピードを使う
			Vector3 move = m_skillMove;
			m_rotation.SetRotationYFromDirectionXZ(move);
			move.y = 0.0f;
			move *= 200.0f;

			m_position = m_charCon.Execute(move, 1.0f / 60.0f);
			move.Normalize();
			m_forwardNow = move;
		}

		//攻撃
		Attack();

		//反転
		Rotation();
		
		if (m_charState == enCharState_LastAttack)
		{
			Vector3 LastAttackMove = m_LastAttackMove;
			m_rotation.SetRotationYFromDirectionXZ(m_LastAttackMove);
			LastAttackMove.y = 0.0f;
			LastAttackMove *= 100.0f;
			m_position = m_charCon.Execute(LastAttackMove, 1.0f / 60.0f);
			LastAttackMove.Normalize();
			m_forwardNow = LastAttackMove;
		}
		
		//回避クールタイムの処理
		CoolTime(m_avoidanceCoolTime, AvoidanceEndFlag, m_avoidanceTimer);
	}
	else
	{
		m_charState = enCharState_Idle;
		m_modelRender.Update();
	}

	if (m_moveSpeed.LengthSq() != 0.0f) {
		m_forwardNow.Normalize();
		m_forwardNow.y = 0.0f;
	}

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}


//アクターたちの情報を計算
void KnightAI::CalculatAIAttackEvaluationValue()
{
	//アクターたちの情報を取得する
	std::vector<Actor*>& actors = m_game->GetActors();

	for (auto actor : actors)
	{
		EvalData eval = CalculateTargetAI(actor);
		Evaluation_valueActor.push_back(eval);
	}
}

//衝突したときに呼ばれる関数オブジェクト(すり抜ける壁用)
struct SweepResultSlipThroughWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	const btCollisionObject* hitObject = nullptr;
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_SlipThroughWall) {
			//衝突したのは壁ではない。
			return 0.0f;
		}
		//壁とぶつかったら。
		//フラグをtrueに。
		hitObject = convexResult.m_hitCollisionObject;
		isHit = true;
		return 0.0f;
	}
};

//距離によってターゲットの評価値を決める
KnightAI::EvalData KnightAI::CalculateTargetAI(Actor* actor)
{
	int eval = 0;
	bool chaseOrEscape = false;
	EvalData returnData;

	//対象が自分だったら評価値を無限小にする
	if (actor == this)
	{
		eval = -9999999;
		chaseOrEscape = false;

		returnData.eval = eval;
		returnData.chaseOrEscape = chaseOrEscape;

		return returnData;
	}

	// 対象が死亡ステートなら評価値を無限小にする
	if (actor->GetCharState()== enCharState_Death)
	{
		eval = -9999999;
		chaseOrEscape = false;

		returnData.eval = eval;
		returnData.chaseOrEscape = chaseOrEscape;

		return returnData;
	}
	//アクターの座標を取得
	Vector3 actorPos = actor->GetPosition();
	//アクターたちと自分の座標を引く
	Vector3 diff = actorPos - m_position;
	float Distance = diff.Length();
	eval += 5000 - (int)Distance;

	//自分にすごく近い敵が居たらをターゲットする
	if (Distance <= 200.0f)
	{
		eval += 2500;
	}

	if (Distance <= 50.0f)
	{
		eval += 5000;
	}

	//ゲーム時間1分40秒以下ポイントのためにちょっと動く
	if (m_game->GetMinutesTimer() == 1.0f && m_game->GetSecondsTimer() <= 40.0f)
	{
		eval += 1000;
	}
	// リスポーン前のアクターは狙わない
	if (actorPos.y >= 100)
	{
		eval = -9999999;
		chaseOrEscape = false;

		returnData.eval = eval;
		returnData.chaseOrEscape = chaseOrEscape;

		return returnData;
	}
	if (actor->GetCharState() == enCharState_Ult_liberation)
	{
		eval += 20000;
	}
	// 今狙っているターゲットと同じなら優先度を上げる
	if (m_targetActor == actor) {
		eval += 2000;
	}

	//自分より相手のポイントが高い
	if (actor->GetPoint() - m_point >= 10)
	{
		eval += 5000;
		if (actor->GetLevel() > 5 && m_lv >= 5)
		{
			eval += 2000;
		}
	}
	//ゲーム時間が残り30秒なったらポイント稼ぐために動く
	if (m_game->GetMinutesTimer() == 0.0f && m_game->GetSecondsTimer() <= 30.0f)
	{
		eval += 3000;
	}
	//自分のHPが少ない時は逃げる
	if (m_status.GetHp() <= 80)
	{
		eval += 5000;
		chaseOrEscape = true;
	}
	
	// 壁の向こうに対象がいるなら評価値を下げる
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点は自分の座標。
	start.setOrigin(btVector3(m_position.x, 20.0f, m_position.z));
	//終点はランダムの座標。
	end.setOrigin(btVector3(actorPos.x, 20.0f, actorPos.z));
	SweepResultSlipThroughWall callback;
	//コライダーを始点から終点まで動かして。
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した！
	if (callback.isHit == true)
	{
		eval = -999999;
	}

	//今狙ってるアクターのレベルを取得する
	int actorlv = actor->GetLevel();
	//相手より自分のレベルが高かったら評価値は高くなる
	eval += (m_lv - actorlv) * 500;

	//自分のレベルが一定以上達したらキルモードに入る
	if (m_lv >= 7)
	{
		eval += 2000;
	}

	if (m_lv <= 6)
	{
		if (m_game->GetMinutesTimer() >= 2.0f && m_game->GetSecondsTimer() >= 30.0f)
		{
			eval += 4000;
		}
	}


	//自分を攻撃した相手が近い ＆ 相手とのHP差が大きかったら 逃げる
	if (actor == m_lastAttackActor)
	{
		if (Distance <= 400.0f && (actor->GetHP()- m_status.GetHp()) >= 20) {
			eval += 2000;
			chaseOrEscape = true;
		}
	}
	else
	{
		eval-= 4000;
	}
	

	//相手のレベルが自分よりたかったら逃げる
	if (actor->GetLevel()- m_lv > 5)
	{
		eval += 2000;
		chaseOrEscape=true;
	}

	//逃げ状態の処理
	EscapeChange(returnData, actorPos);

	//最後
	returnData.eval = eval;
	returnData.chaseOrEscape = chaseOrEscape;

	return returnData;
}


//中立の敵の評価値の計算
void KnightAI::CalculatEnemyAttackEvaluationValue()
{
	//エネミーたちの情報を取得する
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();

	for (auto enemy : enemys)
	{
		EvalData eval = CalculateTargetEnemy(enemy);
		Evaluation_valueEnemy.push_back(eval);
	}
}


//どのエネミーをターゲットするの計算
KnightAI::EvalData KnightAI::CalculateTargetEnemy(Neutral_Enemy* enemy)
{
	int eval = 0;
	bool chaseOrEscape = false;
	EvalData returnData;

	Neutral_Enemy::EnEnemyKinds ColorEnemy = enemy->GetenemyColor();
	//エネミーの座標を取得
	Vector3 enemyPos = enemy->GetPosition();

	//自分の座標と引く
	Vector3 diff = enemyPos - m_position;
	float Distance = diff.Length();
	// 距離で評価値を決める 近いほど高い
	eval += 5000 - (int)Distance;

	if (m_game->GetMinutesTimer() >= 1.5f)
	{
		eval += 4000;
	}
	else
	{
		eval -= 4000;
	}
	//エネミーが他のアクターに狙わているなら評価値を下げる
	if (enemy->GetBetargetCount()>=1)
	{
		eval -= 2000;
	}

	//自分のレベルが低い時はエネミーを狙ってほしい
	if (m_lv <= 3)
	{
		eval += 3000;
	}
	//レベルがMAXなら中立はあまり狙わない
	if (m_lv >= 10)
	{
		eval -= 4000;
	}

	//自分にすごく近い敵が居たらをターゲットする
	if (Distance <= 50.0f)
	{
		eval += 400;
	}
	// ピンチの時は緑の敵を優先
	if (m_status.GetMaxHp() - m_status.GetHp() >= 60 && ColorEnemy == Neutral_Enemy::EnEnemyKinds::enEnemyKinds_Green)
	{
		eval += 600;
	}
	//HPがほぼMAXの時は赤の敵を優先敵に狙う
	if (m_status.GetMaxHp() - m_status.GetHp() <= 30 && ColorEnemy == Neutral_Enemy::EnEnemyKinds::enEnemyKinds_Red)
	{
		eval += 600;
	}
	// ウサギを優先
	if (ColorEnemy == Neutral_Enemy::enEnemyKinds_Rabbit && m_lv >= 4 && m_lv <= 8)
	{
		eval += 800;
	}

	// 今狙っているターゲットと同じなら優先度を上げる
	if (m_targetEnemy == enemy) {
		eval += 2000;
	}

	// 壁の向こうに対象がいるなら評価値を下げる
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点は自分の座標。
	start.setOrigin(btVector3(m_position.x, 20.0f, m_position.z));
	//終点はランダムの座標。
	end.setOrigin(btVector3(enemyPos.x, 20.0f, enemyPos.z));
	SweepResultSlipThroughWall callback;
	//コライダーを始点から終点まで動かして。
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した！
	if (callback.isHit == true)
	{
		eval = -99999;
	}

	//逃げ状態の処理
	EscapeChange(returnData, enemyPos);

	//最後
	returnData.eval = eval;
	returnData.chaseOrEscape = chaseOrEscape;

	return returnData;
}

void KnightAI::EscapeChange(EvalData& evaldata, Vector3 targetPos)
{
	// 逃げ状態ではない
	if (m_EscapeTimer <= 0.0f || m_escapeActorBackup == nullptr) {
		return;
	}
	Vector3 a = m_escapeActorBackup->GetPosition() - m_position;
	Vector3 b = m_escapeActorBackup->GetPosition() - targetPos;

	//内積
	float nai;
	nai = a.Dot(b);
	//なす角
	float A = a.Length();
	float B = b.Length();

	float cos_sita = nai / (A * B);
	float sita = acos(cos_sita);
	sita = Math::RadToDeg(sita);

	if (sita > 80.0f)
	{
		evaldata.eval += 10000;
	}	
}

//狙う敵を選ぶ
void KnightAI::LotNextAction()
{
	/*if (CantMove) {
		return;
	}*/

	if (EvalTimer > 0.0f)
	{
		EvalTimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	EvalTimer = 1.0f;
	
	//初期化
	Evaluation_valueEnemy.clear();
	Evaluation_valueActor.clear();

	//中立の敵の評価値の計算
	CalculatEnemyAttackEvaluationValue();
	//アクターの評価値の計算
	CalculatAIAttackEvaluationValue();

	//評価値無限小
	int noweval_Target = INT_MIN;
	int noweval_Escape = INT_MIN;
	//今のターゲットエネミー
	Neutral_Enemy* m_nowEnemyTarget = nullptr;
	Actor* m_nowActorTarget = nullptr;
	//今の逃げるアクター
	Actor* m_nowActorEscape = nullptr;

	//falseだったらエネミーを狙う、trueだったらアクターを狙う
	bool TargetChange = false;

	//中立から判定する
	std::vector<Neutral_Enemy*> enemys = m_game->GetNeutral_Enemys();
	for (int i = 0; i < Evaluation_valueEnemy.size(); i++) {

		// 追いかける判定
		if (Evaluation_valueEnemy[i].eval > noweval_Target &&
			Evaluation_valueEnemy[i].chaseOrEscape == false)
		{
			noweval_Target = Evaluation_valueEnemy[i].eval;
			m_nowEnemyTarget = enemys[i];
			TargetChange = false;
		}

	}

	//アクターを判定する
	std::vector<Actor*> actors = m_game->GetActors();
	for (int i = 0; i < Evaluation_valueActor.size(); i++)
	{
		// 追いかける判定
		if (Evaluation_valueActor[i].eval > noweval_Target &&
			Evaluation_valueActor[i].chaseOrEscape == false &&
			actors[i] != this)
		{
			noweval_Target = Evaluation_valueActor[i].eval;
			m_nowActorTarget = actors[i];
			TargetChange = true;
		}

		// 逃げる判定
		if (Evaluation_valueActor[i].eval > noweval_Escape &&
			Evaluation_valueActor[i].chaseOrEscape == true)
		{
			noweval_Escape = Evaluation_valueActor[i].eval;
			m_nowActorEscape = actors[i];
		}
	}

	// 追いかける対象を決める
	if (TargetChange == false)
	{
		TargePos = m_nowEnemyTarget->GetPosition();

		// 今からターゲットする敵と今ターゲットしてる敵が違う
		if (m_targetEnemy != m_nowEnemyTarget && m_targetEnemy != nullptr) {
			m_targetEnemy->RemoveActorFromList(this);
			m_nowEnemyTarget->AddActorFromList(this);
		}
		else if (m_targetEnemy != m_nowEnemyTarget && m_targetEnemy == nullptr) {
			m_nowEnemyTarget->AddActorFromList(this);
		}

		// ターゲットがActorから中立の敵に切り替わった時
		if (m_targetActor != nullptr) {
			m_targetActor->RemoveActorFromList(this);
			m_targetActor = nullptr;
		}

		//ターゲット変更
		m_targetEnemy = m_nowEnemyTarget;

	}
	else
	{
		TargePos = m_nowActorTarget->GetPosition();

		// 今からターゲットする敵と今ターゲットしてる敵が違う
		if (m_targetActor != m_nowActorTarget && m_targetActor != nullptr) {
			m_targetActor->RemoveActorFromList(this);
			m_nowActorTarget->AddActorFromList(this);
		}

		else if (m_targetActor != m_nowActorTarget && m_targetActor == nullptr) {
			m_nowActorTarget->AddActorFromList(this);
		}

		// ターゲットが中立の敵からActorに切り替わった時
		if (m_targetEnemy != nullptr) {
			m_targetEnemy->RemoveActorFromList(this);
			m_targetEnemy = nullptr;
		}

		//ターゲット変更
		m_targetActor = m_nowActorTarget;
	}

	// 逃げる対象を決める
	m_escapeActor = m_nowActorEscape;
}

void KnightAI::ChaseAndEscape()
{
	if (IsEnableMove() == false)
	{
		return;
	}
	/*if (CantMove) {
		return;
	}*/
	if (m_charState == enCharState_LastAttack)
	{
		return;
	}

	// タイマーを減らす
	if (m_EscapeTimer > 0.0f) {
		m_EscapeTimer -= g_gameTime->GetFrameDeltaTime();
		m_escapeActorBackup = nullptr;
	}

	Vector3 diffTarget = TargePos - m_position;

	if (m_escapeActor != nullptr && m_position.y < 1.0f)
	{
		Vector3 diffEscape = m_escapeActor->GetPosition() - m_position;

		Vector3 a = diffTarget;
		Vector3 b = diffEscape;

		//内積
		float nai;
		nai = a.Dot(b);
		//なす角
		float A = a.Length();
		float B = b.Length();

		float cos_sita = nai / (A * B);
		float sita = acos(cos_sita);
		sita = Math::RadToDeg(sita);

		if (sita <= 60.0f) {
			// [自分] [Escape] [Target] みたいな配置
			// とにかく逃げる

			diffTarget.Normalize();

			//移動速度を設定する。
			m_moveSpeed = diffTarget * m_status.GetSpeed();

			//逃げタイマー開始
			m_EscapeTimer = 5.0f;
			m_escapeActorBackup = m_escapeActor;
		}

		else {
			// [Target] [自分] [Escape] みたいな配置
			// 普通にターゲットを狙う
			diffTarget.Normalize();

			//移動速度を設定する。
			m_moveSpeed = diffTarget * m_status.GetSpeed();
		}
	}
	else if (m_position.y < 1.0f){
		diffTarget.Normalize();

		//移動速度を設定する。
		m_moveSpeed = diffTarget * m_status.GetSpeed();
	}

	//リスポーン中なら特殊な計算
	if (m_position.y >= 0.0f)
	{
		diffTarget.Normalize();
		m_moveSpeed.x = diffTarget.x * (m_status.GetSpeed() * 1.5f);
		m_moveSpeed.z = diffTarget.z * (m_status.GetSpeed() * 1.5f);
	}

	// 移動する（衝突解決）
	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	m_forwardNow = m_moveSpeed;
	m_forwardNow.Normalize();
}

void KnightAI::Move()
{
	/*if (CantMove) {
		return;
	}*/

	//重力を付与する
	m_moveSpeed.y -= 600.0f * g_gameTime->GetFrameDeltaTime();

	//地面についた。
	if (m_charCon.IsOnGround()) {

		m_moveSpeed.y = 0.0f;

		//ジャンプフラグがtrueだったら
		if (m_RespawnJumpFlag == true)
		{
			//座標を上げる
			RespawnMove();
			//フラグをfalseにする
			m_RespawnJumpFlag = false;
		}
	}
}
const bool KnightAI::CanSkill()
{
	if (m_isSkillReady == false)
	{
		Vector3 diff = TargePos - m_position;

		if (diff.LengthSq() <= 150 * 100.0f)
		{
			//追加した
			
			return true;
		}
	}
	
	return false;
}

const bool KnightAI::CanUlt()
{
	Vector3 diff = TargePos - m_position;

	if (diff.LengthSq() >= 200.0f * 200.0f)
	{
		return true;
	}
	return false;
}

const bool KnightAI::CanAttack()
{

	// 対象が倒れているなら問答無用でfalse
	if (m_targetEnemy != nullptr) {
		if (m_targetEnemy->GetEnemyState() == Neutral_Enemy::enNeutral_Enemy_Death) {
			return false;
		}
	}

	Vector3 diff = TargePos - m_position;

	if (diff.LengthSq() <= 50.0f * 50.0f)
	{
		return true;
	}
	return false;
}


void KnightAI::Attack()
{
	//攻撃かスキルを使用しているなら
	//コリジョン作成
	if (AtkCollistionFlag == true) {
		AtkCollisiton();
	}

	if (IsEnableMove() == false)
	{
		return;
	}
	

	if (CanSkill())
	{	
		if (m_charState == enCharState_UltimateSkill || m_charState == enCharState_Ult_liberation)
		{
			return;
		}
			//連打で攻撃できなくなる
		//スキルを発動する処理
			if (m_targetActor != nullptr && SkillEndFlag == false)
			{
				//m_charState = enCharState_Skill;
				//スキルを打つ
				m_isSkillReady = true;
				m_skillMove = TargePos - m_position;
				m_skillMove.Normalize();

				//剣にまとわせるエフェクト
				if (m_effectKnightSkill != nullptr) {
					m_effectKnightSkill->DeleteEffect();
				}
				m_effectKnightSkill = NewGO <ChaseEFK>(4);
				m_effectKnightSkill->SetEffect(EnEFK::enEffect_Knight_Skill, this, Vector3::One * 30.0f);
				m_effectKnightSkill->AutoRot(true);
				m_effectKnightSkill->SetAutoRotAddY(360.0f);
				// 座標の加算量を計算
				Vector3 effectAddPos = Vector3::Zero;
				effectAddPos.y = 50.0f;
				m_effectKnightSkill->SetAddPos(effectAddPos);

				//床のエフェクト
				EffectEmitter* EffectKnightSkillGround_;
				EffectKnightSkillGround_ = NewGO <EffectEmitter>(0);
				EffectKnightSkillGround_->Init(EnEFK::enEffect_Knight_SkillGround);
				EffectKnightSkillGround_->SetScale(Vector3::One * 40.0f);
				EffectKnightSkillGround_->Play();
				Vector3 effectPosition = m_position;
				//Quaternion EffRot = m_rotation;
				//EffRot.AddRotationDegY(360.0f);
				Quaternion EffRot = Quaternion::Identity;
				EffRot.SetRotationYFromDirectionXZ(m_skillMove);
				EffRot.AddRotationDegY(360.0f);
				EffectKnightSkillGround_->SetPosition(effectPosition);
				EffectKnightSkillGround_->SetRotation(EffRot);
				EffectKnightSkillGround_->Update();

				//土煙のエフェクト
				if (m_footSmoke != nullptr) {
					m_footSmoke->DeleteEffect();
				}
				m_footSmoke = NewGO<ChaseEFK>(4);
				m_footSmoke->SetEffect(EnEFK::enEffect_Knight_FootSmoke, this, Vector3::One * 20.0f);
				m_footSmoke->AutoRot(true);

			}
	}

	if (CanAttack()) {

		if (m_position.y > 10.0f)
		{
			return;
		}
		//一段目のアタックをしていないなら
		if (AtkState == false)
		{
			Vector3 diff = TargePos - m_position;
			m_rotation.SetRotationYFromDirectionXZ(diff);
			m_charState = enCharState_Attack;
			m_AtkTmingState = SecondAtk_State;

			AtkState = true;
		}

		
	}
	
	if (CanUlt())
	{
		if (m_charState == enCharState_Attack || m_charState == enCharState_SecondAttack ||
			m_charState == enCharState_LastAttack || m_charState == enCharState_Avoidance)
		{
			return;
		}
		if (m_position.y > 10.0f)
		{
			return;
		}
		//必殺技を発動する処理
		if (m_targetActor != nullptr && m_lv >= 6 && /*(m_status.m_maxHp - m_status.m_hp) <= 120 && m_targetActor->GetHP() <= 200 &&*/ m_game->GetUltCanUseFlag() == false)
		{
			//画面を暗くする
			m_game->SetUltTimeSkyFlag(true);

			UltimateDarknessFlag = true;
			//魔法陣生成
			CreatMagicCircle();

			//必殺技の溜めステートに移行する
			m_charState = enCharState_Ult_liberation;

			Vector3 m_SwordPos = Vector3::Zero;
			Quaternion m_SwordRot;
			Ult_Swordeffect = NewGO<EffectEmitter>(2);
			Ult_Swordeffect->Init(enEffect_Knight_Ult_Aura);
			Ult_Swordeffect->SetScale({ 20.0f,40.0f,20.0f });
			Ult_Swordeffect->SetPosition(m_position);
			//Ult_Swordeffect->SetRotation(m_SwordRot);
			//Ult_Swordeffect->Update();
				//エフェクトを再生
			Ult_Swordeffect->Play();
			//アルティメットSE
			SetAndPlaySoundSource(enSound_Knight_Charge_Power);
			//SoundSource* se = NewGO<SoundSource>(0);
			//se->Init(enSound_Knight_Charge_Power);
			////se->SetVolume(1.0f);
			//se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
			//se->Play(false);

			//必殺技発動フラグをセット
			//UltimateSkillFlag = true;
		}
	}

}

//必殺技を打っている間の処理
bool KnightAI::UltimaitSkillTime()
{
	

	if (m_UseUltimaitSkillFlag == true)
	{
		if (m_UltshootTimer > 1)
		{
			
				MakeUltSkill();
		}
		else
		{
			m_UltshootTimer += g_gameTime->GetFrameDeltaTime();
		}

		//全ての雷が落ちてから

		//地上にいるキャラに必殺技を打ち終わったら
		if (DamegeUltActor.empty() == true/*m_OnGroundCharCounter <= 0 */ )
		{
			//対象のアクターがいなかったフラグをfalseに戻す
			m_NoTargetActor = false;
			//レベルを下げる
			UltimateSkill();
			//時間を動かす
			UltEnd();
			m_game->SetStopFlag(false);
			//画面を明るくする
			UltimateDarknessFlag = false;
			//画面を暗くするフラグをfalseにする
			m_game->SetUltTimeSkyFlag(false);
			//画面が暗いのをリセットする
			m_game->LightReset();
			//中身を全て消す
			DamegeUltActor.clear();
		}


		return true;
	}

	return false;
}


/// <summary>
/// 攻撃時の当たり判定の処理
/// </summary>
void KnightAI::AtkCollisiton()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition, //座標。
		Quaternion::Identity, //回転。
		Vector3(70.0f, 15.0f, 15.0f) //大きさ。
	);
	collisionObject->SetName("player_attack");
	collisionObject->SetCreatorName(GetName());

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//matrix.MakeRotationZ(90.0f);
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

/// <summary>
/// 必殺技の当たり判定生成する
/// </summary>
void KnightAI::MakeUltSkill()
{
	for (auto actor : DamegeUltActor)
	{
		//必殺技の雷の生成
		WizardUlt* wizardUlt = NewGO<WizardUlt>(0, "wizardUlt");
		//生成したのがプレイヤーなのでtrue
		wizardUlt->SetThisCreatCharcter(true);
		//自分のオブジェクトの名前をセット
		wizardUlt->SetCreatorName(GetName());
		//攻撃するアクターのオブジェクト名をセット
		wizardUlt->SetActor(actor->GetName());
		//攻撃力を決める
		wizardUlt->SetAboutUlt(m_lv);
		//攻撃するアクターの座標取得
		Vector3 UltPos = actor->GetPosition();
		UltPos.y += 100.0f;
		wizardUlt->SetPosition(UltPos);
		wizardUlt->SetGame(m_game);

		//効果音再生
		SetAndPlaySoundSource(enSound_Sword_Ult);

		//雷を落とすキャラがリストの最後なら
		if (actor == DamegeUltActor.back())
		{
			//最後であることを知らせる
			wizardUlt->ChangeUltEndFlag(true);

			m_UltshootTimer = 0.0f;
			//一人ずつ必殺技を打つのでぬける
			return;
		}

		m_UltshootTimer = 0.0f;
	}
}

void KnightAI::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//一段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		m_AtkTmingState = FirstAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣１段目音
		SetAndPlaySoundSource(enSound_ComboONE);
		/*SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_ComboONE);
		se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
		se->Play(false);*/
		//se->SetVolume(1.0f);
	}
	//二段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣２段目音
		SetAndPlaySoundSource(enSound_ComboTwo);
		/*SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_ComboTwo);
		se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
		se->Play(false);*/
		//se->SetVolume(1.0f);
	}
	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣３段目音
		SetAndPlaySoundSource(enSound_ComboThree);
		/*SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_ComboThree);
		se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
		se->Play(false);*/
		//se->SetVolume(1.0f);
	}
	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"Move_True") == 0)
	{
		CantMove = true;
		/*m_moveSpeed = Vector3::Zero;*/
	}
	//スキルのアニメーションが始まったら
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		//m_status.m_attackPower += 20;
		//m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;

		//スキル音を発生
		SetAndPlaySoundSource(enSound_Sword_Skill);
		/*SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Sword_Skill);
		se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
		se->Play(false);*/
		//se->SetVolume(1.0f);
	}

	//必殺技のアニメーションが始まったら
	if (wcscmp(eventName, L"timeStop") == 0)
	{
		m_game->SetStopFlag(true);
		m_game->SetUltActor(this);
	}

	//必殺技 剣を空に掲げたら
	if (wcscmp(eventName, L"UltimateAttack_Charge") == 0)
	{
		//雷チャージエフェクト生成
		EffectEmitter* ThunderCharge = NewGO<EffectEmitter>(0);
		ThunderCharge->Init(EnEFK::enEffect_Knight_Thunder_Charge);

		Vector3 ChargePos = Vector3::Zero;
		//「Sword」ボーンのワールド行列を取得する。
		Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
		matrix.Apply(ChargePos);

		ChargePos.y += 20.0f;
		ThunderCharge->SetPosition(ChargePos);
		ThunderCharge->SetScale(Vector3::One * 5.0f);
		ThunderCharge->Play();
	}

	//必殺技のアニメーションが始まったら
	if (wcscmp(eventName, L"UltimateAttack_Start") == 0)
	{
		//必殺技使用時の処理ができるようにする
		m_UseUltimaitSkillFlag = true;
		//地上にいるキャラをカウントする
		for (auto actor : m_game->GetActors())
		{
			if (GetName() == actor->GetName()) {
				continue;
			}

			if (actor->GetIsGroundFlag() == true) {
				//雷を打たれるキャラの情報を入れる
				DamegeUltActor.push_back(actor);
			}
		}

		//攻撃対象のアクターがいなかったら
		if (DamegeUltActor.empty() == true) {
			m_NoTargetActor = true;
		}

		//カメラで見るのを終わりにする
		ChangeChaseCamera(false);

		//必殺技の当たり判定のクラスを作成
		//MakeUltSkill();
		
		////必殺技の当たり判定のクラスを作成
		//MakeUltSkill();
		////レベルを下げる
		//UltimateSkill();
		//エフェクトを移動
		//m_swordEffectFlag = false;

	}
	//ジャンプのアニメーションが始まったら
	if (wcscmp(eventName, L"Jump_Start") == 0)
	{
		m_RespawnJumpFlag = true;
	}
	//////////////////////////////////////////////////////////////////////////
	//一段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"FirstAttack_End") == 0)
	{
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	///一段目のアタックのアニメーションが終わったら
	if (wcscmp(eventName, L"FirstToIdle") == 0)
	{
		if (CanAttack()) {
			m_AtkTmingState = SecondAtk_State;
		}
		else
		{
			AtkState = false;
			m_charState = enCharState_Idle;
			m_AtkTmingState = Num_State;
		}
	}

	//二段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SecondAttack_End") == 0)
	{
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;

		if (CanAttack()) {
			m_AtkTmingState = LastAtk_State;
		}
		else
		{
			AtkState = false;
			m_charState = enCharState_Idle;
			m_AtkTmingState = Num_State;
		}
	}
	
	//三段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"LastAttack_End") == 0)
	{
		CantMove = false;
		m_AtkTmingState = Num_State;
		AtkState = false;
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false) {
		m_charState = enCharState_Idle;
		AtkState = false;
	}
	//三段目のアタックのアニメーションで剣を振り終わったら移動できないように
	if (wcscmp(eventName, L"Move_False") == 0)
	{
		CantMove = false;
		
	}

	//スキルのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SkillAttack_End") == 0)
	{
		//m_status.m_attackPower -= 20;
		m_AtkTmingState = Num_State;
		AtkState = false;
		//スキルの移動処理をしないようにする
		m_isSkillReady = false;
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	//回避アニメーションが終わったら
	if (wcscmp(eventName, L"Avoidance_End") == 0)
	{
		//移動処理をしないようにする

		AvoidanceFlag = false;
		//m_AtkTmingState = Num_State;

	}
}


void KnightAI::AvoidanceSprite()
{
	if (m_charState == enCharState_LastAttack)
	{
		return;
	}
	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	if (IsEnableMove() == false)
	{
		return;
	}

	// クールタイム中は呼ばない
	if (AvoidanceEndFlag) {
		return;
	}

	const auto& atkcollisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す
	for (auto atkcollision : atkcollisions)
	{
		//このコリジョンを作ったアクターを検索
		Actor* collisionActor = FindGO<Actor>(atkcollision->GetCreatorName());

		// 自分だったら回避しない
		if (collisionActor == this) {
			continue;
		}

		Vector3 atkPos = atkcollision->GetPosition();
		Vector3 diff = atkPos - m_position;
		diff.Normalize();
		float angle = acosf(diff.Dot(m_forward));

		atkPos.y = m_position.y;
		Vector3 kyori = atkPos - m_position;

		//プレイヤーが視界内に居たら
		if (Math::PI * 0.5f >= fabsf(angle) && kyori.Length()<=80)
		{
			AvoidanceFlag = true;
			//回避ステート
			m_charState = enCharState_Avoidance;
			return;
		}
	}
}

void  KnightAI::IsLevelEffect(int oldlevel, int nowlevel)
{
	if (nowlevel > oldlevel)
	{
		if (LevelUp_efk != nullptr) {
			LevelUp_efk->DeleteEffect();
		}
		LevelUp_efk = NewGO<ChaseEFK>(4);
		LevelUp_efk->SetEffect(EnEFK::enEffect_Knight_LevelUp, this, Vector3::One * 15.0f);
		SoundSource* se = NewGO<SoundSource>(0);

		SetAndPlaySoundSource(enSound_Level_UP);
	}
	else if (nowlevel < oldlevel)
	{
		if (LevelDown_efk != nullptr) {
			LevelDown_efk->DeleteEffect();
		}
		LevelDown_efk = NewGO<ChaseEFK>(4);
		LevelDown_efk->SetEffect(EnEFK::enEffect_Knight_LevelDown, this, Vector3::One * 15.0f);
		SoundSource* se = NewGO<SoundSource>(0);

		SetAndPlaySoundSource(enSound_Level_Down);
	}
}

void KnightAI::CharacterUpperHpBar()
{
	for (int i = 0; i < g_renderingEngine->GetGameMode(); i++)
	{
		m_enemyHpBar[i]->CalcHpBarPosition(i, &m_status, m_position, m_lv);
		m_enemyHpBar[i]->SetDrawFlag(false);
		//HPバーを描画する条件を満たしたら
		if (DrawHP(i))
		{
			if (m_game->GetStopFlag() == false && m_game->NowGameState() != enPause)
			{
				m_enemyHpBar[i]->SetDrawFlag(true);
			}
		}
	}
}

void KnightAI::SetAndPlaySoundSource(EnSound soundNumber)
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(soundNumber);
	se->SetVolume(SoundSet(player, m_game->GetSoundEffectVolume(), 0.0f));
	se->Play(false);
}

void KnightAI::Render(RenderContext& rc)
{
	if (DarwFlag == true) {
		m_modelRender.Draw(rc);
	}
}

