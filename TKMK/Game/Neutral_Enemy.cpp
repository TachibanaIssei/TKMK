#include "stdafx.h"
#include "Neutral_Enemy.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>
#include"KnightPlayer.h"
#include "GameCamera.h"
//#include <vector>
//#include <algorithm>

namespace {
	const float HP_WINDOW_WIDTH = 1152.0f;
	const float HP_WINDOW_HEIGHT = 648.0f;
	const float HP_BER_WIDTH = 178.0f;
	const float HP_BER_HEIGHT = 22.0f;
	const Vector3 HP_BER_SIZE = Vector3(HP_BER_WIDTH, HP_BER_HEIGHT, 0.0f);
}
Neutral_Enemy::Neutral_Enemy()
{

}

Neutral_Enemy::~Neutral_Enemy()
{




	
}

bool Neutral_Enemy::Start()
{
	//アニメーションを読み込む。
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
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Neutral_Enemy.tkm", m_animationClips, enAnimationClip_Num);

	//座標を設定
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rot);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	//大きさ調整
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		25.0f,			//半径。
		50.0f,			//高さ。
		m_position,		//座標。
		true
	);

	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);

	//剣のボーンのIDを取得する
	m_AttackBoneId = m_modelRender.FindBoneID(L"HeadTipJoint");

	m_HPBar.Init("Assets/sprite/zako_HP_bar.DDS", HP_BER_WIDTH, HP_BER_HEIGHT);
	//m_HPBar.SetPivot(PIVOT);

	m_HPBack.Init("Assets/sprite/zako_HP_background.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);

	m_HPFrame.Init("Assets/sprite/HP_flame_mushroom.DDS", HP_WINDOW_WIDTH, HP_WINDOW_HEIGHT);



	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_knightplayer = FindGO<KnightPlayer>("m_knightplayer");

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisY;
	m_rot.Apply(m_forward);
	//ステータスを読み込む
	m_Status.Init("Enemy");

	

	m_EnemyPoslevel.Init("Assets/level3D/enemyPos.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			//左上の座標
			if (objData.number == 0) {
				SetPatrolPos(objData.position, objData.number);
				P = 0;
				return true;
			}
			if (objData.number == 1) {
				SetPatrolPos(objData.position, objData.number);
				P = 1;
				return true;
				
			}
			if (objData.number == 2) {
				SetPatrolPos(objData.position, objData.number);
				P = 2;
				return true;
			}
			if (objData.number == 3) {
				SetPatrolPos(objData.position, objData.number);
				P = 3;
				return true;
			}
			if (objData.number == 4) {
				SetPatrolPos(objData.position, objData.number);
				P = 4;
				return true;
			}
			if (objData.number == 5) {
				SetPatrolPos(objData.position, objData.number);
				P = 5;
				return true;
			}
			if (objData.number == 6) {
				SetPatrolPos(objData.position, objData.number);
				P = 6;
				return true;
			}
			if (objData.number == 7) {
				SetPatrolPos(objData.position, objData.number);
				P = 7;
				return true;
			}
			if (objData.number == 8) {
				SetPatrolPos(objData.position, objData.number);
				P = 8;
				return true;
			}
			if (objData.number == 9) {
				SetPatrolPos(objData.position, objData.number);
				P = 9;
				return true;
			}
			if (objData.number == 10) {
				SetPatrolPos(objData.position, objData.number);
				P = 10;
				return true;
			}
		}
	});
	return true;
}

void Neutral_Enemy::Update()
{

	SearchEnemy();


	if (m_Neutral_EnemyState == enNeutral_Enemy_Pause) {
		return;
	}

	//追跡処理。
	Chase();
	//回転処理。
	Rotation();
	//当たり判定。
	Collision();
	//攻撃処理。
	Attack();
	//アニメーションの再生。
	PlayAnimation();
	//ステートの遷移処理。
	ManageState();
	HPBar();
	//モデルの更新。
	m_modelRender.Update();
}
void Neutral_Enemy::Move()
{
	Vector3 diff = m_forward;
	diff.Normalize();
	//移動速度を設定する。
	m_moveSpeed = diff * m_Status.Speed;
	m_forward.Normalize();
	Vector3 moveSpeed = m_forward * 50.0f;
	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);

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
//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			return 0.0f;
		}

		//壁とぶつかったら。
		//フラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};

void Neutral_Enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_Neutral_EnemyState != enNeutral_Enemy_Chase)
	{
		return;
	}
	//Vector3 diff = m_knightplayer->GetPosition() - m_position;
	//diff.Normalize();
	////移動速度を設定する。
	//m_moveSpeed = diff * m_Status.Speed;
	//m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//if (m_charaCon.IsOnGround()) {
	//	//地面についた。
	//	m_moveSpeed.y = 0.0f;
	//}
	//Vector3 modelPosition = m_position;
	////ちょっとだけモデルの座標を挙げる。
	//modelPosition.y += 2.5f;
	//m_modelRender.SetPosition(modelPosition);


}


void Neutral_Enemy::Collision()
{
	//攻撃中、デス中は当たり判定の処理を行わない
	if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
	{
		return;
	}

	//敵の攻撃用のコリジョンを取得する
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//子リジョンの配列をfor文で回す
	for (auto collision : collisions)
	{
		if (collision->IsHit(m_charaCon))
		{
			//プレイヤーの攻撃力を取得
			//何故かm_knightplayerがnull
			//HPを減らす
			m_Status.Hp -= m_knightplayer->SetKnightAtk();

			
			//HPが0になったら
			if (m_Status.Hp <= 0)
			{
				//剣士に経験値を渡す
				m_knightplayer->ExpProcess(Exp);
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
	//敵の攻撃用のコリジョンを取得する
	const auto& Ultcollisions = g_collisionObjectManager->FindCollisionObjects("player_UltimateSkill");
	//子リジョンの配列をfor文で回す
	for (auto collision : Ultcollisions)
	{
		if (collision->IsHit(m_charaCon))
		{
			//hpを減らす
			m_Status.Hp -= 100;
			if (m_Status.Hp < 0)
			{
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
	//攻撃判定中であれば
	if (m_UnderAttack = true)
	{
		//攻撃用のコリジョンを作成
		MakeAttackCollision();
	}

}

void Neutral_Enemy::SearchEnemy()
{

	//Vector3 playerPosition = m_knightplayer->GetPosition();
	//Vector3 diff = playerPosition - m_position;
	//diff.Normalize();
	//float angle = acosf(diff.Dot(m_forward));
	////プレイヤーが視界内に居なかったら。
	//if (Math::PI * 0.1f <= fabsf(angle))
	//{
	//	//プレイヤーは見つかっていない。
	//	return;
	//}

	//btTransform start, end;
	//start.setIdentity();
	//end.setIdentity();
	////始点はエネミーの座標。
	//start.setOrigin(btVector3(m_position.x, 50.0f, m_position.z));
	////終点はプレイヤーの座標。
	//end.setOrigin(btVector3(playerPosition.x, 50.0f, playerPosition.z));

	//SweepResultWall callback;
	////コライダーを始点から終点まで動かして。
	////衝突するかどうかを調べる。
	//PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	////壁と衝突した！
	//if (callback.isHit == true)
	//{
	//	//プレイヤーは見つかっていない。
	//	return;
	//}

	////壁と衝突してない！！
	////プレイヤー見つけたフラグをtrueに。
	//m_isSearchPlayer = true;
	
}

void Neutral_Enemy::MakeAttackCollision()
{
	//攻撃判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//頭のボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_AttackBoneId)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateSphere(m_position, Quaternion::Identity,20.0f);
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");

}

void Neutral_Enemy::ProcessCommonStateTransition()
{
	
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//敵を見つかったら攻撃
	//プレイヤーを見つけたら。
	if (m_isSearchPlayer == true)
	{
		Vector3 diff = m_knightplayer->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff;
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;

		//攻撃できる距離なら。
		if (CanAttack() == true)
		{
		
			{
				//攻撃ステートに移行する。
				m_Neutral_EnemyState = enNeutral_Enemy_Attack;
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
	//プレイヤーを見つけられなければ。
	else
	{
		m_Neutral_EnemyState = enNEutral_Enemy_Patrol;
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
	//攻撃できる距離なら。
	if (CanAttack())
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
		return;
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
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
		Vector3 diff = m_knightplayer->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * m_Status.Speed;
	}
}

void Neutral_Enemy::ProcessDeathStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//m_game->GetNeutral_EnemyContaier().erase(std::remove(m_game->GetNeutral_EnemyContaier().begin(),
		//	m_game->GetNeutral_EnemyContaier().end(), this), m_game->GetNeutral_EnemyContaier().end()/*std::cend(m_game->GetNeutral_EnemyContaier()*/);
		m_game->SubNeutral_EnemyContaier();
		//自身を削除する。
		DeleteGO(this);
	}
}
void Neutral_Enemy::ProcessPatrolStateTransition()
{
	if (Patrol)
	{
		if (P == 0)
		{
			//position1に向かうコード
			//もしもposition1に到着したらP=1;
			//patrol=true;
			Vector3 newForward = m_patrolPos[0] - m_position;
			Vector3 distance = newForward;
			newForward.Normalize();
			m_forward = newForward;
			Move();
			if (distance.Length() <= 10.0f)
			{
				//1からにしかったら+１しろ
				int ram = rand() % 100 /*+ 1*/;
				if (ram >= 0)
				{
					P = 1;
				}
				if (ram > 25)
				{
					P = 4;
				}
				if (ram > 50)
				{
					P = 6;
				}
				if (ram > 75)
				{
					P = 8;
				}
			}


		}
		 if (P == 1)
		{
			//position2に向かうコード
			//もしもposition2に到着したらP=0;
			//patrol=true;
			Vector3 newForward2 = m_patrolPos[1] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false；
				int ram = rand() % 100;
				if (ram < 50)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 10;
				}
			}

		}
		 if (P == 2)
		{
			Vector3 newForward2 = m_patrolPos[2] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 50)
				{
					P = 1;
				}
				if (ram > 50)
				{
					P = 3;
				} 
			}

		}
		 if (P == 3)
		{
			Vector3 newForward2 = m_patrolPos[3] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 50)
				{
					P = 2;
				}
				if (ram > 50)
				{
					P = 4;
				}
			}
		}
		 if (P == 4)
		{
			Vector3 newForward2 = m_patrolPos[4] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 45)
				{
					P = 3;
				}
				if (ram > 45)
				{
					P = 0;
				}
				if (ram > 75)
				{
					P = 5;
				}
			}
		}
		 if (P == 5)
		{
			Vector3 newForward2 = m_patrolPos[5] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 50)
				{
					P = 4;
				}
				if (ram > 50)
				{
					P = 6;
				}
			}
		}
		 if (P == 6)
		{
			Vector3 newForward2 = m_patrolPos[6] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 45)
				{
					P = 5;
				}
				if (ram > 45)
				{
					P = 0;
				}
				if (ram > 75)
				{
					P = 7;
				}
			}
		}
		else if (P == 7)
		{
			Vector3 newForward2 = m_patrolPos[7] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 50)
				{
					P = 6;
				}
				if (ram > 50)
				{
					P = 8;
				}
			}
		}
		 if (P == 8)
		{
			Vector3 newForward2 = m_patrolPos[8] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 45)
				{
					P = 7;
				}
				if (ram > 45)
				{
					P = 0;
				}
				if (ram > 75)
				{
					P = 9;
				}
			}
		}
		 if (P == 9)
		{
			Vector3 newForward2 = m_patrolPos[9] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 50)
				{
					P = 8;
				}
				if (ram > 50)
				{
					P = 10;
				}
			}
		}
		 if (P == 10)
		{
			Vector3 newForward2 = m_patrolPos[10] - m_position;
			Vector3 distance2 = newForward2;
			newForward2.Normalize();
			m_forward = newForward2;
			Move();
			if (distance2.Length() <= 10.0f)
			{
				//Patrol = false;
				int ram = rand() % 100;
				if (ram < 50)
				{
					P = 9;
				}
				if (ram > 50)
				{
					P = 1;
				}
			}
		
		}
	}
	else
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
	case enNEutral_Enemy_Patrol:
		ProcessPatrolStateTransition();
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
	case enNEutral_Enemy_Patrol:
		m_modelRender.PlayAnimation(enNEutral_Enemy_Patrol, 0.5f);
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


	Vector3 diff = m_knightplayer->GetPosition() - m_position;

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


		//大きさを設定する。

		//座標を調整

		//エフェクト再生


		//効果音再生
		//攻撃の声
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(21);
		se->Play(false);
		se->SetVolume(0.5f);

		//効果音を再生する


	}
	//キーの名前がattack_endの時
	else if (wcscmp(eventName,L"Attack_end")==0){
		m_UnderAttack = false;
	
	}
}

const bool Neutral_Enemy::CanAttack()const
{
	//中立の敵からプレイヤーに向かうベクトルを計算する
	Vector3 diff = m_knightplayer->GetPosition() - m_position;
	//距離が近かったら
	if (diff.LengthSq() <= 50.0f * 50.0f)
	{
		//攻撃できる
		return true;
	}
	//攻撃できない
	return false;
}

void Neutral_Enemy::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
	if (m_knightplayer->GetSpriteFlag())
	{
		if (DrawHP())
		{
			m_HPBack.Draw(rc);
			m_HPBar.Draw(rc);
			m_HPFrame.Draw(rc);
		}
	}
	
}