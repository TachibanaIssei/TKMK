#pragma once

//class KnightPlayer;
//class WizardPlayer;
class Player;
class Game;
class Actor;
class Fade;
class ExpforKnight;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	enum EnPlayerNumber
	{
		enPlayerNumber_1P = 0,
		enPlayerNumber_2P = 1,
		enPlayerNumber_Num = 2
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
	EnExpProssesState m_enExpProssesState = enChackExpState;

	bool Start();
	void Update();

	void ExpState();
	void ChackExp();
	void UpExp();
	void DownExp();
	void LevelUp();
	void LevelDown();


	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}

	void ChangePlayerLevel(int lv)
	{
		m_NowPlayerLevel = lv;
	}

	void ChangeEXPUpFlag(bool flag)
	{
		m_EXPupFlag = flag;
	}

	/// <summary>
	/// プレイヤーのレベルに合わせてレベルの画像を変更する
	/// </summary>
	/// <param name="lv">プレイヤーの現在のレベル</param>
	void LevelFontChange(int lv)
	{
		switch (lv)
		{
		case 1:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv1.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv1_back.DDS", 150.0f, 150.0f);
			break;
		case 2:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv2.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv2_back.DDS", 150.0f, 150.0f);
			break;
		case 3:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv3.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv3_back.DDS", 150.0f, 150.0f);
			break;
		case 4:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv4.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv4_back.DDS", 150.0f, 150.0f);
			break;
		case 5:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv5.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv5_back.DDS", 150.0f, 150.0f);
			break;
		case 6:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv6.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv6_back.DDS", 150.0f, 150.0f);
			break;
		case 7:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv7.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv7_back.DDS", 150.0f, 150.0f);
			break;
		case 8:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv8.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv8_back.DDS", 150.0f, 150.0f);
			break;
		case 9:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv9.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv9_back.DDS", 150.0f, 150.0f);
			break;
		case 10:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv10.DDS", 150.0f, 150.0f);
			m_LvNumber_back.Init("Assets/sprite/gameUI/Lv10_back.DDS", 150.0f, 150.0f);
			break;
		default:
			break;
		}
	}

	void Render(RenderContext& rc);

	/// HPバーの表示
    /// </summary>
	void HPBar();

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
	/// 
	/// </summary>
	/// <param name="size"></param>
	/// <param name="scale"></param>
	/// <returns></returns>
	Vector3& HPBerSend(Vector3 size, Vector3 scale);

	/// <summary>
	/// スキルのアイコンをグレースケールにする
	/// </summary>
	/// <param name="flag">trueの場合グレースケールにする</param>
	void SetSkillIconGrayScale(bool flag)
	{
		m_SkillRenderIN.SetGrayScale(flag);
	}

	/// <summary>
	/// 必殺のアイコンをグレースケールにする
	/// </summary>
	/// <param name="flag">trueの場合グレースケールにする</param>
	void SetULTIconGrayScale(bool flag)
	{
		m_UltRenderIN.SetGrayScale(flag);
	}

private:
	void InitAssets();

private:
	FontRender m_ExpFont;

	FontRender m_Skillfont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_PointFont[4];
	FontRender m_LevelFont[3];

	Player* m_player1P = nullptr;
	Player* m_player2P = nullptr;
	Game* m_game = nullptr;
	Actor* actor = nullptr;
	Fade* fade = nullptr;
	ExpforKnight* expforKnight = nullptr;

	std::vector<Actor*> m_Actors;

	//UI
	std::array<SpriteRender,enPlayerNumber_Num>		m_hpBar;		//HPバーの画像
	std::array<SpriteRender, enPlayerNumber_Num>	m_HpBar_White;	//HPバーの白い部分
	std::array<SpriteRender, enPlayerNumber_Num>	m_hpBarBack;	//ステータスの画像
	std::array<SpriteRender, enPlayerNumber_Num>	m_HPFrame;		//HPバーのフレーム
	SpriteRender			m_playerFaceBack;					//プレイヤーの顔画像の背景
	SpriteRender            m_SkillRenderIN;					//スキルのアイコン
	SpriteRender            m_SkillRenderOUT;					//スキルのアイコンのフレーム
	SpriteRender            m_UltRenderIN;						//必殺アイコン
	SpriteRender            m_UltRenderOUT;						//必殺アイコンのフレーム
	SpriteRender            m_TimeAndPointRender;				//制限時間と獲得ポイント
	SpriteRender            m_Lv;
	SpriteRender            m_LvNumber;
	SpriteRender            m_LvNumber_back;							//レベルの裏
	float					m_LvNumberColor = 1.0f;
	bool					m_flashNumberFlag = false;
	SpriteRender            m_MaxLv;
	SpriteRender            m_Flame;							//制限時間と獲得ポイントやHPバーの画像を
	SpriteRender            m_Point;							//ポイント
	SpriteRender            m_ExperienceFlame;					//経験値のフレーム
	SpriteRender            m_ExperienceBar_flont;				//経験値バーの表
	SpriteRender            m_ExperienceBar_back;				//経験値バーの裏
	SpriteRender			m_Crown;							//ポイントが一番多いキャラにつける王冠マーク
	SpriteRender			m_PointFlame[4];					//ポイントを表示するフレーム
	SpriteRender            m_CharIcon[4];                      //キャラのアイコン
	SpriteRender			m_CountNumper;						//カウントダウン
	SpriteRender			m_RespawnIn;						//Respawn inの画像
	SpriteRender			m_Respawn_Back;						//リスポーン時の背景
	SpriteRender			m_RespawnCountNumber;				//リスポーン時のカウントダウン
	SpriteRender			m_FinishCountNumber;				//制限時間残り10秒のカウントダウン

	Vector3				m_EXPBerPos = /*Vector3(750.0f, -500.0f, 0.0f)*/Vector3::Zero;				   //EXPバーのポジション

	Vector3				m_gameCountScale = Vector3(0.2f,0.2f,0.0f);
	Vector3				m_FightScale = Vector3(1.0f, 1.0f, 0.0f);
	bool				m_fightFlag = false;
	float				m_GameCountScale = 20.0f;
	float               m_Color = 1.0f;

	Vector3				m_finishCountScale = Vector3::One;

	Vector3 PointPos[4] = {
		Vector3(-860.0f, 170.0f, 0.0f),
		Vector3(-860.0f, 60.0f, 0.0f), 
		Vector3(-860.0f, -50.0f, 0.0f), 
		Vector3(-860.0f, -160.0f, 0.0f), 
	};															//ポイント

	Vector3 LevelPos[3] = {
		Vector3(-940.0f, 10.0f, 0.0f),
		Vector3(-940.0f, -100.0f, 0.0f),
		Vector3(-940.0f, -210.0f, 0.0f),
	};															//レベル

	Vector3 PointFlamePos[4] = {
		Vector3(-850.0f, 120.0f, 0.0f),
		Vector3(-850.0f, 10.0f, 0.0f),
		Vector3(-850.0f, -100.0f, 0.0f),
		Vector3(-850.0f, -210.0f, 0.0f),
	};															//ポイントのフレーム

	Vector3 CharIconPos[4] = {
		Vector3(-905.0f, 120.0f, 0.0f),
		Vector3(-905.0f, 35.0f, 0.0f),
		Vector3(-905.0f, -75.0f, 0.0f),
		Vector3(-905.0f, -185.0f, 0.0f),
	};															//アイコン
	
	Vector3 CrownPos[4] = {
		Vector3(-920.0f, 120.0f, 0.0f),
		Vector3(-920.0f, 10.0f, 0.0f),
		Vector3(-920.0f, -100.0f, 0.0f),
		Vector3(-920.0f, -210.0f, 0.0f),
	};															//王冠マーク

	FontRender m_time_left;
	float timerScale = 1.65f;
	bool timerScaleFlag = false;
	Vector4 limitColor = { 1.0f,1.0f,1.0f,1.0f };

	const char* knightname = "knightplayer";
	const char* knightname2 = "knightplayer2";
	const char* wizardname = "wizardplayer";
	const char* KnightAI_Red = "KnightAI";
	const char* KnightAI_Green = "KnightAI1";
	const char* KnightAI_Yellow = "KnightAI2";

	int oldtStartCount = 0;

	std::array<int, enPlayerNumber_Num> oldRespawnCount = { 0,0 };

	int oldFinishCount = 0;

	//
	float PlayerCoolTime = 0;

	//秒を計るタイマー
	float SecondsTimer=0.0f;
	//分を計るタイマー
	float MinutesTimer=5.0f;
	float m_timer = 300.0f;

	bool GameEndFlag=false;

	
	int							m_NowPlayerLevel = 1;							//現在のプレイヤーのレベルを格納
	int							m_ChangePlayerLevel = 0;						//画像表示のための徐々にレベルを下げる
	int 						PlayerLevel = 1;							//レベルが上がった時にtrueにする
	int							final_exp = 0;									//最終的な経験値バーの増量
	int							m_nowEXP = 0;									//現在のプレイヤーの経験値
	float						nowEXPTable = 0;								//現在のプレイヤーの経験値テーブル
	bool						m_DownFlag = false;
	int 						m_MathExp = 0;									//経験値を増やしたり減らしたりする時はこれ
	int							m_SaveExp = 0;
	int							m_ExpTable = 0;
	bool                        m_EXPupFlag = false;
	

	int m_oldSaveExp = 0;

	//キャラのポイント
	int charPoint[4];

	int MaxPoint = 1;

	std::array<int, enPlayerNumber_Num> White_BackHp = { 0,0 };
	std::array<float, enPlayerNumber_Num> WhiteHp_Timer = { 0.0f,0.0f };

	std::array<int, enPlayerNumber_Num> BackUPLV = { 0,0 };

	bool FightScale = false;
	bool FightshotStopFlag = false;

	bool m_isMultiPlay = false;	//2人プレイかどうか（tureだったら2人プレイ）
};

