#pragma once

#define PLAYER 4
#define MOVE 7

class Tittle;

class Result :public IGameObject
{
public:
	Result();
	~Result();
	void Update();
	bool Start();
	void Render(RenderContext& rc);
	void Rank();
	void Move();
	void Select();

	//Result�̃V�[���ϊ�
	enum EnChange {
		enChange_first,		//�ŏ��̏���
		enChange_move,		//���`�⊮��
		enChange_stop,		//�I�������
		enChange_Num
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
		char Name[30];		//�v���C���[�l�[��
		int Rank = 1;		//����
	};

private:
	int charPoints[PLAYER];		//�v���C���[�̃|�C���g
	int titleScene = 1;			//�^�C�g���̃V�[���ԍ�
	int m_timer = 0;			//�^�C�}�[
	int select = 0;				//�Z���N�g�|�W
	float Complement = 0.01f;	//���`��Ԃ̕⊮��
	float WordScale = 3.0f;

	char playername1[30] = "Player1";
	char playername2[30] = "CPU1";
	char playername3[30] = "CPU2";
	char playername4[30] = "CPU3";

	Score Player[PLAYER];

	FontRender m_PlayerRank1;
	FontRender m_PlayerRank2;
	FontRender m_PlayerRank3;
	FontRender m_PlayerRank4;
	EnChange m_change;
	EnCursorPos m_cursor;

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;	//�w�i
	SpriteRender m_GOtitle;			//"�^�C�g���֖߂�"
	SpriteRender m_gameover;		//"�Q�[���I��"
	SpriteRender m_choice;			//�I���J�[�\��
	Tittle* tittle = nullptr;

	//���`��Ԃł����܂œ�����
	Vector3 RankPos[MOVE] = {
		Vector3(-250.0f, 500.0f, 0.0f),		//���ʁA�P��
		Vector3(-250.0f, 350.0f, 0.0f),		//�Q��
		Vector3(-250.0f, 200.0f, 0.0f),		//�R��
		Vector3(-250.0f, 50.0f, 0.0f),		//�S��
		Vector3(-500.0f, -350.0f, 0.0f),	//"�^�C�g���ɖ߂�"
		Vector3(525.0f, -350.0f, 0.0f),		//"�Q�[�����I��"
		Vector3(-775.0f, -370.0f, 0.0f),	//�I���J�[�\��
	};

	//���`��Ԃœ������Ƃ��̃|�W�V����
	Vector3 MovePos[MOVE] = {
		Vector3(0.0f, 0.0f, 0.0f),		//���ʁA�P��
		Vector3(0.0f, 0.0f, 0.0f),		//�Q��
		Vector3(0.0f, 0.0f, 0.0f),		//�R��
		Vector3(0.0f, 0.0f, 0.0f),		//�S��
		Vector3(0.0f, 0.0f, 0.0f),		//"�^�C�g���ɖ߂�"
		Vector3(0.0f, 0.0f, 0.0f),		//"�Q�[�����I��"
		Vector3(0.0f, 0.0f, 0.0f),		//�I���J�[�\��
	};

	//��ʊO
	Vector3 FirstPos[MOVE] = {
		Vector3(-250.0f, -1000.0f, 0.0f),	//���ʁA�P��
		Vector3(-250.0f, -1000.0f, 0.0f),	//�Q��
		Vector3(-250.0f, -1000.0f, 0.0f),	//�R��
		Vector3(-250.0f, -1000.0f, 0.0f),	//�S��
		Vector3(-250.0f, -1000.0f, 0.0f),	//"�^�C�g���ɖ߂�"
		Vector3(-250.0f, -1000.0f, 0.0f),	//"�Q�[�����I��"
		Vector3(-250.0f, -1000.0f, 0.0f),	//�I���J�[�\��
	};

	//�J�[�\�������ɂ��炷Vector
	Vector3 Leftcursor = { -230.0f,-20.0f,0.0f };
};