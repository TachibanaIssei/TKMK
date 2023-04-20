#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Game.h"
#include "KnightUlt.h"

namespace
{
	const float MAX_CAMERA_TOP = -0.1f;		//カメラの上向きの最大値
	const float MAX_CAMERA_UNDER = 0.6f;	//カメラの下向きの最大値

	///注視点から視点までのベクトルを設定。
	const float CAMERA_POS_X = -160.0f;
	const float CAMERA_POS_Y = 80.0f;

	const float KNIGHT_CAMERA_POS_X = 50.0f;
	const float KNIGHT_CAMERA_POS_Y = 0.0f;

	const float KNIGHT_ULT_POS_X = -130.0f;
	const float KNIGHT_ULT_POS_Y = 100.0f;
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

	//注視点から視点までのベクトルを設定。80-160
	m_toCameraPos.Set(0.0f, 50.0f, -160.0f);
	//カメラをプレイヤーの後ろにするときに使う
	m_position = m_toCameraPos;

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);
	m_cameraCollisionSolver.Init(1.0f);
	m_cameraState = enGameState;

	//最初にキャラの背中を映すようにする
	CameraTarget(CAMERA_POS_X, CAMERA_POS_Y);

	return true;
}

void GameCamera::Update()
{
	//ポーズステートのときは処理をしない
	if (m_cameraState == enPauseState) {
		return;
	}


	//もしプレイヤーが必殺技を打ったら(7=必殺技ステート)
	if (player->CharGetState() == 7/*&& KnightUltMoveFlag==false*/)
	{
		KnightUltMoveFlag = true;
		knightUlt = FindGO<KnightUlt>("knightUlt");
	}

	//必殺技フラグが立ったら
	if (KnightUltMoveFlag)
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer < 1) {
			KnightUltCamera();
		}
		//knightUltが生成されている間
		else if (knightUlt->IsActive()==true)
		{
			//カメラがエフェクトを追うようにする
			ChaseUltEff();
		}
		else 
		{
			//全てリセット
			KnightUltMoveFlag = false;
			SetCameraCharFrontFlag = false;
			m_timer = 0;
			CameraTarget(CAMERA_POS_X, CAMERA_POS_Y);
		}
		
		return;
	}


	//Yボタンが押されたら
	//カメラの視点を最初の状態に戻す
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		CameraTarget(CAMERA_POS_X, CAMERA_POS_Y);
	}
	//何も押されていないなら
	else
	{
		//カメラの視点を設定
		FollowThePlayer();
	}

}

void GameCamera::FollowThePlayer()
{
	////注視点の計算
	//Vector3 TargetPos;
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

/// <summary>
/// カメラの視点をプレイヤーの背中を捉えるものに変更する
/// </summary>
void GameCamera::CameraTarget(float X, float Y)
{
	//プレイヤーの前方向を取得
	Vector3 toCameraPosXZ = player->CharSetForward();
	//移動していないなら抜け出す
	if (toCameraPosXZ.x == 0.0f && toCameraPosXZ.y == 0.0f)
	{
		return;
	}
	//正規化
	toCameraPosXZ.Normalize();
	//注視点から視点に向かうベクトルのXZ方向をかける
	toCameraPosXZ *= (X);
	//注視点から視点に向かうベクトルのY方向の設定
	Vector3 toCameraPosY=Vector3::AxisY;
	//注視点から視点に向かうベクトルのY方向をかける
	toCameraPosY.y *= Y;

	//視点を計算する
	Vector3 newCameraPos = toCameraPosXZ + toCameraPosY;

	//視点(m_toCameraPos)を書き換える
	m_toCameraPos.Set(newCameraPos);

	//注視点の計算
	Vector3 TargetPos;
	TargetPos = player->GetCharPosition();
	TargetPos.y += 40.0f;
	//視点から注視点へのベクトルを求める
	newCameraPos += TargetPos;

	//カメラの位置の衝突解決する
	Vector3 newCamPos;
	m_cameraCollisionSolver.Execute(
		newCamPos,
		newCameraPos,
		TargetPos
	);

	//視点と注視点を設定
	g_camera3D->SetTarget(TargetPos);
	g_camera3D->SetPosition(newCamPos);

	//カメラの更新。
	g_camera3D->Update();
}

/// <summary>
/// 剣士が必殺技を打った時のカメラワーク
/// </summary>
void GameCamera::KnightUltCamera()
{
	//カメラを剣士の正面にセットしていないなら
	if (SetCameraCharFrontFlag == false)
	{
		CameraTarget(KNIGHT_CAMERA_POS_X, KNIGHT_CAMERA_POS_Y);
		SetCameraCharFrontFlag = true;
	}
	//剣士に合わせてカメラを回転させる
	else
	{
		////注視点の計算
	//Vector3 TargetPos;
		TargetPos = player->GetCharPosition();

		TargetPos.y += 60.0f;

		Vector3 toCameraPosOld = m_toCameraPos;

		//パッドの入力を使ってカメラを回す。
		float x = 2.8f;
		float y = 0.3f;

		//Y軸周りの回転
		Quaternion qRot;
		qRot.SetRotationDeg(Vector3::AxisY, x);
		qRot.Apply(m_toCameraPos);

		//X軸周りの回転。
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, m_toCameraPos);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, -1.3f * y);
		qRot.Apply(m_toCameraPos);

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
}

void GameCamera::ChaseUltEff()
{
	//プレイヤーの前方向を取得
	Vector3 toCameraPosXZ = knightUlt->GetForward();
	//正規化
	toCameraPosXZ.Normalize();
	//注視点から視点に向かうベクトルのXZ方向をかける
	toCameraPosXZ *= (KNIGHT_ULT_POS_X);
	//注視点から視点に向かうベクトルのY方向の設定
	Vector3 toCameraPosY = Vector3::AxisY;
	//注視点から視点に向かうベクトルのY方向をかける
	toCameraPosY.y *= KNIGHT_ULT_POS_Y;

	//視点を計算する
	Vector3 newCameraPos = toCameraPosXZ + toCameraPosY;

	//視点(m_toCameraPos)を書き換える
	m_toCameraPos.Set(newCameraPos);

	//注視点の計算
	Vector3 TargetPos;
	TargetPos = knightUlt->GetPosition();
	TargetPos.y += 10.0f;
	//視点から注視点へのベクトルを求める
	newCameraPos += TargetPos;

	//視点と注視点を設定
	g_camera3D->SetTarget(TargetPos);
	g_camera3D->SetPosition(newCameraPos);

	//カメラの更新。
	g_camera3D->Update();



}

