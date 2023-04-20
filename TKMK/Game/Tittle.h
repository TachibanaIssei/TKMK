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
	void Scene();
	void Select();
	void Operation();
	void CharacterOp();

	/// <summary>
	/// �L�����I����ʂ���^�C�g���ɖ߂������Ƀ^�C�g���̃Z���N�g��ʂɖ߂��֐�
	/// </summary>
	/// <param name="scene">�^�C�g���̃V�[���@0.PlessA��� 1.�^�C�g���Z���N�g���</param>
	void SetTitleScene(int scene)
	{
		titleScene = scene;
	}

	//�ǂ̉�ʂ��\������Ă��邩
	enum EnTitleScene {
		enTitleScene_PressAScene,					//�ŏ���pressA�{�^���̉��
		enTitleScene_Select,						//��������Ȃǂ̉��
		enTitleScene_Num,
	};

	//�J�[�\�����ǂ��ɕ\������Ă��邩
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

	SpriteRender m_spriteRender;				//�^�C�g���w�i
	SpriteRender m_titleLogo;					//�^�C�g�����S
	SpriteRender m_start;						//"�͂��߂�"
	SpriteRender m_operation;					//"�������"
	SpriteRender m_charaExplanation;			//"�L�����N�^�[����"
	SpriteRender m_choice;						//�Z���N�g
	SpriteRender m_operationPic;				//��������̉摜
	SpriteRender m_KnightOp;					//���m�̐����摜
	SpriteRender m_WizardOp;					//���@�g���̐����摜
	SpriteRender m_ZombieOp;					//�]���r�̐����摜
	SpriteRender m_Opchoice;					//�L���������̃Z���N�g�摜


	EnTitleScene m_titleScene;			//�^�C�g���̃|�W�V����
	EnSelectPosition m_tSelectPosition;			//�Z���N�g�̃|�W�V����
	EnOperationLook m_operationLook;			//��������摜�̕\��
	EnCharacterOpLook m_characterOpLook;		//�L�����N�^�[�����̕\��
	EnCharacterOpPosition m_characterOpPosition;//�L�����N�^�[�����̃|�W�V����

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { g_vec3Zero };				//�Z���N�g�̃|�W�V����
	Vector3 m_firstPosition = { 0.0f,-2000.0f,0.0f };	//�ǂݍ��ނƂ��̃|�W�V����
	Vector3 m_opPosition = { g_vec3Zero };				//�摜��\������|�W�V����

	Vector3 m_operationPosition = { 0.0f,-150.0f,0.0f };			//"�������"
	Vector3 m_startPosition = { 0.0f,-250.0f,0.0f };				//"�͂��߂�"
	Vector3 m_charaExplanationPosition = { 0.0f,-350.0f,0.0f };		//"�L�����N�^�[����"

	const Vector3 m_titleLogoPosition = { 0.0f,200.0f,0.0f };				//�N�����̉�ʂ̃^�C�g�����S�̃|�W�V����
	const Vector3 m_selectLogoPosition = { -500.0f,400.0f,0.0f };			//�Z���N�g��ʂ̃^�C�g�����S�̃|�W�V����
	
	Vector3 m_titleLogoScale = { 3.0f, 1.5f, 1.0f };				//�N�����̃^�C�g�����S�̑傫��
	Vector3 m_selectLogoScale = { 1.7f, 0.7f, 1.0f };				//�Z���N�g��ʂ̃^�C�g�����S�̑傫��
	Vector3 m_LogoPosition;											//���S����`�⊮����Ƃ��Ɏg��
	Vector3 m_LogoScale;

	//�Z���N�g�̍��ڂ��Ƃ̈ʒu
	Vector3 m_Top = { -500.0f,-150.0f,0.0f };						//��
	Vector3 m_Central = { -500.0f,-250.0f,0.0f };					//����
	Vector3 m_Under = {-500.0f,-350.0f,0.0f };						//��

	Vector4 m_color = { 1.0f,1.0f,1.0f,0.3f };		//�I������ĂȂ��Ƃ��ɓ����x���������ċ������Ȃ��悤�ɂ��鎞�Ɏg��

	int selectPosition = 0;							//�^�C�g���̃Z���N�g��ʂŃJ�[�\�����ړ�����Ƃ��Ɏg���ϐ�
	int characterOpPosition = 0;					//�L����������ʂŃJ�[�\�����ړ�����Ƃ��Ɏg���ϐ�
	int titleScene = 0;								//�^�C�g���̃V�[���ϊ�����Ƃ��̕ϐ�
	float LogoComplement = 0.01f;					//�⊮��
	int m_timer = 0;
};

