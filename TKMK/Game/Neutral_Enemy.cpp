#include "stdafx.h"
#include "Neutral_Enemy.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>
#include"KnightPlayer.h"

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
		10.0f,			//半径。
		10.2f,			//高さ。
		m_position		//座標。
	);
	//剣のボーンのIDを取得する
	m_AttackBoneId = m_modelRender.FindBoneID(L"HeadTipJoint");

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisY;
	m_rot.Apply(m_forward);
	//ステータスを読み込む
	m_Status.Init("Enemy");
	return true;
}

void Neutral_Enemy::Update()
{

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

	//モデルの更新。
	m_modelRender.Update();
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
	//m_targetPointPosition = m_knightPlayer->GetPosition();
	//bool isEnd;
	////if(){
	//	// パス検索
	//m_pathFiding.Execute(
	//	m_path,							// 構築されたパスの格納先
	//	m_nvmMesh,						// ナビメッシュ
	//	m_position,						// 開始座標
	//	m_targetPointPosition,			// 移動目標座標
	//	PhysicsWorld::GetInstance(),	// 物理エンジン	
	//	20.0f,							// AIエージェントの半径
	//	50.0f							// AIエージェントの高さ。
	//);
	////}
	//// パス上を移動する。
	//m_position = m_path.Move(
	//	m_position,
	//	3.0f,
	//	isEnd
	//);
	Vector3 pos = m_position;
	m_charaCon.SetPosition(pos);
	m_modelRender.SetPosition(pos);
}

void Neutral_Enemy::Collision()
{
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
			//hpを減らす

			//if (m_hp == 0)
			//{
			//	//死亡ステートに遷移する。
			//	m_Neutral_EnemyState = enNeutral_Enemy_Death;
			//}
			//else {
			//	//被ダメージステートに遷移する。
			//	m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
			//	//効果音再生
			//}
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

const bool Neutral_Enemy::SearchEnemy()const
{
	//剣士からエネミーに向かうベクトルを計算する。
	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
	//ボスとプレイヤーの距離がある程度近かったら。
	if (diff.LengthSq() <= 100.0 * 100.0f)
	{
		//エネミーからプレイヤーに向かうベクトルを正規化する。
		diff.Normalize();
		//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff);
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が180°より小さければ。
		if (angle <= (Math::PI / 180.0f) * 180.0f)
		{
			//プレイヤーを見つけた！
			return true;
		}
	}

	return false;
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
	if (SearchEnemy() == true)
	{
		Vector3 diff = m_knightPlayer->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 320.0f;
		//攻撃できる距離なら。
		if (CanAttack() == true)
		{
			//乱数によって、攻撃するか待機させるかを決定する。	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//攻撃ステートに移行する。
				m_Neutral_EnemyState = enNeutral_Enemy_Attack;
				m_UnderAttack = false;
				return;
			}
			else
			{
				//待機ステートに移行する。
				m_Neutral_EnemyState = enNeutral_Enemy_Idle;
				return;
			}

		}
		//攻撃できない距離なら。
		else
		{
			//乱数によって、追跡させる
			int ram = rand() % 100;
			if (ram > 40)
			{
				//追跡ステートに移行する。
				m_Neutral_EnemyState = enNeutral_Enemy_Chase;
				return;
			}
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに移行する。
		m_Neutral_EnemyState = enNeutral_Enemy_Idle;
		return;

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
		Vector3 diff = m_knightPlayer->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 100.0f;
	}
}

void Neutral_Enemy::ProcessDeathStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		
		//自身を削除する。
		DeleteGO(this);
	}
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
	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
	//距離が近かったら
	if (diff.LengthSq() <= 75.0f * 75.0f)
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
}