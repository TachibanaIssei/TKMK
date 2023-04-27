#pragma once

#define PLAYER 4

class Tittle;

class Result:public IGameObject
{
public:
	Result();
	~Result();
	void Update();
	bool Start();
	void Render(RenderContext& rc);
	void Rank();

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	Tittle* tittle=nullptr;

	//�v���C���[�̔ԍ�
	enum EnPlayerNum {
		enPlayerNum_play,		//���������삵�Ă����L����
		enPlayerNum_enm1,		//�G�l�~�[�P
		enPlayerNum_enm2,		//�G�l�~�[�Q
		enPlayerNum_enm3,		//�G�l�~�[�R
		enPlayerNum_Num
	};

	//����
	enum EnRankPoint {
		enRankPoint_1st,		//�P��
		enRankPoint_2st,		//�Q��
		enRankPoint_3st,		//�R��
		enRankPoint_4st,		//�S��
		enRankPoint_Num,
	};

	struct Score{
		int Point;
		int NameNum;
		int Rank =1;
	};

private:
	int charPoints[PLAYER];
	int titleScene = 1;
	int playerpoint[PLAYER];
	int tmp = 0;

	Score Player[4];

	FontRender PlayerRank1;
	FontRender PlayerRank2;
	FontRender PlayerRank3;
	FontRender PlayerRank4;
	EnRankPoint m_rankPoint;
	EnPlayerNum m_playerNum;
	
	Vector3 RankPos[4] = {
		Vector3(0.0f, 200.0f, 0.0f),
		Vector3(0.0f, 100.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, -100.0f, 0.0f),
	};
};

