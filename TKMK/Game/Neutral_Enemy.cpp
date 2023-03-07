#include "stdafx.h"
#include "Neutral_Enemy.h"
#include "Game.h"
#include "KnightBase.h"
#include <time.h>
#include <stdlib.h>

Neutral_Enemy::Neutral_Enemy()
{

}

Neutral_Enemy::~Neutral_Enemy()
{

}

bool Neutral_Enemy::Start()
{
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Neutral_Enemy/Idle.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Neutral_Enemy/Move.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Neutral_Enemy/Attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Neutral_Enemy/Death.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);

	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/Neutral_Enemy/Neutral_Enemy.tkm", m_animationClips, enAnimationClip_Num);
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

}

void Neutral_Enemy::Chase()
{

}

void Neutral_Enemy::Collision()
{

}

void Neutral_Enemy::Attack()
{

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
	////敵を見つかったら攻撃
	//if ()
	//{

	//}
	////敵を見つけられなければ。
	//else
	//{
	//	//待機ステートに移行する。
	//	m_NEState = enNEState_Idle;
	//	return;

	//}
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
		//Vector3 diff = m_player->GetPosition() - m_position;
		//diff.Normalize();
		//移動速度を設定する。
		//m_moveSpeed = diff * 100.0f;
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

}

void Neutral_Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.5f);
	switch(m_NEState)
	{
		//待機ステート
	case enNEState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		//追跡ステート
	case enNEState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		//攻撃ステート
	case enNEState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		//

	}
}

void Neutral_Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

const bool Neutral_Enemy::CanAttack()const
{
	return true;
}

void Neutral_Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}