#pragma once
#include "Actor.h"

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
	/// �K�E�Z�̓����蔻��̏���
	/// </summary>
	//void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// �A�j���[�V�����̃X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();

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
		return m_wizardState != enWizardState_Death;
	}

	/// <summary>
	/// ���݂̃q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>���݂�HP</returns>
	int& SetHp() {
		return m_Status.Hp;
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
	void SetGameUI(GameUI* gameUI)
	{
		gameUI = gameUI;
	}
	GameUI* GetSGameUI()
	{
		return gameUI;
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
	Status m_Status;
	//���x���A�b�v���ɑ�������X�e�[�^�X
	LvUpStatus LvUpStatus = { 30,5,10.0f };

	WizardState m_wizardState=enWizardState_Idle;

	Game* game = nullptr;
	GameUI* gameUI = nullptr;

	Vector3 m_position = Vector3::Zero;
	float m_position_YUp = 33.0f;                         //���f���̎������ɂ���̂�Y���W��50.0f�グ��
	Vector3 m_forward = Vector3::AxisZ;                   //���ʃx�N�g��
	Vector3 collisionRot = Vector3::Zero;                  //�K�E�Z
	CollisionObject* collisionObject;                     //�R���W����
	Vector3 UltCollisionPos = Vector3::Zero;               //�K�E�Z�̓����蔻��̍��W
	Vector3 m_Skill_Right = Vector3::Zero;                 //�J�����̉E����
	Vector3 m_Skill_Forward = Vector3::Zero;               //�J�����̑O����
	CharacterController m_charCon;                        //�L�����N�^�[�R���g���[��
	Quaternion m_rot = Quaternion::Identity;              //��]
	ModelRender m_modelRender;                            //���f�������_�[

	//�X�L���̃N�[���^�C�����v�Z����^�C�}�[
	float SkillTimer = 0;
	//����̃N�[���^�C�����v�Z����^�C�}�[
	float AvoidanceTimer = 0;
};

