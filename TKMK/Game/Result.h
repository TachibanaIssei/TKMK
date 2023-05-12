#pragma once

#define PLAYER 4
#define MOVE 8

class Tittle;
class Fade;

class Result :public IGameObject
{
public:
	Result();
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//Result�̃V�[���ϊ�
	enum EnChange {
		enChange_first,		//�ŏ��̏���
		enChange_move,		//���`�⊮��
		enChange_stop,		//�I�������
		enChange_Num,
	};

	//�J�[�\���̏ꏊ
	enum EnCursorPos {
		enCursorPos_title,	//�^�C�g���ɖ߂�̏ꏊ
		enCursorPos_exit,	//�Q�[���I���̏ꏊ
		enCursorPos_Num,
	};
	
	//�e�v���C���[�̃X�R�A��ۑ����邽�߂̂���
	struct Score {
		int Point = 0;		//�l���|�C���g
		int NameNum = 1;	
		int Rank = 1;		//����
	};

private:
	void InitSprite();
	void Rank();
	void Move();
	void Select();
	void NameMove();


private:
	int charPoints[PLAYER];		//�v���C���[�̃|�C���g
	int titleScene = 1;			//�^�C�g���̃V�[���ԍ�
	int m_timer = 0;			//�^�C�}�[
	int select = 0;				//�Z���N�g�|�W
	float Complement = 0.0f;	//���`��Ԃ̕⊮��
	float WordScale = 3.0f;
	float BGMVolume = 0.4f;
	float SEVolume = 0.6f;

	Score Player[PLAYER];

	ModelRender m_No1Charctar;		//��ʂ̃L�����̃��f��

	FontRender m_PlayerRank1;
	FontRender m_PlayerRank2;
	FontRender m_PlayerRank3;
	FontRender m_PlayerRank4;
	EnChange m_change;
	EnCursorPos m_cursor;

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;	//�w�i
	SpriteRender m_GOtitle;			//"�^�C�g���֖߂�"��I��
	SpriteRender m_GOtitleST;		//"�^�C�g���֖߂�"�I��
	SpriteRender m_gameover;		//"�Q�[���I��"��I��
	SpriteRender m_gameoverST;		//"�^�C�g���֖߂�"�I��
	SpriteRender m_Ranking;			//���ʕ\
	SpriteRender m_PlayerNameP;		//"Player"���O���Ԃ肾����ς���
	SpriteRender m_CPUName1;		//CPU1
	SpriteRender m_CPUName2;		//CPU2
	SpriteRender m_CPUName3;		//CPU3
	SpriteRender m_ResultLogo;		//���U���g�̃��S

	SpriteRender m_choiceCursor;	//�I�����̃J�[�\��


	Tittle* tittle = nullptr;
	SoundSource* m_bgm = nullptr;
	Fade* fade = nullptr;

	Vector3 PointRight = { 200.0f,85.0f,0.0f };

	//���`��Ԃł����܂œ�����
	Vector3 RankPos[MOVE] = {
		Vector3(0.0f, 160.0f, 0.0f),		//���ʁA�P��
		Vector3(0.0f, 20.0f, 0.0f),			//�Q��
		Vector3(0.0f, -120.0f, 0.0f),		//�R��
		Vector3(0.0f, -255.0f, 0.0f),		//�S��
		Vector3(-500.0f, -450.0f, 0.0f),	//"�^�C�g���ɖ߂�"
		Vector3(525.0f, -450.0f, 0.0f),		//"�Q�[�����I��"
		Vector3(0.0f, -30.0f, 0.0f),		//���ʕ\
		Vector3(0.0f, 400.0f,0.0f),		//���U���g�̃��S
	};

	//���`��Ԃœ������Ƃ��̃|�W�V����
	Vector3 MovePos[MOVE] = {
		Vector3(g_vec3Zero),		//���ʁA�P��
		Vector3(g_vec3Zero),		//�Q��
		Vector3(g_vec3Zero),		//�R��
		Vector3(g_vec3Zero),		//�S��
		Vector3(g_vec3Zero),		//"�^�C�g���ɖ߂�"
		Vector3(g_vec3Zero),		//"�Q�[�����I��"
		Vector3(g_vec3Zero),		//���ʕ\
		Vector3(g_vec3Zero),		//���U���g�̃��S
	};

	//��ʊO
	Vector3 FirstPos[MOVE] = {
		Vector3(0.0f, -1000.0f, 0.0f),	//���ʁA�P��
		Vector3(0.0f, -1000.0f, 0.0f),	//�Q��
		Vector3(0.0f, -1000.0f, 0.0f),	//�R��
		Vector3(0.0f, -1000.0f, 0.0f),	//�S��
		Vector3(0.0f, -1000.0f, 0.0f),	//"�^�C�g���ɖ߂�"
		Vector3(0.0f, -1000.0f, 0.0f),	//"�Q�[�����I��"
		Vector3(0.0f, -1000.0f, 0.0f),	//���ʕ\
		Vector3(0.0f, -1000.0f, 0.0f),	//���U���g�̃��S
	};

	//�����x
	Vector4 m_color = { 0.0f,0.0f,0.0f,0.0f };	//����
	Vector4 m_colorST = { g_vec4White };		//�s����
};