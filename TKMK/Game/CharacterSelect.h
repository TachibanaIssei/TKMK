#pragma once
#include "physics/PhysicsGhostObject.h"

class Tittle;
class Game;

class CharacterSelect:public IGameObject
{
public:
	CharacterSelect();
	~CharacterSelect();
	bool Start();
	void Update();
	void Cursor();

	/// <summary>
	/// �|�C���^�[�̈ړ�����
	/// </summary>
	void PointerMove();

	/// <summary>
	/// �R���W�����쐬
	/// </summary>
	void SetCollision();

	/// <summary>
	/// �S�[�X�g�I�u�W�F�N�g�̓����蔻��̏���
	/// </summary>
	void GhostCollision();

	void SetKnightModel();

	void Render(RenderContext& rc);
	
	//�A�j���[�V����
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_ChainAtk,
		enAnimationClip_SecondAtk,
		enAnimationClip_lastAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Jump,
		enAnimationClip_Fall,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v

	enum EnCharacterSelect {
		enCharacterSelect_Knight,		//���m
		enCharacterSelect_Wizard,		//���@�g��
		enCharacterSelect_Zombie,		//�]���r
		enCharacterSelect_Mitei2,		//����
		enCharacterSelect_Num,			//�L�����N�^�[�̎�ނ̐�
	};
 

private:
	SpriteRender m_SelectCursor;			//�J�[�\��

	SpriteRender m_pointer_black;					//�|�C���^�[
	SpriteRender m_pointer_white;					//�|�C���^�[


	SpriteRender m_Status;					//�X�e�[�^�X
	SpriteRender m_Attack_Icon;				//�U���A�X�L���A�K�E�Z�A�C�R��
	SpriteRender m_UnderBar;				//��ʉ��̃o�[
	SpriteRender m_name;					//���O
	
	SpriteRender m_Attack_explanation;		//�U���̐�����
	SpriteRender Skill_explanation;			//�X�L���̐�����
	SpriteRender Ult_explanation;			//�K�E�Z�̐�����

	ModelRender m_Knight;					//���m�̃��f��
	Quaternion m_KnightRot;

	ModelRender m_platform;					//���f�����悹���
	

	EnCharacterSelect m_characterSelect;	//�L�����N�^�[�Z���N�g

	Vector3 curPosition = { -510.0f,75.0f,0.0f };


	//�|�C���^�[
	CharacterController Poimter;
	Vector3 m_Pointerposition = Vector3(0.0f,200.0f,-0.0f);
	Vector3 m_moveSpeed = Vector3::Zero;

	PhysicsGhostObject Attack;
	bool Attack_explanationFlag = false;
	PhysicsGhostObject Skill;
	bool Skill_explanationFlag = false;
	PhysicsGhostObject UltimateSkill;
	bool Ult_explanationFlag = false;

	float time;

	int CursorNum = 0;
	int TitleScene = 1;
};

