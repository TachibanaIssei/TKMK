#pragma once

#define PLAYER 4
#define MOVE 4

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
		enChange_1st = 0,
		enChange_2nd = 1,
		enChange_3rd = 2,
		enChange_4th = 3,
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
	void MoveLerp();
	void MovePointFont();
	void Select();
	void MoveName();


private:
	int charPoints[PLAYER];		//プレイヤーのポイント
	int titleScene = 1;			//タイトルのシーン番号
	int m_nowMoveRank = 0;
	int m_nowMoveCharacter = 0;
	int m_timer = 0;			//タイマー
	int select = 0;				//セレクトポジ
	float m_complement = 0.0f;	//線形補間の補完率
	float WordScale = 3.0f;
	float BGMVolume = 0.4f;
	float SEVolume = 0.6f;

	Score Player[PLAYER];

	ModelRender m_No1Charctar;		//一位のキャラのモデル

	FontRender m_PlayerRank1;
	FontRender m_PlayerRank2;
	FontRender m_PlayerRank3;
	FontRender m_PlayerRank4;
	EnChange m_change = enChange_first;
	EnCursorPos m_cursor;

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;	//背景
	SpriteRender m_GOtitle;			//"タイトルへ戻る"非選択
	SpriteRender m_GOtitleST;		//"タイトルへ戻る"選択
	SpriteRender m_gameover;		//"ゲーム終了"非選択
	SpriteRender m_gameoverST;		//"タイトルへ戻る"選択
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
		Vector3(0.0f, -255.0f, 0.0f)		//４位
	};

	//線形補間で動かすときのポジション
	Vector3 MovePos[MOVE] = {
		Vector3(g_vec3Zero),		//順位、１位
		Vector3(g_vec3Zero),		//２位
		Vector3(g_vec3Zero),		//３位
		Vector3(g_vec3Zero)			//４位
	};

	//画面外
	Vector3 FirstPos[MOVE] = {
		Vector3(-2500.0f, 160.0f, 0.0f),	//順位、１位
		Vector3(-2500.0f, 20.0f, 0.0f),	//２位
		Vector3(-2500.0f, -120.0f, 0.0f),	//３位
		Vector3(-2500.0f, -255.0f, 0.0f)	//４位
	};

	//透明度
	Vector4 m_color = { 0.0f,0.0f,0.0f,0.0f };	//透明
	Vector4 m_colorST = { g_vec4White };		//不透明
};