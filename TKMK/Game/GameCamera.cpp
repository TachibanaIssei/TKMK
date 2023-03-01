#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_toCameraPos = { 0.0f,100.0f,0.0f };
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}

void GameCamera::Update()
{
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_toCameraPos.y += 20.0f;
	}

	if (g_pad[0]->IsPress(enButtonDown)) {
		m_toCameraPos.y -= 20.0f;
	}

	if (g_pad[0]->IsPress(enButtonRight)) {
		m_toCameraPos.x += 20.0f;
	}

	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_toCameraPos.x -= 20.0f;
	}

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * y);
	qRot.Apply(m_toCameraPos);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.3f) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	g_camera3D->SetPosition(m_toCameraPos);
	
	//カメラの更新。
	g_camera3D->Update();
}


