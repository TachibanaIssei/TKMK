#pragma once

#include "Actor.h"
class Game;

class KnightBase:public Actor
{
public:
	KnightBase();
	~KnightBase();
	bool Start();
	void Update();
	void Move();
	void Render(RenderContext& rc);
	void Attack();
	void Dameged(int damege);
	void Skill();
	void UltimateSkill();
	void Death();
	void Rotation();
	//�X�e�[�g�Ǘ��B
	void ManageState();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	inline void SetPosition(Vector3 PS) { m_position = PS; }
	inline Vector3 GetPosition() { return m_position; }
	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	bool IsEnableMove() const
	{
		return m_animState != enKnightState_FirstAtk && m_animState != enKnightState_SecondAtk;
	}


private:
	enum AnimationState {
		enKnightState_Idle,
		enKnightState_Run,
		enKnightState_FirstAtk,
		enKnightState_SecondAtk,
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_FirstAtk,
		enAnimationClip_SecondAtk,
		enAnimationClip_lastAtk,
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
	//�����X�e�[�^�X
	Status status = { 150,35,150.0f };
	//���x���A�b�v���ɑ�������X�e�[�^�X
	LvUpStatus LvUpStatus = { 30,10,30.0f };
	AnimationState m_animState = enKnightState_Idle;
	//���݂̃R���{
	int ComboState = 0;
	//�R���{���p�����鎞�Ԃ��L�^����
	float ComboTimer = 0;

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
};

