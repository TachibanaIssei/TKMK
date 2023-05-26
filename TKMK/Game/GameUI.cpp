#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Actor.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Fade.h"
#include "ExpforKnight.h"
#include "Sounds.h"
//todo 2体同時に倒すとレベル上がらない

namespace
{
	const int Characters = 4;

	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//ゲージのピボット
	const Vector2 HPGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//HPゲージのピボット
	const Vector2 EXPERIENCEGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//経験値ゲージのピボット

	const Vector3 STATUS_BAR_POS = Vector3(-450.0f, -500.0f, 0.0f);	//ステータスバーポジション
	const Vector3 TIME_POS = Vector3(0.0,470.0f, 0.0f);	//制限時間の座標
	const Vector3 TIME_FONT_POS = Vector3(7.0, 534.0f,0.0f);	//制限時間の座標

	const Vector3 HP_BAR_POS = Vector3(-670.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_FLONT_POS = Vector3(-960.0f, -480.0f, 0.0f);	//HPバーの表のポジション

	const float HP_BAR_WIDTH = 1400.0f;     //HPバーの長さ
	const float HP_BAR_HIGHT = 200.0f;      //HPバーの高さ
	const Vector3 HP_BAR_SIZE = Vector3(HP_BAR_WIDTH, HP_BAR_HIGHT, 0.0f);  //HPバーのサイズ

	const Vector3 Skill_Pos = Vector3(520.0f, -280.0f, 0.0f);   //スキルアイコンポジション
	const Vector3 Ult_Pos = Vector3(470.0f, -445.0f, 0.0f);     //必殺技アイコンポジション

	const Vector3 LV_NUBER_POS = Vector3(780.0f, -360.0f, 0.0f);
	const Vector3 LvPos = Vector3(640.0f, -310.0f, 0.0f);       //Lv
	const Vector3 MaxLvPos = Vector3(920.0f, -400.0f, 0.0f);       // /10

	const Vector3 FLAME_POS = Vector3(920.0f,-480.0f,0.0f);    //レベルや経験値のフレーム

	const float DownPointPosY = 100.0f;

	const Vector3 EXPERIENCE_POS = Vector3(750.0f, -500.0f, 0.0f);  //経験値テーブル

	const float EXPBAR_WIDTH = 300.0f;
	const float EXPBAR_HEIGHT = 70.0f;
	const Vector3 EXPERIENCE_BAR_POS = Vector3(600.0f, -500.0f, 0.0f);	//経験値バーの座標
	const Vector3 EXPBAR_SIZE = Vector3(EXPBAR_WIDTH, EXPBAR_HEIGHT, 0.0f);	//経験値バーのサイズ

	const Vector3 UPTOLEVEL_POS = Vector3(820.0f, -480.0f, 0.0f);		//レベルアップまでに必要な経験値の量

	const Vector3 RESPWANCOUNT_POS = Vector3(0.0f, 0.0f, 0.0f);		//リスポーンした後のカウント

	const Vector3 SmallScale = Vector3(0.1f, 0.1f, 0.0f);

	const Vector3 FightSmallScale = Vector3(0.5f, 0.5f, 0.0f);

	const Vector3 RespawnInPos = Vector3(0.0f, 300.0f, 0.0f);		//Respawn inの座標

	const Vector3 RespawnCountPos = Vector3(0.0f, -200.0f, 0.0f);		//の座標

	const Vector3 ADDPOINTPOS = Vector3(20.0f, 11.0f, 0.0f);

	const float WHITEHP_WAIT = 0.2f;

	const float CHAR_ICON_SIZE = 74.0f;

	const Vector3 CHAR_ICON_MAXSIZE = Vector3(1.2f, 1.2f, 1.0f);

	const float TIMERSCALE = 1.65f;

}
GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_game = FindGO<Game>("game");
	player = FindGO<Player>("player");
	fade = FindGO<Fade>("fade");
	//キャラのアイコン
	//ブルー
	m_CharIcon[0].Init("Assets/sprite/gameUI/Knight_Blue.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
	//レッド
	m_CharIcon[3].Init("Assets/sprite/gameUI/Knight_Red.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
	//グリーン
	m_CharIcon[2].Init("Assets/sprite/gameUI/Knight_Green.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
	//イエロー
	m_CharIcon[1].Init("Assets/sprite/gameUI/Knight_Yellow.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);

	//ポイント関連
	{
		//キャラのポイントを表示
		m_Actors = m_game->GetActors();
		int num = 0;
		for (auto actor: m_Actors)
		{
			//ポイントを表示
			m_PointFont[num].SetPosition(PointPos[num]);
			m_PointFont[num].SetScale(1.1f);
			m_PointFont[num].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_PointFont[num].SetRotation(0.0f);
			m_PointFont[num].SetShadowParam(true, 2.0f, g_vec4Black);

			//プレイヤーが剣士なら
			if (actor->IsMatchName(knightname))
			{
				//アイコンを剣士にする(ブルー)
				
				m_CharIcon[0].SetPosition(CharIconPos[num]);
				//フレームをプレイヤー用にする
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame_player.DDS", 300.0f, 100.0f);
			}
			else
			{
				/*m_CharIcon[num].SetPosition(CharIconPos[num]);
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 360.0f, 120.0f);*/
				//レベル
				m_LevelFont[num - 1].SetPosition(LevelPos[num-1]);
				m_LevelFont[num - 1].SetScale(0.6f);
				m_LevelFont[num - 1].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_LevelFont[num - 1].SetRotation(0.0f);
				m_LevelFont[num - 1].SetShadowParam(true, 2.0f, g_vec4Black);
			}
			//赤の剣士AIなら
			if(actor->IsMatchName(KnightAI_Red))
			{
				m_CharIcon[1].SetPosition(CharIconPos[num]);
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}
			else if (actor->IsMatchName(KnightAI_Green))
			{
				m_CharIcon[2].SetPosition(CharIconPos[num]);
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}
			else if (actor->IsMatchName(KnightAI_Yellow))
			{
				m_CharIcon[3].SetPosition(CharIconPos[num]);
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}

			m_PointFlame[num].SetPosition(PointFlamePos[num]);
			m_PointFlame[num].SetScale(1.0f, 1.0f, 1.0f);
			m_PointFlame[num].Update();
			//
			m_CharIcon[num].Update();

			num++;
		}

	}
	
	//リスポーン関連
	{
		//Respawn inの画像
		m_RespawnIn.Init("Assets/sprite/gameUI/RespawnIn.DDS", 900.0f, 200.0f);
		m_RespawnIn.SetPosition(RespawnInPos);
		m_RespawnIn.SetScale(Vector3::One);

		//リスポーンの背景の画像
		m_Respawn_Back.Init("Assets/sprite/gameUI/Respawn_back.DDS", 1920, 1080.0f);
		m_Respawn_Back.SetPosition(Vector3::Zero);
		m_Respawn_Back.SetScale(Vector3::One);

		//リスポーンのカウントダウンの画像
		m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
		m_RespawnCountNumber.SetPosition(RespawnCountPos);
		m_RespawnCountNumber.SetScale(Vector3::One);

		m_RespawnIn.Update();
		m_Respawn_Back.Update();
		m_RespawnCountNumber.Update();
	}

	//スタートまでのカウントダウン
	m_CountNumper.Init("Assets/sprite/gameUI/count3.DDS", 1920.0f, 1080.0f);
	m_CountNumper.SetPosition(Vector3::Zero);
	m_CountNumper.SetScale(m_gameCountScale);
	m_CountNumper.Update();

	//試合終了のカウントダウン
	m_FinishCountNumber.Init("Assets/sprite/gameUI/finish.DDS", 1920.0f, 1080.0f);
	m_FinishCountNumber.SetPosition(Vector3::Zero);
	m_FinishCountNumber.SetScale(m_finishCountScale);
	m_FinishCountNumber.Update();

	//右下のフレーム
	{
		//スキルのクールタイムを表示するフォントの設定
		m_Skillfont.SetPosition(479.0f, -220.0f, 0.0f);
		m_Skillfont.SetScale(1.7f);
		m_Skillfont.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_Skillfont.SetRotation(0.0f);
		m_Skillfont.SetShadowParam(true, 2.0f, g_vec4Black);
		PlayerCoolTime = player->CharGetSkillCoolTimer();

		//レベルや経験値のフレーム
		m_Flame.Init("Assets/sprite/gameUI/LevelBar.DDS", 1200.0f, 500.0f);
		m_Flame.SetPosition(FLAME_POS);
		m_Flame.SetScale(1.0, 1.0, 1.0);
		
		//経験値のフレーム
		m_ExperienceFlame.Init("Assets/sprite/gameUI/ExperienceBar.DDS", 600.0f, 120.0f);
		m_ExperienceFlame.SetPosition(EXPERIENCE_POS);
		m_ExperienceFlame.SetScale(0.5, 0.5, 1.0);
		
		//経験値バーの表ピボットにする
		m_EXPBerPos = EXPERIENCE_BAR_POS;
		m_ExperienceBar_flont.Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_WIDTH, EXPBAR_HEIGHT);
		m_ExperienceBar_flont.SetPosition(m_EXPBerPos);
		m_ExperienceBar_flont.SetPivot(EXPERIENCEGAUGE_PIVOT);
		m_ExperienceBar_flont.SetScale(0.5, 0.5, 1.0);
		
		//経験値テーブルと初期経験値
		m_ExpTable = player->CharSetEXPTable();
		m_MathExp = player->CharGetEXP();

		//経験値バーの裏
		m_ExperienceBar_back.Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", 600.0f, 120.0f);
		m_ExperienceBar_back.SetPosition(EXPERIENCE_POS);
		m_ExperienceBar_back.SetScale(0.5, 0.5, 1.0);

		//レベルアップまでに必要な経験値の量
		m_ExpFont.SetPosition(UPTOLEVEL_POS);
		m_ExpFont.SetScale(1.0f);
		m_ExpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_ExpFont.SetRotation(0.0f);
		m_ExpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//Lvの画像を読み込む
		m_Lv.Init("Assets/sprite/gameUI/Lv.DDS", 196.0f, 150.0f);
		m_Lv.SetPosition(LvPos);
		m_Lv.SetScale(0.4, 0.4, 1.0);

		//Lv1の画像を読み込む
		m_LvNumber.Init("Assets/sprite/gameUI/Lv1.DDS", 150.0f, 150.0f);
		m_LvNumber.SetPosition(LV_NUBER_POS);
		m_LvNumber.SetScale(1.4, 1.4, 1.0);
		m_ChangePlayerLevel = player->CharSetLevel();

		//Lv1の裏の画像の読み込み
		m_LvNumber_back.Init("Assets/sprite/gameUI/Lv1_back.DDS", 150.0f, 150.0f);
		m_LvNumber_back.SetPosition(LV_NUBER_POS);
		m_LvNumber_back.SetScale(1.4, 1.4, 1.0);

		// /10の画像を読み込む
		m_MaxLv.Init("Assets/sprite/gameUI/maxLv.DDS", 196.0f, 150.0f);
		m_MaxLv.SetPosition(MaxLvPos);
		m_MaxLv.SetScale(0.5, 0.5, 1.0);

		//スキルのアイコン
		m_SkillRenderIN.Init("Assets/sprite/gameUI/Skill_Icon_IN.DDS", 162, 162);
		m_SkillRenderIN.SetPosition(Skill_Pos);
		m_SkillRenderIN.SetScale(1.1, 1.1);
		//スキルのアイコンフレーム
		m_SkillRenderOUT.Init("Assets/sprite/gameUI/Skill_Icon_OUT.DDS", 162, 162);
		m_SkillRenderOUT.SetPosition(Skill_Pos);
		m_SkillRenderOUT.SetScale(1.1, 1.1);
		//必殺技のアイコン
		m_UltRenderIN.Init("Assets/sprite/gameUI/Ult_Thunder_IN.DDS", 162, 162);
		m_UltRenderIN.SetPosition(Ult_Pos);
		m_UltRenderIN.SetScale(1.2, 1.2);
		//必殺のアイコンフレーム
		m_UltRenderOUT.Init("Assets/sprite/gameUI/ULT_Icon_OUT.DDS", 162, 162);
		m_UltRenderOUT.SetPosition(Ult_Pos);
		m_UltRenderOUT.SetScale(1.2, 1.2);

		m_Lv.Update();
		m_LvNumber.Update();
		m_LvNumber_back.Update();
		m_MaxLv.Update();
		m_Flame.Update();
		m_ExperienceFlame.Update();
		m_ExperienceBar_flont.Update();
		m_ExperienceBar_back.Update();
		m_SkillRenderIN.Update();
		m_SkillRenderOUT.Update();
		m_UltRenderIN.Update();
		m_UltRenderOUT.Update();
	}

	//HP関連
	{
		//HPのフォント
		m_HpFont.SetPosition(-650.0f, -445.0f, 0.0f);
		m_HpFont.SetScale(1.0f);
		m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_HpFont.SetRotation(0.0f);
		m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//HPゲージ裏の画像を読み込む
		m_statusBar.Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", 600.0f, 120.0f);
		m_statusBar.SetPosition(HP_BAR_POS);
		m_statusBar.SetScale(1.0, 0.7, 1.0);

		//HPゲージの表の画像を読み込む
		m_hpBar.Init("Assets/sprite/gameUI/HPBar_HP.DDS", 580.0f, 80.0f);
		//ピボットを設定する
		m_hpBar.SetPivot(HPGAUGE_PIVOT);
		m_hpBar.SetPosition(HP_BAR_FLONT_POS);

		//HPゲージのフレームの画像を読み込む
		m_HPFrame.Init("Assets/sprite/gameUI/HPBar_flame.DDS", 600.0f, 120.0f);
		m_HPFrame.SetPosition(HP_BAR_POS);
		m_HPFrame.SetScale(1.0, 0.7, 1.0);
		//HPバーの白い部分
		m_HpBar_White.Init("Assets/sprite/gameUI/HPBar_backwhite.DDS", 580.0f, 80.0f);
		m_HpBar_White.SetPivot(HPGAUGE_PIVOT);
		m_HpBar_White.SetPosition(HP_BAR_FLONT_POS);
		//更新処理
		m_statusBar.Update();
		m_HPFrame.Update();
		m_HpBar_White.Update();
		m_hpBar.Update();
		//プレイヤーのHPを取得　白い部分用
		White_BackHp = player->CharSetHp();
		WhiteHp_Timer = WHITEHP_WAIT;
		BackUPLV = player->CharSetLevel();

	}

	//制限時間と獲得ポイント
	{
		m_TimeAndPointRender.Init("Assets/sprite/gameUI/timer.DDS", 1100.0f, 400.0f);
		m_TimeAndPointRender.SetPosition(TIME_POS);
		m_TimeAndPointRender.SetScale(0.35, 0.3, 1.0);

		//フォントの設定。
		//m_GameTimePos = TIME_FONT_POS;
		
		//フォントの大きさを設定。
		m_time_left.SetScale(TIMERSCALE);
		Vector2 aa = {80.0f, 0.0f };
		//オフセットの設定
		m_time_left.SetPivot(aa);
		//
		m_time_left.SetPosition(TIME_FONT_POS);
		//フォントの色を設定。
		m_time_left.SetColor(limitColor);
		m_time_left.SetShadowParam(true, 2.0f, g_vec4Black);

		m_TimeAndPointRender.Update();
	}

	m_GameUIState = m_GameStartState;

	return true;
}

void GameUI::Update()
{
	//ゲームの制限時間に達しているなら
	if (m_game->IsGameEnd() == true) {
		return;
	}

	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState== m_PauseState) {
		return;
	}

	//ゲームのステートがgameStartなら
	if (m_game->NowGameState() == Game::enGameState_Start)
	{
		CountDown();
	}
	
	//プレイヤーがリスポーン待機中なら
	if (player->CharGetRespawnTime() > 0)
	{
		RespawnCountDown();
	}
	
	//gameクラスのスタートのフラグが立っている間処理を行わない
	if (m_GameUIState == m_GameStartState) {
		return;
	}

	
	//制限時間
	Timer();
	//左のフレーム
	CharPoint();
	//左のフレームのレベル
	Level();
	
	int SkillCoolTime = player->CharGetSkillCoolTimer();
	wchar_t Skill[255];
	swprintf_s(Skill, 255, L"%d", SkillCoolTime);
	m_Skillfont.SetText(Skill);
	
	//レベルの点滅
	if (m_flashNumberFlag==false)
	{
		m_LvNumberColor -= 0.02f;

		if (m_LvNumberColor < 0.0f)
		{
			m_LvNumberColor = 0.0f;
			m_flashNumberFlag = true;
		}

		m_LvNumber_back.SetMulColor(Vector4(1.0f,1.0f, 1.0f, m_LvNumberColor));
	}
	else if(m_flashNumberFlag == true)
	{
		m_LvNumberColor += 0.02f;

		if (m_LvNumberColor > 1.0f)
		{
			m_LvNumberColor = 1.0f;
			m_flashNumberFlag = false;
		}

		m_LvNumber_back.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
	}
	m_LvNumber_back.Update();

	ExpState();
	//EXPBar();

	HPBar();
}



//ゲームスタートのカウントダウン
void GameUI::CountDown()
{
	//カウントダウン
	//int COUNTDOWNTIMER = m_game->CountDownMinutes();
	int StartCountDown= m_game->CountDownMinutes();

	if(m_game->CountDownMinutes() <= 0)
	{
		m_CountNumper.Init("Assets/sprite/gameUI/fight!.DDS", 1920.0f, 1080.0f);
		//m_gameCountScale = Vector3(5.0f, 5.0f, 0.0f);
		m_Color = 1.0f;
		m_fightFlag = true;
	}

	if (oldtStartCount != StartCountDown)
	{
		SoundSource* se;
		switch (StartCountDown)
		{
		case 1:
			m_CountNumper.Init("Assets/sprite/gameUI/count1.DDS", 1920.0f, 1080.0f);
			m_gameCountScale = Vector3(0.2f, 0.2f, 0.0f);
			m_Color = 1.0f;
			se = NewGO<SoundSource>(0);
			se->Init(enSound_CountDown1);
			se->SetVolume(1.0f);
			se->Play(false);
			break;
		case 2:
			m_CountNumper.Init("Assets/sprite/gameUI/count2.DDS", 1920.0f, 1080.0f);
			m_gameCountScale = Vector3(0.2f, 0.2f, 0.0f);
			m_Color = 1.0f;
			se = NewGO<SoundSource>(0);
			se->Init(enSound_CountDown2);
			se->SetVolume(1.0f);
			se->Play(false);
			break;
		case 3:
			m_CountNumper.Init("Assets/sprite/gameUI/count3.DDS", 1920.0f, 1080.0f);
			m_gameCountScale = Vector3(0.2f, 0.2f, 0.0f);
			m_Color = 1.0f;
			se = NewGO<SoundSource>(0);
			se->Init(enSound_CountDown3);
			se->SetVolume(1.0f);
			se->Play(false);
			break;
		default:
			break;
		}
	}

	//画像がFight!でないかつスケールが100以下なら
	else if(m_fightFlag == false&&m_gameCountScale.x<100.0f)
	{
		//徐々に文字を大きくする
		m_gameCountScale += SmallScale;
		//少しずつ透明にする
		m_Color -= 0.02f;
		
		m_CountNumper.SetScale(m_gameCountScale);
		
		m_CountNumper.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_Color));
	}

	//画像がFight!なら
	if (m_fightFlag == true)
	{

		if (FightScale == true && FightshotStopFlag == false)
		{
			m_gameCountScale += {0.2f, 0.2f, 0.0f};

			if (m_gameCountScale.x > 0.8f) {
				FightshotStopFlag = true;
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(enSound_CountFight);
				se->SetVolume(1.0f);
				se->Play(false);
			}
		}
		else if(FightshotStopFlag == false) {
			//徐々に文字を小さくする
			m_gameCountScale -= FightSmallScale;

			if (m_gameCountScale.x < 0.6f)
			{
				FightScale = true;
			}
		}

	}
		
		m_CountNumper.SetScale(m_gameCountScale);
		
		m_CountNumper.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_Color));
	

	
	m_CountNumper.Update();

	oldtStartCount = StartCountDown;
}

//リスポーンするまでのカウントダウン
void GameUI::RespawnCountDown()
{
	//カウントダウン
	int RESPAWNCOUNTDOWN = player->CharGetRespawnTime();

	if (oldRespawnCount != RESPAWNCOUNTDOWN)
	{
		switch (RESPAWNCOUNTDOWN)
		{
		case 0:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut0.DDS", 300, 500.0f);
			//画面を暗くしてゆく
			fade->StartFadeIn(2.0f);
			break;
		case 1:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut1.DDS", 300, 500.0f);
			break;
		case 2:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
			break;
		default:
			break;
		}
	}

	oldRespawnCount = RESPAWNCOUNTDOWN;

	m_RespawnCountNumber.Update();
}

//プレイヤーのHPの表示の処理
void GameUI::HPBar()
{
	int HP = player->CharSetHp();
	int MaxHP = player->CharSetMaxHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d/%d", HP, MaxHP);
	m_HpFont.SetText(hp);

	Vector3 HpScale = Vector3::One;
	//HPバーの減っていく割合。
	HpScale.x = (float)player->CharSetHp() / (float)player->CharSetMaxHp();
	m_hpBar.SetScale(HpScale);

	m_hpBar.Update();
	
	//レベルが下がった時の処理
	if (BackUPLV > player->CharSetLevel())
	{
		White_BackHp = HP;
	}
	BackUPLV = player->CharSetLevel();

	//Hp削られたら白い部分も減らす
	if (HP < White_BackHp)
	{
		if (WhiteHp_Timer > 0.0f)
		{
			WhiteHp_Timer -= g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			White_BackHp -= 2;

			//HPバーの減っていく割合。
			HpScale.x = (float)White_BackHp / (float)MaxHP;
			m_HpBar_White.SetScale(HpScale);

			if (White_BackHp <= HP)
			{
				White_BackHp = HP;
				WhiteHp_Timer = WHITEHP_WAIT;
			}
		}
	}
	else if (HP > White_BackHp)
	{
		White_BackHp = HP;
	}
	m_HpBar_White.Update();


	//AI用のHP
}

//AIのレベルの表示
void GameUI::Level()
{
	int num = 0;
	for (auto actor : m_Actors)
	{
		//プレイヤーなら
		if (actor->IsMatchName(knightname))
		{
			num++;
			continue;
		}
		//AIなら
		int Lv = actor->GetLevel();
		wchar_t AILv[255];
		swprintf_s(AILv, 255, L"Lv%d", Lv);
		m_LevelFont[num-1].SetText(AILv);

		num++;
	}


}

//残り時間10秒の時の処理
void GameUI::Timer()
{
	//制限時間の表示
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d:%02d", int(m_game->GetMinutesTimer()), int(m_game->GetSecondsTimer()));
	//表示するテキストを設定。
	
	m_time_left.SetText(wcsbuf);

	if (m_game->GetMinutesTimer() < 1 && m_game->GetSecondsTimer() < 10&& m_game->GetSecondsTimer() > 1 )
	{
		if (timerScaleFlag == false)
		{
			if (timerScale < 2.8f)
			{
				timerScale += 1.9f*g_gameTime->GetFrameDeltaTime();
				//
				
			}
			else
			{
				timerScale = 2.8f;
				timerScaleFlag = true;
			}
			
			if (limitColor.y == limitColor.z > 0.0f) {
				limitColor.y -= 0.1f * g_gameTime->GetFrameDeltaTime();
				limitColor.z -= 0.1f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				limitColor.y = 0.0f;
				limitColor.z = 0.0f;
			}
			

		}
		else if (timerScaleFlag == true)
		{
			if (timerScale > TIMERSCALE)
			{
				timerScale -= 3.4f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				timerScale = TIMERSCALE;
				timerScaleFlag = false;
			}

			if (limitColor.y == limitColor.z < 1.0f) {
				limitColor.y += 0.1f * g_gameTime->GetFrameDeltaTime();
				limitColor.z += 0.1f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				limitColor.y = 1.0f;
				limitColor.z = 1.0f;
			}
		}

		if (m_game->GetSecondsTimer() > 1) {
			m_time_left.SetScale(timerScale);
		}
		
		
		m_time_left.SetColor(limitColor);
	}
	else
	{
		m_time_left.SetScale(TIMERSCALE);
	}
}

void GameUI::ExpState()
{
	switch (m_enExpProssesState)
	{
	case GameUI::enChackExpState:
		ChackExp();
		break;
	case GameUI::enUpExpState:
		UpExp();
		break;
	case GameUI::enDownExpState:
		DownExp();
		break;
	case GameUI::enLevelUpState:
		LevelUp();
		break;
	case GameUI::enLevelDownState:
		LevelDown();
		break;
	default:
		break;
	}

	m_oldSaveExp = m_SaveExp;

	//経験値の表示
	Vector3 EXPScale = Vector3::One;
	
		//HPバーの増えていく割合。
	EXPScale.x = (float)m_MathExp / (float)m_ExpTable;


	m_ExperienceBar_flont.SetScale(EXPScale);
	m_ExperienceBar_flont.Update();


	//m_oldPlayerLevel = m_NowPlayerLevel;

	//デバッグ用
	//レベルアップまでに必要な経験値の量
	int UpToLevel = m_MathExp/* - m_nowEXP*/;
	wchar_t UTL[255];
	swprintf_s(UTL, 255, L"%d", UpToLevel);
	m_ExpFont.SetText(UTL);
}

//取得した経験値の量が変わったか調べる
void GameUI::ChackExp()
{
	//レベルが下がったら
	if (PlayerLevel > player->CharSetLevel()) {
		m_enExpProssesState = enLevelDownState;
		return;
	}

	//プレイヤーの経験値を取得
	//m_nowEXP = player->CharSetEXP();
	////今の経験値テーブルを取得
	//nowEXPTable = player->CharSetEXPTable();
	////セーブした経験値が前フレームのセーブした経験値と違うなら
	if (player->CharGetSaveEXP() != m_oldSaveExp) {
		m_SaveExp = player->CharGetSaveEXP();

		m_enExpProssesState = enUpExpState;
	}
	
}

void GameUI::UpExp()
{
	//タイミングによって経験値が計算されない
	//当たった当たり判定が違うと
	if (m_MathExp >= m_ExpTable)
	{
		m_enExpProssesState = enLevelUpState;
	}
	else if (m_MathExp < m_SaveExp) {
		m_MathExp++;
	}
	else {
		m_enExpProssesState = enChackExpState;
	}
}

void GameUI::LevelUp()
{

	m_SaveExp -= m_ExpTable;

	m_MathExp = 0;

	//レベルアップの処理
	if (PlayerLevel < player->CharSetLevel()) {
		PlayerLevel++;
	}
	//レベルに応じた経験値テーブルにする
	m_ExpTable = player->CharGetEXPTableForLevel(PlayerLevel);

	//フォント更新
	LevelFontChange(PlayerLevel);

	if (PlayerLevel == 10) {
		m_SaveExp = 10;
		player->CharResatSaveEXP(m_SaveExp);
		m_oldSaveExp = m_SaveExp;
		m_MathExp = m_SaveExp;
		m_enExpProssesState = enChackExpState;
		return;
	}

	if (m_SaveExp > 0) {
		//セーブした経験値をリセット
		//m_saveExpとプレイヤーのセーブした経験値を同じにする
		if (player->CharGetEXP() > 0) {
			player->CharResatSaveEXP(m_SaveExp);
		}
		
		m_oldSaveExp = player->CharGetSaveEXP();
		m_enExpProssesState = enUpExpState;
	}
	else if (m_SaveExp <= 0) {
		//セーブした経験値をリセット
		player->CharResatSaveEXP(0);
		m_SaveExp = player->CharGetSaveEXP();
		m_oldSaveExp = m_SaveExp;

		m_enExpProssesState = enChackExpState;
	}
}

void GameUI::DownExp()
{
	if (m_MathExp <= 0) {
		m_enExpProssesState = enLevelDownState;
	}
	else
	{
		m_MathExp--;
		//m_DownFlag = true;
	}
}

void GameUI::LevelDown()
{
	PlayerLevel--;
	//レベルに応じた経験値テーブルにする
	m_ExpTable = player->CharGetEXPTableForLevel(PlayerLevel);

	//セーブした経験値をリセット
	//player->CharResatSaveEXP(0);
	//動く値調
	
	//m_MathExp = 0;
	
	LevelFontChange(PlayerLevel);

	if (PlayerLevel <= player->CharSetLevel()) {
		//レベルダウンの処理を終わる
		m_enExpProssesState = enChackExpState;
		m_SaveExp = 0;
		m_oldSaveExp = m_SaveExp;
		m_MathExp = 0;
		//セーブした経験値をリセット
		player->CharResatSaveEXP(0);
		return;
	}
	else
	{
		m_MathExp = m_ExpTable;

		m_enExpProssesState = enDownExpState;
	}
}

//
Vector3& GameUI::HPBerSend(Vector3 size, Vector3 scale)
{
	Vector3 expBerSize = size;								//画像の元の大きさ
	Vector3 changeBerSize = Vector3::Zero;					//画像をスケール変換したあとの大きさ
	Vector3 BerSizeSubtraction = Vector3::Zero;				//画像の元と変換後の差

	changeBerSize.x = expBerSize.x * scale.x;
	BerSizeSubtraction.x = expBerSize.x - changeBerSize.x;
	BerSizeSubtraction.x /= 2.0f;

	return BerSizeSubtraction;
}

//キャラのポイントと。ポイントが一番多いキャラに王冠マークをつける表示の処理
void GameUI::CharPoint()
{
	//キャラのポイントを表示
	m_Actors = m_game->GetActors();

	int num = 0;
	for (auto actor:m_Actors)
	{
		//死んでいるかリスポーン中なら
		if (actor->NowCharState() == Actor::enCharState_Death || actor->GetRespawnFlag() == true)
		{
			m_CharIcon[num].SetGrayScale(true);
		}
		else
		{
			m_CharIcon[num].SetGrayScale(false);
		}

		//制限時間が残り1分なら
		if (m_game->GetMinutesTimer() < 1)
		{
			wchar_t P[255];
			swprintf_s(P, 255, L"?p");
			m_PointFont[num].SetText(P);
			num++;
			continue;
		}

		charPoint[num] = actor->GetPoint();

		//ポイントの表示
		int POINT = charPoint[num];
		wchar_t P[255];
		swprintf_s(P, 255, L"%dp", POINT);
		m_PointFont[num].SetText(P);

		//一番ポイントが多いキャラのフレーム
		if (MaxPoint <= charPoint[num])
		{
			m_PointFont[num].SetScale(1.4f);
			Vector3 FontPos;
			FontPos= ADDPOINTPOS + PointPos[num];
			m_PointFont[num].SetPosition(FontPos);

			m_PointFlame[num].SetScale(1.45f,1.2f,0.0f);
			m_PointFlame[num].Update();
			m_CharIcon[num].Update();
			MaxPoint = charPoint[num];
		}
		else
		{
			m_PointFont[num].SetScale(1.1f);
			m_PointFont[num].SetPosition(PointPos[num]);
			m_PointFlame[num].SetScale(1.0f, 1.0f, 0.0f);
			m_PointFlame[num].Update();
			m_CharIcon[num].Update();
		}


		num++;
	}

	
	//誰に王冠マークつけるか決める
}

void GameUI::Render(RenderContext& rc)
{
	if (m_game->GetStopFlag() == true)
	{
		return;
	}
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState != m_PauseState && m_GameUIState != m_GameStartState) {
		//レベルや経験値のフレーム
		m_Flame.Draw(rc);
		//経験値の裏
		m_ExperienceBar_back.Draw(rc);
		//経験値の表 変動する
		if (m_MathExp != 0) {
			m_ExperienceBar_flont.Draw(rc);
		}

		//スキルのクールタイムとタイマーが違う時だけ表示
		if (player->CharGetSkillCoolTimer() != PlayerCoolTime)
		{
			m_Skillfont.Draw(rc);
		}
		
	
		m_ExperienceFlame.Draw(rc);
		//m_ExpFont.Draw(rc);

		m_HpNameFont.Draw(rc);

		m_TimeAndPointRender.Draw(rc);
		m_time_left.Draw(rc);

		m_statusBar.Draw(rc);
		m_HpBar_White.Draw(rc);
		m_hpBar.Draw(rc);
		m_HPFrame.Draw(rc);

		m_SkillRenderIN.Draw(rc);
		m_SkillRenderOUT.Draw(rc);

		m_UltRenderIN.Draw(rc);
		m_UltRenderOUT.Draw(rc);

		m_HpFont.Draw(rc);
		m_Lv.Draw(rc);
		m_LvNumber_back.Draw(rc);
		m_LvNumber.Draw(rc);
		m_MaxLv.Draw(rc);

		//ポイントを描画
		//左のフレームの色々を描画

		int num = 0;
		for (auto actor:m_Actors) {
			m_PointFlame[num].Draw(rc);
			m_PointFont[num].Draw(rc);
			//m_CharIcon[num].Draw(rc);
			if(num>=1)
			m_LevelFont[num-1].Draw(rc);
			num++;
		}
		int numa = 0;
		for (auto actor : m_Actors) {
			m_CharIcon[numa].Draw(rc);
			numa++;
		}
		
		//リスポーンするまでの時間
		if (player->CharGetRespawnTime() > 0)
		{
			m_Respawn_Back.Draw(rc);
			m_RespawnIn.Draw(rc);
			m_RespawnCountNumber.Draw(rc);
		}

		//試合終了まで残り10秒なら
		if (m_game->GetMinutesTimer() < 1 && m_game->GetSecondsTimer() <= 0)
		{
			m_FinishCountNumber.Draw(rc);
		}
		
	}
	else
	{
		//カウントダウンの表示
		if (m_game->NowGameState() == 0&& m_game->CountDownMinutes() <= 3) {
			m_CountNumper.Draw(rc);
		}
	}
	
}
