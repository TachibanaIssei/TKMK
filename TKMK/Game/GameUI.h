#pragma once

//class KnightPlayer;
//class WizardPlayer;
class Player;
class Game;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	enum GameUIState
	{
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
			m_LvNumber.Init("Assets/sprite/Lv1.DDS", 320.0f, 150.0f);

			break;
		case 2:
			m_LvNumber.Init("Assets/sprite/Lv2.DDS", 320.0f, 150.0f);

			break;
		case 3:
			m_LvNumber.Init("Assets/sprite/Lv3.DDS", 320.0f, 150.0f);

			break;
		case 4:
			m_LvNumber.Init("Assets/sprite/Lv4.DDS", 320.0f, 150.0f);

			break;
		case 5:
			m_LvNumber.Init("Assets/sprite/Lv5.DDS", 320.0f, 150.0f);

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
	/// ゲージを左寄せする処理
	/// </summary>
	/// <param name="size">画像の元の大きさ</param>
	/// <param name="scale">現在のスケール倍率</param>
	/// <returns>変換前と変換後の差</returns>
	//Vector3 HPBerSend(Vector3 size, Vector3 scale)
	//{
	//	Vector3 hpBerSize = size;								//画像の元の大きさ
	//	Vector3 changeBerSize = Vector3::Zero;					//画像をスケール変換したあとの大きさ
	//	Vector3 BerSizeSubtraction = Vector3::Zero;				//画像の元と変換後の差

	//	changeBerSize.x = hpBerSize.x * scale.x;
	//	BerSizeSubtraction.x = hpBerSize.x - changeBerSize.x;
	//	BerSizeSubtraction.x /= 2.0f;

	//	return BerSizeSubtraction;
	//}
private:
	FontRender m_LevelFont;
	FontRender m_LevelNameFont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_AtkFont;
	FontRender m_SpeedFont;

	/*KnightPlayer* m_knightplayer=nullptr;
	WizardPlayer* wizardPlayer = nullptr;*/
	Player* player = nullptr;
	Game* m_game = nullptr;
	//UI
	SpriteRender			m_hpBar;				//HPバーの画像
	SpriteRender			m_statusBar;			//ステータスの画像
	SpriteRender			m_playerFaceFrame;		//プレイヤーの顔画像の枠
	SpriteRender			m_playerFaceBack;		//プレイヤーの顔画像の背景
	SpriteRender            m_SkillRender;          //スキルアイコン
	SpriteRender            m_UltRender;            //必殺アイコン
	SpriteRender            m_TimeAndPointRender;   //制限時間と獲得ポイント
	SpriteRender            m_Lv;
	SpriteRender            m_LvNumber;
	SpriteRender            m_Flame;                //制限時間と獲得ポイントやHPバーの画像を
	
	Vector2				m_HPBerPos = Vector2::Zero;				//HPバーのポジション
	Vector2				m_HPWindowPos = Vector2::Zero;			//HP枠のポジション
	Vector2				m_HPBackPos = Vector2::Zero;			//HP背景のポジション
	
	
	FontRender m_time_left;

	//秒を計るタイマー
	float SecondsTimer=0.0f;
	//分を計るタイマー
	float MinutesTimer=5.0f;
	float m_timer = 300.0f;

	bool GameEndFlag=false;
	//int LEVEL;
};

