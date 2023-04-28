#pragma once

#define PLAYER 4

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

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	Tittle* tittle = nullptr;

	//�v���C���[�̔ԍ�
	enum EnChange {
		enChange_first,		//�ŏ��̏���
		enChange_move,		//���`�⊮��
		enChange_stop,		//�I�������
		enChange_Num
	};

	//����
	enum EnRankPoint {
		enRankPoint_1st = 1,	//�P��
		enRankPoint_2st,		//�Q��
		enRankPoint_3st,		//�R��
		enRankPoint_4st,		//�S��
		enRankPoint_Num,
	};
	
	//�e�v���C���[�̃|�C���g��ۑ����邽�߂̂���
	struct Score {
		int Point;
		int NameNum;
		int Rank = 1;
	};

private:
	int charPoints[PLAYER];		//�v���C���[�̃|�C���g
	int titleScene = 1;			//�^�C�g���̃V�[���ԍ�
	int m_timer = 0;			//�^�C�}�[
	float Complement = 0.01f;	//���`��Ԃ̕⊮��
	float scale = 3.0f;

	Score Player[4];

	FontRender PlayerRank1;
	FontRender PlayerRank2;
	FontRender PlayerRank3;
	FontRender PlayerRank4;
	EnRankPoint m_rankPoint;
	EnChange m_change;

	//���`��Ԃł����܂œ�����
	Vector3 RankPos[4] = {
		Vector3(-300.0f, 500.0f, 0.0f),
		Vector3(-300.0f, 350.0f, 0.0f),
		Vector3(-300.0f, 200.0f, 0.0f),
		Vector3(-300.0f, 50.0f, 0.0f),
	};

	//���`��Ԃœ������Ƃ��̃|�W�V����
	Vector3 MovePos[4] = {
		Vector3(0.0f, 200.0f, 0.0f),
		Vector3(0.0f, 100.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, -100.0f, 0.0f),
	};

	//��ʊO
	Vector3 FirstPos[4] = {
		Vector3(0.0f, -1000.0f, 0.0f),
		Vector3(0.0f, -1000.0f, 0.0f),
		Vector3(0.0f, -1000.0f, 0.0f),
		Vector3(0.0f, -1000.0f, 0.0f),
	};
};