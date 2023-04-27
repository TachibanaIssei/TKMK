#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "KnightBase.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "KnightUlt.h"
#include "Actor.h"

namespace
{
	const float MAX_CAMERA_TOP = -0.1f;		//カメラの上向きの最大値
	const float MAX_CAMERA_UNDER = 0.6f;	//カメラの下向きの最大値

	const float TARGETPOS_YUP = 45.0f;
	const float TARGETPOS_ULT_YUP = 50.0f;

	///注視点から視点までのベクトルを設定。
	const float CAMERA_POS_X = -160.0f;
	const float CAMERA_POS_Y = 80.0f;

	const float KNIGHT_CAMERA_POS_X = 80.0f;
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
	player_name = player->GetName();

	m_actors = game->GetActors();
	for (auto player : m_actors)
	{
		if (player->IsMatchName(player_name) == true)
		{
			player_actor = player;
		}
	}

	//注視点から視点までのベクトルを設定。80-160
	m_toCameraPos.Set(0.0f, 50.0f, -160.0f);
	//カメラをプレイヤーの後ろにするときに使う
	m_position = m_toCameraPos;

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);
	m_cameraCollisionSolver.Init(1.0f);
	m_cameraState = enGameState;

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,			//カメラの移動速度の最大値。
		false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	
	//最初にキャラの背中を映すようにする
	CameraTarget(CAMERA_POS_X, CAMERA_POS_Y);
	m_springCamera.Refresh();

	return true;
}

void GameCamera::Update()
{
	//ポーズステートのときは処理をしない
	if (m_cameraState == enPauseState) {
		return;
	}

	StateControl();
}

void GameCamera::StateControl()
{
	switch (m_enCameraState)
	{
	case m_enNomarlCameraState:
		NomarlCamera();
		break;
	case m_enUltRotCameraState:
		UltRotCamera();
		break;
	case m_enChaseCameraState:
		ChaseCamera();
		break;
	default:
		break;
	}
}

void GameCamera::NomarlCamera()
{
	for (auto actor : m_actors) {
		//もしプレイヤーが必殺技を打ったら(7=必殺技ステート)
		if (actor->NowCharState() == Actor::enCharState_UltimateSkill && KnightUltFlag==false)
		{
			KnightUltFlag = true;
			//カメラステートを回転ステートに移る
			m_enCameraState = m_enUltRotCameraState;
			ultactor = actor;

			return;
		}
	}


	//Yボタンが押されたら
	//カメラの視点を最初の状態に戻す
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, player_actor);
	}
	//何も押されていないなら
	else
	{
		//カメラの視点を設定
		FollowThePlayer();
	}


}

void GameCamera::UltRotCamera()
{
	//カメラを剣士の正面にセットしていないなら
	if (SetCameraCharFrontFlag == false)
	{
		m_springCamera.Refresh();
		CameraTarget(KNIGHT_CAMERA_POS_X, KNIGHT_CAMERA_POS_Y, ultactor);
		//キャラの正面からカメラに向かうベクトル
		m_keepDiff = m_toCameraPos;

		SetCameraCharFrontFlag = true;

		KnightUltCamera(ultactor,true);
	}

	//プレイヤーからカメラに向かうベクトルを計算する
	Vector3 start = m_keepDiff;
	Vector3 end = m_CameraFromActorDiff;
	start.y = 0.0f;
	end.y = 0.0f;
	//内積
	float nai;
	nai = start.Dot(end);
	//成す角
	float Start = start.Length();
	float End = end.Length();

	float cos_sita = nai / (Start * End);
	sita = acos(cos_sita);
	sita = Math::RadToDeg(sita);

	if (sita < 175.0f) {
		KnightUltCamera(ultactor,false);
	}
	else
	{
		m_springCamera.Refresh();
		m_enCameraState = m_enChaseCameraState;
	}


}

void GameCamera::ChaseCamera()
{
	knightUlt = FindGO<KnightUlt>("knightUlt");
	//knightUltが生成されている間
	    if (knightUlt != nullptr)
		{
			//カメラがエフェクトを追うようにする
			ChaseUltEff();
		}
		else
		{
			//m_springCamera.Refresh();
			////全てリセット
			//SetCameraCharFrontFlag = false;
			//KnightUltFlag = false;

			////必殺技を撃ったActorのステートを戻す
			////ultactor->UltSkillEnd();

			////CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, ultactor);
			////プレイヤーのカメラをリセットする
			//CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, player_actor);

			//m_enCameraState = m_enNomarlCameraState;
		
		}
}

void GameCamera::FollowThePlayer()
{
	////注視点の計算
	//Vector3 TargetPos;
	TargetPos = player->GetCharPosition();

	TargetPos.y += TARGETPOS_YUP;

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
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCamPos);

	//カメラの更新。
	m_springCamera.Update();
}

/// <summary>
/// カメラの視点をプレイヤーの背中を捉えるものに変更する
/// </summary>
void GameCamera::CameraTarget(float X, float Y,Actor*actor)
{

	//プレイヤーの前方向を取得
	Vector3 toCameraPosXZ = actor->GetForward();
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
	TargetPos = actor->GetPosition();
	TargetPos.y += TARGETPOS_YUP;
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
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCamPos);

	//カメラの更新。
	m_springCamera.Update();
}

/// <summary>
/// 剣士が必殺技を打った時のカメラワーク　　回りながら遠ざけたい
/// </summary>
void GameCamera::KnightUltCamera(Actor*actor, bool reset)
{
	
	//剣士に合わせてカメラを回転させる
		
		////注視点の計算
	//Vector3 TargetPos;
		TargetPos = actor->GetPosition();

		TargetPos.y += TARGETPOS_ULT_YUP;

		Vector3 toCameraPosOld = m_toCameraPos;

		//パッドの入力を使ってカメラを回す。x2.8
		float x = 1.9f;
		float y = 0.1f;

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

		//m_toCameraPos.z -= 2.0f;

		//カメラの位置の衝突解決する
		Vector3 newCamPos;
		m_cameraCollisionSolver.Execute(
			newCamPos,
			TargetPos + m_toCameraPos,
			TargetPos
		);

		//アクターからカメラに向かうベクトル
		m_CameraFromActorDiff = newCamPos - TargetPos;

		if (reset == false) {
			//視点と注視点を設定
			m_springCamera.SetTarget(TargetPos);
			m_springCamera.SetPosition(newCamPos);

			//カメラの更新。
			m_springCamera.Update();

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
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCameraPos);

	//カメラの更新。
	m_springCamera.Update();



}

void GameCamera::GameCameraUltEnd() {

	m_springCamera.Refresh();
	//全てリセット
	SetCameraCharFrontFlag = false;
	KnightUltFlag = false;

	//必殺技を撃ったActorのステートを戻す
	//ultactor->UltSkillEnd();

	//CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, ultactor);
	//プレイヤーのカメラをリセットする
	CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, player_actor);

	m_enCameraState = m_enNomarlCameraState;

}