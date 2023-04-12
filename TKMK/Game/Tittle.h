#pragma once

class Game;
class CharacterSelect;

class Tittle:public IGameObject
{
public:
	Tittle();
	~Tittle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Select();
	void Operation();

	enum EnSelectPosition {
		enSelectPosition_Start,						//�X�^�[�g�|�W�V����
		enSelectPosition_Operation,					//��������̃|�W�V����
		enSelectPosition_CharaExplanation,			//�L���������̃|�W�V����
		enSelectPosition_Num,						//�Z���N�g�̐�
	};
	
	//���������ʂ������邩�����Ȃ���
	enum EnOperationPosition {
		enOperationPosition_UnSeem,					//�����Ȃ�
		enOperationPosition_Seem,					//������
		enOperationPosition_Num,
	};


private:
	Quaternion m_sRotation;

	SpriteRender m_spriteRender;
	SpriteRender m_start;						//"�͂��߂�"
	SpriteRender m_operation;					//"�������"
	SpriteRender m_charaExplanation;			//"�L�����N�^�[����"
	SpriteRender m_choice;						//�Z���N�g
	SpriteRender m_operationPic;				//��������̉摜

	EnSelectPosition m_tSelectPosition;			//�Z���N�g�̃|�W�V����
	EnOperationPosition m_operationPosition;	//��������摜�̃|�W�V����

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { 0.0f,-150.0f,0.0f };	//�Z���N�g�̃|�W�V����
	Vector3 m_opPosition = { 0.0f,2000.0f,0.0f };	//�R���g���[���[�̉摜�̃|�W�V����

	int selectPosition = 0;
	int opPosition = 0;
};

