#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Game.h"

namespace
{
	const float MAX_CAMERA_TOP = -0.1f;		//カメラの上向きの最大値
	const float MAX_CAMERA_UNDER = 0.6f;	//カメラの下向きの最大値
}

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//ゲームのインスタンスを探す
	game = FindGO<Game>("game");
	//プレイヤーのインスタンスを探す
	player = FindGO<Player>("player");

	//
	/*wizardPlayer = FindGO<WizardPlayer>("wizardPlayer");
	m_knightplayer= FindGO<KnightPlayer>("m_knightplayer");*/

	//注視点から視点までのベクトルを設定。80-160
	m_toCameraPos.Set(0.0f, 80.0f, -160.0f);
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	m_cameraCollisionSolver.Init(1.0f);

	m_cameraState = enGameState;

	return true;
}

void GameCamera::Update()
{
	//ポーズステートのときは処理をしない
	if (m_cameraState == enPauseState) {
		return;
	}

	//注視点の計算
	Vector3 TargetPos;
	//TargetPos = m_knightplayer->GetPosition();
	//TargetPos = wizardPlayer->GetPosition();
	TargetPos = player->GetCharPosition();

	TargetPos.y += 40.0f;

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
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < MAX_CAMERA_TOP) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > MAX_CAMERA_UNDER) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	//カメラの位置の衝突解決する
	Vector3 newCamPos;
	m_cameraCollisionSolver.Execute(
		newCamPos,
		TargetPos + m_toCameraPos,
		TargetPos
	);

	//視点と注視点を設定
	g_camera3D->SetTarget(TargetPos);
	g_camera3D->SetPosition(newCamPos);

	//カメラの更新。
	g_camera3D->Update();
}



