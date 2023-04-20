#pragma once
#include "CameraCollisionSolver.h"

class KnightPlayer;
class WizardPlayer;
class Player;
class Game;
class KnightUlt;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void FollowThePlayer();
	void CameraTarget();

	/// <summary>
	/// 剣士が必殺技を打った時のカメラワーク
	/// </summary>
	void KnightUltCamera();

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

	/// <summary>
	/// 剣士の必殺技のインスタンスを代入する
	/// </summary>
	/// <param name="ultobj">剣士の必殺技クラス</param>
	void SetKnightUlt(KnightUlt* ultobj)
	{
		knightUlt = ultobj;
	}

	CameraCollisionSolver	m_cameraCollisionSolver;

	KnightPlayer*			m_knightplayer	= nullptr;
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	Game* game = nullptr;
	KnightUlt* knightUlt = nullptr;

	Vector3					m_toCameraPos			= Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3					m_position				= Vector3::Zero;		//カメラ座標
	Vector3					m_target				= Vector3::Zero;		//カメラ注視点
	Quaternion				m_rotation				= Quaternion::Identity;	//回転

	//注視点の計算
	Vector3 TargetPos;
	//Quaternion oldgetRot;

	bool PlayerCameraSet = false;

	bool ok = false;
	float rotamount = 0;
};

