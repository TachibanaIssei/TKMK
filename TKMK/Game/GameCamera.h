#pragma once
#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class KnightPlayer;
class WizardPlayer;
class Player;
class Game;
class KnightUlt;
class Actor;
class WizardUlt;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void FollowThePlayer();
	void StateControl();
	void NomarlCamera();
	void UltRotCamera();
	void ChaseCamera();

	enum EnCameraState {
		m_enNomarlCameraState,
		m_enUltRotCameraState,
		m_enChaseCameraState
	};
	EnCameraState  m_enCameraState = m_enNomarlCameraState;
	

	/// <summary>
	/// カメラの視点を最初の状態に戻す
	/// </summary>
	void CameraTarget(float X,float Y,Actor*actor);

	/// <summary>
	/// 剣士が必殺技を打った時のカメラワーク
	/// </summary>
	void KnightUltCamera(Actor* actor, bool reset);

	/// <summary>
	/// 剣士の斬撃エフェクトを追いかける
	/// </summary>
	void ChaseUltEff();

	/// <summary>
	/// カメラを揺らす
	/// </summary>
	void CameraShake(bool UpDown);

	void ChangeCameraShakeFlag(bool flag)
	{
		m_cameraShakeFlag = flag;
	}

	//カメラのステート
	enum CameraState
	{
		enGameState,
		enPauseState,
	};
	CameraState m_cameraState;

	void SetKnight(KnightPlayer* knightplayer)
	{
		m_knightplayer = knightplayer;
	}

	/// <summary>
	/// カメラのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetCameraState(CameraState gamescene) {
		m_cameraState = gamescene;

	}

	void ChangeMoveCameraState(EnCameraState state) {
		m_enCameraState = state;
	}

	/// <summary>
	/// 剣士の必殺技のインスタンスを代入する
	/// </summary>
	/// <param name="ultobj">剣士の必殺技クラス</param>
	void SetKnightUlt(KnightUlt* ultobj)
	{
		knightUlt = ultobj;
	}

	void ChangeTunderCameraFlag(bool flag) {
		TunderCameraFlag = flag;
	}

	bool UltTime(bool UltMoveFlag);

	// 必殺カメラの終了
	void GameCameraUltEnd();

private:

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	KnightPlayer*			m_knightplayer	= nullptr;
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	Game* game = nullptr;

	WizardUlt* wizardUlt = nullptr;

	KnightUlt* knightUlt = nullptr;
	//必殺技を打ったキャラ
	Actor* ultactor = nullptr;
	//プレイヤー
	Actor* player_actor = nullptr;
	//必殺技を打たれたキャラ
	Actor* victim_actor = nullptr;
	
	Vector3					m_keepDiff				= Vector3::Zero;
	Vector3					m_CameraFromActorDiff   = Vector3::One;

	Vector3					m_toCameraPos			= Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3					m_position				= Vector3::Zero;		//カメラ座標
	Vector3					m_target				= Vector3::Zero;		//カメラ注視点
	Quaternion				m_rotation				= Quaternion::Identity;	//回転

	//注視点の計算
	Vector3 TargetPos;
	//Quaternion oldgetRot;
	//カメラの最終的な視点
	Vector3 newCamPos = Vector3::Zero;

	bool PlayerCameraSet = false;

	bool ok = false;
	float rotamount = 0;

	//剣士のフラグ
	bool KnightUltFlag = false;
	bool SetCameraCharFrontFlag = false;
	//必殺技の溜めが始まったらtrue
	bool UltChargeFlag = false;

	float m_timer = 0.0f;
	const char* player_name = nullptr;

	float sita = 0.0f;

	bool m_cameraShakeFlag = false;
	bool m_ShakeMoveFlag = false;
	float m_cameraShakeTimer = 0.1f;

	///////////////////////////////////////
	//キャラが雷を打たれているなら
	bool TunderCameraFlag = false;
	//雷に打たれたキャラの名前を格納
	const char* Damege_actor_Name = nullptr;
	//////////////////////////////////////
	
	//前フレームのプレイヤーがリスポーン待機中かのフラグ
	bool PlayerRespawnFlag = false;

	//アクターの情報
	std::vector<Actor*> m_actors;
};

