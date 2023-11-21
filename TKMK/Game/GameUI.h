#pragma once

class Player;
class Game;
class Actor;
class Fade;
class ExpforKnight;
class KnightAI;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	enum EnPlayerNumber
	{
		enPlayerNumber_1P,
		enPlayerNumber_2P,
		enPlayerNumber_3P,
		enPlayerNumber_4P,
		enPlayerNumber_Num,
	};

	enum GameUIState
	{
		m_GameStartState,
		m_GameState,
		m_PauseState,
		m_BattleEndState,
	};
	GameUIState m_GameUIState;

	enum EnExpProssesState
	{
		enChackExpState,
		enUpExpState,
		enDownExpState,
		enLevelUpState,
		enLevelDownState
	};
	std::array<EnExpProssesState, enPlayerNumber_Num> m_enExpProcessState = { enChackExpState,enChackExpState,enChackExpState,enChackExpState };

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void ExpState(const Player* player);
	void ChackExp(const Player* player, const EnPlayerNumber playerNumber);
	void UpExp(const EnPlayerNumber playerNumber);
	void DownExp(const EnPlayerNumber playerNumber);
	void LevelUp(const Player* player, const EnPlayerNumber playerNumber);
	void LevelDown(const Player* player, const EnPlayerNumber playerNumber);

	void CpuExpState(KnightAI* knight);
	void CpuChackExp(KnightAI* knight, const EnPlayerNumber playerNumber);
	void CpuLevelUp(KnightAI* knight, const EnPlayerNumber playerNumber);
	void CpuLevelDown(KnightAI* knight, const EnPlayerNumber playerNumber);

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}

	void ChangeEXPUpFlag(bool flag)
	{
		m_expUpFlag[enPlayerNumber_1P] = flag;
		m_expUpFlag[enPlayerNumber_2P] = flag;
		m_expUpFlag[enPlayerNumber_3P] = flag;
		m_expUpFlag[enPlayerNumber_4P] = flag;
	}

	/// <summary>
	/// プレイヤーのレベルに合わせてレベルの画像を変更する
	/// </summary>
	void LevelSpriteChange(const int lv, const EnPlayerNumber playerNumber = enPlayerNumber_1P);

	/// <summary>
	/// HPバーの表示
    /// </summary>
	void HPBar(const Player* player);
	void CpuHpBar(KnightAI* character);

	/// <summary>
	/// AIのレベルの表示
	/// </summary>
	void Level();

	/// <summary>
	/// GamwUIのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetGameUIState(GameUIState gamescene) {
		m_GameUIState = gamescene;

	}

	void Timer();

	/// <summary>
	/// 
	/// </summary>
	void CountDown();

	/// <summary>
	/// 
	/// </summary>
	void RespawnCountDown(EnPlayerNumber playerNumber);

	/// <summary>
	/// 
	/// </summary>
	void CharPoint();

	/// <summary>
	/// スキルのアイコンをグレースケールにする
	/// </summary>
	/// <param name="flag">trueの場合グレースケールにする</param>
	void SetSkillIconGrayScale(bool flag, EnPlayerNumber playerNumber)
	{
		m_SkillRenderIN[playerNumber].SetGrayScale(flag);
	}

	/// <summary>
	/// 必殺のアイコンをグレースケールにする
	/// </summary>
	/// <param name="flag">trueの場合グレースケールにする</param>
	void SetULTIconGrayScale(bool flag, EnPlayerNumber playerNumber)
	{
		m_UltRenderIN[playerNumber].SetGrayScale(flag);
	}

private:
	/// <summary>
	/// 画像と文字の初期化
	/// </summary>
	void InitAssets();

	/// <summary>
	//	スキルのクールタイムを表示する文字を設定
	/// </summary>
	void SkillCoolTimeFont();
	
	/// <summary>
	/// ポイント関連のUIの初期化
	/// </summary>
	void InitPointUI();

	/// <summary>
	/// リスポーン関連のUIの初期化
	/// </summary>
	void InitRespawnUI();

	/// <summary>
	/// 右下のUI.レベルや経験値バーのUI
	/// </summary>
	void InitExpelienceUI();

	/// <summary>
	/// HP関連のUIの初期化
	/// </summary>
	void InitHpUI();

	/// <summary>
	/// タイマー関連のUIの初期化
	/// </summary>
	void InitTimerUI();

	/// <summary>
	/// プレイヤー死亡時のDraw呼び出し
	/// </summary>
	void RenderDeathPlayerSprite(RenderContext& rc);

	/// <summary>
	/// クールタイムのフォントのDraw呼び出し
	/// </summary>
	void RenderCoolTimeFont(RenderContext& rc);

	/// <summary>
	/// 制限時間関連のDraw呼び出し
	/// </summary>
	void RenderTimeLimit(RenderContext& rc);

	/// <summary>
	/// ポイント関連のDraw呼び出し
	/// </summary>
	void RenderPoint(RenderContext& rc);
	
	/// <summary>
	/// 経験値関連のDraw呼び出し
	/// </summary>
	void RenderExpelience(RenderContext& rc);

	/// <summary>
	/// HP関連のDraw呼び出し
	/// </summary>
	void RenderHp(RenderContext& rc);

	/// <summary>
	/// スキルと必殺技のアイコンのDraw呼び出し
	/// </summary>
	/// <param name="rc"></param>
	void RenderIcon(RenderContext& rc);
private:
	FontRender m_ExpFont;

	std::array<FontRender,enPlayerNumber_Num> m_skillFont;
	std::array<FontRender,enPlayerNumber_Num> m_HpFont;

	std::array < FontRender, 4> m_PointFont;
	std::array < FontRender, 4> m_LevelFont;
	std::array <SpriteRender, 4> m_playerCpuName;	//プレイヤーとCPUの名前画像

	Player* m_player1P = nullptr;
	Player* m_player2P = nullptr;
	Player* m_player3P = nullptr;
	Player* m_player4P = nullptr;
	KnightAI* m_knightAI = nullptr;
	Game* m_game = nullptr;
	Actor* actor = nullptr;
	Fade* m_fade = nullptr;
	ExpforKnight* expforKnight = nullptr;

	std::vector<Actor*> m_Actors;

	//UI
	std::array<SpriteRender,enPlayerNumber_Num>		m_hpBar;		//HPバーの画像
	std::array<SpriteRender, enPlayerNumber_Num>	m_HpBar_White;	//HPバーの白い部分
	std::array<SpriteRender, enPlayerNumber_Num>	m_hpBarBack;	//ステータスの画像
	std::array<SpriteRender, enPlayerNumber_Num>	m_HPFrame;		//HPバーのフレーム
	SpriteRender			m_playerFaceBack;					//プレイヤーの顔画像の背景
	std::array<SpriteRender, enPlayerNumber_Num>    m_SkillRenderIN;	//スキルのアイコン
	std::array<SpriteRender, enPlayerNumber_Num>    m_SkillRenderOUT;	//スキルのアイコンのフレーム
	std::array<SpriteRender, enPlayerNumber_Num>    m_UltRenderIN;		//必殺アイコン
	std::array<SpriteRender, enPlayerNumber_Num>    m_UltRenderOUT;		//必殺アイコンのフレーム
	SpriteRender            m_TimeAndPointRender;				//制限時間と獲得ポイント
	std::array<SpriteRender, enPlayerNumber_Num>	m_Lv;
	std::array<SpriteRender, enPlayerNumber_Num>	m_LvNumber;
	std::array<SpriteRender, enPlayerNumber_Num>	m_LvNumber_back;							//レベルの裏
	float					m_LvNumberColor = 1.0f;
	bool					m_flashNumberFlag = false;
	std::array<SpriteRender, enPlayerNumber_Num>	m_MaxLv;
	std::array<SpriteRender, enPlayerNumber_Num>	m_Flame;		//制限時間と獲得ポイントやHPバーの画像を
	SpriteRender            m_Point;							//ポイント
	std::array<SpriteRender,enPlayerNumber_Num>     m_ExperienceFlame;					//経験値のフレーム
	std::array<SpriteRender, enPlayerNumber_Num>	m_experienceBarFlont;				//経験値バーの表
	std::array<SpriteRender, enPlayerNumber_Num>	m_experienceBarBack;				//経験値バーの裏
	SpriteRender			m_Crown;							//ポイントが一番多いキャラにつける王冠マーク
	std::array<SpriteRender,4>	m_pointFlame;					//ポイントを表示するフレーム
	std::array<SpriteRender, 4> m_charIcon;                      //キャラのアイコン
	SpriteRender			m_CountNumper;						//カウントダウン
	std::array<SpriteRender, enPlayerNumber_Num>	m_respawnIn;						//Respawn inの画像
	std::array<SpriteRender, enPlayerNumber_Num>	m_respawnBack;						//リスポーン時の背景
	std::array<SpriteRender, enPlayerNumber_Num>	m_respawnCountNumber;				//リスポーン時のカウントダウン
	SpriteRender			m_FinishCountNumber;				//制限時間残り10秒のカウントダウン

	Vector3				m_gameCountScale = Vector3(0.2f,0.2f,0.0f);
	Vector3				m_fightScale = Vector3(1.0f, 1.0f, 0.0f);
	bool				m_fightFlag = false;
	float				m_GameCountScale = 20.0f;
	float               m_Color = 1.0f;

	Vector3				m_finishCountScale = Vector3::One;
	
	std::array<Vector3, 4> CrownPos = {
		Vector3(-920.0f, 120.0f, 0.0f),
		Vector3(-920.0f, 10.0f, 0.0f),
		Vector3(-920.0f, -100.0f, 0.0f),
		Vector3(-920.0f, -210.0f, 0.0f),
	};															//王冠マーク

	FontRender m_minutes;
	FontRender m_seconds;
	FontRender m_coron;
	float m_timerScale = 1.65f;
	bool m_timerScaleFlag = false;
	Vector4 m_limitColor = { 1.0f,1.0f,1.0f,1.0f };

	const char* knightname = "knightplayer";
	const char* knightname2 = "knightplayer2";
	const char* knightname3 = "knightplayer3";
	const char* knightname4 = "knightplayer4";
	const char* wizardname = "wizardplayer";
	const char* KnightAI_Red = "KnightAI";
	const char* KnightAI_Green = "KnightAI1";
	const char* KnightAI_Yellow = "KnightAI2";

	int oldtStartCount = 0;

	std::array<int, enPlayerNumber_Num> oldRespawnCount = { 0, 0 };

	int oldFinishCount = 0;

	std::array<float, enPlayerNumber_Num> playerCoolTime = { 0, 0, 0, 0 };
	std::array<int, enPlayerNumber_Num>		m_playerLevel = { 1, 1, 1, 1};
	std::array<int, enPlayerNumber_Num>		m_mathExp = { 0,0,0,0 };			//経験値を増やしたり減らしたりする時はこれ
	std::array<int, enPlayerNumber_Num>		m_saveExp = { 0,0,0,0 };
	std::array<int, enPlayerNumber_Num>		m_expTable = { 0,0,0,0 };
	std::array<bool, enPlayerNumber_Num>	m_expUpFlag = { false,false,false,false };
	

	std::array<int, enPlayerNumber_Num>		m_oldSaveExp = { 0,0,0,0 };

	//キャラのポイント
	int charPoint[4];

	int MaxPoint = 1;

	std::array<int, enPlayerNumber_Num> White_BackHp = { 0,0,0,0 };
	std::array<float, enPlayerNumber_Num> WhiteHp_Timer = { 0.0f,0.0f,0.0f,0.0f };

	std::array<int, enPlayerNumber_Num> BackUPLV = { 0,0,0,0 };

	bool FightScale = false;
	bool FightshotStopFlag = false;

	RenderingEngine::EnGameMode m_gameMode = RenderingEngine::enGameMode_SoloPlay;	//プレイモード
};

