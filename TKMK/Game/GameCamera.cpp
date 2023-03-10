#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{

	//注視点から視点までのベクトルを設定。80-160
	m_toCameraPos.Set(0.0f, 80.0f, -160.0f);
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}

void GameCamera::Update()
{
	Vector3 TargetPos;
	/// <summary>
	/// ////////////////////
	/// </summary>
	TargetPos = m_knightplayer->GetPosition();
	TargetPos.y += 80.0f;



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
	//if (toPosDir.y < -0.3f) {
	//	//カメラが上向きすぎ。
	//	m_toCameraPos = toCameraPosOld;
	//}
	//else if (toPosDir.y > 0.9f) {
	//	//カメラが下向きすぎ。
	//	m_toCameraPos = toCameraPosOld;
	//}

	Vector3 pos;
	//視点と注視点を足す
	pos = TargetPos + m_toCameraPos;
	g_camera3D->SetTarget(TargetPos);
	g_camera3D->SetPosition(pos);
	//g_camera3D->SetPosition(m_toCameraPos);

	//カメラの更新。
	g_camera3D->Update();
}



