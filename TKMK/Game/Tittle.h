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
	SpriteRender m_start;						//�͂��߂�
	SpriteRender m_operation;					//�������
	SpriteRender m_charaExplanation;			//�L�����N�^�[����
	SpriteRender m_choice;						//�Z���N�g

	EnSelectPosition m_tSelectPosition;			//�Z���N�g�̃|�W�V����

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { 0.0f,-150.0f,0.0f };	//�Z���N�g�̃|�W�V����
	Vector3 m_conPosition = { 0.0f,2000.0f,0.0f };	//�R���g���[���[�̉摜�̃|�W�V����

	int selectPosition = 0;
	int conPosition = 0;
};

