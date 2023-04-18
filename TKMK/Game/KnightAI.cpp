#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Actor.h"
#include "WizardUlt.h"


KnightAI::KnightAI()
{
	m_Status.Init("Knight");
	
	SetModel();
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標0番の取得
	GetRespawnPos();
	respawnNumber = 1;        //リスポーンする座標の番号
	m_respawnPos[respawnNumber].y /*+= m_position_YUp*/;
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);
	
}
KnightAI::~KnightAI()
{

}
//衝突したときに呼ばれる関数オブジェクト(すり抜ける壁用)
struct SweepResultSlipThroughWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	const btCollisionObject* hitObject = nullptr;
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//衝突点を記録している
		//convexResult.m_hitPointLocal
		//ワープの距離が長すぎると当たったオブジェクトが別々のときに使う
		//hitObjectに記録する

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

void KnightAI::CalculatAIAttackEvaluationValue()
{
	//エネミーたちの情報を取得する
	std::vector<Actor*>& actors = m_game->GetActors();

	for (auto actor : actors)
	{
		if (actor == this)
		{
			continue;
		}
		int eval = CalculateTargetAI(actor);
		Evaluation_valueActor.push_back(eval);
	}
}

int KnightAI::CalculateTargetAI(Actor* actor)
{
	int eval = 0;
	//アクターたちの座標を取得
	Vector3 actorPos = actor->GetPosition();
	//アクターたちと自分の座標を引く
	Vector3 diff = actorPos - m_position;
	float Distance = diff.Length();
	eval += 5000 - (int)Distance;

	//今狙ってるアクターのレベルを取得する
	int actorlv = actor->GetLevel();
	//相手より自分のレベルが高かったら評価値は高くなる
	eval += (Lv - actorlv) * 500;
	//対象が自分だったら評価値を無限小にする
	if (actor == this)
	{
		eval = -9999999;
	}
	return eval;
}

void KnightAI::CalculatEnemyAttackEvaluationValue()
{
	//エネミーたちの情報を取得する
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();

	for (auto enemy : enemys)
	{
		int eval = CalculateTargetEnemy(enemy);
		Evaluation_valueEnemy.push_back(eval);
	}
}

int KnightAI::CalculateTargetEnemy(Neutral_Enemy* enemy)
{
	int eval = 0;
	Neutral_Enemy::EnEnemyKinds ColorEnemy = enemy->GetenemyColor();

	//エネミーの座標を取得
	Vector3 enemyPos = enemy->GetPosition();
	//自分の座標と引く
	Vector3 diff = enemyPos - m_position;
	float Distance = diff.Length();
	// 距離で評価値を決める 近いほど高い
	eval += 5000 - (int)Distance;

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


	return eval;
}

void KnightAI::LotNextAction()
{
	//初期化
	Evaluation_valueEnemy.clear();
	Evaluation_valueActor.clear();

	//中立の敵の評価値の計算
	CalculatEnemyAttackEvaluationValue();
	//アクターの評価値の計算
	CalculatAIAttackEvaluationValue();
	//評価値無限小
	int noweval = -999999;
	//今のターゲットエネミー
	Neutral_Enemy* m_nowEnemyTarget = nullptr;
	Actor* m_nowActorTarget = nullptr;
	//falseだったらエネミーを狙う、trueだったらアクターを狙う
	bool TargetChange = false;
	
	//中立から判定する
	std::vector<Neutral_Enemy*> enemys = m_game->GetNeutral_Enemys();
	for (int i = 0; i < Evaluation_valueEnemy.size(); i++) {

		if (Evaluation_valueEnemy[i] > noweval)
		{
			noweval = Evaluation_valueEnemy[i];
			m_nowEnemyTarget = enemys[i];
			TargetChange = false;
		}

	}
	//アクターを判定する
	std::vector<Actor*> actors = m_game->GetActors();
	for (int i = 0; i < Evaluation_valueActor.size(); i++)
	{
		if (Evaluation_valueActor[i] > noweval)
		{
			noweval = Evaluation_valueActor[i];
			m_nowActorTarget = actors[i];
			TargetChange = true;
		}
	}

	if (TargetChange == false)
	{
		TargePos = m_nowEnemyTarget->GetPosition();
		m_targetEnemy = m_nowEnemyTarget;
	}
	else
	{
		TargePos = m_nowActorTarget->GetPosition();
		m_targetActor = m_nowActorTarget;
	}
}

void KnightAI::Update()
{
	//スキルクールタイムの処理
	COOlTIME(Cooltime, SkillEndFlag, SkillTimer);
	// 次のアクションを抽選 
	LotNextAction();
	SearchEnemy();
	Attack();
    //ステート
	ManageState();
  
	//アニメーションの再生

	PlayAnimation();
	Collition();
	Rotation();

	if (SkillState == true)
	{
		m_knightState = enKnightState_Skill;
		//スキルを使うときのスピードを使う
		m_aiForward = TargePos - m_position;
		m_aiForward.Normalize();
		m_rot.SetRotationYFromDirectionXZ(m_aiForward);
		////スキルを使うときのスピードを使う
		////AnimationMove(SkillSpeed, m_forward);
		m_aiForward.y = 0.0f;
		m_aiForward *= 300.0f;

		m_position = m_charCon.Execute(m_aiForward, g_gameTime->GetFrameDeltaTime());

	}

	//回避クールタイムの処理
	COOlTIME(AvoidanceCoolTime, AvoidanceEndFlag, AvoidanceTimer);
	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rot);
	m_modelRender.Update();
}

void KnightAI::SearchEnemy()
{
	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	if (m_knightState == enKnightState_Damege ||
		m_knightState == enKnightState_Death ||
		m_knightState == enKnightState_UltimateSkill ||
		m_knightState == enKnightState_ChainAtk ||
		m_knightState == enKnightState_Skill ||
		m_knightState == enKnightState_Avoidance)
	{
		return;
	}

	Vector3 diff = TargePos - m_position;
	diff.Normalize();
	//移動速度を設定する。
	m_moveSpeed = diff * m_Status.Speed;
	m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	
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
const bool KnightAI::CanAttack()
{
	Vector3 diff = TargePos - m_position;

	if (diff.LengthSq() <= 70.0f * 70.0f)
	{
		return true;
	}
	return false;
}

void KnightAI::Attack()
{	



	if (CanSkill())
	{
		
		
		//連打で攻撃できなくなる
		//スキルを発動する処理
		if (m_targetActor!=nullptr&& SkillEndFlag==false&&pushFlag == false)
		{
			//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
			if (m_knightState == enKnightState_Damege ||
				m_knightState == enKnightState_Death ||
				m_knightState == enKnightState_UltimateSkill ||
				m_knightState == enKnightState_ChainAtk ||
				m_knightState == enKnightState_Skill ||
				m_knightState == enKnightState_Avoidance)
			{
				return;
			}j
			//スキルを打つ
			SkillState = true;
			pushFlag = true;
		
			
			
		}
	}
	if (CanAttack()) {
		
		//必殺技を発動する処理
		if (m_Status.Hp < m_Status.MaxHp / 2 && m_targetActor != nullptr && Lv >= 4)
		{
			//アニメーション再生、レベルを３
			UltimateSkill();



			//アルティメットSE
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(16);
			se->Play(false);
			se->SetVolume(0.3f);

			//必殺技発動フラグをセット
			UltimateSkillFlag = true;
		}

		//必殺技発動フラグがセットされているなら
		if (UltimateSkillFlag == true)
		{
			UltimateSkillTimer += g_gameTime->GetFrameDeltaTime();
			//必殺技タイマーが3.0fまでの間
			if (UltimateSkillTimer <= 3.0f)
			{
				//コリジョンの作成、移動処理
				UltimateSkillCollistion(OldPosition, m_position);
			}
			else
			{
				//攻撃が有効な時間をリセット
				UltimateSkillTimer = 0;
				//必殺技発動フラグをリセット
				UltimateSkillFlag = false;
				//コリジョン削除
				DeleteGO(collisionObject);
				//コリジョン作成フラグをリセット
				UltCollisionSetFlag = false;
			}
		}
		//一段目のアタックをしていないなら
		if (AtkState == false&&pushFlag == false)
		{
			Vector3 diff = TargePos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			m_knightState = enKnightState_ChainAtk;

			//FirstAtkFlag = true;
			//コンボを1増やす
			//ComboState++;

			AtkState = true;
		}
		//一段目のアタックのアニメーションがスタートしたなら
		if (m_AtkTmingState == FirstAtk_State)
		{
			Vector3 diff = TargePos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			//ステートを二段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = SecondAtk_State;

		}

		if (m_AtkTmingState == SecondAtkStart_State)
		{
			Vector3 diff = TargePos - m_position;
			m_rot.SetRotationYFromDirectionXZ(diff);
			//ステートを三段目のアタックのアニメーションスタートステートにする
			m_AtkTmingState = LastAtk_State;

		}
	}
	

	//攻撃かスキルを使用しているなら
	//コリジョン作成
	if (AtkCollistionFlag == true) AtkCollisiton();

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
		se->Init(13);
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
		se->Init(14);
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
		se->Init(15);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//スキルのアニメーションが始まったら
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		m_Status.Atk += 20;
		//m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;

		//スキル音を発生
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(11);
		se->Play(false);
		se->SetVolume(0.3f);
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
		//ボタンが押されていなかったら
		if (m_AtkTmingState != SecondAtk_State)
		{
			//ボタンプッシュフラグをfalseにする
			pushFlag = false;
			AtkState = false;
			m_knightState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}

	//二段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SecondAttack_End") == 0)
	{

		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
		//ボタンが押されていなかったら
		if (m_AtkTmingState != LastAtk_State)
		{
			//ボタンプッシュフラグをfalseにする
			pushFlag = false;
			AtkState = false;
			m_knightState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}
	//三段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"LastAttack_End") == 0)
	{
		m_AtkTmingState = Num_State;
		AtkState = false;
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false) {
		m_knightState = enKnightState_Idle;
		AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
	}

	//スキルのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SkillAttack_End") == 0)
	{
		m_Status.Atk -= 20;
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

}
void KnightAI::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rot.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rot);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rot.Apply(m_forward);
}
void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}

