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
	void Render(RenderContext& rc);

	
	
	enum EnCharacterSelect {
		enCharacterSelect_Knight,		//���m
		enCharacterSelect_Wizard,		//���@�g��
		enCharacterSelect_Mitei,		//����
		enCharacterSelect_Mitei2,		//����
		enCharacterSelect_Num,			//�L�����N�^�[�̎�ނ̐�
	};
 

private:
	SpriteRender m_CharacterSelectPic;		//�Z���N�g���
	SpriteRender m_SelectCursor;			//�J�[�\��
	SpriteRender m_KnightPic;				//���m�̉摜
	SpriteRender m_WizardPic;				//���@�g���̉摜

	EnCharacterSelect m_characterSelect;	//�L�����N�^�[�Z���N�g

	Vector3 curPosition = { -510.0f,75.0f,0.0f };

	int CursorNum = 0;
};

