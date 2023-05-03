#pragma once

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
	SpriteRender m_CharacterSelectPic;		//�Z���N�g���
	SpriteRender m_SelectCursor;			//�J�[�\��
	SpriteRender m_KnightPic;				//���m�̉摜
	SpriteRender m_WizardPic;				//���@�g���̉摜

	SpriteRender m_Status;					//�X�e�[�^�X
	SpriteRender m_Attack_Icon;				//�U���A�X�L���A�K�E�Z�A�C�R��
	SpriteRender m_UnderBar;				//��ʉ��̃o�[
	SpriteRender m_name;					//���O

	ModelRender m_Knight;					//���m�̃��f��
	Quaternion m_KnightRot;
	

	EnCharacterSelect m_characterSelect;	//�L�����N�^�[�Z���N�g

	Vector3 curPosition = { -510.0f,75.0f,0.0f };

	Vector3 m_CameraPosition = { -30.0f, 50.0f, -150.0f };

	int CursorNum = 0;
	int TitleScene = 1;
};

