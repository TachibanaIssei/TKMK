#pragma once

class Game;

class Tittle:public IGameObject
{
public:
	Tittle();
	~Tittle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Select();

	enum EnSelectPosition {
		enSelectPosition_Start,						//�X�^�[�g�|�W�V����
		enSelectPosition_Operation,					//��������̃|�W�V����
		enSelectPosition_CharaExplanation,			//�L���������̃|�W�V����
		enSelectPosition_Num,						//�Z���N�g�̐�
	};

private:
	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	SpriteRender m_start;
	SpriteRender m_operation;
	SpriteRender m_charaExplanation;
	SpriteRender m_choice;
	EnSelectPosition m_tSelectPosition;
	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_position = { 0.0f,-150.0f,0.0f };

	int selectPosition = 0;
};

