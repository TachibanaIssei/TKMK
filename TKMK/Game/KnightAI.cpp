#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Actor.h"
#include "WizardUlt.h"
#include "KnightUlt.h"
//todo誰かが塔の上にいると処理が止まる
//地上にいるフラグがfalseのやつがいると止まる
namespace
{
	const float HP_WINDOW_WIDTH = 1152.0f;
	const float HP_WINDOW_HEIGHT = 648.0f;
	const float HP_BER_WIDTH = 178.0f;
	const float HP_BER_HEIGHT = 22.0f;
	const Vector3 HP_BER_SIZE = Vector3(HP_BER_WIDTH, HP_BER_HEIGHT, 0.0f);
	const float RADIUS = 100.0f;

}
KnightAI::KnightAI()
{
	
}


KnightAI::~KnightAI()
{

}

bool KnightAI::Start() {

	m_Status.Init("Knight");

	SetModel();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});


	//リスポーンする座標0番の取得
	GetRespawnPos();
	//m_respawnPos[respawnNumber].y /*+= m_position_YUp*/;
	////リスポーンする座標のセット
	////キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

	//リスポーン時に向いている方向の前方向を取得
	ForwardSet();

	m_modelRender.Update();

	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);

	m_HP_Bar.Init("Assets/sprite/zako_HP_bar.DDS", HP_BER_WIDTH, HP_BER_HEIGHT);
	//m_HPBar.SetPivot(PIVOT);

	m_HP_Back.Init("Assets/sprite/zako_HP_background.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);

	m_HP_Frame.Init("Assets/sprite/HP_flame_mushroom.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);
	return true;
}

void KnightAI::Update()
{
	
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameState == enPause) {
		return;
	}
	//当たり判定
	Collition();
	//アニメーションの再生
	PlayAnimation();
	// 追尾エフェクトのリセット
	EffectNullptr();
	
	//必殺技を打った時
	if (UltimaitSkillTime() == true) {
		return;
	}

	//自分の以外の必殺中は止まります
	if (m_game->GetStopFlag() == true && m_game->GetUltActor() != this)
	{
		//ステートは死亡と被ダメの時アニメーションは再生する
		if (m_charState == enCharState_Death || m_charState == enCharState_Damege)
		{
			m_modelRender.Update();
		}
		return;
	}
	HPBar();

	
	//やられたらリスポーンするまで実行する
	if (DeathToRespawnTimer(m_DeathToRespwanFlag,m_fade,false) == true)
	{
		//m_charState = enCharState_Death;
		//アニメーションの再生
		PlayAnimation();
		//ステート
		ManageState();
		m_modelRender.Update();
		return;
	}
	//攻撃アップ中の処理
	//AttackUP();

	//重力
	Move();
	//カントダウン中だったら
	if (m_game->NowGameState() == 0)
	{
		m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		//ステート
		ManageState();
	}
	if (m_Status.Hp <= 0)
	{
		m_charState = enCharState_Death;
	}
	//必殺技の溜めのときに動かないようにする
	//if (m_charState == enCharState_Ult_liberation)
	//{
	//	m_modelRender.Update();
	//	return;
	//}
		
	//ゲームのステートがスタート,エンド、リザルトでないなら
	if (m_game->NowGameState() < 3 && m_game->NowGameState() != 0)
	{
		//リスポーンしたときしか使えない
		//飛び降りる処理
		//地上にいないならジャンプしかしないようにする
		if (IsActorGroundChack() != true) {
			if (m_charCon.IsOnGround())
			{
				
				m_charState = enCharState_Jump;
			}
		}
		else
		{
			//やられているなら
			//if (m_charState == enCharState_Death)
			//{
			//	//地上にいない
			//	IsGroundFlag = false;
			//}
			//else
			//	//地上にいる
			//	IsGroundFlag = true;
			
		}

		//ステート
		ManageState();
		//回避
		AvoidanceSprite();
		//スキルクールタイムの処理
		COOlTIME(Cooltime, SkillEndFlag, SkillTimer);
		
		// 次のアクションを抽選 
		LotNextAction();

		if (m_charState != enCharState_Ult_liberation)
		{
			//追跡
			ChaseAndEscape();
		}
		
		//攻撃
		Attack();

		//反転
		Rotation();
		
		//無敵時間
		Invincible();

		if (SkillState == true && CantMove == false)
		{
			m_charState = enCharState_Skill;
			//スキルを使うときのスピードを使う
			Vector3 move = m_skillMove;
			m_rot.SetRotationYFromDirectionXZ(move);
			////スキルを使うときのスピードを使う
			////AnimationMove(SkillSpeed, m_forward);
			move.y = 0.0f;
			move *= 200.0f;

			m_position = m_charCon.Execute(move, g_gameTime->GetFrameDeltaTime());
		}

		if (m_charState == enCharState_LastAttack)
		{
			Vector3 LastAttackMove = m_LastAttackMove;
			m_rot.SetRotationYFromDirectionXZ(m_LastAttackMove);
			LastAttackMove.y = 0.0f;
			LastAttackMove *= 100.0f;
			m_position = m_charCon.Execute(LastAttackMove, g_gameTime->GetFrameDeltaTime());
		}
		
		
		//回避クールタイムの処理
		COOlTIME(AvoidanceCoolTime, AvoidanceEndFlag, AvoidanceTimer);


	}

	// 名前描画
	Vector2 namePos = Vector2::Zero;
	g_camera3D->CalcScreenPositionFromWorldPosition(namePos, m_position);
	namePos.y += 60.0f;

	wchar_t wcsbuf[256];
	std::size_t len = std::strlen(GetName());
	std::size_t converted = 0;
	wchar_t* wcstr = new wchar_t[len + 1];
	mbstowcs_s(&converted, wcstr, len + 1, GetName(), _TRUNCATE);
	m_Name.SetText(wcstr);
	m_Name.SetPosition(Vector3(namePos.x, namePos.y, 0.0f));
	//フォントの大きさを設定。
	m_Name.SetScale(0.5f);
	//フォントの色を設定。
	m_Name.SetColor({ 1.0f,0.0f,0.0f,1.0f });

	if (m_moveSpeed.LengthSq() != 0.0f) {
		m_forwardNow = m_moveSpeed;
		m_forwardNow.Normalize();
		m_forwardNow.y = 0.0f;
	}

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rot);
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

	if (m_game->GetMinutesTimer() <= 1.5f)
	{
		eval += 4000;
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

	if (Distance <= 100.0f)
	{
		eval += 2500;
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

	// 今狙っているターゲットと同じなら優先度を上げる
	if (m_targetActor == actor) {
		eval += 2000;
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
	eval += (Lv - actorlv) * 500;

	//自分のレベルが一定以上達したらキルモードに入る
	if (Lv >= 4)
	{
		eval += 2000;
	}
	//パワーアップ中はアクターを狙う
	if (PowerUpTimer > 0.0f)
	{
		eval += 3000;
	}
	//自分を攻撃した相手が近い ＆ 相手とのHP差が大きかったら 逃げる
	if (actor == m_lastAttackActor)
	{
		if (Distance <= 400.0f && (actor->GetHP()- m_Status.Hp) >= 20) {
			eval += 2000;
			chaseOrEscape = true;
		}
	}

	//相手のレベルが自分よりたかったら逃げる
	if (actor->GetLevel()- Lv > 5)
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
	if (Lv <= 3)
	{
		eval += 3000;
	}
	//レベルがMAXなら中立はあまり狙わない
	if (Lv >= 10)
	{
		eval -= 4000;
	}

	//自分にすごく近い敵が居たらをターゲットする
	if (Distance <= 50.0f)
	{
		eval += 400;
	}
	// ピンチの時は緑の敵を優先
	if (m_Status.MaxHp - m_Status.Hp >= 60 && ColorEnemy == Neutral_Enemy::EnEnemyKinds::enEnemyKinds_Green)
	{
		eval += 600;
	}
	//HPがほぼMAXの時は赤の敵を優先敵に狙う
	if (m_Status.MaxHp - m_Status.Hp <= 30 && ColorEnemy == Neutral_Enemy::EnEnemyKinds::enEnemyKinds_Red)
	{
		eval += 600;
	}
	// ウサギを優先
	if (ColorEnemy == Neutral_Enemy::enEnemyKinds_Rabbit && Lv >= 4 && Lv <= 8)
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
	int noweval_Target = -99999999;
	int noweval_Escape = -99999999;
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
			//diffEscape *= -1.0f;
			//diffEscape.Normalize();

			////移動速度を設定する。
			//m_moveSpeed = diffEscape * m_Status.Speed;

			diffTarget.Normalize();

			//移動速度を設定する。
			m_moveSpeed = diffTarget * m_Status.Speed;

			//逃げタイマー開始
			m_EscapeTimer = 5.0f;
			m_escapeActorBackup = m_escapeActor;
		}

		else {
			// [Target] [自分] [Escape] みたいな配置
			// 普通にターゲットを狙う
			diffTarget.Normalize();

			//移動速度を設定する。
			m_moveSpeed = diffTarget * m_Status.Speed;
		}
	}
	else if (m_position.y < 1.0f){
		diffTarget.Normalize();

		//移動速度を設定する。
		m_moveSpeed = diffTarget * m_Status.Speed;
	}

	//リスポーン中なら特殊な計算
	if (m_position.y >= 0.0f)
	{
		diffTarget.Normalize();
		m_moveSpeed.x = diffTarget.x * (m_Status.Speed * 1.5f);
		m_moveSpeed.z = diffTarget.z * (m_Status.Speed * 1.5f);
	}

	// 移動する（衝突解決）
	m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	
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
	if (SkillState == false)
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

	if (diff.LengthSq() <= 450.0f * 450.0f)
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
				//スキルを打つ
				SkillState = true;
				m_skillMove = TargePos - m_position;
				m_skillMove.Normalize();

				//剣にまとわせるエフェクト
				if (EffectKnightSkill != nullptr) {
					EffectKnightSkill->DeleteEffect();
				}
				EffectKnightSkill = NewGO <ChaseEFK>(4);
				EffectKnightSkill->SetEffect(EnEFK::enEffect_Knight_Skill, this, Vector3::One * 30.0f);
				EffectKnightSkill->AutoRot(true);
				EffectKnightSkill->SetAutoRotAddY(360.0f);
				// 座標の加算量を計算
				Vector3 effectAddPos = Vector3::Zero;
				effectAddPos.y = 50.0f;
				EffectKnightSkill->SetAddPos(effectAddPos);

				//床のエフェクト
				EffectEmitter* EffectKnightSkillGround_;
				EffectKnightSkillGround_ = NewGO <EffectEmitter>(0);
				EffectKnightSkillGround_->Init(EnEFK::enEffect_Knight_SkillGround);
				EffectKnightSkillGround_->SetScale(Vector3::One * 40.0f);
				EffectKnightSkillGround_->Play();
				Vector3 effectPosition = m_position;
				//Quaternion EffRot = m_rot;
				//EffRot.AddRotationDegY(360.0f);
				Quaternion EffRot = Quaternion::Identity;
				EffRot.SetRotationYFromDirectionXZ(m_skillMove);
				EffRot.AddRotationDegY(360.0f);
				EffectKnightSkillGround_->SetPosition(effectPosition);
				EffectKnightSkillGround_->SetRotation(EffRot);
				EffectKnightSkillGround_->Update();

				//土煙のエフェクト
				if (FootSmoke != nullptr) {
					FootSmoke->DeleteEffect();
				}
				FootSmoke = NewGO<ChaseEFK>(4);
				FootSmoke->SetEffect(EnEFK::enEffect_Knight_FootSmoke, this, Vector3::One * 20.0f);
				FootSmoke->AutoRot(true);

			}
	}

	if (CanAttack()) {

		//一段目のアタックをしていないなら
		if (AtkState == false)
		{
			Vector3 diff = TargePos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			m_charState = enCharState_Attack;
			m_AtkTmingState = SecondAtk_State;

			AtkState = true;
		}

		
	}
	
	if (CanUlt())
	{
		//必殺技を打たない
		//return;
		//必殺技を発動する処理
		if (pushFlag == false && Lv >= 4&& m_targetActor!=nullptr&&m_targetActor->GetHp()<80&&m_game->GetUltCanUseFlag()==false)
		{
			//画面を暗くする
			m_game->SetUltTimeSkyFlag(true);

			m_game->SetUltCanUseFlag(true);
			UltimateDarknessFlag = true;
			//魔法陣生成
			CreatMagicCircle();
			pushFlag = true;
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
			/*SoundSource* se = NewGO<SoundSource>(0);
			se->Init(16);
			se->Play(false);
			se->SetVolume(m_game->SetSoundEffectVolume());*/

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
	//座標をプレイヤーの少し前に設定する。
	//collisionPosition += forward * 50.0f;
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
		wizardUlt->SetAboutUlt(Lv);
		//攻撃するアクターの座標取得
		Vector3 UltPos = actor->GetPosition();
		UltPos.y += 100.0f;
		wizardUlt->SetPosition(UltPos);
		wizardUlt->SetGame(m_game);

		//必殺技を打たれたのでフラグを立てる
		//actor->ChangeDamegeUltFlag(true);

		//雷を落とすキャラがリストの最後なら
		if (actor == DamegeUltActor.back())
		{
			//最後であることを知らせる
			wizardUlt->ChangeUltEndFlag(true);

			//DamegeUltActor.clear();

			m_UltshootTimer = 0.0f;
			//一人ずつ必殺技を打つのでぬける
			return;
		}

		m_UltshootTimer = 0.0f;
		//一人ずつ必殺技を打つのでぬける
		//return;

	}

	

	//KnightUlt* knightUlt = NewGO<KnightUlt>(0, "knightUlt");
	////製作者の名前を入れる
	//knightUlt->SetCreatorName(GetName());
	//// 制作者を教える
	//knightUlt->SetActor(this);
	//knightUlt->SetUltColorNumb(respawnNumber);
	////キャラのレベルを入れる
	//knightUlt->GetCharLevel(Lv);
	////座標の設定
	//Vector3 UltPos = m_position;
	//UltPos.y += 60.0f;
	//knightUlt->SetPosition(UltPos);
	//knightUlt->SetRotation(m_rot);
	//knightUlt->SetEnUlt(KnightUlt::enUltSkill_Player);
	//knightUlt->SetGame(m_game);

}

void KnightAI::HPBar()
{
	if (m_Status.Hp < 0)
	{
		m_Status.Hp = 0;
	}

	Vector3 scale = Vector3::One;
	scale.x = float(m_Status.Hp) / float(m_Status.MaxHp);
	m_HP_Bar.SetScale(scale);

	Vector3 BerPosition = m_position;
	BerPosition.y += 75.0f;
	//座標を変換する
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPBer_Pos, BerPosition);
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPWindow_Pos, BerPosition);
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPBack_Pos, BerPosition);

	//HPバー画像を左寄せに表示する
	Vector3 BerSizeSubtraction = HPBerSend(HP_BER_SIZE, scale);	//画像の元の大きさ
	m_HPBer_Pos.x -= BerSizeSubtraction.x;
	wchar_t wcsbuf[256];
	std::size_t len = std::strlen(GetName());
	std::size_t converted = 0;
	wchar_t* wcstr = new wchar_t[len + 1];
	mbstowcs_s(&converted, wcstr, len + 1, GetName(), _TRUNCATE);
	m_Name.SetText(wcstr);
	m_Name.SetPosition(Vector3(m_HPBer_Pos.x, m_HPBer_Pos.y, 0.0f));
	//フォントの大きさを設定。
	m_Name.SetScale(0.5f);
	//フォントの色を設定。
	m_Name.SetColor({ 1.0f,0.0f,0.0f,1.0f });

	m_HP_Bar.SetPosition(Vector3(m_HPBer_Pos.x, m_HPBer_Pos.y, 0.0f));
	m_HP_Frame.SetPosition(Vector3(m_HPWindow_Pos.x, m_HPWindow_Pos.y, 0.0f));
	m_HP_Back.SetPosition(Vector3(m_HPBack_Pos.x, m_HPBack_Pos.y, 0.0f));

	m_HP_Bar.Update();
	m_HP_Frame.Update();
	m_HP_Back.Update();
}
Vector3 KnightAI::HPBerSend(Vector3 size, Vector3 scale)
{
	Vector3 hpBerSize = size;								//画像の元の大きさ
	Vector3 changeBerSize = Vector3::Zero;					//画像をスケール変換したあとの大きさ
	Vector3 BerSizeSubtraction = Vector3::Zero;				//画像の元と変換後の差

	changeBerSize.x = hpBerSize.x * scale.x;
	BerSizeSubtraction.x = hpBerSize.x - changeBerSize.x;
	BerSizeSubtraction.x /= 2.0f;

	return BerSizeSubtraction;
}

bool KnightAI::DrawHP()
{
	Vector3 toCameraTarget = g_camera3D->GetTarget() - g_camera3D->GetPosition();
	Vector3 toMush = m_position - g_camera3D->GetPosition();
	toCameraTarget.y = 0.0f;
	toMush.y = 0.0f;
	toCameraTarget.Normalize();
	toMush.Normalize();

	float cos = Dot(toCameraTarget, toMush);
	float angle = acos(cos);

	//カメラの後ろにあるなら描画しない
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーに向かう距離を計算する
	float playerdistance = diff.Length();

	if (fabsf(angle) < Math::DegToRad(45.0f) && playerdistance < 800.0f && m_player->GetPosition().y <= 10.0f)
	{
		return true;
	}
	else
	{
		return false;
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
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_ComboONE);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//二段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣２段目音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_ComboTwo);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
		//剣３段目音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_ComboThree);
		se->Play(false);
		se->SetVolume(0.3f);
		
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
		//m_Status.Atk += 20;
		//m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;

		//スキル音を発生
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Sword_Skill);
		se->Play(false);
		se->SetVolume(0.3f);
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
				/*m_OnGroundCharCounter++;
				//このキャラはグラウンドにいる
				actor->ChangeGroundChackflag(true);*/
			if (actor->IsActorGroundChack() == true) {
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
		//m_Status.Atk -= 20;
		m_AtkTmingState = Num_State;
		AtkState = false;
		//スキルの移動処理をしないようにする
		SkillState = false;
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
		if (Math::PI * 0.5f >= fabsf(angle) && kyori.Length()<=100)
		{
			AvoidanceFlag = true;
			//回避ステート
			m_charState = enCharState_Avoidance;
			return;
		}
	}
}


//void KnightAI::Rotation()
//{
//	
//
//	if (fabsf(m_moveSpeed.x) < 0.001f
//		&& fabsf(m_moveSpeed.z) < 0.001f) {
//		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
//		//このフレームではキャラは移動していないので旋回する必要はない。
//		return;
//	}
//	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
//	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
//	//atan2を使用して、角度を求めている。
//	//これが回転角度になる。
//	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
//	//atanが返してくる角度はラジアン単位なので3
//	//SetRotationDegではなくSetRotationを使用する。
//	m_rot.SetRotationY(-angle);
//
//	//回転を設定する。
//	m_modelRender.SetRotation(m_rot);
//
//	//プレイヤーの前ベクトルを計算する。
//	m_forward = Vector3::AxisZ;
//	m_rot.Apply(m_forward);
//}


void KnightAI::Render(RenderContext& rc)
{
	if (DarwFlag == true) {
		m_modelRender.Draw(rc);
	}

	//スプライトフラグがtureなら
	if (m_player->GetSpriteFlag())
	{
		if (DrawHP())
		{
			m_HP_Back.Draw(rc);
			m_HP_Bar.Draw(rc);
			m_HP_Frame.Draw(rc);
		}
	}
	//フォントを描画する。
	//m_Name.Draw(rc);

}

