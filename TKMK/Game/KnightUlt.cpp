#include "stdafx.h"
#include "KnightUlt.h"

namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 120.0f, 15.0f);
}

KnightUlt::KnightUlt()
{

}

KnightUlt::~KnightUlt()
{
	DeleteGO(UltCollision);
}

bool KnightUlt::Start()
{
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);

	m_position += m_moveSpeed * 20.0f;
	m_moveSpeed *= 200.0f;
	m_rotation.AddRotationDegY(360.0f);

	SetCollision(CollsionSize);

	

	return true;
}

void KnightUlt::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	//コリジョンオブジェクトに座標を設定する。
	UltCollision->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 4.0f) {
		//自身を削除する。
		DeleteGO(this);
	}

	UltCollision->Update();
}