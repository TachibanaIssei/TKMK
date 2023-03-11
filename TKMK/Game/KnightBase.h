#pragma once

#include "Actor.h"


//class Status;
class Game;

class KnightBase:public Actor
{
public:
	KnightBase();
	virtual ~KnightBase();

	/// <summary>
	/// ���f����Init�A�L�����R���̏�����
	/// </summary>
	/// <param name="Model"></param>
	/// <param name="charCon"></param>
	/// bool Start()
	void SetModel();

	/// <summary>
	/// �����̓G��|�����Ƃ��̌o���l�̏���
	/// </summary>
	/// <param name="GetExp">�����̓G�̌o���l</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack()=0;

	/// <summary>
	/// ���g�̓����蔻��
	/// </summary>
	void Collition();

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
	/// �X�L�����g�p�����Ƃ��̏���
	/// </summary>
	//void Skill(Vector3& right,Vector3& forward);

	/// <summary>
	/// �K�E�Z�𔭓������Ƃ��̏���
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// ���X�|�[��������W�̃Z�b�g
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// ���g���|���ꂽ�Ƃ��̏���
	/// </summary>
	void Rotation();

	/// <summary>
	/// �ʏ�U���̓����蔻��̏���
	/// </summary>
	void AtkCollisiton();

	/// <summary>
	/// �K�E�Z�̓����蔻��̏���
	/// </summary>
	void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// �A�j���[�V�����̃X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// 
	/// </summary>
	void AnimationMove();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="right"></param>
	/// <param name="forward"></param>
	void MoveStraight(Vector3& right, Vector3& forward);

	/// <summary>
	/// �A�j���[�V�����C�x���g
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)=0;

	/// <summary>
	/// ���W�̃Z�b�g
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// ���m�̍��W��Ԃ�l�Ƃ��ĕԂ�
	/// </summary>
	virtual Vector3 GetPosition() = 0;

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ����Ȃ�false��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_animState != enKnightState_ChainAtk &&
			m_animState != enKnightState_UltimateSkill &&
			m_animState != enKnightState_Skill &&
			m_animState != enKnightState_Avoidance &&
			m_animState!= enKnightState_Damege&&
			m_animState != enKnightState_Death;
	}

	/// <summary>
	/// ���݂̃��x����Ԃ�
	/// </summary>
	/// <returns>���݂̃��x��</returns>
	int& SetLevel(){
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

protected:
	void PlayAnimation();
	//���ʂ̃X�e�[�g�̑J�ڏ���
	void OnProcessCommonStateTransition();
	//�ҋ@�̃X�e�[�g�̑J�ڏ���
	void OnProcessIdleStateTransition();
	//�����̃X�e�[�g�̑J�ڏ���
	void OnProcessRunStateTransition();
	//�`�F�C���A�^�b�N�̃X�e�[�g�̑J�ڏ���
	void OnProcessChainAtkStateTransition();
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

	enum PlayerState {
		enKnightState_Idle,
		enKnightState_Run,
		enKnightState_ChainAtk,
		enKnightState_Damege,
		enKnightState_Death,
		enKnightState_Skill,
		enKnightState_UltimateSkill,
		enKnightState_Avoidance,
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_ChainAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Num,
	};

	Game* m_game=nullptr;

	//�����X�e�[�^�X �ő�HP�AHP�A�U���́A�X�s�[�h
	Status m_Status;

	Vector3 firstposition;                                //�ŏ��̍��W
	Vector3 OldPosition = Vector3::Zero;                  //�O�̃t���[���̍��W
	Vector3 m_position = Vector3::Zero;                   //���W
	float m_position_YUp = 36.0f;                         //���f���̎������ɂ���̂�Y���W��50.0f�グ��
	Vector3 m_forward = Vector3::AxisZ;                   //���ʃx�N�g��
	Vector3 collisionRot= Vector3::Zero;                  //�K�E�Z
	CollisionObject* collisionObject;                     //�R���W����
	Vector3 UltCollisionPos= Vector3::Zero;               //�K�E�Z�̓����蔻��̍��W
	Vector3 m_Skill_Right = Vector3::Zero;                 //�J�����̉E����
	Vector3 m_Skill_Forward = Vector3::Zero;               //�J�����̑O����
	CharacterController m_charCon;                        //�L�����N�^�[�R���g���[��
	Quaternion m_rot = Quaternion::Identity;              //�N�H�[�^�j�I��
	ModelRender m_modelRender;                            //���f�������_�[
	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v
	
	//���x���A�b�v���ɑ�������X�e�[�^�X
	LvUpStatus LvUpStatus = { 30,10,30.0f };
	PlayerState m_animState = enKnightState_Idle;
	//���݂̃R���{
	int ComboState = 0;
	//�R���{���p�����鎞�Ԃ��L�^����
	float ComboTimer = 0;
	//�{�^���������ꂽ���̔���
	bool pushFlag = false;
	//��i�ڂ̃A�^�b�N���������̔���
	bool AtkState = false;
	//�X�L���̃A�j���[�V�����Đ����I��������̔���
	bool SkillEndFlag = false;
	//����A�j���[�V�������Đ��������̔���
	bool AvoidanceFlag = false;
	//
	bool AvoidanceEndFlag = false;
	//�u�v�{�[����ID
	int m_swordBoneId = -1;
	//�U���A�j���[�V�����C�x���g�Đ����̌��m�̍��W���擾����
	int AtkEndPosId= -1;
	//�X�L���̃N�[���^�C�����v�Z����^�C�}�[
	float SkillTimer = 0;
	//����̃N�[���^�C�����v�Z����^�C�}�[
	float AvoidanceTimer = 0;

	//�l�������o���l��
	int exp=5;
	//Newtral_Enemy�̍U����
	int Enemy_atk = 10;
	//�K�E�Z�g�p�̃t���O
	bool UltCollisionSetFlag = false;
	//�U�����̌��̃R���W������\�����邩�̃t���O
	bool AtkCollistionFlag = false;

};

