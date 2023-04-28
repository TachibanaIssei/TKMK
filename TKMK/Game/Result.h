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

	//プレイヤーの番号
	enum EnChange {
		enChange_first,		//最初の処理
		enChange_move,		//線形補完中
		enChange_stop,		//終わった後
		enChange_Num
	};

	//順位
	enum EnRankPoint {
		enRankPoint_1st = 1,	//１位
		enRankPoint_2st,		//２位
		enRankPoint_3st,		//３位
		enRankPoint_4st,		//４位
		enRankPoint_Num,
	};
	
	//各プレイヤーのポイントを保存するためのもの
	struct Score {
		int Point;
		int NameNum;
		int Rank = 1;
	};

private:
	int charPoints[PLAYER];		//プレイヤーのポイント
	int titleScene = 1;			//タイトルのシーン番号
	int m_timer = 0;			//タイマー
	float Complement = 0.01f;	//線形補間の補完率
	float scale = 3.0f;

	Score Player[4];

	FontRender PlayerRank1;
	FontRender PlayerRank2;
	FontRender PlayerRank3;
	FontRender PlayerRank4;
	EnRankPoint m_rankPoint;
	EnChange m_change;

	//線形補間でここまで動かす
	Vector3 RankPos[4] = {
		Vector3(-300.0f, 500.0f, 0.0f),
		Vector3(-300.0f, 350.0f, 0.0f),
		Vector3(-300.0f, 200.0f, 0.0f),
		Vector3(-300.0f, 50.0f, 0.0f),
	};

	//線形補間で動かすときのポジション
	Vector3 MovePos[4] = {
		Vector3(0.0f, 200.0f, 0.0f),
		Vector3(0.0f, 100.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, -100.0f, 0.0f),
	};

	//画面外
	Vector3 FirstPos[4] = {
		Vector3(0.0f, -1000.0f, 0.0f),
		Vector3(0.0f, -1000.0f, 0.0f),
		Vector3(0.0f, -1000.0f, 0.0f),
		Vector3(0.0f, -1000.0f, 0.0f),
	};
};