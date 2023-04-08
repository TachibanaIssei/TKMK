#pragma once
#include "Actor.h"
#include "CameraCollisionSolver.h"

class Game;
class GameUI;

class WizardBase:public Actor
{
public:
	WizardBase();
	virtual ~WizardBase();

	enum WizardState
	{
		enWizardState_Idle,
		enWizardState_Walk,
		enWizardState_Run,
		enWizardState_Attack,
		enWizardState_Damege,
		enWizardState_Death,
		enWizardState_Skill,
		enWizardState_UltimateSkill,
		enWizardState_Avoidance,
		enWizardState_Pause,
		enWizardState_Num,
	};

	void SetModel();

	/// <summary>
	/// �����̓G��|�����Ƃ��̌o���l�̏���
	/// </summary>
	/// <param name="GetExp">�����̓G�̌o���l</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack() = 0;

	/// <summary>
	/// �_���[�W���󂯂��Ƃ��̏���
	/// </summary>
	/// <param name="damege">�G�̍U����</param>
	void Dameged(int damege);

	/// <summary>
	/// ���g���|���ꂽ�Ƃ��̏���
	/// </summary>
	void Death();

	/// <summary>
	/// �K�E�Z�𔭓������Ƃ��̏���
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// ���X�|�[��������W�̃Z�b�g
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �ʏ�U���̓����蔻��̏���
	/// </summary>
	//void AtkCollisiton();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	void Skill(Vector3& position,Quaternion& rotation,CharacterController& charCon);

	/// <summary>
	/// �K�E�Z�̓����蔻��̏���
	/// </summary>
	//void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// �A�j���[�V�����̃X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// �A�j���[�V�����Đ����̈ړ������A�ړ����x�����߂�
	/// </summary>
	/// <param name="moveSpeed">�X�e�B�b�N�̈ړ��ʂƏ�Z���������X�s�[�h�̒l</param>
	/// <param name="stickL">�X�e�B�b�N�̈ړ��̓��͗�</param>
	void AnimationMove(float moveSpeed, Vector3 stickL);

	/// <summary>
	/// �����ړ�
	/// </summary>
	/// <param name="right"></param>
	/// <param name="forward"></param>
	void MoveStraight(Vector3& right, Vector3& forward);

	/// <summary>
	/// �A�j���[�V�����C�x���g
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName) = 0;

	/// <summary>
	/// ���W�̃Z�b�g
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// ���@�g���̍��W��Ԃ�l�Ƃ��ĕԂ�
	/// </summary>
	virtual Vector3 GetPosition() = 0;

	/// <summary>
	/// ����̃A�j���[�V�������Đ����Ȃ�false��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_wizardState != enWizardState_Attack &&
			m_wizardState != enAnimationClip_Damege &&
			m_wizardState != enAnimationClip_Death &&
			m_wizardState != enAnimationClip_Avoidance &&
			m_wizardState != enAnimationClip_Skill;
	}

	/// <summary>
	/// ���݂̃��x����Ԃ�
	/// </summary>
	/// <returns>���݂̃��x��</returns>
	int& SetLevel() {
		return Lv;
	}

	/// <summary>
	/// ���݂̃q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>���݂�HP</returns>
	int& SetHp() {
		return m_Status.Hp;
	}

	/// <summary>
	/// ���݂̃}�b�N�X�q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns></returns>
	int& SetMaxHp() {
		return m_Status.MaxHp;
	}

	void SetSGame(Game* Cgame)
	{
		game = Cgame;
	}
	Game* GetSGame()
	{
		return game;
	}
	//
	void SetGameUI(GameUI* mgameUI)
	{
		gameUI = mgameUI;
	}
	GameUI* GetSGameUI()
	{
		return gameUI;
	}

	/// <summary>
	///	�v���C���[�̑O�������擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_Forward;
	}

	/// <summary>
	/// �v���C���[�̉�]�ʂ��擾
	/// </summary>
	/// <returns></returns>
	Quaternion& GetRot()
	{
		return m_rot;
	}

	/// <summary>
	/// ���@�g���̃X�e�[�g��ύX
	/// </summary>
	/// <param name="gamescene">�ύX�������X�e�[�g�̖��O</param>
	void SetWizardState(WizardState gamescene) {
		m_wizardState = gamescene;

	}

protected:
	void PlayAnimation();
	//���ʂ̃X�e�[�g�̑J�ڏ���
	void OnProcessCommonStateTransition();
	//�ҋ@�̃X�e�[�g�̑J�ڏ���
	void OnProcessIdleStateTransition();
	//�����̃X�e�[�g�̑J�ڏ���
	void OnProcessRunStateTransition();
	//�A�^�b�N�̃X�e�[�g�̑J�ڏ���
	void OnProcessAttackStateTransition();
	//�X�L���̃X�e�[�g�̑J�ڏ���
	void OnProcessSkillAtkStateTransition();
	//����̃X�e�[�g�̑J�ڏ���
	void OnProcessAvoidanceStateTransition();
	//�K�E�Z�̃X�e�[�g�̑J�ڏ���
	void OnProcessUltimateSkillAtkStateTransition();
	//�_���[�W���󂯂��Ƃ��̃X�e�[�g�̑J�ڏ���
	void OnProcessDamegeStateTransition();
	//HP��0�ɂȂ����Ƃ��̃X�e�[�g�̑J�ڏ���
	void OnProcessDeathStateTransition();


	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Atk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v

	//�����X�e�[�^�X �ő�HP�AHP�A�U���́A�X�s�[�h
	//Status m_Status;
	//���x���A�b�v���ɑ�������X�e�[�^�X
	LvUpStatus LvUpStatus = { 30,5,10.0f };

	WizardState m_wizardState=enWizardState_Idle;

	Game* game = nullptr;
	GameUI* gameUI = nullptr;

	Vector3 oldPosition = Vector3::Zero;         //�O�t���[���̍��W
	float m_position_YUp = 34.0f;                         //���f���̎������ɂ���̂�Y���W��50.0f�グ��
	Vector3 m_forward = Vector3::AxisZ;                   //���ʃx�N�g��
	Vector3 collisionRot = Vector3::Zero;                  //�K�E�Z
	CollisionObject* collisionObject;                     //�R���W����
	Vector3 UltCollisionPos = Vector3::Zero;               //�K�E�Z�̓����蔻��̍��W
	Vector3 m_Skill_Right = Vector3::Zero;                 //�J�����̉E����
	Vector3 m_Skill_Forward = Vector3::Zero;               //�J�����̑O����
	CharacterController m_charCon;                        //�L�����N�^�[�R���g���[��
	CameraCollisionSolver	m_WarpCollisionSolver;    //���[�v

	//�X�L���̃N�[���^�C�����v�Z����^�C�}�[
	float SkillTimer = 0;
	//����̃N�[���^�C�����v�Z����^�C�}�[
	float AvoidanceTimer = 0;

	//�{�^���������ꂽ���̔���
	bool pushFlag = false;

	//�X�L���̃A�j���[�V�����Đ����I��������̔���
	bool SkillEndFlag = false;
	//����A�j���[�V�������Đ��������̔���
	bool AvoidanceFlag = false;
	//����A�j���[�V�������I��������̔���
	bool AvoidanceEndFlag = false;
};

