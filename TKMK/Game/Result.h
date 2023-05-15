#pragma once

#include <array>

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

	float m_complement = 0.0f;	//線形補間の補完率
	float WORD_SCALE = 3.0f;
	float m_bgmVolume = 0.4f;
	float m_seVolume = 0.6f;

	bool m_selectFlag			= false;	//選択項目の切り替えフラグ
	bool m_drawSelectSpriteFlag = false;	//選択項目の表示のフラグ

	Score m_playerScore[PLAYER];

	ModelRender m_No1Charctar;		//一位のキャラのモデル

	FontRender	m_playerRank1;
	FontRender	m_playerRank2;
	FontRender	m_playerRank3;
	FontRender	m_playerRank4;
	EnChange	m_change = enChange_first;
	EnCursorPos m_cursor;

	SpriteRender	m_spriteRender;		//背景
	SpriteRender	m_goTitle;			//"タイトルへ戻る"非選択
	SpriteRender	m_goTitleSelect;	//"タイトルへ戻る"選択
	SpriteRender	m_gameover;			//"ゲーム終了"非選択
	SpriteRender	m_gameoverST;		//"タイトルへ戻る"選択
	SpriteRender	m_playerName;		//"Player"名前かぶりだから変える
	SpriteRender	m_cpuName1;			//CPU1
	SpriteRender	m_cpuName2;			//CPU2
	SpriteRender	m_cpuName3;			//CPU3
	SpriteRender	m_resultLogo;		//リザルトのロゴ

	SpriteRender m_choiceCursor;		//選択時のカーソル

	Tittle* tittle = nullptr;
	SoundSource* m_bgm = nullptr;
	Fade* fade = nullptr;

	Vector3 PointRight = { 200.0f,85.0f,0.0f };

	//線形補間でここまで動かす
	std::array<Vector3,MOVE> m_lerpMoveEnd = {		//順位
		Vector3(0.0f, 160.0f, 0.0f),		//１位
		Vector3(0.0f, 20.0f, 0.0f),			//２位
		Vector3(0.0f, -120.0f, 0.0f),		//３位
		Vector3(0.0f, -255.0f, 0.0f)		//４位
	};

	//線形補間で動かすときの座標
	std::array<Vector3, MOVE> m_lerpMoving = {		//順位
		Vector3(g_vec3Zero),		//１位
		Vector3(g_vec3Zero),		//２位
		Vector3(g_vec3Zero),		//３位
		Vector3(g_vec3Zero)			//４位
	};

	//線形補間前の座標
	std::array<Vector3, MOVE> m_lerpStartPos = {	//順位
		Vector3(-2500.0f, 160.0f, 0.0f),	//１位
		Vector3(-2500.0f, 20.0f, 0.0f),		//２位
		Vector3(-2500.0f, -120.0f, 0.0f),	//３位
		Vector3(-2500.0f, -255.0f, 0.0f)	//４位
	};

	//透明度
	Vector4 m_alphaColorUnSelect	= { 0.0f,0.0f,0.0f,0.0f };	//透明
	Vector4 m_alphaColorSelect		= { g_vec4White };			//不透明
};