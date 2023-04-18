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
	void CharacterOp();

	enum EnTitlePosition {
		enTitlePosition_Top,						//��
		enTitlePosition_Central,					//����
		enTitlePosition_Under,						//��
		enTitlePosition_Num,
	};


	enum EnSelectPosition {
		enSelectPosition_Start,						//�X�^�[�g�|�W�V����
		enSelectPosition_Operation,					//��������̃|�W�V����
		enSelectPosition_CharaExplanation,			//�L���������̃|�W�V����
		enSelectPosition_Num,						//�Z���N�g�̐�
	};
	
	//���������ʂ������邩�����Ȃ���
	enum EnOperationLook {
		enOperationLook_UnSeem,						//�����Ȃ�
		enOperationLook_Seem,						//������
		enOperationLook_Num,
	};

	//�L�����N�^�[������ʂ������邩�����Ȃ���
	enum EnCharacterOpLook {
		enCharacterOpLook_UnSeem,					//�����Ȃ�
		enCharacterOpLook_Seem,						//������
		enCharacterOpLook_Num,
	};

	//�ǂ̃L�����N�^�[������\�����邩
	enum EnCharacterOpPosition {
		enCharacterOpPosition_Knight,				//���m�̐���
		enCharacterOpPosition_Wizard,				//���@�g���̐���
		enCharacterOpPosition_Zombie,				//�]���r�̐���
		enCharacterOpPosition_Mitei,				//�L��������
		enCharacterOpPosition_Num,
	};

private:
	Quaternion m_sRotation;

	SpriteRender m_spriteRender;
	SpriteRender m_start;						//"�͂��߂�"
	SpriteRender m_operation;					//"�������"
	SpriteRender m_charaExplanation;			//"�L�����N�^�[����"
	SpriteRender m_choice;						//�Z���N�g
	SpriteRender m_operationPic;				//��������̉摜
	SpriteRender m_KnightOp;					//���m�̐����摜
	SpriteRender m_WizardOp;					//���@�g���̐����摜
	SpriteRender m_ZombieOp;					//�]���r�̐����摜
	SpriteRender m_Opchoice;					//�L���������̃Z���N�g�摜

	EnSelectPosition m_tSelectPosition;			//�Z���N�g�̃|�W�V����
	EnOperationLook m_operationLook;			//��������摜�̕\��
	EnCharacterOpLook m_characterOpLook;		//�L�����N�^�[�����̕\��
	EnCharacterOpPosition m_characterOpPosition;//�L�����N�^�[�����̃|�W�V����

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { g_vec3Zero };		//�Z���N�g�̃|�W�V����
	Vector3 m_firstPosition = { 0.0f,2000.0f,0.0f };	//�ǂݍ��ނƂ��̃|�W�V����
	Vector3 m_opPosition = { g_vec3Zero };			//�摜��\������|�W�V����

	Vector3 m_TopPosition = { 0.0f,-150.0f,0.0f };			//��
	Vector3 m_CentralPosition = { 0.0f,-250.0f,0.0f };		//����
	Vector3 m_UnderPosition = { 0.0f,-350.0f,0.0f };		//��

	int selectPosition = 0;
	int characterOpPosition = 0;
};

