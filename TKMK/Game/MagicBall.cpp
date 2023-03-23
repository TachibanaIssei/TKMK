#include "stdafx.h"
#include "MagicBall.h"

MagicBall::MagicBall()
{

}

MagicBall:: ~MagicBall()
{
	DeleteGO(BallCollision);
}

bool MagicBall::Start()
{
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);

	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 200.0f;
	m_rotation.AddRotationDegY(360.0f);

	//コリジョンオブジェクトを作成する。
	BallCollision = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	BallCollision->CreateSphere(m_position, Quaternion::Identity, 20.0f/*50.0f * m_scale.z*/);

	//術者がプレイヤーだったら。
	if (m_enMagician == enMagician_Player)
	{
		//名前をplayer_fireballにする。
		BallCollision->SetName("player_fireball");
	}
	//術者が魔法使いだったら。
	else if (m_enMagician == enMagician_Wizard)
	{
		//名前をenemy_fireballにする。
		BallCollision->SetName("enemy_fireball");
	}

	//懲り所オブジェクトが自動で削除されないようにする。
	BallCollision->SetIsEnableAutoDelete(false);

	return true;
}

void MagicBall::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	
	//コリジョンオブジェクトに座標を設定する。
	BallCollision->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 2.0f) {
		//自身を削除する。
		DeleteGO(this);
	}

	BallCollision->Update();
}

