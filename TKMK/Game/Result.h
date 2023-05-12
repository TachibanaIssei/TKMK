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

	//Resultのシーン変換
	enum EnChange {
		enChange_first,		//最初の処理
		enChange_move,		//線形補完中
		enChange_stop,		//終わった後
		enChange_Num,
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
		int Rank = 1;		//順位
	};

private:
	void InitSprite();
	void Rank();
	void Move();
	void Select();
	void NameMove();


private:
	int charPoints[PLAYER];		//プレイヤーのポイント
	int titleScene = 1;			//タイトルのシーン番号
	int m_timer = 0;			//タイマー
	int select = 0;				//セレクトポジ
	float Complement = 0.0f;	//線形補間の補完率
	float WordScale = 3.0f;
	float BGMVolume = 0.4f;
	float SEVolume = 0.6f;

	Score Player[PLAYER];

	ModelRender m_No1Charctar;		//一位のキャラのモデル

	FontRender m_PlayerRank1;
	FontRender m_PlayerRank2;
	FontRender m_PlayerRank3;
	FontRender m_PlayerRank4;
	EnChange m_change;
	EnCursorPos m_cursor;

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;	//背景
	SpriteRender m_GOtitle;			//"タイトルへ戻る"非選択
	SpriteRender m_GOtitleST;		//"タイトルへ戻る"選択
	SpriteRender m_gameover;		//"ゲーム終了"非選択
	SpriteRender m_gameoverST;		//"タイトルへ戻る"選択
	SpriteRender m_Ranking;			//順位表
	SpriteRender m_PlayerNameP;		//"Player"名前かぶりだから変える
	SpriteRender m_CPUName1;		//CPU1
	SpriteRender m_CPUName2;		//CPU2
	SpriteRender m_CPUName3;		//CPU3
	SpriteRender m_ResultLogo;		//リザルトのロゴ

	SpriteRender m_choiceCursor;	//選択時のカーソル


	Tittle* tittle = nullptr;
	SoundSource* m_bgm = nullptr;
	Fade* fade = nullptr;

	Vector3 PointRight = { 200.0f,85.0f,0.0f };

	//線形補間でここまで動かす
	Vector3 RankPos[MOVE] = {
		Vector3(0.0f, 160.0f, 0.0f),		//順位、１位
		Vector3(0.0f, 20.0f, 0.0f),			//２位
		Vector3(0.0f, -120.0f, 0.0f),		//３位
		Vector3(0.0f, -255.0f, 0.0f),		//４位
		Vector3(-500.0f, -450.0f, 0.0f),	//"タイトルに戻る"
		Vector3(525.0f, -450.0f, 0.0f),		//"ゲームを終了"
		Vector3(0.0f, -30.0f, 0.0f),		//順位表
		Vector3(0.0f, 400.0f,0.0f),		//リザルトのロゴ
	};

	//線形補間で動かすときのポジション
	Vector3 MovePos[MOVE] = {
		Vector3(g_vec3Zero),		//順位、１位
		Vector3(g_vec3Zero),		//２位
		Vector3(g_vec3Zero),		//３位
		Vector3(g_vec3Zero),		//４位
		Vector3(g_vec3Zero),		//"タイトルに戻る"
		Vector3(g_vec3Zero),		//"ゲームを終了"
		Vector3(g_vec3Zero),		//順位表
		Vector3(g_vec3Zero),		//リザルトのロゴ
	};

	//画面外
	Vector3 FirstPos[MOVE] = {
		Vector3(0.0f, -1000.0f, 0.0f),	//順位、１位
		Vector3(0.0f, -1000.0f, 0.0f),	//２位
		Vector3(0.0f, -1000.0f, 0.0f),	//３位
		Vector3(0.0f, -1000.0f, 0.0f),	//４位
		Vector3(0.0f, -1000.0f, 0.0f),	//"タイトルに戻る"
		Vector3(0.0f, -1000.0f, 0.0f),	//"ゲームを終了"
		Vector3(0.0f, -1000.0f, 0.0f),	//順位表
		Vector3(0.0f, -1000.0f, 0.0f),	//リザルトのロゴ
	};

	//透明度
	Vector4 m_color = { 0.0f,0.0f,0.0f,0.0f };	//透明
	Vector4 m_colorST = { g_vec4White };		//不透明
};