#pragma once

#include "../k2EngineLow/mySourceCode/Camera/SpringCamera.h"

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
		int m_point = 0;		//獲得ポイント
		int NameNum = 1;	
		int Rank = 1;		//順位
	};

private:
	void InitNameSprite();
	void InitSprite();
	void InitModel();
	void InitSkyCube();
	void PlayEffect();
	void SetCamera();
	void SetCharacterState();
	void Rank();
	void MoveLerp();
	void MovePointFont();
	void Select();
	void MoveName();
	void PlayAnimation();

	enum EnAnimationClip
	{
		enAnimationClip_Idle,
		enAnimationClip_Win,
		enAnimationClip_Lose,
		enAnimationClip_4th,
		enAnimationClip_Num
	};

	enum EnCharacterState
	{
		enCharacterState_Idle,
		enCharacterState_Win,
		enCharacterState_Lose,
		enCharacterState_4th,
		enCharacterState_Num
	};

private:
	const static int m_maxPlayer = 4;

	SpringCamera m_camera;
	SkyCube* m_skyCube;					//スカイキューブ
	std::array<ModelRender, m_maxPlayer> m_knightModel;	//剣士のモデル
	ModelRender m_backGround;			//地面
	ModelRender m_backWall;				//壁
	EffectEmitter* m_fireWorks;
	RenderingEngine::EnGameMode	m_gameMode;
	AnimationClip m_animationClips[enAnimationClip_Num];
	std::array<EnCharacterState, m_maxPlayer> m_charaState = { enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle };
	std::array<EnCharacterState, m_maxPlayer> m_stayCharaState = { enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle };

	std::array<int, m_maxPlayer> charPoints = {0,0,0,0};		//プレイヤーのポイント

	int titleScene = 1;			//タイトルのシーン番号
	int m_nowMoveRank = 0;
	int m_nowMoveCharacter = 0;
	int m_timer = 0;			//タイマー

	float m_complement = 0.0f;	//線形補間の補完率
	float m_bgmVolume = 0.4f;
	float m_seVolume = 1.0f;

	bool m_selectFlag			= false;	//選択項目の切り替えフラグ
	bool m_drawSelectSpriteFlag = false;	//選択項目の表示のフラグ
	bool m_fireWorksPlayFlag = false;		//花火エフェクト初期化フラグ

	bool m_isInit = false;

	std::array<Score, m_maxPlayer> m_playerScore;

	ModelRender m_No1Charctar;		//一位のキャラのモデル

	FontRender	m_playerRank1;
	FontRender	m_playerRank2;
	FontRender	m_playerRank3;
	FontRender	m_playerRank4;
	EnChange	m_change = enChange_first;
	EnCursorPos m_cursor;

	SpriteRender	m_spriteRender;		//背景
	SpriteRender	m_resultLogo;		//リザルトのロゴ
	SpriteRender	m_goTitle;			//"タイトルへ戻る"非選択
	SpriteRender	m_goTitleSelect;	//"タイトルへ戻る"選択
	SpriteRender	m_gameover;			//"ゲーム終了"非選択
	SpriteRender	m_gameoverST;		//"タイトルへ戻る"選択

	std::array<SpriteRender, 4> m_pointsUnit;	//"p"の画像
	std::array<SpriteRender, 4> m_knightFace;
	std::array<SpriteRender, 4> m_namePlate;	//名前の背景画像
	std::array<SpriteRender, 4> m_gameRank;		//順位の画像

	SpriteRender	m_playerName;		//"Player"
	SpriteRender	m_cpuName1;			//CPU1
	SpriteRender	m_cpuName2;			//CPU2
	SpriteRender	m_cpuName3;			//CPU3

	SpriteRender m_choiceCursor;		//選択時のカーソル

	Tittle* tittle = nullptr;
	SoundSource* m_bgm = nullptr;
	Fade* m_fade = nullptr;

	//フォントの線形補間前の座標
	std::array<Vector3, m_maxPlayer> m_lerpStartPos = {	//順位
		Vector3(-2100.0f, 300.0f, 0.0f),		//１位
		Vector3(-2100.0f, 145.0f, 0.0f),		//２位
		Vector3(-2100.0f, 0.0f, 0.0f),		//３位
		Vector3(-2100.0f, -145.0f, 0.0f)		//４位
	};

	//スプライトの線形補間前の座標
	std::array<Vector3, m_maxPlayer> m_spriteLerpStartPos = {	//順位
		Vector3(-2100.0f, 180.0f, 0.0f),		//１位
		Vector3(-2100.0f, 25.0f, 0.0f),			//２位
		Vector3(-2100.0f, -117.0f, 0.0f),		//３位
		Vector3(-2100.0f, -263.0f, 0.0f)		//４位
	};

	//フォントを線形補間でここまで動かす
	std::array<Vector3, m_maxPlayer> m_lerpMoveEnd = {		//順位
		Vector3(370.0f, 300.0f, 0.0f),		//１位
		Vector3(370.0f, 145.0f, 0.0f),		//２位
		Vector3(370.0f, 4.0f, 0.0f),		//３位
		Vector3(370.0f, -145.0f, 0.0f)		//４位
	};

	//スプライトを線形補間でここまで動かす
	std::array<Vector3, m_maxPlayer> m_spriteLerpMoveEnd = {		//順位
		Vector3(300.0f, 180.0f, 0.0f),		//１位
		Vector3(300.0f, 25.0f, 0.0f),		//２位
		Vector3(300.0f, -117.0f, 0.0f),		//３位
		Vector3(300.0f, -265.0f, 0.0f)		//４位
	};

	//フォントを線形補間で動かすときの座標
	std::array<Vector3, m_maxPlayer> m_lerpMoving = {		//順位
		Vector3(g_vec3Zero),		//１位
		Vector3(g_vec3Zero),		//２位
		Vector3(g_vec3Zero),		//３位
		Vector3(g_vec3Zero)			//４位
	};

	//スプライトを線形補間で動かすときの座標
	std::array<Vector3, m_maxPlayer> m_spriteLerpMoving = {		//順位
		Vector3(g_vec3Zero),		//１位
		Vector3(g_vec3Zero),		//２位
		Vector3(g_vec3Zero),		//３位
		Vector3(g_vec3Zero)			//４位
	};

	//透明度
	Vector4 m_alphaColorUnSelect	= { 0.0f,0.0f,0.0f,0.0f };	//透明
	Vector4 m_alphaColorSelect		= { g_vec4White };			//不透明
};