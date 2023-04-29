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

	//Resultのシーン変換
	enum EnChange {
		enChange_first,		//最初の処理
		enChange_move,		//線形補完中
		enChange_stop,		//終わった後
		enChange_Num
	};

	//カーソルの場所
	enum EnCursorPos {
		enCursorPos_title,	//タイトルに戻るの場所
		enCursorPos_exit,	//ゲーム終了の場所
		enCursorPos_Num,
	};
	
	//各プレイヤーのスコアを保存するためのもの
	struct Score {
		int Point = 0;		//獲得ポイント
		int NameNum = 1;	
		char Name[30];		//プレイヤーネーム
		int Rank = 1;		//順位
	};

private:
	int charPoints[PLAYER];		//プレイヤーのポイント
	int titleScene = 1;			//タイトルのシーン番号
	int m_timer = 0;			//タイマー
	int select = 0;				//セレクトポジ
	float Complement = 0.01f;	//線形補間の補完率
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
	SpriteRender m_spriteRender;	//背景
	SpriteRender m_GOtitle;			//"タイトルへ戻る"
	SpriteRender m_gameover;		//"ゲーム終了"
	SpriteRender m_choice;			//選択カーソル
	Tittle* tittle = nullptr;

	//線形補間でここまで動かす
	Vector3 RankPos[MOVE] = {
		Vector3(-250.0f, 500.0f, 0.0f),		//順位、１位
		Vector3(-250.0f, 350.0f, 0.0f),		//２位
		Vector3(-250.0f, 200.0f, 0.0f),		//３位
		Vector3(-250.0f, 50.0f, 0.0f),		//４位
		Vector3(-500.0f, -350.0f, 0.0f),	//"タイトルに戻る"
		Vector3(525.0f, -350.0f, 0.0f),		//"ゲームを終了"
		Vector3(-775.0f, -370.0f, 0.0f),	//選択カーソル
	};

	//線形補間で動かすときのポジション
	Vector3 MovePos[MOVE] = {
		Vector3(0.0f, 0.0f, 0.0f),		//順位、１位
		Vector3(0.0f, 0.0f, 0.0f),		//２位
		Vector3(0.0f, 0.0f, 0.0f),		//３位
		Vector3(0.0f, 0.0f, 0.0f),		//４位
		Vector3(0.0f, 0.0f, 0.0f),		//"タイトルに戻る"
		Vector3(0.0f, 0.0f, 0.0f),		//"ゲームを終了"
		Vector3(0.0f, 0.0f, 0.0f),		//選択カーソル
	};

	//画面外
	Vector3 FirstPos[MOVE] = {
		Vector3(-250.0f, -1000.0f, 0.0f),	//順位、１位
		Vector3(-250.0f, -1000.0f, 0.0f),	//２位
		Vector3(-250.0f, -1000.0f, 0.0f),	//３位
		Vector3(-250.0f, -1000.0f, 0.0f),	//４位
		Vector3(-250.0f, -1000.0f, 0.0f),	//"タイトルに戻る"
		Vector3(-250.0f, -1000.0f, 0.0f),	//"ゲームを終了"
		Vector3(-250.0f, -1000.0f, 0.0f),	//選択カーソル
	};

	//カーソルを左にずらすVector
	Vector3 Leftcursor = { -230.0f,-20.0f,0.0f };
};