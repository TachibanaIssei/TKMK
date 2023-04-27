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

	//プレイヤーの番号
	enum EnPlayerNum {
		enPlayerNum_play,		//自分が操作していたキャラ
		enPlayerNum_enm1,		//エネミー１
		enPlayerNum_enm2,		//エネミー２
		enPlayerNum_enm3,		//エネミー３
		enPlayerNum_Num
	};

	//順位
	enum EnRankPoint {
		enRankPoint_1st,		//１位
		enRankPoint_2st,		//２位
		enRankPoint_3st,		//３位
		enRankPoint_4st,		//４位
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

