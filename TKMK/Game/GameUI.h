#pragma once

//class KnightPlayer;
//class WizardPlayer;
class Player;
class Game;
class Actor;
class Fade;

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
	/// GamwUIのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetGameUIState(GameUIState gamescene) {
		m_GameUIState = gamescene;

	}

	void FinishTimer();

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
	FontRender m_ExpFont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_PointFont[4];

	Player* player = nullptr;
	Game* m_game = nullptr;
	Actor* actor = nullptr;
	Fade* fade = nullptr;

	std::vector<Actor*> m_Actors;

	//UI
	SpriteRender			m_hpBar;							//HPバーの画像
	SpriteRender            m_HpBar_White;                      //HPバーの白い部分
	SpriteRender			m_statusBar;						//ステータスの画像
	SpriteRender			m_HPFrame;							//プレイヤーの顔画像の枠
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

	Vector2				m_HPBerPos = Vector2::Zero;				//HPバーのポジション
	Vector2				m_HPWindowPos = Vector2::Zero;			//HP枠のポジション
	Vector2				m_HPBackPos = Vector2::Zero;			//HP背景のポジション
	

	Vector3				m_EXPBerPos = /*Vector3(750.0f, -500.0f, 0.0f)*/Vector3::Zero;				   //EXPバーのポジション

	Vector3				m_gameCountScale = Vector3(0.2f,0.2f,0.0f);
	Vector3				m_FightScale = Vector3(1.0f, 1.0f, 0.0f);
	bool				m_fightFlag = false;
	float				m_GameCountScale = 20.0f;
	float               m_Color = 1.0f;

	Vector3				m_finishCountScale = Vector3::One;

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

	int oldFinishCount = 0;

	//秒を計るタイマー
	float SecondsTimer=0.0f;
	//分を計るタイマー
	float MinutesTimer=5.0f;
	float m_timer = 300.0f;

	bool GameEndFlag=false;

	//前フレームの経験値
	int oldEXP=0;
	int oldEXPTable = 0;

	//キャラのポイント
	int charPoint[4];

	int MaxPoint = 1;

	int White_BackHp = 0;
	float WhiteHp_Timer = 0.0f;

};

