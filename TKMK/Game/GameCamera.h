#pragma once
#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class KnightPlayer;
class WizardPlayer;
class Player;
class Game;
class KnightUlt;
class Actor;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void FollowThePlayer();

	/// <summary>
	/// �J�����̎��_���ŏ��̏�Ԃɖ߂�
	/// </summary>
	void CameraTarget(float X,float Y,Actor*actor);

	/// <summary>
	/// ���m���K�E�Z��ł������̃J�������[�N
	/// </summary>
	void KnightUltCamera(Actor* actor);

	/// <summary>
	/// ���m�̎a���G�t�F�N�g��ǂ�������
	/// </summary>
	void ChaseUltEff();

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

	/// <summary>
	/// ���m�̕K�E�Z�̃C���X�^���X��������
	/// </summary>
	/// <param name="ultobj">���m�̕K�E�Z�N���X</param>
	void SetKnightUlt(KnightUlt* ultobj)
	{
		knightUlt = ultobj;
	}

	bool UltTime(bool UltMoveFlag);

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	KnightPlayer*			m_knightplayer	= nullptr;
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	Game* game = nullptr;
	KnightUlt* knightUlt = nullptr;
	Actor* ultactor = nullptr;
	Actor* player_actor = nullptr;
	
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

	//���m�̃t���O
	bool KnightUltMoveFlag = false;
	bool SetCameraCharFrontFlag = false;

	float m_timer = 0.0f;
	const char* player_name = nullptr;
	//�A�N�^�[�̏��
	std::vector<Actor*> m_actors;
};

