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

	/// <summary>
	/// ��ʕ������ɉ�ʂ̂ǂ�����ڂ��J������
	/// </summary>
	enum EnSplitCameraLR
	{
		enSplitCamera_Left,
		enSplitCamera_Right,
		enSplitCamera_Solo
	};

	enum EnCameraState {
		enNormalCameraState,
		enUltRotCameraState,
		enChaseCameraState
	};	

	//�J�����̃X�e�[�g
	enum CameraState
	{
		enGameState,
		enPauseState,
	};

	enum PlayerNumber
	{
		enPlayerNumber_1P = 0,
		enPlayerNumber_2P = 1
	};

	/// <summary>
	/// �J�����̎��_���ŏ��̏�Ԃɖ߂�
	/// </summary>
	void CameraTarget(float targrtYUp, float X,float Y,Actor*actor, bool cameraCollisionSolverFlag);

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

	/// <summary>
	/// �������������ɉ�ʂ�h�炷
	/// </summary>
	void ThunderCameraShake();

	void ThunderCameraShakeForPlayer();

	void ChangeCameraShakeFlag(bool flag)
	{
		m_cameraShakeFlag = flag;
	}

	

	void SetKnight(KnightPlayer* knightplayer)
	{
		m_knightplayer = knightplayer;
	}

	/// <summary>
	/// �J�����̃X�e�[�g��ύX
	/// </summary>
	/// <param name="gamescene">�ύX�������X�e�[�g�̖��O</param>
	void SetCameraState(CameraState gamescene) {
		if (this == nullptr) return;
		m_cameraState = gamescene;

	}

	void ChangeMoveCameraState(EnCameraState state) {
		m_enCameraState = state;
	}

	/// <summary>
	/// ��ʕ������ɉ�ʂ̂ǂ�����ڂ��J��������ݒ肷��
	/// </summary>
	/// <param name="cameraLR"></param>
	void SetSplitCameraLR(EnSplitCameraLR cameraLR)
	{
		m_splitCameraLR = cameraLR;
	}

	/// <summary>
	/// ��ʕ������ɉ�ʂ̂ǂ�����ڂ��J���������擾����
	/// </summary>
	/// <returns></returns>
	EnSplitCameraLR GetSplitCameraLR()
	{
		return m_splitCameraLR;
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

	//bool UltTime(bool UltMoveFlag);

	// �K�E�J�����̏I��
	void GameCameraUltEnd();

	Vector3 GetCameraForword()
	{
		return newCamPos;
	}

	SpringCamera* GetSpringCamera() {
		return &m_springCamera;
	}

	void SetPlayerShakeFlag(bool flag)
	{
		PlayerShakeFlag = flag;
	}

	bool GetPlayerShakeFlag()
	{
		return PlayerShakeFlag;
	}

	void setShakeMulPower(int revel);

private:
	EnCameraState  m_enCameraState = enNormalCameraState;
	EnSplitCameraLR m_splitCameraLR = enSplitCamera_Solo;
	CameraState m_cameraState = enGameState;
	PlayerNumber m_playerNumber = enPlayerNumber_1P;

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
	Vector3 TargetPos = Vector3::Zero;
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

	//false= true=
	bool ShakeLeftAndLightFlag = false;
	//////////////////////////////////////
	
	//�O�t���[���̃v���C���[�����X�|�[���ҋ@�����̃t���O
	bool PlayerRespawnFlag = false;

	//�v���C���[���K�E�Z���󂯂��Ƃ��ɃJ������h�炷�t���O
	bool PlayerShakeFlag = false;

	//�A�N�^�[�̏��
	std::vector<Actor*> m_actors;

	float shakePower = 0.0f;

	float shakeMulPower = 0.0f;
};

