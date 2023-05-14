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
	/// �J�����̎��_���ŏ��̏�Ԃɖ߂�
	/// </summary>
	void CameraTarget(float X,float Y,Actor*actor);

	/// <summary>
	/// ���m���K�E�Z��ł������̃J�������[�N
	/// </summary>
	void KnightUltCamera(Actor* actor, bool reset);

	/// <summary>
	/// ���m�̎a���G�t�F�N�g��ǂ�������
	/// </summary>
	void ChaseUltEff();

	/// <summary>
	/// �J������h�炷
	/// </summary>
	void CameraShake(bool UpDown);

	void ChangeCameraShakeFlag(bool flag)
	{
		m_cameraShakeFlag = flag;
	}

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

	void ChangeMoveCameraState(EnCameraState state) {
		m_enCameraState = state;
	}

	/// <summary>
	/// ���m�̕K�E�Z�̃C���X�^���X��������
	/// </summary>
	/// <param name="ultobj">���m�̕K�E�Z�N���X</param>
	void SetKnightUlt(KnightUlt* ultobj)
	{
		knightUlt = ultobj;
	}

	void ChangeTunderCameraFlag(bool flag) {
		TunderCameraFlag = flag;
	}

	bool UltTime(bool UltMoveFlag);

	// �K�E�J�����̏I��
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
	//�K�E�Z��ł����L����
	Actor* ultactor = nullptr;
	//�v���C���[
	Actor* player_actor = nullptr;
	//�K�E�Z��ł��ꂽ�L����
	Actor* victim_actor = nullptr;
	
	Vector3					m_keepDiff				= Vector3::Zero;
	Vector3					m_CameraFromActorDiff   = Vector3::One;

	Vector3					m_toCameraPos			= Vector3::Zero;		//�J�����ʒu���璍���_�Ɍ������x�N�g��
	Vector3					m_position				= Vector3::Zero;		//�J�������W
	Vector3					m_target				= Vector3::Zero;		//�J���������_
	Quaternion				m_rotation				= Quaternion::Identity;	//��]

	//�����_�̌v�Z
	Vector3 TargetPos;
	//Quaternion oldgetRot;
	//�J�����̍ŏI�I�Ȏ��_
	Vector3 newCamPos = Vector3::Zero;

	bool PlayerCameraSet = false;

	bool ok = false;
	float rotamount = 0;

	//���m�̃t���O
	bool KnightUltFlag = false;
	bool SetCameraCharFrontFlag = false;
	//�K�E�Z�̗��߂��n�܂�����true
	bool UltChargeFlag = false;

	float m_timer = 0.0f;
	const char* player_name = nullptr;

	float sita = 0.0f;

	bool m_cameraShakeFlag = false;
	bool m_ShakeMoveFlag = false;
	float m_cameraShakeTimer = 0.1f;

	///////////////////////////////////////
	//�L����������ł���Ă���Ȃ�
	bool TunderCameraFlag = false;
	//���ɑł��ꂽ�L�����̖��O���i�[
	const char* Damege_actor_Name = nullptr;
	//////////////////////////////////////
	
	//�O�t���[���̃v���C���[�����X�|�[���ҋ@�����̃t���O
	bool PlayerRespawnFlag = false;

	//�A�N�^�[�̏��
	std::vector<Actor*> m_actors;
};

