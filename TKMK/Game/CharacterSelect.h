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

	void InitSprite();

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
	void SetModel();

	/// <summary>
	/// ���f������]������
	/// </summary>
	void ModelRotation();

	/// <summary>
	/// �A�C�R�����J�[�\���Əd�Ȃ��Ă��邩���`�F�b�N����
	/// </summary>
	void CheckIconOverlap();

	/// <summary>
	/// �ʏ�U���̃A�C�R���ƃJ�[�\�����d�Ȃ��Ă��邩���`�F�b�N����
	/// </summary>
	/// <returns>�d�Ȃ��Ă�����true</returns>
	bool CheckNormalAttackIconOverlap();

	/// <summary>
	/// �X�L���̃A�C�R���ƃJ�[�\�����d�Ȃ��Ă��邩���`�F�b�N����
	/// </summary>
	/// <returns>�d�Ȃ��Ă�����true</returns>
	bool CheckSkillIconOverlap();

	/// <summary>
	/// �K�E�Z�̃A�C�R���ƃJ�[�\�����d�Ȃ��Ă��邩���`�F�b�N����
	/// </summary>
	/// <returns>�d�Ȃ��Ă�����true</returns>
	bool CheckUltIconOverlap();

	/// <summary>
	/// �A���_�[�o�[�̃t���[���ƃJ�[�\�����d�Ȃ��Ă��邩���`�F�b�N����
	/// </summary>
	/// <returns>�d�Ȃ��Ă�����true</returns>
	bool CheckUnderBarOverlap();

	/// <summary>
	/// �A�C�R���摜�̏c���̍ő�ƍŏ��̍��W���v�Z����
	/// </summary>
	/// <param name="posX">�摜��X���W</param>
	/// <param name="posY">�摜��Y���W</param>
	/// <param name="W">�摜�̕�</param>
	/// <param name="H">�摜�̍���</param>
	/// <returns>�E�[�A���[�A��A���̒l��������Vector4��Ԃ�</returns>
	Vector4 CalcIconPos(float posX,float posY, float W, float H);


private:
	SkyCube* m_skyCube = nullptr;

	Fade* fade = nullptr;
	SoundSource* m_bgm = nullptr;

	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v

	SpriteRender m_selectCursor;			//�J�[�\��
	SpriteRender m_pointerBlack;			//���|�C���^�[
	SpriteRender m_pointerWhite;			//���|�C���^�[
	SpriteRender m_status;					//�X�e�[�^�X
	SpriteRender m_attackIcon;				//�U���A�C�R��
	SpriteRender m_skillIcon;				//�X�L���A�C�R��
	SpriteRender m_ultIcon;					//�K�E�Z�A�C�R��
	SpriteRender m_underBar;				//��ʉ��̃o�[
	SpriteRender m_underBarYellow;			//��ʉ��̉��F�o�[
	SpriteRender m_underBarFrame;			//��ʉ��o�[�̃t���[��
	SpriteRender m_hpBerFrame;				//HP�o�[�̃t���[��
	SpriteRender m_atkBerFrame;				//�U���o�[�̃t���[��
	SpriteRender m_hpBerLv1;				//���x��1�̂Ƃ���HP�o�[
	SpriteRender m_hpBerLvmax;				//���x���}�b�N�X�̂Ƃ���HP�o�[
	SpriteRender m_atkBerLv1;				//���x��1�̂Ƃ��̍U���o�[
	SpriteRender m_atkBerLvmax;				//���x���}�b�N�X�̂Ƃ��̍U���o�[
	SpriteRender m_startSprite;				//�X�^�[�g�摜
	SpriteRender m_name;					//���O
	SpriteRender m_attackExplanation;		//�U���̐�����
	SpriteRender m_skillExplanation;		//�X�L���̐�����
	SpriteRender m_ultExplanation;			//�K�E�Z�̐�����

	ModelRender m_knight;					//���m�̃��f��
	ModelRender m_platform;					//���f�����悹���
	ModelRender m_stage;
	ModelRender m_wall;

	Quaternion m_knightRot;					//���m�̉�]

	EnCharacterSelect m_characterSelect;	//�L�����N�^�[�Z���N�g

	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_pointerPosition	= Vector3(0.0f,90.0f,0.0f);

	bool m_attackExplanationFlag = false;	//�ʏ�U�������摜�\���t���O
	bool m_skillExplanationFlag = false;	//�X�L�������摜�\���t���O
	bool m_ultExplanationFlag = false;		//�K�E�Z�����摜�\���t���O
	bool m_underBarDrawFlag = false;		//���̃o�[�̕\���t���O

	bool m_readyFlag = false;				//�C���Q�[���J�n�t���O

	float time;
	float m_bgmVolume = 0.4f;				//BGM�̃{�����[��

};

