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

	m_scale = Vector3(0.3f,0.3f, 0.3f);
	//座標を設定
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rot);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisY;
	m_rot.Apply(m_forward);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
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

}

void Neutral_Enemy::Collision()
{
	if (m_NEState == enNEState_ReceiveDamage || m_NEState == enNEState_Death)
	{
		return;
	}
	//敵の攻撃用のコリジョンを取得する
	//const auto& collisions = g_collisionObjectManager->FindCollisionObject();

}

void Neutral_Enemy::Attack()
{
	//攻撃ステートではなかったら
	if (m_NEState != enNEState_Attack)
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
	return true;
}

void Neutral_Enemy::MakeAttackCollision()
{

}
void Neutral_Enemy::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//敵を見つかったら攻撃
	if (SearchEnemy()==true)
	{

		Vector3 diff = m_knightPlayer->GetPosition() - m_position;
		diff.Normalize();
		m_moveSpeed = diff;
		m_NEState = enNEState_Chase;
		//攻撃できる距離なら
		if (CanAttack() == true)
		{
			int ram = rand() % 100;
			if (ram > 70)
			{
				//攻撃ステートに移行する
				m_NEState = enNEState_Attack;
				m_UnderAttack = false;
				return;
			}
			else
			{
				m_NEState = enNEState_Idle;
				return;
			}
		}
		else
		{
			//乱数によって、追跡させる
			int ram = rand() % 100;
			if (ram > 30)
			{
				m_NEState = enNEState_Chase;
				return;
			}
		}

	}
	//敵を見つけられなければ。
	else
	{
		//待機ステートに移行する。
		m_NEState = enNEState_Idle;
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
		m_NEState = enNEState_Chase;
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
	switch (m_NEState)
	{
		//待機ステート
	case enNEState_Idle:
		ProcessIdleStateTransition();
		break;
		//追跡ステート
	case enNEState_Chase:
		ProcessChaseStateTransition();
		break;
		//攻撃ステート
	case enNEState_Attack:
		ProcessAttackStateTransition();
		break;
		//被ダメージステート
	case enNEState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//死亡ステート
	case enNEState_Death:
		ProcessDeathStateTransition();
		break;
	}
}

void Neutral_Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.5f);
	switch(m_NEState)
	{
		//待機ステート
	case enNEState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステート
	case enNEState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
		//攻撃ステート
	case enNEState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		break;
		//被ダメージステート
	case enNEState_ReceiveDamage:
		m_modelRender.PlayAnimation(enNEState_ReceiveDamage, 0.5f);
		break;
		//死亡ステート
	case enNEState_Death:
		m_modelRender.PlayAnimation(enNEState_Death, 0.5f);
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
	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//攻撃できる
		return true;
	}
	//攻撃できない
	return false;
}

void Neutral_Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}