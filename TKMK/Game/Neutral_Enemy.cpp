#include "stdafx.h"
#include "Neutral_Enemy.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>
#include "KnightPlayer.h"
#include "GameCamera.h"
#include "KnightAI.h"
#include "Player.h"
#include "MagicBall.h"
#include "Actor.h"
#include <cstring>
#include <cwchar>
//#include <vector>
//#include <algorithm>

namespace {
	const float HP_WINDOW_WIDTH = 1152.0f;
	const float HP_WINDOW_HEIGHT = 648.0f;
	const float HP_BER_WIDTH = 178.0f;
	const float HP_BER_HEIGHT = 22.0f;
	const Vector3 HP_BER_SIZE = Vector3(HP_BER_WIDTH, HP_BER_HEIGHT, 0.0f);
	const float RADIUS = 100.0f;
	const int POS = 40;
	const float HP_BER_RABBIT = 1.9f;
	const float HP_BER_RABBIT_HEIGHT = 505.0f;
}
Neutral_Enemy::Neutral_Enemy()
{

}

Neutral_Enemy::~Neutral_Enemy()
{
	// ゲームを取得
	m_game = FindGO<Game>("game");

	//m_game->Enemys();
	if (m_game != nullptr) {
		m_game->RemoveEnemyFromList(this);


		//自分がウサギなら、ウサギ生成フラグを戻す
		if (m_enemyKinds == enEnemyKinds_Rabbit)
		{
			m_game->SetRabbitFlag(false);
			rabbitLife = false;
		}
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

bool Neutral_Enemy::Start()
{
//アニメーションを読み込む。
	if (m_enemyKinds == enEnemyKinds_Rabbit)
	{
		m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Rabbit/Run.tka");
		m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		m_animationClips[enAnimationClip_Run].Load("Assets/animData/Rabbit/Run.tka");
		m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
		m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Rabbit/Run.tka");
		m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
		m_animationClips[enAnimationClip_Death].Load("Assets/animData/Rabbit/Death.tka");
		m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
		m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Rabbit/Damage.tka");
		m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);

		m_modelRender.Init("Assets/modelData/character/Rabbit/Rabbit.tkm", m_animationClips, enAnimationClip_Num);
	}
	else
	{
		m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Neutral_Enemy/Idle.tka");
		m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		m_animationClips[enAnimationClip_Run].Load("Assets/animData/Neutral_Enemy/Run.tka");
		m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
		m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Neutral_Enemy/Attack.tka");
		m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
		m_animationClips[enAnimationClip_Death].Load("Assets/animData/Neutral_Enemy/Death.tka");
		m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
		m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Neutral_Enemy/Damage.tka");
		m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);

		enemyColorRam = rand() % 10;

		if (enemyColorRam <= 5)
		{
			//白
			m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Ghost_White/Ghost_White.tkm", m_animationClips, enAnimationClip_Num);
			m_enemyKinds = enEnemyKinds_White;
		}
		else if (enemyColorRam <= 7)
		{
			//緑
			m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Neutral_Enemy.tkm", m_animationClips, enAnimationClip_Num);
			m_enemyKinds = enEnemyKinds_Green;
		}
		else if (enemyColorRam <= 9)
		{
			//赤
			m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Ghost_Red/Ghost_Red.tkm", m_animationClips, enAnimationClip_Num);
			m_enemyKinds = enEnemyKinds_Red;
		}

		//頭のボーンのIDを取得する
		m_AttackBoneId = m_modelRender.FindBoneID(L"HeadTipJoint");
	}

	m_HPBar.Init("Assets/sprite/zako_HP_bar.DDS", HP_BER_WIDTH, HP_BER_HEIGHT);
	//m_HPBar.SetPivot(PIVOT);

	m_HPBack.Init("Assets/sprite/zako_HP_background.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);

	m_HPFrame.Init("Assets/sprite/HP_flame_mushroom.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);

    //座標を設定
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rot);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	//大きさ調整
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		9.0f,			//半径。
		20.0f,			//高さ。
		m_position		//座標。
	);
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//乱数を初期化。
	srand((unsigned)time(NULL));

	m_forward = Vector3::AxisY;
	m_rot.Apply(m_forward);

	//ステータスを読み込む
	if (m_enemyKinds == enEnemyKinds_Rabbit)
	{
		m_Status.Init("Rabbit");
	}
	else {
		m_Status.Init("Enemy");
	}
	//巡回用のパスを読み込む
	m_EnemyPoslevel.Init("Assets/level3D/RabbitPatrolPos.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			SetPatrolPos(objData.position, objData.number);
			RadiusPos = objData.position;

			return true;
		}

	});

	//最初に行くポジションを決める
	randam = rand() % POS;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//行こうしてる場所の間に壁があるかどうか
	while (true)
	{
		//始点は自分の座標。
		start.setOrigin(btVector3(m_position.x, 20.0f, m_position.z));
		//終点はランダムの座標。
		end.setOrigin(btVector3(m_patrolPos[randam].x, 20.0f, m_patrolPos[randam].z));

		// 始点と終点が近すぎる
		if ((start.getOrigin() - end.getOrigin()).length() <= 0.01f) {
			randam = rand() % POS;
			continue;
		}

		SweepResultSlipThroughWall callback;
		//コライダーを始点から終点まで動かして。
		//衝突するかどうかを調べる。
		PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
		//壁と衝突した！
		if (callback.isHit == true)
		{
			randam = rand() % POS;
			//プレイヤーは見つかっていない。
			continue;
		}

		break;
	}

	// 準備完了
	isStart = true;

	return true;
}

void Neutral_Enemy::Update()
{
	if (isStart == false) {
		return;
	}

	//当たり判定。
	Collision();
	//アニメーションの再生。
	PlayAnimation();
	if (m_game->GetStopFlag() == true)
	{
		if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
		{
			m_modelRender.Update();
		}
		return;
	}
	// ポーズ中は何もしない
	if (m_Neutral_EnemyState == enNeutral_Enemy_Pause) {
		return;
	}

	HPreductionbyTimer -= g_gameTime->GetFrameDeltaTime();
	if (HPreductionbyTimer >= 10)
	{
		HPreductionbytime();
	}
	//回転処理。
	Rotation();
	//ステートの遷移処理。
	ManageState();
	HPBar();
	
	// タイマーを減らす
	if (isPatrolTimer > 0.0f) {
		isPatrolTimer -= g_gameTime->GetFrameDeltaTime();
	}

	//モデルの更新。
	m_modelRender.Update();
}

void Neutral_Enemy::Move()
{
	Vector3 diff = m_forward;
	diff.Normalize();
	////移動速度を設定する。
	m_moveSpeed = diff * m_Status.Speed;
	m_forward.Normalize();
	Vector3 moveSpeed = m_forward * m_Status.Speed + m_hagikiPower;
	if (m_hagikiPower.Length() < 10.0f) {
		m_hagikiPower *= 0.99f;
	}
	else {
		m_hagikiPower = Vector3::Zero;
	}
	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);

}

void Neutral_Enemy::HPreductionbytime()
{
	
	//if (m_enemyKinds == enEnemyKinds_Rabbit)
	//{
	//	m_Status.Hp -= 1;
	//	HPreductionbyTimer = 0.0f;
	//}
	
}

void Neutral_Enemy::Rotation()
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

void Neutral_Enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_Neutral_EnemyState != enNeutral_Enemy_Chase)
	{
		return;
	}
	Vector3 m_targetActorPos = m_targetActor->GetPosition();
	Vector3 diff = m_targetActorPos - m_position;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//始点は自分の座標。
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//終点はランダムの座標。
	end.setOrigin(btVector3(m_targetActorPos.x, m_targetActorPos.y + 70.0f, m_targetActorPos.z));
	SweepResultSlipThroughWall callback;
	//コライダーを始点から終点まで動かして。
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した！
	if (callback.isHit == true)
	{
		// 目的地に辿り着くまで敵を無視する
		m_backPatrol = true;
		m_backPatrolFarst = false;
		m_Neutral_EnemyState = enNeutral_Enemy_Patrol;
	}
	
	diff.Normalize();
	//移動速度を設定する。
	m_moveSpeed = diff * m_Status.Speed;
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 3.0f;
	m_modelRender.SetPosition(modelPosition);

}
void Neutral_Enemy::Collision()
{
	//攻撃中、デス中は当たり判定の処理を行わない
	if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
	{
		return;
	}

	//敵の攻撃用のコリジョンを取得する
	const auto& AIcollisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//子リジョンの配列をfor文で回す
	for (auto AIcollision : AIcollisions)
	{
		if (AIcollision->IsHit(m_charaCon))
		{
			//このコリジョンを作ったアクターを検索
			m_lastAttackActor = FindGO<Actor>(AIcollision->GetCreatorName());

			//プレイヤーの攻撃力を取得
			//何故かm_knightAIがnull
			//HPを減らす
			// //HPを減らす
			if (m_enemyKinds == enEnemyKinds_Rabbit)
			{
				m_Status.Hp -= 1;
			}
			else
			{
				m_Status.Hp -= m_lastAttackActor->GetAtk();
				//m_Status.Hp -= m_knightAI->SetKnightAIAtk();
			}

				//HPが0になったら
			if (m_Status.Hp <= 0)
			{
				if (m_enemyKinds == enEnemyKinds_Rabbit)
				{
					//相手に経験値を渡す
					m_lastAttackActor->ExpProcess(60);
				}
				else
				{
					//相手に経験値を渡す
					m_lastAttackActor->ExpProcess(Exp);
				}				

				//倒した時の報酬を倒した人に渡す
				// 赤…攻撃力を50あげる 緑…体力を上げる　白…何もしない
				//緑の場合
				if (m_enemyKinds == enEnemyKinds_Green)
				{
					m_lastAttackActor->HpUp(HpPass);
					if (m_lastAttackActor->GetMaxHp() < m_lastAttackActor->GetHp())
					{
						m_lastAttackActor->HpReset(m_lastAttackActor->GetMaxHp());
					}
				}

				//赤の場合
				else if (m_enemyKinds == enEnemyKinds_Red)
				{
					m_lastAttackActor->AtkUp(AtkPass);
				}
				//Deathflag = true;
				//���S�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_Death;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
				//��ʉ��Đ�
			}
		}
	}

    //敵の攻撃用のコリジョンを取得する
	const auto& Ultcollisions = g_collisionObjectManager->FindCollisionObjects("player_UltimateSkill");
	//子リジョンの配列をfor文で回す
	for (auto collision : Ultcollisions)
	{
		if (collision->IsHit(m_charaCon))
		{
			//このコリジョンを作ったアクターを検索
			m_lastAttackActor = FindGO<Actor>(collision->GetCreatorName());

			//hpを減らす
			if (m_enemyKinds == enEnemyKinds_Rabbit)
			{
				m_Status.Hp -= 1;
			}
			else
			{
				m_Status.Hp -= 100;
			}
			
			if (m_Status.Hp < 0)
			{
				//相手に経験値を渡す
				m_lastAttackActor->ExpProcess(Exp/2);
				//死亡ステートに遷移する。
				m_Neutral_EnemyState = enNeutral_Enemy_Death;
			}
			else {
				//被ダメージステートに遷移する。
				m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
				//効果音再生
			}
		}
	}
	////攻撃中、デス中は当たり判定の処理を行わない
	//if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
	//{
	//	return;
	//}
	//魔法使いの攻撃用のコリジョンを取得する
	const auto& Wizardcollisions = g_collisionObjectManager->FindCollisionObjects("Wizard_MagicBall");
	//コリジョンの配列をfor文で回す
	for (auto Wizardcollision : Wizardcollisions)
	{
		if (Wizardcollision->IsHit(m_charaCon))
		{
			//このコリジョンを作ったアクターを検索
			m_lastAttackActor = FindGO<Actor>(Wizardcollision->GetCreatorName());
			//magicBall = FindGO<MagicBall>("magicBall");
			//魔法使いの攻撃力を取得
			//HPを減らす
			m_Status.Hp -= m_lastAttackActor->GetAtk();

			//HPが0になったら
			if (m_Status.Hp <= 0)
			{
				//player = FindGO<Player>("player");
				//相手に経験値を渡す
				m_lastAttackActor->ExpProcess(Exp);
				//魔法使いに経験値を渡す
				//player->CharSetExpProcess(Exp);
				//Deathflag = true;
				//死亡ステートに遷移する。
				m_Neutral_EnemyState = enNeutral_Enemy_Death;
			}
			else {
				//被ダメージステートに遷移する。
				m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
				//効果音再生
			}
		}
	}
}

void Neutral_Enemy::Attack()
{
    //攻撃ステートではなかったら
	if (m_Neutral_EnemyState != enNeutral_Enemy_Attack)
	{
		return;
	}
}

bool Neutral_Enemy::RabbitSearch()
{
	//全てのActorを調べる
	for (Actor* actor : m_game->GetActors()) {

		Vector3 ActorPosition = actor->GetPosition();
		Vector3 diff = ActorPosition - m_position;
		diff.y = 0.0f;

		//Actorが視界内に居たら。
		if ((ActorPosition - m_position).LengthSq() <= 100.0f * 100.0f)
		{
			m_targetActor = actor;

			return true;
		}
	}

	m_targetActor = nullptr;
	return false;
}

bool Neutral_Enemy::Search()
{
	//全てのActorを調べる
	for (Actor* actor : m_game->GetActors()) {

		Vector3 ActorPosition = actor->GetPosition();
		Vector3 diff = ActorPosition - m_position;
		diff.Normalize();
		float angle = acosf(diff.Dot(m_forward));
	
		//Actorが視界内に居たら。
		if (Math::PI * 0.8f > fabsf(angle) && (ActorPosition - m_position).LengthSq() <= 100.0f * 100.0f)
		{
			m_targetActor = actor;

			return true;
		}
	}

	//どのActorも視界内にいない。
	m_targetActor = nullptr;

	return false;
}

void Neutral_Enemy::MakeAttackCollision()
{
	if (m_enemyKinds == enEnemyKinds_Rabbit) {
		return;
	}

	//攻撃判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//頭のボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_AttackBoneId)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateSphere(m_position, Quaternion::Identity,20.0f);
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
	collisionObject->SetCreatorName(GetName());
}
void Neutral_Enemy::ProcessCommonStateTransition()
{
	
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	if (m_targetActor!=nullptr)
	{
		Vector3 diff = m_targetActor->GetPosition() - m_position;

		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff;
		

		//攻撃できる距離なら。
		if (CanAttack() == true)
		{
			//乱数によって、攻撃するか待機させるかを決定する。	
			{
				//乱数によって、攻撃するか待機させるかを決定する。	
		/*		int ram = rand() % 100;
				if (ram > 50)*/

				//攻撃ステートに移行する。
				if (m_Neutral_EnemyState == enNeutral_Enemy_Attack) {
					m_Neutral_EnemyState = enNeutral_Enemy_Chase;
				}
				else {
					m_Neutral_EnemyState = enNeutral_Enemy_Attack;
				}
				m_UnderAttack = false;
				return;
			}
		}
		//攻撃できない距離なら。
		else
		{

			//追跡ステートに移行する。
			m_Neutral_EnemyState = enNeutral_Enemy_Chase;
			return;

		}
	}
	//敵を見つけられなければ。
	else
	{
		m_Neutral_EnemyState = enNeutral_Enemy_Patrol;
	}
}
void Neutral_Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();

	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.5f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();

	}

}
void Neutral_Enemy::ProcessRunStateTransition()
{
	//他のステートへ遷移する。
	ProcessCommonStateTransition();
}

void Neutral_Enemy::ProcessChaseStateTransition()
{
	// ウサギの追尾
	if (m_enemyKinds == enEnemyKinds_Rabbit) {

		if (RabbitSearch() == false) {
			m_Neutral_EnemyState = enNeutral_Enemy_Patrol;

			if (isPatrolTimer <= 0.0f) {
				isPatrolRandom = true;
				isPatrolTimer = 3.0f;
			}
		}
		else {
			// 逃げる
			Vector3 Escapediff = m_targetActor->GetPosition() - m_position;
			Escapediff.y = 0.0f;

			Escapediff.Normalize();
			m_moveSpeed = Escapediff * (m_Status.Speed * -1);
			m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

			m_modelRender.SetPosition(m_position);
		}

		return;
	}

	Chase();

	if (Search() == false) {
		m_Neutral_EnemyState = enNeutral_Enemy_Patrol;
	}
	else {
		//攻撃できる距離なら。
		if (CanAttack())
		{
			//他のステートへ遷移する。
			ProcessCommonStateTransition();
			return;
		}
	}

	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.5f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}
}

void Neutral_Enemy::ProcessAttackStateTransition()
{
	Attack();
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}
}

void Neutral_Enemy::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_lastAttackActor == nullptr) {
			//対象が居ないので巡回する。
			m_Neutral_EnemyState = enNeutral_Enemy_Patrol;

			return;
		}

		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
		//Vector3 diff = player->GetCharPosition() - m_position;
		//diff.Normalize();
		//移動速度を設定する。
		//m_moveSpeed = diff * m_Status.Speed;
		m_targetActor = m_lastAttackActor;

	}
}

void Neutral_Enemy::ProcessDeathStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//全てのアクターに自分が死んだことを通知する
		for (Actor* actor : m_game->GetActors()) {
			//それがAIだったらターゲットから外させる
			if (actor->GetAIorPlayer() == Actor::EnAIorPlayer::enAI) {

				actor->ClearEnemyTarget(this);
			}
		}

		//自身を削除する。
		DeleteGO(this);
	}
}

void Neutral_Enemy::ProcessBackPatrolStateTransition()
{
	m_Neutral_EnemyState = enNeutral_Enemy_Patrol;
}

void Neutral_Enemy::ProcessPatrolStateTransition()
{
	//エネミーたちの情報を取得する
	std::vector<Neutral_Enemy*>& enemys = m_game->GetNeutral_Enemys();
	for (auto Enemys : enemys)
	{
		//取得したエネミーたちの座標を取得
		Vector3 enemyPos = Enemys->GetPosition();
		Vector3 diff = m_position - enemyPos;
		if (diff.Length() <= 50.0f)
		{
			diff.Normalize();
			m_hagikiPower += diff * 50.0f;
		}
	}
	    
		Vector3 newForward2 = m_patrolPos[randam] - m_position;
		Vector3 distance2 = newForward2;
		newForward2.Normalize();
		m_forward = newForward2;

		if (isPatrolRandom == false) {
			Move();
		}

		if (distance2.Length() <= 10.0f || isPatrolRandom == true)
		{
			isPatrolRandom = false;
			// バックパトロール（追加）
			/*if (m_backPatrol) {
				if (m_backPatrolFarst == false) {
					m_backPatrolFarst = true;
				}
				else {
					m_backPatrolFarst = false;
					m_backPatrol = false;
				}
			}*/

			randam = rand() % POS;
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			
			while (true)
			{
				//始点は自分の座標。
				start.setOrigin(btVector3(m_position.x, 20.0f, m_position.z));
				//終点はランダムの座標。
				end.setOrigin(btVector3(m_patrolPos[randam].x, 20.0f, m_patrolPos[randam].z));

				// 始点と終点が近すぎる
				if ((start.getOrigin() - end.getOrigin()).length() <= 0.01f) {
					randam = rand() % POS;
					continue;
				}

				SweepResultSlipThroughWall callback;
				//コライダーを始点から終点まで動かして。
				//衝突するかどうかを調べる。
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
				//壁と衝突した！
				if (callback.isHit == true)
				{				
					randam = rand() % POS;
					//プレイヤーは見つかっていない。
					continue;
				}

				//自分がウサギの場合、TargetActorと目的地のなす角が小さかったらやり直す
				if (m_enemyKinds == enEnemyKinds_Rabbit&& m_targetActor != nullptr)
				{
					Vector3 a = m_position - m_targetActor->GetPosition();
					Vector3 b = m_position - m_patrolPos[randam];

					//内積
					float nai;
					nai = a.Dot(b);
					//なす角
					float A = a.Length();
					float B = b.Length();

					float cos_sita = nai / (A * B);
					float sita = acos(cos_sita);
					sita = Math::RadToDeg(sita);

					if (sita < 90.0f)
					{
						randam = rand() % POS;
						continue;
					}
				}

				break;
			}

			Vector3 newForward2 = m_patrolPos[randam] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();

		}

		if (m_enemyKinds == enEnemyKinds_Rabbit)
		{
			RabbitSearch();
			EscapeSearch();

			return;
		}

		//対象を探す
		if (Search() && m_backPatrol == false)
		{
			m_Neutral_EnemyState = enNeutral_Enemy_Chase;
		}

		ProcessCommonStateTransition();
}
void Neutral_Enemy::ManageState()
{
	switch (m_Neutral_EnemyState)
	{
		//待機ステート
	case enNeutral_Enemy_Idle:
		ProcessIdleStateTransition();
		break;
		//追跡ステート
	case enNeutral_Enemy_Chase:
		ProcessChaseStateTransition();
		break;
		//攻撃ステート
	case enNeutral_Enemy_Attack:
		ProcessAttackStateTransition();
		break;
		//被ダメージステート
	case enNeutral_Enemy_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//死亡ステート
	case enNeutral_Enemy_Death:
		ProcessDeathStateTransition();
		break;
	case enNeutral_Enemy_Patrol:
		ProcessPatrolStateTransition();
		break;
	case enNeutral_Enemy_BackPatrol:
		ProcessBackPatrolStateTransition();
		break;
	}
}

void Neutral_Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.5f);
	switch(m_Neutral_EnemyState)
	{
		//待機ステート
	case enNeutral_Enemy_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステート
	case enNeutral_Enemy_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
		//攻撃ステート
	case enNeutral_Enemy_Attack:
		m_modelRender.SetAnimationSpeed(0.8f);
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		break;
		//被ダメージステート
	case enNeutral_Enemy_ReceiveDamage:
		m_modelRender.PlayAnimation(enNeutral_Enemy_ReceiveDamage, 0.5f);
		break;
		//死亡ステート
	case enNeutral_Enemy_Death:
		m_modelRender.PlayAnimation(enNeutral_Enemy_Death, 0.5f);
		break;
	case enNeutral_Enemy_Patrol:
		m_modelRender.PlayAnimation(enNeutral_Enemy_Patrol, 0.5f);
		break;
	}
}

//
void Neutral_Enemy::HPBar()
{
	if (m_Status.Hp < 0)
	{
		m_Status.Hp = 0;
	}

	Vector3 scale = Vector3::One;
	scale.x = float(m_Status.Hp) / float(m_Status.MaxHp);
	m_HPBar.SetScale(scale);

	Vector3 BerPosition = m_position;
	BerPosition.y += 75.0f;
	//座標を変換する
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPBerPos, BerPosition);
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPWindowPos, BerPosition);
	g_camera3D->CalcScreenPositionFromWorldPosition(m_HPBackPos, BerPosition);

	//HPバー画像を左寄せに表示する
	Vector3 BerSizeSubtraction = HPBerSend(HP_BER_SIZE, scale);	//画像の元の大きさ
	m_HPBerPos.x -= BerSizeSubtraction.x;
	wchar_t wcsbuf[256];
	std::size_t len = std::strlen(GetName());
	std::size_t converted = 0;
	wchar_t* wcstr = new wchar_t[len + 1];
	mbstowcs_s(&converted, wcstr, len + 1, GetName(), _TRUNCATE);
	m_Name.SetText(wcstr);
	m_Name.SetPosition(Vector3(m_HPBerPos.x, m_HPBerPos.y, 0.0f));
	//フォントの大きさを設定。
	m_Name.SetScale(0.5f);
	//フォントの色を設定。
	m_Name.SetColor({ 1.0f,0.0f,0.0f,1.0f });

	m_HPBar.SetPosition(Vector3(m_HPBerPos.x, m_HPBerPos.y, 0.0f));
	m_HPFrame.SetPosition(Vector3(m_HPWindowPos.x, m_HPWindowPos.y, 0.0f));
	m_HPBack.SetPosition(Vector3(m_HPBackPos.x, m_HPBackPos.y, 0.0f));

	m_HPBar.Update();
	m_HPFrame.Update();
	m_HPBack.Update();
}
Vector3 Neutral_Enemy::HPBerSend(Vector3 size, Vector3 scale)
{
	Vector3 hpBerSize = size;								//画像の元の大きさ
	Vector3 changeBerSize = Vector3::Zero;					//画像をスケール変換したあとの大きさ
	Vector3 BerSizeSubtraction = Vector3::Zero;				//画像の元と変換後の差

	changeBerSize.x = hpBerSize.x * scale.x;
	BerSizeSubtraction.x = hpBerSize.x - changeBerSize.x;
	BerSizeSubtraction.x /= 2.0f;

	return BerSizeSubtraction;
}

bool Neutral_Enemy::DrawHP()
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

	if (fabsf(angle) < Math::DegToRad(45.0f)&& playerdistance<800.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Neutral_Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前がAttack_startの時
	if (wcscmp(eventName, L"Attack_start") == 0) {
		//攻撃中の判定をtrueにする
		m_UnderAttack = true;
		//攻撃エフェクトを発生させる
		
		//攻撃用のコリジョンを作成
		MakeAttackCollision();

		//大きさを設定する。

		//座標を調整

		//エフェクト再生


		//効果音再生
		//攻撃の声
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(21);
		se->Play(false);
		se->SetVolume(m_game->SetSoundEffectVolume());

		//効果音を再生する


	}
	//キーの名前がattack_endの時
	else if (wcscmp(eventName,L"Attack_end")==0){
		m_UnderAttack = false;

		//ステートを切り替える
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
	}
}

const bool Neutral_Enemy::CanAttack()const
{
	//中立の敵からプレイヤーに向かうベクトルを計算する
	Vector3 diff = m_targetActor->GetPosition() - m_position;
	//距離が近かったら
	if (diff.LengthSq() <= 50.0f * 50.0f)
	{
		//攻撃できる
		return true;
	}
	//攻撃できない
	return false;
}

void Neutral_Enemy::EscapeSearch()
{
	if (m_targetActor == nullptr)
	{
		return;
	}

	//ウサギからプレイヤーに向かうベクトルを計算する
	Vector3 Escapediff = m_targetActor->GetPosition() - m_position;
	Escapediff.y = 0.0f;

	if (Escapediff.Length() <= 200.0f && isPatrolTimer <= 0.0f)
	{
		// Chase(逃げステートに変更）
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
	}	

}
void Neutral_Enemy::modelUpdate()
{
	//座標を設定
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rot);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);

	//モデルの更新。
	m_modelRender.Update();
}
void Neutral_Enemy::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
	//フォントを描画する。
	//m_Name.Draw(rc);

	if (m_game->GetStopFlag() == true)
	{
		return;
	}

	// ポーズ中は何もしない
	if (m_Neutral_EnemyState == enNeutral_Enemy_Pause) {
		return;
	}

	//スプライトフラグがtureなら
	if (m_player->GetSpriteFlag())
	{
		if (DrawHP())
		{
			m_HPBack.Draw(rc);
			m_HPBar.Draw(rc);
			m_HPFrame.Draw(rc);
		}
	}

	
}