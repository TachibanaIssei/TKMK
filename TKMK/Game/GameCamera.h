#pragma once
#include "CameraCollisionSolver.h"

class KnightPlayer;
class WizardPlayer;
class Player;
class Game;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Target();
	void StartCameraSet();
	void CameraReset();

	//�J�����̃X�e�[�g
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
	/// �J�����̃X�e�[�g��ύX
	/// </summary>
	/// <param name="gamescene">�ύX�������X�e�[�g�̖��O</param>
	void SetCameraState(CameraState gamescene) {
		m_cameraState = gamescene;

	}

	

	CameraCollisionSolver	m_cameraCollisionSolver;

	KnightPlayer*			m_knightplayer	= nullptr;
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	Game* game = nullptr;

	Vector3					m_toCameraPos			= Vector3::Zero;		//�J�����ʒu���璍���_�Ɍ������x�N�g��
	Vector3					m_position				= Vector3::Zero;		//�J�������W
	Vector3					m_target				= Vector3::Zero;		//�J���������_
	Quaternion				m_rotation				= Quaternion::Identity;	//��]

	//�����_�̌v�Z
	Vector3 TargetPos;
	//Quaternion oldgetRot;

	bool PlayerCameraSet = false;

	bool ok = false;
	float rotamount = 0;
};

