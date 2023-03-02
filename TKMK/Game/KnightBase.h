#pragma once

#include "Actor.h"


class KnightBase:public Actor,public IGameObject
{
public:
	KnightBase();
	~KnightBase();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Attack();
	void Dameged(int damege);
	void Skill();
	void UltimateSkill();
	void Death();
	void Rotation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	enum EnAnimationClip {
		enAnimationClip_Idle,
		//enAnimationClip_Run,
		//enAnimationClip_Jump,
		enAnimationClip_Num,
	};

	Vector3 firstposition;                                //�ŏ��̍��W
	Vector3 m_position = Vector3::Zero;                   //���W
	Vector3 m_forward = Vector3::AxisZ;                   //���ʃx�N�g��
	Vector3 m_moveSpeed;                                  //�ړ����x
	CharacterController m_charCon;                        //�L�����N�^�[�R���g���[��
	Quaternion m_rot = Quaternion::Identity;              //�N�H�[�^�j�I��
	ModelRender m_modelRender;                            //���f�������_�[
	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v


};

