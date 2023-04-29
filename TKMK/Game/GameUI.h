#pragma once

//class KnightPlayer;
//class WizardPlayer;
class Player;
class Game;
class Actor;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	enum GameUIState
	{
		m_GameStartState,
		m_GameState,
		m_PauseState,
		m_BattleEndState,
	};
	GameUIState m_GameUIState;

	bool Start();
	void Update();

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
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
			break;
		case 2:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv2.DDS", 150.0f, 150.0f);
			break;
		case 3:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv3.DDS", 150.0f, 150.0f);
			break;
		case 4:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv4.DDS", 150.0f, 150.0f);
			break;
		case 5:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv5.DDS", 150.0f, 150.0f);
			break;
		case 6:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv6.DDS", 150.0f, 150.0f);
			break;
		case 7:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv7.DDS", 150.0f, 150.0f);
			break;
		case 8:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv8.DDS", 150.0f, 150.0f);
			break;
		case 9:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv9.DDS", 150.0f, 150.0f);
			break;
		case 10:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv10.DDS", 150.0f, 150.0f);
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
	void RespawnCountDown();

	/// <summary>
	/// 
	/// </summary>
	void EXPBar();

	/// <summary>
	/// 
	/// </summary>
	void CharPoint();

private:
	FontRender m_CountDownFont;

	FontRender m_LevelFont;
	FontRender m_LevelNameFont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_AtkFont;
	FontRender m_SpeedFont;

	FontRender m_PointFont[4];

	FontRender m_RespawnCount;

	Player* player = nullptr;
	Game* m_game = nullptr;
	Actor* actor = nullptr;

	std::vector<Actor*> m_Actors;

	//UI
	SpriteRender			m_hpBar;							//HPバーの画像
	SpriteRender			m_statusBar;						//ステータスの画像
	SpriteRender			m_HPFrame;							//プレイヤーの顔画像の枠
	SpriteRender			m_playerFaceBack;					//プレイヤーの顔画像の背景
	SpriteRender            m_SkillRender;						//スキルアイコン
	SpriteRender            m_UltRender;						//必殺アイコン
	SpriteRender            m_TimeAndPointRender;				//制限時間と獲得ポイント
	SpriteRender            m_Lv;
	SpriteRender            m_LvNumber;
	SpriteRender            m_MaxLv;
	SpriteRender            m_Flame;							//制限時間と獲得ポイントやHPバーの画像を
	SpriteRender            m_Point;							//ポイント
	SpriteRender            m_ExperienceFlame;					//経験値のフレーム
	SpriteRender            m_ExperienceBar_flont;				//経験値バーの表
	SpriteRender			m_Crown;							//ポイントが一番多いキャラにつける王冠マーク
	SpriteRender			m_PointFlame[4];					//ポイントを表示するフレーム
	SpriteRender            m_CharIcon[4];                      //キャラのアイコン
	SpriteRender			m_CountNumper;					//カウントダウン
	SpriteRender			m_RespawnIn;						//Respawn inの画像
	SpriteRender			m_Respawn_Back;						//リスポーン時の背景
	SpriteRender			m_RespawnCountNumber;				//リスポーン時のカウントダウン

	
	Vector2				m_HPBerPos = Vector2::Zero;				//HPバーのポジション
	Vector2				m_HPWindowPos = Vector2::Zero;			//HP枠のポジション
	Vector2				m_HPBackPos = Vector2::Zero;			//HP背景のポジション

	Vector3				m_gameCountScale = Vector3(0.2f,0.2f,0.0f);
	Vector3				m_FightScale = Vector3(1.0f, 1.0f, 0.0f);
	bool				m_fightFlag = false;
	float				m_GameCountScale = 20.0f;
	float               m_Color = 1.0f;

	Vector3 PointPos[4] = {
		Vector3(-850.0f, 150.0f, 0.0f),
		Vector3(-850.0f, 50.0f, 0.0f), 
		Vector3(-850.0f, -50.0f, 0.0f), 
		Vector3(-850.0f, -150.0f, 0.0f), 
	};															//ポイント

	Vector3 PointFlamePos[4] = {
		Vector3(-850.0f, 120.0f, 0.0f),
		Vector3(-850.0f, 20.0f, 0.0f),
		Vector3(-850.0f, -80.0f, 0.0f),
		Vector3(-850.0f, -180.0f, 0.0f),
	};															//ポイント

	Vector3 CharIconPos[4] = {
		Vector3(-920.0f, 120.0f, 0.0f),
		Vector3(-920.0f, 20.0f, 0.0f),
		Vector3(-920.0f, -80.0f, 0.0f),
		Vector3(-920.0f, -180.0f, 0.0f),
	};															//アイコン
	
	Vector3 CrownPos[4] = {
		Vector3(-920.0f, 120.0f, 0.0f),
		Vector3(-920.0f, 20.0f, 0.0f),
		Vector3(-920.0f, -80.0f, 0.0f),
		Vector3(-920.0f, -180.0f, 0.0f),
	};															//王冠マーク

	FontRender m_time_left;

	const char* knightname = "knightplayer";
	const char* wizardname = "wizardplayer";

	int oldtStartCount = 0;

	int oldRespawnCount = 0;

	//秒を計るタイマー
	float SecondsTimer=0.0f;
	//分を計るタイマー
	float MinutesTimer=5.0f;
	float m_timer = 300.0f;

	bool GameEndFlag=false;

	int nowEXP;
	//前フレームの経験値
	int oldEXP=0;

	int nowEXPTable=0;
	//前フレームの経験値テーブル
	int oldEXPTable;
	//キャラのポイント
	int charPoint[4];

	int MaxPoint = 0;

	//float StartCountDown = 4.0f;

	//int LEVEL;
};

