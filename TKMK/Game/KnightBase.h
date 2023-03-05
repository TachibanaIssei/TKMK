#pragma once

#include "Actor.h"



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

	//void Update();
	void Move();
	//void Render(RenderContext& rc);

	/// <summary>
	/// �����̓G��|�����Ƃ��̌o���l�̏���
	/// </summary>
	/// <param name="GetExp">�����̓G�̌o���l</param>
	void ExpProcess(int Exp);

	void Attack();

	/// <summary>
	/// �_���[�W���󂯂��Ƃ��̏���
	/// </summary>
	/// <param name="damege">�G�̍U����</param>
	void Dameged(int damege);

	/// <summary>
	/// �X�L�����g�p�����Ƃ��̏���
	/// </summary>
	void Skill();

	/// <summary>
	/// 
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// 
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// 
	/// </summary>
	void Death();

	/// <summary>
	/// 
	/// </summary>
	void Rotation();

	/// <summary>
	/// �����蔻��̏���
	/// </summary>
	void AtkCollisiton();

	/// <summary>
	/// �A�j���[�V�����̃X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	//inline Vector3 GetPosition() { return m_position; }
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
		return m_animState != enKnightState_FirstAtk &&
			m_animState != enKnightState_SecondAtk&&
			m_animState!= enKnightState_Damege&&
			m_animState != enKnightState_Death;
	}

	/// <summary>
	/// ���݂̃��x����int�^�̕ϐ��ɑ������
	/// </summary>
	/// <param name="level"></param>
	int SetLevel(int& level){
		level = Lv;
		return level;
	}


protected:
	void PlayAnimation();
	//���ʂ̃X�e�[�g�̑J�ڏ���
	void OnProcessCommonStateTransition();
	//�ҋ@�̃X�e�[�g�̑J�ڏ���
	void OnProcessIdleStateTransition();
	//�����̃X�e�[�g�̑J�ڏ���
	void OnProcessRunStateTransition();
	//��i�ڂ̃A�^�b�N�̃X�e�[�g�̑J�ڏ���
	void OnProcessFirstAtkStateTransition();
	//��i�ڂ̃A�^�b�N�̃X�e�[�g�̑J�ڏ���
	void OnProcessSecondAtkStateTransition();
	//�_���[�W���󂯂��Ƃ��̃X�e�[�g�̑J�ڏ���
	void OnProcessDamegeStateTransition();
	//HP��0�ɂȂ����Ƃ��̃X�e�[�g�̑J�ڏ���
	void OnProcessDeathStateTransition();

	enum PlayerState {
		enKnightState_Idle,
		enKnightState_Run,
		enKnightState_FirstAtk,
		enKnightState_SecondAtk,
		enKnightState_Damege,
		enKnightState_Death,
		enKnightState_Skill,
		enKnightState_UltimateSkill,
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_FirstAtk,
		enAnimationClip_SecondAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		/*enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,*/
		enAnimationClip_Num,
	};
	Game* m_game;
	Vector3 firstposition;                                //�ŏ��̍��W
	Vector3 m_position = Vector3::Zero;                   //���W
	Vector3 m_forward = Vector3::AxisZ;                   //���ʃx�N�g��
	Vector3 m_moveSpeed;                                  //�ړ����x
	CharacterController m_charCon;                        //�L�����N�^�[�R���g���[��
	Quaternion m_rot = Quaternion::Identity;              //�N�H�[�^�j�I��
	ModelRender m_modelRender;                            //���f�������_�[
	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v
	//�����X�e�[�^�X �ő�HP�AHP�A�U���́A�X�s�[�h
	Status status = { 150,150,35,150.0f };
	//���x���A�b�v���ɑ�������X�e�[�^�X
	LvUpStatus LvUpStatus = { 30,10,30.0f };
	PlayerState m_animState = enKnightState_Idle;
	//���݂̃R���{
	int ComboState = 0;
	//�R���{���p�����鎞�Ԃ��L�^����
	float ComboTimer = 0;
	//��i�ڂ̃A�^�b�N���������̔���
	bool FirstAtkState = false;

	//�l�������o���l��
	int exp=5;
	//�󂯂��_���[�W��
	int dddd = 50;

};

