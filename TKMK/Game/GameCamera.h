#pragma once
#include "CameraCollisionSolver.h"

class KnightPlayer;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	void SetKnight(KnightPlayer* knightplayer)
	{
		m_knightplayer = knightplayer;
	}

	CameraCollisionSolver	m_cameraCollisionSolver;

	KnightPlayer*			m_knightplayer	= nullptr;

	Vector3					m_toCameraPos			= Vector3::Zero;		//�J�����ʒu���璍���_�Ɍ������x�N�g��
	Vector3					m_position				= Vector3::Zero;		//�J�������W
	Vector3					m_target				= Vector3::Zero;		//�J���������_
	Quaternion				m_rotation				= Quaternion::Identity;	//��]
};

