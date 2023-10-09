#pragma once

class Game;
class CharacterSelect;
class Fade;

class Tittle:public IGameObject
{
public:
	Tittle();
	~Tittle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void InitSprite();
	void ResistSound();
	void SpriteUpdate();
	void Scene();
	void Select();
	void Operation();
	void CharacterOp();
	void Ilust();
	void FadeSprite();

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
		enTitleScene_Change,						//���`�⊮�̃^�C�~���O
		enTitleScene_Select,						//��������Ȃǂ̉��
		enTitleScene_Num,
	};

	//���`��Ԃ̃^�C�~���O
	enum EnLinear {
		enLinear_sword1,								//���P
		enLinear_sword2,								//���Q
		enLinear_delay,									//�^�C�����O�����
		enLinear_Logo,									//�^�C�g�����S
		enLinear_End,									//�I���
		enLinear_Num,
	};

	//�ǂ���I�����Ă��邩
	enum EnSelectPosition {
		enSelectPosition_Start,						//�X�^�[�g�|�W�V����
		enSelectPosition_Operation,					//��������̃|�W�V����
		enSelectPosition_Option,					//�I�v�V�����̃|�W�V����
		enSelectPosition_Num,						//�Z���N�g�̐�
	};
	
	//���������ʂ������邩�����Ȃ���
	enum EnOperationLook {
		enOperationLook_UnSeem,						//�����Ȃ�
		enOperationLook_Seem,						//������
		enOperationLook_Num,
	};

	/// <summary>
	/// ���������ʂ̃y�[�W�̎��
	/// </summary>
	enum EnOperationPage {
		enOperationPage_nonGame = 0,	//�v���C�ȊO�̑������
		enOperationPage_InGame	= 1,		//�v���C���̑������
		enOperationPage_Ghost	= 2,		//�S�[�X�g�̐���
		enOperationpage_Num
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
	Fade* m_fade = nullptr;
	Quaternion m_sRotation;

	SpriteRender m_spriteRender;				//�^�C�g���w�i
	SpriteRender m_titleLogo;					//�^�C�g�����S
	SpriteRender m_start;						//"START"��I��
	SpriteRender m_startST;						//"START"�I��
	SpriteRender m_operation;					//"HOWTOPLAY"��I��
	SpriteRender m_operationST;					//"HOWTOPLAY"�I��
	SpriteRender m_option;						//"OPTION"��I��
	SpriteRender m_optionST;					//"OPTION"�I��
	SpriteRender m_fire;						//�Z���N�g
	SpriteRender m_operationPic;				//��������̉摜
	SpriteRender m_operationInGame;				//�Q�[�����̑���
	SpriteRender m_ghostExplanation;			//�S�[�X�g�̐����摜
	SpriteRender m_KnightOp;					//���m�̐����摜
	SpriteRender m_WizardOp;					//���@�g���̐����摜
	SpriteRender m_ZombieOp;					//�]���r�̐����摜
	SpriteRender m_Opchoice;					//�L���������̃Z���N�g�摜
	SpriteRender m_iluststart;					//"START"�̎��̃C���X�g
	SpriteRender m_iluststartOp;				//"START"�̎��̐�����
	SpriteRender m_ilusthowtoplay;				//"HOWTOPLAY"�̎��̃C���X�g
	SpriteRender m_ilusthowtoplayOp;			//"HOWTOPLAY"�̎��̐�����
	SpriteRender m_ilustoption;					//"OPTION"�̎��̃C���X�g
	SpriteRender m_ilustoptionOp;				//"OPTION"�̎��̐�����
	SpriteRender m_titleswordwhite;				//������
	SpriteRender m_titleswordbrack;				//������
	SpriteRender m_pressAButton;				//�v���XA�{�^��
	SpriteRender m_choiceCursor;				//�I�𒆂̂��̂��w�����摜

	EnTitleScene m_titleScene;					//�^�C�g���̃|�W�V����
	EnSelectPosition m_tSelectPosition;			//�Z���N�g�̃|�W�V����
	EnOperationLook m_operationLook;			//��������摜�̕\��
	EnOperationPage m_enOperationPage = EnOperationPage::enOperationpage_Num;	//���������ʂ̎��
	EnCharacterOpLook m_characterOpLook;		//�L�����N�^�[�����̕\��
	EnCharacterOpPosition m_characterOpPosition;//�L�����N�^�[�����̃|�W�V����
	EnLinear m_linear;

	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

	Vector3 m_sPosition = { g_vec3Zero };					//�Z���N�g�̃|�W�V����
	Vector3 m_firstPosition = { 0.0f,-2000.0f,0.0f };		//���ɓǂݍ��ނƂ��̃|�W�V����
	Vector3 m_Toprightfirstposition = { 1000.0f,2000.0f,0.0f };//�E��ɓǂݍ��ނƂ��̃|�W�V����
	Vector3 m_Topleftfirstposition = { -1000.0f,2000.0f,0.0f };	//����ɓǂݍ��ނƂ��̃|�W�V����
	Vector3 m_RightfirstPosition = { 2000.0f,0.0f,0.0f };	//�E�ɓǂݍ��ނƂ��̃|�W�V����
	Vector3 m_opPosition = { g_vec3Zero };					//�摜��\������|�W�V����

	Vector3 m_operationPosition = { 0.0f,-150.0f,0.0f };			//"HowToPlay"
	Vector3 m_startPosition = { 0.0f,-250.0f,0.0f };				//"Start"
	Vector3 m_optionPosition = { 0.0f,-350.0f,0.0f };				//"Option"

	const Vector3 m_titleLogoPosition = { -20.0f,160.0f,0.0f };				//�N�����̉�ʂ̃^�C�g�����S�̃|�W�V����
	const Vector3 m_selectLogoPosition = { -500.0f,400.0f,0.0f };			//�Z���N�g��ʂ̃^�C�g�����S�̃|�W�V����
	
	Vector3 m_firsttitleScale = {5.0f,5.0f,1.0f };					//�^�C�g�����S�̍ŏ��̑傫��
	Vector3 m_changeLogoScale = Vector3::Zero;
	Vector3 m_titleLogoScale = { 1.3f,1.1f,0.0f };						//�N�����̃^�C�g�����S�̑傫��
	Vector3 m_selectLogoScale = { 0.7f, 0.45f, 1.0f };				//�Z���N�g��ʂ̃^�C�g�����S�̑傫��
	Vector3 m_LogoPosition = Vector3::Zero;											//���S����`�⊮����Ƃ��Ɏg��
	Vector3 m_LogoScale = Vector3::Zero;
	Vector3 m_titlefireScale = { 1.0f, 1.0f, 1.0f };				//�N�����̉ΉԂ̑傫��
	Vector3 m_selectfireScale = { 0.6f, 0.6f, 1.0f };
	Vector3 m_fireScale = Vector3::Zero;											//�ΉԂ̐��`���
	Vector3 m_selectilust = { 500.0f,0.0f,0.0f };					//�Z���N�g��ʂ̃C���X�g�̏ꏊ
	Vector3 m_ilust = Vector3::Zero;												//�C���X�g�̐��`���
	Vector3 m_selectilustOp = { 500.0f,-400.0f,0.0f };				//�Z���N�g��ʂ̐������̏ꏊ
	Vector3 m_ilustOp = Vector3::Zero;												//�������̐��`���
	Vector3 m_swordright = Vector3::Zero;											//�������̐��`���
	Vector3 m_swordleft = Vector3::Zero;											//�������̐��`���
	Vector3 m_titleswordscale = { g_vec3One };						//�N�����̑傫��
	Vector3 m_selectswordscale = { 0.7f, 0.7f, 1.0f };				//�Z���N�g��ʂ̌��̑傫��
	Vector3 m_swordscale = Vector3::Zero;											//���̃X�P�[��
	Vector3 m_swordPosition = { 0.0f,220.0f,0.0f };

	//�Z���N�g�̍��ڂ��Ƃ̈ʒu
	Vector3 m_Top = { -500.0f,-130.0f,0.0f };						//��
	Vector3 m_Central = { -500.0f,-250.0f,0.0f };					//����
	Vector3 m_Under = {-500.0f,-370.0f,0.0f };						//��

	//�L�����I���̎��̃J�[�\���̈ʒu
	Vector3 m_KnightCursor = { -400.0f, 450.0f, 0.0f };
	Vector3 m_WizardCursor = { -130.0f, 450.0f, 0.0f };
	Vector3 m_ZombieCursor = { 145.0f, 450.0f, 0.0f };
	Vector3 m_MiteiCursor = { 425.0f, 450.0f, 0.0f };

	//���`��Ԍ�̃|�W�V���������ۑ�
	Vector3 m_RightPos;                 //�E�̌��̍��W
	Vector3 m_LeftPos;					//���̌��̍��W

	//Logo�̃|�W�V���������ɂ��炷
	Vector3 m_LeftLogo = { -10.0f,0.0f,0.0f };

	Vector4 m_color = { 1.0f,1.0f,1.0f,0.0f };		//�I������ĂȂ��Ƃ��ɓ����x��Ⴍ���ċ������Ȃ��悤�ɂ��鎞�Ɏg��
	Vector4 m_colorST = { g_vec4White };			//�ʏ�̓����x

	int m_operationPageNumber = 3;
	int selectPosition = 0;							//�^�C�g���̃Z���N�g��ʂŃJ�[�\�����ړ�����Ƃ��Ɏg���ϐ�
	int characterOpPosition = 0;					//�L����������ʂŃJ�[�\�����ړ�����Ƃ��Ɏg���ϐ�
	int titleScene = 0;								//�^�C�g���̃V�[���ϊ�����Ƃ��̕ϐ�
	float firstLogo = 0.0f;
	float LogoComplement = 0.04f;					//�⊮��
	float swordright = 0.0f;						//���̕⊮��(�E)

	float swordleft = 0.0f;							//���̕⊮��(��)
	int m_timer = 0;
	int fadetime = 0;
	int delaytime = 0;								//�x�点�鎞��
	float m_alpha = 0.0f;
	bool m_isWaitFadeout = false;					//PressAButton�̕\������Ԋu
	bool m_fadeSeem = true;							//PressAButton��\�����邩
	bool m_titlefadeSeem = false;					//�^�C�g����
	bool m_titleanim = false;						//�^�C�g���̃A�j���[�V��������
	float SEVolume = 0.4f;
	bool m_isComplement = true;						//���`��Ԓ��Htrue�Ȃ�⊮��
	bool GOBack = false;
};

