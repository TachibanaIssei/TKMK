#pragma once
#include "physics/PhysicsGhostObject.h"

class Tittle;
class Game;
class Fade;

class CharacterSelect:public IGameObject
{
public:
	CharacterSelect();
	~CharacterSelect();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
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

	enum EnCharacterSelect {
		enCharacterSelect_Knight,		//���m
		enCharacterSelect_Wizard,		//���@�g��
		enCharacterSelect_Zombie,		//�]���r
		enCharacterSelect_Mitei2,		//����
		enCharacterSelect_Num,			//�L�����N�^�[�̎�ނ̐�
	};

	/// <summary>
	/// �C���Q�[���ւ̏���
	/// </summary>
	void Ready();

	/// <summary>
	/// �|�C���^�[�̈ړ�����
	/// </summary>
	void PointerMove();

	/// <summary>
	/// �|�C���^�[�̉��ړ�����
	/// </summary>
	void PointerMoveX();

	/// <summary>
	/// �|�C���^�[�̏c�ړ�����
	/// </summary>
	void PointerMoveY();

	/// <summary>
	/// ���m���f���̐ݒ�
	/// </summary>
	void SetKnightModel();


private:
	Fade* fade = nullptr;
	SoundSource* m_bgm = nullptr;

	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v

	SpriteRender m_selectCursor;			//�J�[�\��
	SpriteRender m_pointerBlack;			//���|�C���^�[
	SpriteRender m_pointerWhite;			//���|�C���^�[
	SpriteRender m_status;					//�X�e�[�^�X
	SpriteRender m_attackIcon;				//�U���A�X�L���A�K�E�Z�A�C�R��
	SpriteRender m_underBar;				//��ʉ��̃o�[
	SpriteRender m_name;					//���O
	SpriteRender m_attackExplanation;		//�U���̐�����
	SpriteRender m_skillExplanation;		//�X�L���̐�����
	SpriteRender m_ultExplanation;			//�K�E�Z�̐�����

	ModelRender m_knight;					//���m�̃��f��
	ModelRender m_platform;					//���f�����悹���

	Quaternion m_knightRot;					//���m�̉�]

	EnCharacterSelect m_characterSelect;	//�L�����N�^�[�Z���N�g

	Vector3 m_cursorPosition = { -510.0f,75.0f,0.0f };

	Vector3 m_pointerPosition	= Vector3(0.0f,90.0f,0.0f);

	bool m_attackExplanationFlag = false;	//�ʏ�U�������摜�\���t���O
	bool m_skillExplanationFlag = false;	//�X�L�������摜�\���t���O
	bool m_ultExplanationFlag = false;		//�K�E�Z�����摜�\���t���O
	bool m_readyFlag = false;				//�C���Q�[���J�n�t���O

	float time;
	float m_bgmVolume = 0.4f;				//BGM�̃{�����[��

};

