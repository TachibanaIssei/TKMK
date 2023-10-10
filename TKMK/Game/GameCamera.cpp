#include "stdafx.h"
#include "GameCamera.h"

#include "Game.h"
#include "Actor.h"
#include "KnightBase.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "KnightUlt.h"
#include "WizardUlt.h"
#include "KnightAI.h"

namespace
{
	const float MAX_CAMERA_TOP = -0.1f;		//カメラの上向きの最大値
	const float MAX_CAMERA_UNDER = 0.6f;	//カメラの下向きの最大値

	const float TARGETPOS_YUP = 45.0f;
	

	///注視点から視点までのベクトルを設定。
	const float CAMERA_POS_X = -160.0f;
	const float CAMERA_POS_Y = 80.0f;

	const float KNIGHT_CAMERA_POS_X = 120.0f;
	const float KNIGHT_CAMERA_POS_Y = 10.0f;

	const float KNIGHT_ULT_POS_X = -90.0f;
	const float KNIGHT_ULT_POS_Y = 40.0f;


	//雷を落とす
	const float KNIGHT_TUNDER_POS_X = 90.0f;
	const float KNIGHT_TUNDER_POS_Y = -60.0f;
	const float TARGETPOS_ULT_YUP = 80.0f;

	const float SETSHAKEPOWER = 2.0f;
	const float SETSHAKEPOWERFORAI = 1.0f;
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

	//スプリングカメラ初期化時に使用するカメラの番号
	int cameraNumber = 0;
	float targetToPos = CAMERA_POS_X;

	//プレイヤー1を映すカメラ
	if (m_splitCameraDraw == enSplitCamera_Solo || m_splitCameraDraw == enSplitCamera_Left || m_splitCameraDraw == enSplitCamera_LeftUp)
	{
		//プレイヤーのインスタンスを探す
		player = FindGO<Player>("player");
		player_name = player->GetName();
		m_playerNumber = enPlayerNumber_1P;
	}
	//プレイヤー2を映すカメラ
	else if(m_splitCameraDraw == enSplitCamera_Right || m_splitCameraDraw == enSplitCamera_RightUp)
	{
		player = FindGO<Player>("player2");
		player_name = player->GetName();
		cameraNumber = 1;
		m_playerNumber = enPlayerNumber_2P;
	}
	//プレイヤー3を映すカメラ
	else if (m_splitCameraDraw == enSplitCamera_LeftDown)
	{
		player = FindGO<Player>("player3");
		player_name = player->GetName();
		cameraNumber = 2;
		m_playerNumber = enPlayerNumber_3P;
	}
	//プレイヤー4を映すカメラ
	else
	{
		if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay)
		{
			player_name = "KnightAI1";
			cameraNumber = 3;
			m_playerNumber = enPlayerNumber_4P;
		}
		else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
		{
			player = FindGO<Player>("player4");
			player_name = player->GetName();
			cameraNumber = 3;
			m_playerNumber = enPlayerNumber_4P;
		}
	}

	m_actors = game->GetActors();
	for (auto actor : m_actors)
	{
		if (actor->IsMatchName(player_name) == true)
		{
			player_actor = actor;
		}
	}

	//揺れる力を初期化する
	shakePower = SETSHAKEPOWER;

	//注視点から視点までのベクトルを設定。80-160
	m_toCameraPos.Set(0.0f, 50.0f, -160.0f);
	//カメラをプレイヤーの後ろにするときに使う
	m_position = m_toCameraPos;
	m_cameraCollisionSolver.Init(1.0f);
	m_cameraState = enGameState;

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D[cameraNumber],		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,						//カメラの移動速度の最大値。
		false,							//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		1.0f							//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	//最初にキャラの背中を映すようにする
	CameraTarget(TARGETPOS_YUP, -CAMERA_POS_X, CAMERA_POS_Y, player_actor,true);
	m_springCamera.Refresh();

	return true;
}

void GameCamera::Update()
{
	//ポーズステートのときは処理をしない
	if (m_cameraState == enPauseState) {
		return;
	}

	//プレイヤーがやられてリスポーンするまでカウントダウンの処理をしているなら
	if (player_actor->GetRespawnFlag()==true)
	{
		m_springCamera.Refresh();
		//前フレーム
		PlayerRespawnFlag = true;

		return;
	}

	//リスポーンしたらカメラを戻す
	if (player_actor->GetRespawnFlag() == false && PlayerRespawnFlag==true) {
		CameraTarget(TARGETPOS_YUP ,-CAMERA_POS_X, CAMERA_POS_Y, player_actor,true);

		m_springCamera.Refresh();
		//カメラの更新。
		m_springCamera.Update();

		PlayerRespawnFlag = false;

	}

	//もしAIの必殺技が当たったらカメラを揺らす
	if (GetPlayerShakeFlag() == true) {
		wizardUlt = FindGO<WizardUlt>("wizardUlt");
		if (wizardUlt == nullptr)
		{
			//揺れる力を初期化する
			shakePower = SETSHAKEPOWER;
			SetPlayerShakeFlag(false);
		}
		else if (wizardUlt->GetFallTunderFlag() && wizardUlt->GetCameraShakeFlag()) {
			ThunderCameraShakeForPlayer();
			return;
		}
		
	}

	//カメラステート管理
	StateControl();
}

void GameCamera::StateControl()
{
	switch (m_enCameraState)
	{
	case enNormalCameraState:
		NomarlCamera();
		break;
	case enUltRotCameraState:
		UltRotCamera();
		break;
	case enChaseCameraState:
		ChaseCamera();
		break;
	default:
		break;
	}
}

void GameCamera::NomarlCamera()
{
		for (auto actor : m_actors) {
			//AIが打った後に終わるまではこの処理をしないようにする
			//もしアクターが必殺技を打ったら
			if (actor->GetmUseUltimaitSkillFlag() == true)
			{
				break;
			}

			if (actor->NowCharState() == Actor::enCharState_UltimateSkill && KnightUltFlag == false)
			{
				//溜めフラグをfalseにする
				UltChargeFlag = false;
				//必殺技フラグをたてる
				KnightUltFlag = true;
				//カメラステートを回転ステートに移る
				m_enCameraState = enUltRotCameraState;

				ultactor = actor;

				//gameにターゲットのみを映すようにするよう伝える
				game->ToggleObjectActive(true, ultactor);
			}
		}

	//for (auto actor : m_actors) {
	//	//もしプレイヤーが必殺技を打ったら(7=必殺技ステート)
	//	//剣士用
	//	//if (actor->NowCharState() == Actor::enCharState_UltimateSkill && KnightUltFlag==false)
	//	//{
	//	//	KnightUltFlag = true;
	//	//	//カメラステートを回転ステートに移る
	//	//	m_enCameraState = enUltRotCameraState;
	//	//	ultactor = actor;

	//	//	return;
	//	//}
	//}

	//プレイヤーだけ溜めのところも正面から見せる
	//もしプレイヤーが必殺技の溜めを使っていたら
	if (player_actor->NowCharState() == Actor::enCharState_Ult_liberation)
	{
		UltChargeFlag = true;
		m_springCamera.Refresh();
		CameraTarget(TARGETPOS_YUP,KNIGHT_CAMERA_POS_X, KNIGHT_CAMERA_POS_Y, player_actor,true);
		return;
	}
	//もし溜めている間に攻撃されたらカメラをリセット
	else if (UltChargeFlag == true && player_actor->NowCharState() != Actor::enCharState_Ult_liberation)
	{
		UltChargeFlag = false;
		m_springCamera.Refresh();
		CameraTarget(TARGETPOS_YUP,CAMERA_POS_X, CAMERA_POS_Y, player_actor,true);
	}

	//Yボタンが押されたら
	//カメラの視点を最初の状態に戻す
	if (g_pad[m_playerNumber]->IsTrigger(enButtonRB3))
	{
		CameraTarget(TARGETPOS_YUP,CAMERA_POS_X, CAMERA_POS_Y, player_actor,true);
	}
	//何も押されていないなら
	else
	{
		//カメラの視点を設定
		FollowThePlayer();
		//剣士が三段目攻撃をしたらカメラを揺らす
		if (m_cameraShakeFlag == true)
		{
			//CameraShakeを2回実行したら終わり
			CameraShake(m_ShakeMoveFlag);
			//揺れる方向を変える
			if (m_ShakeMoveFlag == false) {
				//反転
				m_ShakeMoveFlag = !m_ShakeMoveFlag;
			}
			//true(2回揺れた)なら
			else {
				//カメラを揺らす処理を止める
				m_cameraShakeFlag = false;
			}
		}
	}


}

//剣士の回りを回る
//雷に打たれていないキャラを探す
void GameCamera::UltRotCamera()
{
	//プレイヤーの場合はこの処理だけする
	//一回だけの処理
	if (SetCameraCharFrontFlag == false) {
		//プレイヤーを下からの見上げるようにする
		m_springCamera.Refresh();
		CameraTarget(TARGETPOS_ULT_YUP,KNIGHT_TUNDER_POS_X, KNIGHT_TUNDER_POS_Y, ultactor,false);

		SetCameraCharFrontFlag = true;
		//揺れる力に掛ける値を決める
		setShakeMulPower(ultactor->GetLevel());
	}
	//必殺技打ったキャラが自分じゃないとき
	if (!ultactor->IsMatchName(player_actor->GetName()))
	{
		//誰もいなかったら視点を戻す
		if (ultactor->GetNoTargetActor() == true)
		{
			GameCameraUltEnd();
		}
		if (ultactor->GetChaseCameraFlag() == true) {
			return;
		}
		else if (ultactor->GetChaseCameraFlag() == false) {
			//必殺技フラグをfalseにする
			KnightUltFlag = false;
			SetCameraCharFrontFlag = false;
			m_springCamera.Refresh();
			CameraTarget(TARGETPOS_YUP,CAMERA_POS_X, CAMERA_POS_Y, player_actor,true);
			m_enCameraState = enNormalCameraState;
			return;
		}
		
		return;
	}
	
	//誰かを見ている間は処理をしない
	if (TunderCameraFlag == false)
	{
		for (auto actor : player_actor->GetDamegeUltActor())
		{
			{
				//雷を打たれているキャラにカメラを向けるフラグ
				TunderCameraFlag = true;
				//カメラで見る対象のキャラ
				victim_actor = actor;

				//gameにターゲットのみを映すようにするよう伝える
				game->ToggleObjectActive(true, victim_actor);

				wizardUlt = FindGO<WizardUlt>("wizardUlt");
				//見るキャラがきまったら抜け出す
				m_enCameraState = enChaseCameraState;
				//揺れる力を初期化する
				shakePower = SETSHAKEPOWERFORAI;
				return;
			}
			return;
		}
	}
	
	//誰もいなかったら視点を戻す
	if (player_actor->GetNoTargetActor() == true)
		GameCameraUltEnd();


	

	//カメラを剣士の正面にセットしていないなら
	//if (SetCameraCharFrontFlag == false)
	//{
	//	m_springCamera.Refresh();
	//	CameraTarget(KNIGHT_CAMERA_POS_X, KNIGHT_CAMERA_POS_Y, ultactor);
	//	//キャラの正面からカメラに向かうベクトル
	//	m_keepDiff = m_toCameraPos;

	//	SetCameraCharFrontFlag = true;

	//	KnightUltCamera(ultactor,true);
	//}

	////プレイヤーからカメラに向かうベクトルを計算する
	//Vector3 start = m_keepDiff;
	//Vector3 end = m_CameraFromActorDiff;
	//start.y = 0.0f;
	//end.y = 0.0f;
	////内積
	//float nai;
	//nai = start.Dot(end);
	////成す角
	//float Start = start.Length();
	//float End = end.Length();

	//float cos_sita = nai / (Start * End);
	//sita = acos(cos_sita);
	//sita = Math::RadToDeg(sita);

	//knightUlt = FindGO<KnightUlt>("knightUlt");
	//if (knightUlt != nullptr)
	//{
	//	m_springCamera.Refresh();
	//	m_enCameraState = enChaseCameraState;
	//}
	//else if (sita < 175.0f) {
	//	KnightUltCamera(ultactor,false);
	//}
	//else
	//{
	//	m_springCamera.Refresh();
	//	m_enCameraState = enChaseCameraState;
	//}


}

void GameCamera::ChaseCamera()
{
	wizardUlt = FindGO<WizardUlt>("wizardUlt");

	if (wizardUlt != nullptr)
	{
		//雷に打たれているなら
		if (TunderCameraFlag == true) {
			//雷に打たれているキャラの正面にカメラを合わせる
			m_springCamera.Refresh();
			CameraTarget(TARGETPOS_YUP,KNIGHT_TUNDER_POS_X, 0, victim_actor,false);

			if(wizardUlt->GetFallTunderFlag() && wizardUlt->GetCameraShakeFlag())
			ThunderCameraShake();
			return;
		}
	}
	

	//knightUlt = FindGO<KnightUlt>("knightUlt");
	////knightUltが生成されている間
	//    if (knightUlt != nullptr)
	//	{
	//		//カメラがエフェクトを追うようにする
	//		ChaseUltEff();
	//	}
	//	else
	//	{
	//		
	//	}
}

void GameCamera::FollowThePlayer()
{
	//注視点の計算
	TargetPos = player_actor->GetPosition();

	TargetPos.y += TARGETPOS_YUP;

	Vector3 toCameraPosOld = m_toCameraPos;

	//パッドの入力を使ってカメラを回す。
	float x = g_pad[m_playerNumber]->GetRStickXF();
	float y = g_pad[m_playerNumber]->GetRStickYF();

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
void GameCamera::CameraTarget(float targrtYUp, float X, float Y,Actor*actor,bool cameraCollisionSolverFlag)
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
	/*Vector3 TargetPos;*/
	TargetPos = actor->GetPosition();
	TargetPos.y += targrtYUp;
	//視点から注視点へのベクトルを求める
	newCameraPos += TargetPos;


	if (cameraCollisionSolverFlag == true)
	{
		//カメラの位置の衝突解決する
	//Vector3 newCamPos;
		m_cameraCollisionSolver.Execute(
			newCamPos,
			newCameraPos,
			TargetPos
		);
	}
	else
	{
		newCamPos = newCameraPos;
	}
	

	

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
		float x = 2.1f;
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
		//Vector3 newCamPos;
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
	//必殺技エフェクトの前方向を取得
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

//カメラを揺らす処理
void GameCamera::CameraShake(bool UpDown)
{
	if (UpDown == false) {
		TargetPos.y += 10.0f;
	}
	else {
		TargetPos.y -= 10.0f;
	}
	
	//視点と注視点を設定
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCamPos);
	m_springCamera.Update();
}

void GameCamera::ThunderCameraShake()
{
	if (ShakeLeftAndLightFlag == false) {
		TargetPos.x += shakePower/2.0f;
		TargetPos.y -= shakePower/2.0f;
		ShakeLeftAndLightFlag = true;
	}
	else {
		TargetPos.x -= shakePower/2.0f;
		TargetPos.y += shakePower/2.0f;
		ShakeLeftAndLightFlag = false;
	}

	
	shakePower *= shakeMulPower;

	//視点と注視点を設定
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCamPos);
	m_springCamera.Update();
}

void GameCamera::ThunderCameraShakeForPlayer()
{


	if (ShakeLeftAndLightFlag == false) {
		newCamPos.y += shakePower;
		TargetPos.y += shakePower;
		ShakeLeftAndLightFlag = true;
	}
	else {
		newCamPos.y -= shakePower;
		TargetPos.y -= shakePower;
		ShakeLeftAndLightFlag = false;
	}

	shakePower *= shakeMulPower*1.4f;

	//視点と注視点を設定
	m_springCamera.SetTarget(TargetPos);
	m_springCamera.SetPosition(newCamPos);
	m_springCamera.Update();
}

void GameCamera::GameCameraUltEnd() {
	//全てリセット
	m_springCamera.Refresh();
	//プレイヤーの正面にカメラを移動するフラグ
	SetCameraCharFrontFlag = false;
	//プレイヤーが必殺技を打ったことを示すフラグ
	KnightUltFlag = false;

	//必殺技を撃ったActorのステートを戻す
	//ultactor->UltSkillEnd();

	//全キャラのカメラで見たかのフラグをfalseにする
	/*for (auto actor : m_game->GetActors())
	{
		actor->ChangeCameraSawCharFlag(false);
	}*/

	//CameraTarget(CAMERA_POS_X, CAMERA_POS_Y, ultactor);
	//プレイヤーのカメラをリセットする
	CameraTarget(TARGETPOS_YUP,CAMERA_POS_X, CAMERA_POS_Y, player_actor,true);

	//gameにターゲットのみを映すようにするよう伝える
	game->ToggleObjectActive(false, victim_actor);

	m_enCameraState = enNormalCameraState;

}

void GameCamera::setShakeMulPower(int revel)
{
	if (revel < 5) {
		shakeMulPower = 1.2f;
	}
	else if (revel < 9) {
		shakeMulPower = 1.3f;
	}
	else if (revel == 10) {
		shakeMulPower = 1.4f;
	}
}
