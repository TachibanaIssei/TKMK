#pragma once
#include "CameraCollisionSolver.h"

class KnightPlayer;
class Game;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

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

	

	CameraCollisionSolver	m_cameraCollisionSolver;

	KnightPlayer*			m_knightplayer	= nullptr;
	Game* game = nullptr;

	Vector3					m_toCameraPos			= Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3					m_position				= Vector3::Zero;		//カメラ座標
	Vector3					m_target				= Vector3::Zero;		//カメラ注視点
	Quaternion				m_rotation				= Quaternion::Identity;	//回転
};

