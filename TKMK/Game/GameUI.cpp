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

namespace
{
	const int Characters = 4;

	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//ゲージのピボット
	const Vector2 HPGAUGE_PIVOT_LEFT = Vector2(0.0f, 0.5f);				//HPゲージのピボット
	const Vector2 HPGAUGE_PIVOT_RIGHT = Vector2(1.0f, 0.5f);				//右のHPゲージのピボット
	const Vector2 EXPERIENCEGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//経験値ゲージのピボット

	const Vector3 STATUS_BAR_POS = Vector3(-450.0f, -500.0f, 0.0f);	//ステータスバーポジション
	const Vector3 TIME_POS = Vector3(0.0f, 470.0f, 0.0f);	//制限時間の座標
	const Vector3 MINUTES_FONT_POS = Vector3(-75.0f, 500.0f, 0.0f);	//制限時間の座標(分)
	const Vector3 CORON_FONT_POS = MINUTES_FONT_POS + Vector3(70.0f, 10.0f, 0.0f);//制限時間の座標(:コロン)
	const Vector3 SECONDS_FONT_POS = CORON_FONT_POS + Vector3(35.0f, -10.0f, 0.0f);//制限時間の座標(秒)

	const float HP_BAR_BACK_WIDTH = 600.0f;
	const float HP_BAR_BACK_HEIGHT = 120.0f;
	const float HP_BAR_FOWARD_WIDTH = 580.0f;
	const float HP_BAR_FOWARD_HEIGHT = 80.0f;
	const float HP_BAR_SPLITSCREEN_WIDTH = 380.0f;
	const float HP_BAR_SPLITSCREEN_HEIGHT = 80.0f;

	const Vector3 HP_BAR_POS = Vector3(-670.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_POS_RIGHT = Vector3(775.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_POS_LEFT = Vector3(-775.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_FLONT_POS = Vector3(-960.0f, -480.0f, 0.0f);	//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_POS_LEFT = Vector3(-965.0f, -480.0f, 0.0f);	//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_POS_RIGHT = Vector3(965.0f, -480.0f, 0.0f);	//HPバーの表のポジション

	const Vector3 HP_FONT_POS = Vector3(-650.0f, -445.0f, 0.0f);
	const Vector3 HP_FONT_POS_LEFT = Vector3(-780.0f, -455.0f, 0.0f);
	const Vector3 HP_FONT_POS_RIGHT = Vector3(610.0f, -455.0f, 0.0f);

	const Vector3 SKILL_COOLTIME_FONT_POS = Vector3(479.0f, -220.0f, 0.0f);
	const Vector3 SKILL_COOLTIME_FONT_POS_1P = Vector3(-425.0f, -320.0f, 0.0f);
	const Vector3 SKILL_COOLTIME_FONT_POS_2P = Vector3(375.0f, -320.0f, 0.0f);
	const float SKILL_COOLTIME_FONT_SHADOW_OFFSET = 2.0f;
	const float SKILL_COOLTIME_FONT_SCALE = 1.7f;
	const float SKILL_COOLTIME_FONT_SCALE_MULTI = 1.0f;

	const float SKILL_AND_ULT_ICON_RESOLUTION_SOLO = 162.0f;
	const float SKILL_AND_ULT_ICON_RESOLUTION_MULTI = 121.0f;
	const Vector3 SKILL_POS_SOLO = Vector3(522.0f, -275.0f, 0.0f);  //スキルアイコンポジション
	const Vector3 SKILL_POS_1P = Vector3(-400.0f, -360.0f, 0.0f);	//スキルアイコンポジション
	const Vector3 SKILL_POS_2P = Vector3(400.0f, -360.0f, 0.0f);	//スキルアイコンポジション
	const Vector3 SKILL_SCALE_SOLO = Vector3(1.1f, 1.1f, 1.0f);
	const Vector3 ULT_POS_SOLO = Vector3(470.0f, -445.0f, 0.0f);    //必殺技アイコンポジション
	const Vector3 ULT_POS_1P = Vector3(-440.0f, -475.0f, 0.0f);     //必殺技アイコンポジション
	const Vector3 ULT_POS_2P = Vector3(440.0f, -475.0f, 0.0f);		//必殺技アイコンポジション
	const Vector3 ULT_ICON_SCALE_SOLO = Vector3(1.2f, 1.2f, 1.0f);
	const Vector3 ULT_ICON_SCALE_MULTI = Vector3(1.05f, 1.05f, 1.0f);

	const Vector3 LEVEL_SPRITE_POS_SOLO = Vector3(640.0f, -310.0f, 0.0f);       //Lv
	const Vector3 LEVEL_SPRITE_POS_1P = Vector3(-300.0f, -360.0f, 0.0f);
	const Vector3 LEVEL_SPRITE_POS_2P = Vector3(70.0f, -360.0f, 0.0f);
	const Vector3 LEVEL_SPRITE_SCALE = Vector3(0.4f, 0.4f, 1.0f);

	const float LEVEL_NUMBER_RESOLUTION = 150.0f;
	const float LEVEL_MAX_RESOLUTION = 196.0f;
	const Vector3 LEVEL_NUMBER_POS_SOLO = Vector3(780.0f, -360.0f, 0.0f);
	const Vector3 LEVEL_NUMBER_POS_1P = Vector3(-180.0f, -400.0f, 0.0f);
	const Vector3 LEVEL_NUMBER_POS_2P = Vector3(185.0f, -400.0f, 0.0f);
	const Vector3 LEVEL_NUMBER_SCALE_SOLO = Vector3(1.4f, 1.4f, 1.0f);
	const Vector3 MAX_LEVEL_POS_SOLO = Vector3(920.0f, -400.0f, 0.0f);       //10
	const Vector3 MAX_LEVEL_POS_1P = Vector3(-60.0f, -420.0f, 0.0f);
	const Vector3 MAX_LEVEL_POS_2P = Vector3(310.0f, -420.0f, 0.0f);
	const Vector3 MAX_LEVEL_SCALE = Vector3(0.5f, 0.5f, 1.0f);

	const float FLAME_WIDTH_SOLO = 1200.0f;
	const float FLAME_HEIGHT_SOLO = 500.0f;
	const float FLAME_WIDTH_MULTI = 520.0f;
	const float FLAME_HEIGHT_MULTI = 250.0f;
	const Vector3 FLAME_POS_SOLO = Vector3(920.0f, -480.0f, 0.0f);    //レベルや経験値のフレーム
	const Vector3 FLAME_POS_1P = Vector3(-200.0f, -435.0f, 0.0f);
	const Vector3 FLAME_POS_2P = Vector3(255.0f, -435.0f, 0.0f);

	const float LEVEL_SPRITE_WIDTH = 196.0f;
	const float LEVEL_SPRITE_HEIGHT = 150.0f;

	const float DownPointPosY = 100.0f;

	const Vector3 EXPERIENCE_FLAME_POS = Vector3(750.0f, -500.0f, 0.0f);  //経験値テーブル
	const Vector3 EXPERIENCE_FLAME_POS_DUO_1P = Vector3(-200.0f, -500.0f, 0.0f);
	const Vector3 EXPERIENCE_FLAME_POS_DUO_2P = Vector3(200.0f, -500.0f, 0.0f);
	const Vector3 EXPERIENCE_SCALE = Vector3(0.5f, 0.5f, 1.0f);

	const float EXPBAR_WIDTH = 300.0f;
	const float EXPBAR_HEIGHT = 70.0f;
	const float EXPBAR_FLAME_WIDTH = 600.0f;
	const float EXPBAR_FLAME_HEIGHT = 120.0f;
	const Vector3 EXPERIENCE_BAR_POS = Vector3(600.0f, -500.0f, 0.0f);	//経験値バーの座標
	const Vector3 EXPERIENCE_BAR_POS_DUO_1P = Vector3(-350.0f, -500.0f, 0.0f);	//経験値バーの座標
	const Vector3 EXPERIENCE_BAR_POS_DUO_2P = Vector3(48.0f, -500.0f, 0.0f);	//経験値バーの座標
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
	m_player1P = FindGO<Player>("player");
	fade = FindGO<Fade>("fade");

	m_gameMode = g_renderingEngine->GetGameMode();

	//マルチプレイなら
	if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
	{
		m_player2P = FindGO<Player>("player2");
	}

	InitAssets();

	m_GameUIState = m_GameStartState;

	return true;
}

void GameUI::InitAssets()
{
	//ポイント関連
	{
		//キャラのポイントを表示
		m_Actors = m_game->GetActors();
		int num = 0;
		int enemyNum = 0;
		for (auto actor : m_Actors)
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
				//ブルー
				m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Blue.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
				m_charIcon[num].SetPosition(CharIconPos[num]);
				m_charIcon[num].Update();
				//フレームをプレイヤー用にする
				m_pointFlame[num].Init("Assets/sprite/gameUI/pointFlame_player.DDS", 300.0f, 100.0f);
			}
			else
			{
				//レベル
				m_LevelFont[enemyNum].SetPosition(LevelPos[enemyNum]);
				m_LevelFont[enemyNum].SetScale(0.6f);
				m_LevelFont[enemyNum].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
				m_LevelFont[enemyNum].SetRotation(0.0f);
				m_LevelFont[enemyNum].SetShadowParam(true, 2.0f, g_vec4Black);
				enemyNum++;
			}

			//赤の剣士なら
			if (actor->IsMatchName(KnightAI_Red) || actor->IsMatchName(knightname2))
			{
				//レッド
				m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Red.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
				m_charIcon[num].SetPosition(CharIconPos[num]);
				m_charIcon[num].Update();
				m_pointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}

			if (actor->IsMatchName(KnightAI_Green))
			{
				//グリーン
				m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Green.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
				m_charIcon[num].SetPosition(CharIconPos[num]);
				m_charIcon[num].Update();
				m_pointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}

			if (actor->IsMatchName(KnightAI_Yellow))
			{
				//イエロー
				m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Yellow.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
				m_charIcon[num].SetPosition(CharIconPos[num]);
				m_charIcon[num].Update();
				m_pointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}

			m_pointFlame[num].SetPosition(PointFlamePos[num]);
			m_pointFlame[num].SetScale(1.0f, 1.0f, 1.0f);
			m_pointFlame[num].Update();

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
		if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
		{
			m_Flame[enPlayerNumber_1P].Init("Assets/sprite/gameUI/LevelBar.DDS", FLAME_WIDTH_MULTI, FLAME_HEIGHT_MULTI);
			m_Flame[enPlayerNumber_2P].Init("Assets/sprite/gameUI/LevelBar2P.DDS", FLAME_WIDTH_MULTI, FLAME_HEIGHT_MULTI);
			m_Flame[enPlayerNumber_1P].SetPosition(FLAME_POS_1P);
			m_Flame[enPlayerNumber_2P].SetPosition(FLAME_POS_2P);

			//必殺技のアイコン
			m_UltRenderIN[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Ult_Thunder_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_UltRenderIN[enPlayerNumber_1P].SetPosition(ULT_POS_1P);
			m_UltRenderIN[enPlayerNumber_1P].SetScale(ULT_ICON_SCALE_MULTI);
			m_UltRenderIN[enPlayerNumber_2P].Init("Assets/sprite/gameUI/Ult_Thunder_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_UltRenderIN[enPlayerNumber_2P].SetPosition(ULT_POS_2P);
			m_UltRenderIN[enPlayerNumber_2P].SetScale(ULT_ICON_SCALE_MULTI);
			//必殺のアイコンフレーム
			m_UltRenderOUT[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ULT_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_UltRenderOUT[enPlayerNumber_1P].SetPosition(ULT_POS_1P);
			m_UltRenderOUT[enPlayerNumber_1P].SetScale(ULT_ICON_SCALE_MULTI);
			m_UltRenderOUT[enPlayerNumber_2P].Init("Assets/sprite/gameUI/ULT_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_UltRenderOUT[enPlayerNumber_2P].SetPosition(ULT_POS_2P);
			m_UltRenderOUT[enPlayerNumber_2P].SetScale(ULT_ICON_SCALE_MULTI);
			//スキルのアイコン
			m_SkillRenderIN[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Skill_Icon_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_SkillRenderIN[enPlayerNumber_1P].SetPosition(SKILL_POS_1P);
			m_SkillRenderIN[enPlayerNumber_2P].Init("Assets/sprite/gameUI/Skill_Icon_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_SkillRenderIN[enPlayerNumber_2P].SetPosition(SKILL_POS_2P);
			//スキルのアイコンフレーム
			m_SkillRenderOUT[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Skill_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_SkillRenderOUT[enPlayerNumber_1P].SetPosition(SKILL_POS_1P);
			m_SkillRenderOUT[enPlayerNumber_2P].Init("Assets/sprite/gameUI/Skill_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION_MULTI, SKILL_AND_ULT_ICON_RESOLUTION_MULTI);
			m_SkillRenderOUT[enPlayerNumber_2P].SetPosition(SKILL_POS_2P);

			//スキルのクールタイムを表示するフォントの設定
			m_skillFont[enPlayerNumber_1P].SetPosition(SKILL_COOLTIME_FONT_POS_1P);
			m_skillFont[enPlayerNumber_1P].SetScale(SKILL_COOLTIME_FONT_SCALE_MULTI);
			m_skillFont[enPlayerNumber_1P].SetColor(g_vec4Red);
			m_skillFont[enPlayerNumber_1P].SetShadowParam(true, SKILL_COOLTIME_FONT_SHADOW_OFFSET, g_vec4Black);
			playerCoolTime[enPlayerNumber_1P] = m_player1P->CharGetSkillCoolTimer();

			m_skillFont[enPlayerNumber_2P].SetPosition(SKILL_COOLTIME_FONT_POS_2P);
			m_skillFont[enPlayerNumber_2P].SetScale(SKILL_COOLTIME_FONT_SCALE_MULTI);
			m_skillFont[enPlayerNumber_2P].SetColor(g_vec4Red);
			m_skillFont[enPlayerNumber_2P].SetShadowParam(true, SKILL_COOLTIME_FONT_SHADOW_OFFSET, g_vec4Black);
			playerCoolTime[enPlayerNumber_2P] = m_player2P->CharGetSkillCoolTimer();

			//Lvの画像を読み込む
			m_Lv[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Lv.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
			m_Lv[enPlayerNumber_1P].SetPosition(LEVEL_SPRITE_POS_1P);
			m_Lv[enPlayerNumber_1P].SetScale(LEVEL_SPRITE_SCALE);
			m_Lv[enPlayerNumber_2P].Init("Assets/sprite/gameUI/Lv.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
			m_Lv[enPlayerNumber_2P].SetPosition(LEVEL_SPRITE_POS_2P);
			m_Lv[enPlayerNumber_2P].SetScale(LEVEL_SPRITE_SCALE);

			//Lv1の画像を読み込む
			m_LvNumber[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Lv1.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_LvNumber[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_1P);
			m_LvNumber[enPlayerNumber_2P].Init("Assets/sprite/gameUI/Lv1.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_LvNumber[enPlayerNumber_2P].SetPosition(LEVEL_NUMBER_POS_2P);

			//Lv1の裏の画像の読み込み
			m_LvNumber_back[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Lv1_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_LvNumber_back[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_1P);
			m_LvNumber_back[enPlayerNumber_2P].Init("Assets/sprite/gameUI/Lv1_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_LvNumber_back[enPlayerNumber_2P].SetPosition(LEVEL_NUMBER_POS_2P);

			//10の画像を読み込む
			m_MaxLv[enPlayerNumber_1P].Init("Assets/sprite/gameUI/maxLv.DDS", LEVEL_MAX_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_MaxLv[enPlayerNumber_1P].SetPosition(MAX_LEVEL_POS_1P);
			m_MaxLv[enPlayerNumber_1P].SetScale(MAX_LEVEL_SCALE);
			m_MaxLv[enPlayerNumber_2P].Init("Assets/sprite/gameUI/maxLv.DDS", LEVEL_MAX_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_MaxLv[enPlayerNumber_2P].SetPosition(MAX_LEVEL_POS_2P);
			m_MaxLv[enPlayerNumber_2P].SetScale(MAX_LEVEL_SCALE);

			//経験値テーブルと初期経験値
			m_expTable[enPlayerNumber_2P] = m_player2P->CharSetEXPTable();
			m_mathExp[enPlayerNumber_2P] = m_player1P->CharGetEXP();

			//経験値のフレーム
			m_ExperienceFlame[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ExperienceBar.DDS", EXPBAR_FLAME_WIDTH, EXPBAR_FLAME_HEIGHT);
			m_ExperienceFlame[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS_DUO_1P);
			m_ExperienceFlame[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);
			m_ExperienceFlame[enPlayerNumber_2P].Init("Assets/sprite/gameUI/ExperienceBar.DDS", EXPBAR_FLAME_WIDTH, EXPBAR_FLAME_HEIGHT);
			m_ExperienceFlame[enPlayerNumber_2P].SetPosition(EXPERIENCE_FLAME_POS_DUO_2P);
			m_ExperienceFlame[enPlayerNumber_2P].SetScale(EXPERIENCE_SCALE);

			//経験値バーの裏
			m_experienceBarBack[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", EXPBAR_FLAME_WIDTH, EXPBAR_FLAME_HEIGHT);
			m_experienceBarBack[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS_DUO_1P);
			m_experienceBarBack[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);
			m_experienceBarBack[enPlayerNumber_2P].Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", EXPBAR_FLAME_WIDTH, EXPBAR_FLAME_HEIGHT);
			m_experienceBarBack[enPlayerNumber_2P].SetPosition(EXPERIENCE_FLAME_POS_DUO_2P);
			m_experienceBarBack[enPlayerNumber_2P].SetScale(EXPERIENCE_SCALE);

			//経験値バー
			m_experienceBarFlont[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_WIDTH, EXPBAR_HEIGHT);
			m_experienceBarFlont[enPlayerNumber_1P].SetPosition(EXPERIENCE_BAR_POS_DUO_1P);
			m_experienceBarFlont[enPlayerNumber_1P].SetPivot(EXPERIENCEGAUGE_PIVOT);
			m_experienceBarFlont[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);
			m_experienceBarFlont[enPlayerNumber_2P].Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_WIDTH, EXPBAR_HEIGHT);
			m_experienceBarFlont[enPlayerNumber_2P].SetPosition(EXPERIENCE_BAR_POS_DUO_2P);
			m_experienceBarFlont[enPlayerNumber_2P].SetPivot(EXPERIENCEGAUGE_PIVOT);
			m_experienceBarFlont[enPlayerNumber_2P].SetScale(EXPERIENCE_SCALE);
		}
		else
		{
			//レベルや経験値のフレーム
			m_Flame[enPlayerNumber_1P].Init("Assets/sprite/gameUI/LevelBar.DDS", FLAME_WIDTH_SOLO, FLAME_HEIGHT_SOLO);
			m_Flame[enPlayerNumber_1P].SetPosition(FLAME_POS_SOLO);

			//必殺技のアイコン
			m_UltRenderIN[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Ult_Thunder_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION_SOLO, SKILL_AND_ULT_ICON_RESOLUTION_SOLO);
			m_UltRenderIN[enPlayerNumber_1P].SetPosition(ULT_POS_SOLO);
			m_UltRenderIN[enPlayerNumber_1P].SetScale(ULT_ICON_SCALE_SOLO);
			//必殺のアイコンフレーム
			m_UltRenderOUT[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ULT_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION_SOLO, SKILL_AND_ULT_ICON_RESOLUTION_SOLO);
			m_UltRenderOUT[enPlayerNumber_1P].SetPosition(ULT_POS_SOLO);
			m_UltRenderOUT[enPlayerNumber_1P].SetScale(ULT_ICON_SCALE_SOLO);
			//スキルのアイコン
			m_SkillRenderIN[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Skill_Icon_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION_SOLO, SKILL_AND_ULT_ICON_RESOLUTION_SOLO);
			m_SkillRenderIN[enPlayerNumber_1P].SetPosition(SKILL_POS_SOLO);
			m_SkillRenderIN[enPlayerNumber_1P].SetScale(SKILL_SCALE_SOLO);
			//スキルのアイコンフレーム
			m_SkillRenderOUT[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Skill_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION_SOLO, SKILL_AND_ULT_ICON_RESOLUTION_SOLO);
			m_SkillRenderOUT[enPlayerNumber_1P].SetPosition(SKILL_POS_SOLO);
			m_SkillRenderOUT[enPlayerNumber_1P].SetScale(SKILL_SCALE_SOLO);

			//スキルのクールタイムを表示するフォントの設定
			m_skillFont[enPlayerNumber_1P].SetPosition(SKILL_COOLTIME_FONT_POS);
			m_skillFont[enPlayerNumber_1P].SetScale(SKILL_COOLTIME_FONT_SCALE);
			m_skillFont[enPlayerNumber_1P].SetColor(g_vec4Red);
			m_skillFont[enPlayerNumber_1P].SetShadowParam(true, SKILL_COOLTIME_FONT_SHADOW_OFFSET, g_vec4Black);
			playerCoolTime[enPlayerNumber_1P] = m_player1P->CharGetSkillCoolTimer();

			//Lvの画像を読み込む
			m_Lv[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Lv.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
			m_Lv[enPlayerNumber_1P].SetPosition(LEVEL_SPRITE_POS_SOLO);
			m_Lv[enPlayerNumber_1P].SetScale(LEVEL_SPRITE_SCALE);

			//Lv1の画像を読み込む
			m_LvNumber[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Lv1.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_LvNumber[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_SOLO);
			m_LvNumber[enPlayerNumber_1P].SetScale(LEVEL_NUMBER_SCALE_SOLO);

			//Lv1の裏の画像の読み込み
			m_LvNumber_back[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Lv1_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_LvNumber_back[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_SOLO);
			m_LvNumber_back[enPlayerNumber_1P].SetScale(LEVEL_NUMBER_SCALE_SOLO);

			//10の画像を読み込む
			m_MaxLv[enPlayerNumber_1P].Init("Assets/sprite/gameUI/maxLv.DDS", LEVEL_MAX_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
			m_MaxLv[enPlayerNumber_1P].SetPosition(MAX_LEVEL_POS_SOLO);
			m_MaxLv[enPlayerNumber_1P].SetScale(MAX_LEVEL_SCALE);

			//経験値のフレーム
			m_ExperienceFlame[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ExperienceBar.DDS", 600.0f, 120.0f);
			m_ExperienceFlame[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS);
			m_ExperienceFlame[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);

			//経験値バーの裏
			m_experienceBarBack[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", 600.0f, 120.0f);
			m_experienceBarBack[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS);
			m_experienceBarBack[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);

			//経験値バー
			m_experienceBarFlont[enPlayerNumber_1P].Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_WIDTH, EXPBAR_HEIGHT);
			m_experienceBarFlont[enPlayerNumber_1P].SetPosition(EXPERIENCE_BAR_POS);
			m_experienceBarFlont[enPlayerNumber_1P].SetPivot(EXPERIENCEGAUGE_PIVOT);
			m_experienceBarFlont[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);
		}

		//経験値テーブルと初期経験値
		m_expTable[enPlayerNumber_1P] = m_player1P->CharSetEXPTable();
		m_mathExp[enPlayerNumber_1P] = m_player1P->CharGetEXP();

		//レベルアップまでに必要な経験値の量
		m_ExpFont.SetPosition(UPTOLEVEL_POS);
		m_ExpFont.SetScale(1.0f);
		m_ExpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_ExpFont.SetRotation(0.0f);
		m_ExpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		for (int i = 0; i < enPlayerNumber_Num; i++)
		{
			//スプライト更新
			m_Lv[i].Update();
			m_Flame[i].Update();
			m_SkillRenderIN[i].Update();
			m_SkillRenderOUT[i].Update();
			m_UltRenderIN[i].Update();
			m_UltRenderOUT[i].Update();
			m_LvNumber[i].Update();
			m_LvNumber_back[i].Update();
			m_MaxLv[i].Update();
			m_ExperienceFlame[i].Update();
			m_experienceBarBack[i].Update();
			m_experienceBarFlont[i].Update();
		}
	}

	//HP関連
	{

		for (int i = 0; i < enPlayerNumber_Num; i++)
		{
			//HPのフォント
			m_HpFont[i].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_HpFont[i].SetRotation(0.0f);
			m_HpFont[i].SetShadowParam(true, 2.0f, g_vec4Black);

			if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
			{
				//HPゲージ裏の画像を読み込む
				m_hpBarBack[i].Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", HP_BAR_SPLITSCREEN_WIDTH, HP_BAR_BACK_HEIGHT);
				//HPゲージの画像を読み込む
				m_hpBar[i].Init("Assets/sprite/gameUI/HPBar_HP.DDS", HP_BAR_SPLITSCREEN_WIDTH, HP_BAR_SPLITSCREEN_HEIGHT);
				//HPバーの白い部分
				m_HpBar_White[i].Init("Assets/sprite/gameUI/HPBar_backwhite.DDS", HP_BAR_SPLITSCREEN_WIDTH, HP_BAR_SPLITSCREEN_HEIGHT);
				//HPゲージのフレームの画像を読み込む
				m_HPFrame[i].Init("Assets/sprite/gameUI/HPBar_flame.DDS", HP_BAR_SPLITSCREEN_WIDTH, HP_BAR_BACK_HEIGHT);
			}
			else
			{
				//HPゲージ裏の画像を読み込む
				m_hpBarBack[i].Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", HP_BAR_BACK_WIDTH, HP_BAR_BACK_HEIGHT);
				//HPゲージの画像を読み込む
				m_hpBar[i].Init("Assets/sprite/gameUI/HPBar_HP.DDS", HP_BAR_FOWARD_WIDTH, HP_BAR_FOWARD_HEIGHT);
				//HPバーの白い部分
				m_HpBar_White[i].Init("Assets/sprite/gameUI/HPBar_backwhite.DDS", HP_BAR_FOWARD_WIDTH, HP_BAR_FOWARD_HEIGHT);
				//HPゲージのフレームの画像を読み込む
				m_HPFrame[i].Init("Assets/sprite/gameUI/HPBar_flame.DDS", HP_BAR_BACK_WIDTH, HP_BAR_BACK_HEIGHT);
			}

			m_hpBar[i].SetPivot(HPGAUGE_PIVOT_LEFT);
			m_HpBar_White[i].SetPivot(HPGAUGE_PIVOT_LEFT);
		}

		//プレイヤーのHPを取得　白い部分用
		White_BackHp[enPlayerNumber_1P] = m_player1P->GetCharacterHp();
		WhiteHp_Timer[enPlayerNumber_1P] = WHITEHP_WAIT;
		BackUPLV[enPlayerNumber_1P] = m_player1P->GetCharacterLevel();
		if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
		{
			White_BackHp[enPlayerNumber_2P] = m_player2P->GetCharacterHp();
			WhiteHp_Timer[enPlayerNumber_2P] = WHITEHP_WAIT;
			BackUPLV[enPlayerNumber_2P] = m_player2P->GetCharacterLevel();

			//1P用画像
			m_hpBar[enPlayerNumber_1P].SetPivot(HPGAUGE_PIVOT_LEFT);
			m_HpBar_White[enPlayerNumber_1P].SetPivot(HPGAUGE_PIVOT_LEFT);

			m_hpBarBack[enPlayerNumber_1P].SetPosition(HP_BAR_POS_LEFT);
			m_hpBarBack[enPlayerNumber_1P].SetScale(1.0f, 0.7f, 1.0f);
			m_hpBar[enPlayerNumber_1P].SetPosition(HP_BAR_FLONT_POS_LEFT);
			m_HpBar_White[enPlayerNumber_1P].SetPosition(HP_BAR_FLONT_POS_LEFT);
			m_HPFrame[enPlayerNumber_1P].SetPosition(HP_BAR_POS_LEFT);
			m_HPFrame[enPlayerNumber_1P].SetScale(1.0f, 0.7f, 1.0f);

			//2P用画像
			m_hpBar[enPlayerNumber_2P].SetPivot(HPGAUGE_PIVOT_RIGHT);
			m_HpBar_White[enPlayerNumber_2P].SetPivot(HPGAUGE_PIVOT_RIGHT);

			m_hpBarBack[enPlayerNumber_2P].SetPosition(HP_BAR_POS_RIGHT);
			m_hpBarBack[enPlayerNumber_2P].SetScale(1.0f, 0.7f, 1.0f);
			m_hpBar[enPlayerNumber_2P].SetPosition(HP_BAR_FLONT_POS_RIGHT);
			m_HPFrame[enPlayerNumber_2P].SetPosition(HP_BAR_POS_RIGHT);
			m_HPFrame[enPlayerNumber_2P].SetScale(1.0f, 0.7f, 1.0f);
			m_HpBar_White[enPlayerNumber_2P].SetPosition(HP_BAR_FLONT_POS_RIGHT);

			//HPフォント
			m_HpFont[enPlayerNumber_1P].SetPosition(HP_FONT_POS_LEFT);
			m_HpFont[enPlayerNumber_1P].SetScale(0.7f);
			m_HpFont[enPlayerNumber_2P].SetPosition(HP_FONT_POS_RIGHT);
			m_HpFont[enPlayerNumber_2P].SetScale(0.7f);
		}
		else
		{
			//1P用画像
			m_hpBarBack[enPlayerNumber_1P].SetPosition(HP_BAR_POS);
			m_hpBarBack[enPlayerNumber_1P].SetScale(1.0f, 0.7f, 1.0f);
			m_hpBar[enPlayerNumber_1P].SetPosition(HP_BAR_FLONT_POS);
			m_HPFrame[enPlayerNumber_1P].SetPosition(HP_BAR_POS);
			m_HPFrame[enPlayerNumber_1P].SetScale(1.0f, 0.7f, 1.0f);
			m_HpBar_White[enPlayerNumber_1P].SetPosition(HP_BAR_FLONT_POS);

			//HPフォント
			m_HpFont[enPlayerNumber_1P].SetPosition(HP_FONT_POS);
			m_HpFont[enPlayerNumber_1P].SetScale(1.0f);
		}

		for (int i = 0; i < enPlayerNumber_Num; i++)
		{
			m_hpBar[i].Update();
			m_hpBarBack[i].Update();
			m_HPFrame[i].Update();
			m_HpBar_White[i].Update();
		}
	}

	//制限時間と獲得ポイント
	{
		m_TimeAndPointRender.Init("Assets/sprite/gameUI/timer.DDS", 1100.0f, 400.0f);
		m_TimeAndPointRender.SetPosition(TIME_POS);
		m_TimeAndPointRender.SetScale(0.35f, 0.3f, 1.0f);

		//フォントの大きさを設定。
		m_minutes.SetScale(TIMERSCALE);
		m_seconds.SetScale(TIMERSCALE);
		m_coron.SetScale(TIMERSCALE);
		Vector2 pivotOffSet = { 30.0f, -20.0f };
		//オフセットの設定
		m_minutes.SetPivotOffSet(pivotOffSet);
		m_seconds.SetPivotOffSet(pivotOffSet);
		m_coron.SetPivotOffSet(pivotOffSet);

		//座標の設定
		m_minutes.SetPosition(MINUTES_FONT_POS);
		m_seconds.SetPosition(SECONDS_FONT_POS);
		m_coron.SetPosition(CORON_FONT_POS);

		//フォントの色を設定。
		m_minutes.SetColor(m_limitColor);
		m_seconds.SetColor(m_limitColor);
		m_coron.SetColor(m_limitColor);

		//輪郭線の設定
		m_minutes.SetShadowParam(true, 2.0f, g_vec4Black);
		m_seconds.SetShadowParam(true, 2.0f, g_vec4Black);
		m_coron.SetShadowParam(true, 2.0f, g_vec4Black);

		m_coron.SetText(L":");
		m_TimeAndPointRender.Update();
	}
}

void GameUI::SkillCoolTimeFont()
{
	int SkillCoolTime = m_player1P->CharGetSkillCoolTimer();
	wchar_t Skill[255];
	swprintf_s(Skill, 255, L"%d", SkillCoolTime);
	m_skillFont[enPlayerNumber_1P].SetText(Skill);

	if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
	{
		SkillCoolTime = m_player2P->CharGetSkillCoolTimer();
		swprintf_s(Skill, 255, L"%d", SkillCoolTime);
		m_skillFont[enPlayerNumber_2P].SetText(Skill);
	}
}

void GameUI::Update()
{
	//ゲームの制限時間に達しているなら
	if (m_game->IsGameEnd() == true) {
		return;
	}

	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState == m_PauseState) {
		return;
	}

	//ゲームのステートがgameStartなら
	if (m_game->NowGameState() == Game::enGameState_Start)
	{
		CountDown();
	}

	//プレイヤーがリスポーン待機中なら
	if (m_player1P->CharGetRespawnTime() > 0.0f)
	{
		RespawnCountDown(enPlayerNumber_1P);
	}
	if (m_gameMode == RenderingEngine::enGameMode_DuoPlay && m_player2P->CharGetRespawnTime() > 0.0f)
	{
		RespawnCountDown(enPlayerNumber_2P);
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

	SkillCoolTimeFont();

	//レベルの点滅
	if (m_flashNumberFlag == false)
	{
		m_LvNumberColor -= 0.02f;

		if (m_LvNumberColor < 0.0f)
		{
			m_LvNumberColor = 0.0f;
			m_flashNumberFlag = true;
		}

		m_LvNumber_back[enPlayerNumber_1P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
		m_LvNumber_back[enPlayerNumber_2P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
	}
	else if (m_flashNumberFlag == true)
	{
		m_LvNumberColor += 0.02f;

		if (m_LvNumberColor > 1.0f)
		{
			m_LvNumberColor = 1.0f;
			m_flashNumberFlag = false;
		}

		m_LvNumber_back[enPlayerNumber_1P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
		m_LvNumber_back[enPlayerNumber_2P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
	}
	m_LvNumber_back[enPlayerNumber_1P].Update();
	m_LvNumber_back[enPlayerNumber_2P].Update();

	ExpState(m_player1P);
	if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
	{
		ExpState(m_player2P);
	}

	HPBar();
}



//ゲームスタートのカウントダウン
void GameUI::CountDown()
{
	//カウントダウン
	//int COUNTDOWNTIMER = m_game->CountDownMinutes();
	int StartCountDown = m_game->CountDownMinutes();

	if (m_game->CountDownMinutes() <= 0)
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
	else if (m_fightFlag == false && m_gameCountScale.x < 100.0f)
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
		else if (FightshotStopFlag == false) {
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
void GameUI::RespawnCountDown(EnPlayerNumber playerNumber)
{
	int respornCountDown = 0;
	if (playerNumber == enPlayerNumber_1P)
	{
		//カウントダウン
		respornCountDown = (int)m_player1P->CharGetRespawnTime();
	}
	else {
		//カウントダウン
		respornCountDown = (int)m_player2P->CharGetRespawnTime();
	}

	if (oldRespawnCount[playerNumber] != respornCountDown)
	{
		switch (respornCountDown)
		{
		case 0:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut0.DDS", 300, 500.0f);
			//画面を暗くしてゆく
			if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
			{
				if (playerNumber == enPlayerNumber_1P) {
					fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_Left);
				}
				else {
					fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_Right);
				}
				break;
			}
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

	oldRespawnCount[playerNumber] = respornCountDown;

	m_RespawnCountNumber.Update();
}

//プレイヤーのHPの表示の処理
void GameUI::HPBar()
{
	int HP = m_player1P->GetCharacterHp();
	int MaxHP = m_player1P->GetCharcterMaxHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d/%d", HP, MaxHP);
	m_HpFont[enPlayerNumber_1P].SetText(hp);

	Vector3 HpScale = Vector3::One;
	//HPバーの減っていく割合。
	HpScale.x = (float)m_player1P->GetCharacterHp() / (float)m_player1P->GetCharcterMaxHp();
	m_hpBar[enPlayerNumber_1P].SetScale(HpScale);

	m_hpBar[enPlayerNumber_1P].Update();

	//レベルが下がった時の処理
	if (BackUPLV[enPlayerNumber_1P] > m_player1P->GetCharacterLevel())
	{
		White_BackHp[enPlayerNumber_1P] = HP;
	}
	BackUPLV[enPlayerNumber_1P] = m_player1P->GetCharacterLevel();

	//Hp削られたら白い部分も減らす
	if (HP < White_BackHp[enPlayerNumber_1P])
	{
		if (WhiteHp_Timer[enPlayerNumber_1P] > 0.0f)
		{
			WhiteHp_Timer[enPlayerNumber_1P] -= g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			White_BackHp[enPlayerNumber_1P] -= 2;

			//HPバーの減っていく割合。
			HpScale.x = (float)White_BackHp[enPlayerNumber_1P] / (float)MaxHP;
			m_HpBar_White[enPlayerNumber_1P].SetScale(HpScale);

			if (White_BackHp[enPlayerNumber_1P] <= HP)
			{
				White_BackHp[enPlayerNumber_1P] = HP;
				WhiteHp_Timer[enPlayerNumber_1P] = WHITEHP_WAIT;
			}
		}
	}
	else if (HP > White_BackHp[enPlayerNumber_1P])
	{
		White_BackHp[enPlayerNumber_1P] = HP;
	}
	m_HpBar_White[enPlayerNumber_1P].Update();

	if (m_gameMode == RenderingEngine::enGameMode_SoloPlay)
	{
		return;
	}

	//マルチプレイ時
	HP = m_player2P->GetCharacterHp();
	MaxHP = m_player2P->GetCharcterMaxHp();
	swprintf_s(hp, 255, L"%d/%d", HP, MaxHP);
	m_HpFont[enPlayerNumber_2P].SetText(hp);

	HpScale = Vector3::One;
	//HPバーの減っていく割合。
	HpScale.x = (float)m_player2P->GetCharacterHp() / (float)m_player2P->GetCharcterMaxHp();
	m_hpBar[enPlayerNumber_2P].SetScale(HpScale);

	m_hpBar[enPlayerNumber_2P].Update();

	//レベルが下がった時の処理
	if (BackUPLV[enPlayerNumber_2P] > m_player2P->GetCharacterLevel())
	{
		White_BackHp[enPlayerNumber_2P] = HP;
	}
	BackUPLV[enPlayerNumber_2P] = m_player2P->GetCharacterLevel();

	//Hp削られたら白い部分も減らす
	if (HP < White_BackHp[enPlayerNumber_2P])
	{
		if (WhiteHp_Timer[enPlayerNumber_2P] > 0.0f)
		{
			WhiteHp_Timer[enPlayerNumber_2P] -= g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			White_BackHp[enPlayerNumber_2P] -= 2;

			//HPバーの減っていく割合。
			HpScale.x = (float)White_BackHp[enPlayerNumber_2P] / (float)MaxHP;
			m_HpBar_White[enPlayerNumber_2P].SetScale(HpScale);

			if (White_BackHp[enPlayerNumber_2P] <= HP)
			{
				White_BackHp[enPlayerNumber_2P] = HP;
				WhiteHp_Timer[enPlayerNumber_2P] = WHITEHP_WAIT;
			}
		}
	}
	else if (HP > White_BackHp[enPlayerNumber_2P])
	{
		White_BackHp[enPlayerNumber_2P] = HP;
	}
	m_HpBar_White[enPlayerNumber_2P].Update();

}

//AIのレベルの表示
void GameUI::Level()
{
	int num = 0;
	int enemyNum = 0;
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
		m_LevelFont[enemyNum].SetText(AILv);
		enemyNum++;
		num++;
	}
}

//残り時間10秒の時の処理
void GameUI::Timer()
{
	//制限時間の表示
	wchar_t minutesBuf[256];
	wchar_t secondsBuf[256];
	swprintf_s(minutesBuf, 256, L"%d", (int)m_game->GetMinutesTimer());
	swprintf_s(secondsBuf, 256, L"%02d", (int)m_game->GetSecondsTimer());
	//表示するテキストを設定。

	m_minutes.SetText(minutesBuf);
	m_seconds.SetText(secondsBuf);

	//ゲームタイマーが1秒より大きく、10秒より少ないとき
	if (m_game->GetMinutesTimer() < 1 && m_game->GetSecondsTimer() < 10 && m_game->GetSecondsTimer() > 1)
	{
		if (m_timerScaleFlag == false)
		{
			if (m_timerScale < 2.8f)
			{
				m_timerScale += 1.9f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				m_timerScale = 2.8f;
				m_timerScaleFlag = true;
			}

			if (m_limitColor.y == m_limitColor.z > 0.0f) {
				m_limitColor.y -= 0.1f * g_gameTime->GetFrameDeltaTime();
				m_limitColor.z -= 0.1f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				m_limitColor.y = 0.0f;
				m_limitColor.z = 0.0f;
			}


		}
		else
		{
			if (m_timerScale > TIMERSCALE)
			{
				m_timerScale -= 3.4f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				m_timerScale = TIMERSCALE;
				m_timerScaleFlag = false;
			}

			if (m_limitColor.y == m_limitColor.z < 1.0f) {
				m_limitColor.y += 0.1f * g_gameTime->GetFrameDeltaTime();
				m_limitColor.z += 0.1f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				m_limitColor.y = 1.0f;
				m_limitColor.z = 1.0f;
			}
		}

		m_minutes.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_minutes.SetScale(m_timerScale);
		m_minutes.SetColor(m_limitColor);
		m_seconds.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_seconds.SetScale(m_timerScale);
		m_seconds.SetColor(m_limitColor);
		m_coron.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_coron.SetScale(m_timerScale);
		m_coron.SetColor(m_limitColor);
	}
	else
	{
		m_minutes.SetScale(TIMERSCALE);
		m_seconds.SetScale(TIMERSCALE);
		m_coron.SetScale(TIMERSCALE);
	}
}

void GameUI::ExpState(const Player* player)
{
	EnPlayerNumber enPlayerNumber = enPlayerNumber_1P;
	if (player == m_player2P)
	{
		enPlayerNumber = enPlayerNumber_2P;
	}

	//レベルが下がったら
	if (m_playerLevel[enPlayerNumber] > player->GetCharacterLevel()) {
		m_expUpFlag[enPlayerNumber] = true;
		m_enExpProcessState[enPlayerNumber] = enLevelDownState;
	}

	if (m_expUpFlag[enPlayerNumber] == false) {
		return;
	}

	switch (m_enExpProcessState[enPlayerNumber])
	{
	case GameUI::enChackExpState:
		ChackExp(player, enPlayerNumber);
		break;
	case GameUI::enUpExpState:
		UpExp(player, enPlayerNumber);
		break;
	case GameUI::enDownExpState:
		DownExp(enPlayerNumber);
		break;
	case GameUI::enLevelUpState:
		LevelUp(player, enPlayerNumber);
		break;
	case GameUI::enLevelDownState:
		LevelDown(player, enPlayerNumber);
		break;
	default:
		break;
	}

	m_oldSaveExp[enPlayerNumber] = m_saveExp[enPlayerNumber];

	//経験値の表示
	Vector3 EXPScale = Vector3::One;

	//HPバーの増えていく割合。
	EXPScale.x = (float)m_mathExp[enPlayerNumber] / (float)m_expTable[enPlayerNumber];


	m_experienceBarFlont[enPlayerNumber].SetScale(EXPScale);
	m_experienceBarFlont[enPlayerNumber].Update();

	//レベルアップまでに必要な経験値の量
	int UpToLevel = m_mathExp[enPlayerNumber];
	wchar_t UTL[255];
	swprintf_s(UTL, 255, L"%d", UpToLevel);
	m_ExpFont.SetText(UTL);
}

//取得した経験値の量が変わったか調べる
void GameUI::ChackExp(const Player* player, const EnPlayerNumber playerNumber)
{
	//レベルが下がったら
	if (m_playerLevel[playerNumber] > player->GetCharacterLevel()) {
		m_enExpProcessState[playerNumber] = enLevelDownState;
		return;
	}

	//セーブした経験値が前フレームのセーブした経験値と違うなら
	if (player->CharGetSaveEXP() != m_oldSaveExp[playerNumber]) {
		m_saveExp[playerNumber] = player->CharGetSaveEXP();

		m_enExpProcessState[playerNumber] = enUpExpState;
	}

}

void GameUI::UpExp(const Player* player, const EnPlayerNumber playerNumber)
{
	if (m_mathExp[playerNumber] >= m_expTable[playerNumber])
	{
		m_enExpProcessState[playerNumber] = enLevelUpState;
	}
	else if (m_mathExp[playerNumber] < m_saveExp[playerNumber]) {
		m_mathExp[playerNumber]++;
	}
	else {
		m_expUpFlag[playerNumber] = false;

		m_enExpProcessState[playerNumber] = enChackExpState;
	}
}

void GameUI::LevelUp(const Player* player, const EnPlayerNumber playerNumber)
{

	m_saveExp[playerNumber] -= m_expTable[playerNumber];

	m_mathExp[playerNumber] = 0;

	//レベルアップの処理
	if (m_playerLevel[playerNumber] < player->GetCharacterLevel()) {
		m_playerLevel[playerNumber]++;
	}
	//レベルに応じた経験値テーブルにする
	m_expTable[playerNumber] = player->CharGetEXPTableForLevel(m_playerLevel[playerNumber]);

	//レベル画像変更
	LevelSpriteChange(m_playerLevel[playerNumber], playerNumber);

	if (m_playerLevel[playerNumber] == 10) {
		m_saveExp[playerNumber] = 10;
		player->CharResatSaveEXP(m_saveExp[playerNumber]);
		m_oldSaveExp[playerNumber] = m_saveExp[playerNumber];
		m_mathExp[playerNumber] = m_saveExp[playerNumber];
		m_enExpProcessState[playerNumber] = enChackExpState;
		m_expUpFlag[playerNumber] = false;
		return;
	}

	//まだセーブした経験値が残っているなら
	if (m_saveExp[playerNumber] > 0) {
		//セーブした経験値をリセット
		//m_saveExpとプレイヤーのセーブした経験値を同じにする
		if (player->CharGetSaveEXP() > 0) {
			//セーブした経験値が変わらない
			player->CharResatSaveEXP(m_saveExp[playerNumber]);
		}

		m_oldSaveExp[playerNumber] = player->CharGetSaveEXP();
		m_enExpProcessState[playerNumber] = enUpExpState;
	}
	//もうレベルアップの処理が終わりなら
	else if (m_saveExp[playerNumber] <= 0) {

		m_expUpFlag[playerNumber] = false;

		//レベルアップの処理の間に中立の敵を倒していたなら
		if (player->CharGetEXP() > 0) {
			player->CharResatSaveEXP(player->CharGetEXP());
			m_saveExp[playerNumber] = player->CharGetSaveEXP();
			m_oldSaveExp[playerNumber] = m_saveExp[playerNumber];
			//経験値の処理にいく
			m_enExpProcessState[playerNumber] = enUpExpState;
		}
		else
		{
			//セーブした経験値をリセット
			player->CharResatSaveEXP(0);
			m_saveExp[playerNumber] = player->CharGetSaveEXP();
			m_oldSaveExp[playerNumber] = m_saveExp[playerNumber];

			m_enExpProcessState[playerNumber] = enChackExpState;
		}
	}
}

void GameUI::DownExp(const EnPlayerNumber playerNumber)
{
	if (m_mathExp[playerNumber] <= 0) {
		m_enExpProcessState[playerNumber] = enLevelDownState;
	}
	else
	{
		m_mathExp[playerNumber]--;
	}
}

void GameUI::LevelDown(const Player* player, const EnPlayerNumber playerNumber)
{
	m_playerLevel[playerNumber]--;
	//レベルに応じた経験値テーブルにする
	m_expTable[playerNumber] = player->CharGetEXPTableForLevel(m_playerLevel[playerNumber]);

	LevelSpriteChange(m_playerLevel[playerNumber], playerNumber);

	if (m_playerLevel[playerNumber] <= player->GetCharacterLevel()) {
		m_expUpFlag[playerNumber] = false;
		//レベルダウンの処理を終わる
		m_enExpProcessState[playerNumber] = enChackExpState;
		m_saveExp[playerNumber] = 0;
		m_oldSaveExp[playerNumber] = m_saveExp[playerNumber];
		m_mathExp[playerNumber] = 0;
		//セーブした経験値をリセット
		player->CharResatSaveEXP(0);
		return;
	}
	else
	{
		m_mathExp[playerNumber] = m_expTable[playerNumber];

		m_enExpProcessState[playerNumber] = enDownExpState;
	}
}

//キャラのポイントと。ポイントが一番多いキャラに王冠マークをつける表示の処理
void GameUI::CharPoint()
{
	//キャラのポイントを表示
	m_Actors = m_game->GetActors();

	int num = 0;
	for (auto actor : m_Actors)
	{
		//死んでいるかリスポーン中なら
		if (actor->NowCharState() == Actor::enCharState_Death || actor->GetRespawnFlag() == true)
		{
			m_charIcon[num].SetGrayScale(true);
		}
		else
		{
			m_charIcon[num].SetGrayScale(false);
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
			FontPos = ADDPOINTPOS + PointPos[num];
			m_PointFont[num].SetPosition(FontPos);

			m_pointFlame[num].SetScale(1.7f, 1.2f, 0.0f);
			m_pointFlame[num].Update();
			m_charIcon[num].Update();
			MaxPoint = charPoint[num];
		}
		else
		{
			m_PointFont[num].SetScale(1.1f);
			m_PointFont[num].SetPosition(PointPos[num]);
			m_pointFlame[num].SetScale(1.0f, 1.0f, 0.0f);
			m_pointFlame[num].Update();
			m_charIcon[num].Update();
		}


		num++;
	}
}

void GameUI::LevelSpriteChange(const int lv, const EnPlayerNumber playerNumber)
{
	switch (lv)
	{
	case 1:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv1.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv1_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 2:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv2.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv2_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 3:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv3.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv3_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 4:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv4.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv4_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 5:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv5.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv5_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 6:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv6.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv6_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 7:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv7.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv7_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 8:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv8.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv8_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 9:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv9.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv9_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	case 10:
		m_LvNumber[playerNumber].Init("Assets/sprite/gameUI/Lv10.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[playerNumber].Init("Assets/sprite/gameUI/Lv10_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		break;
	}
}

void GameUI::Render(RenderContext& rc)
{
	//finishの画像
	if (m_game->GetMinutesTimer() < 1 && m_game->GetSecondsTimer() <= 0)
	{
		m_FinishCountNumber.Draw(rc);
		return;
	}

	if (m_game->GetStopFlag() == true)
	{
		return;
	}

	//リスポーンするまでの時間
#ifdef m_gameMode == RenderingEngine::enGameMode_DuoPlay
	//マルチプレイ時
	if (m_player1P->CharGetRespawnTime() > 0 || m_player2P->CharGetRespawnTime() > 0)
#else
	//ソロプレイ時
	if (m_player1P->CharGetRespawnTime() > 0)
#endif
	{
		//制限時間
		m_TimeAndPointRender.Draw(rc);
		m_minutes.Draw(rc);
		m_seconds.Draw(rc);
		m_coron.Draw(rc);

		m_Respawn_Back.Draw(rc);
		m_RespawnIn.Draw(rc);
		m_RespawnCountNumber.Draw(rc);

		//試合終了まで残り10秒なら
		if (m_game->GetMinutesTimer() < 1 && m_game->GetSecondsTimer() <= 0)
		{
			m_FinishCountNumber.Draw(rc);
		}
		return;
	}

	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState != m_PauseState && m_GameUIState != m_GameStartState) {
		//レベルや経験値のフレーム
		for (int i = 0; i < g_renderingEngine->GetGameMode(); i++)
		{
			m_Flame[i].Draw(rc);
		}

		//スキルのクールタイムとタイマーが違う時だけ表示
		if (m_player1P->CharGetSkillCoolTimer() != playerCoolTime[enPlayerNumber_1P])
		{
			m_skillFont[enPlayerNumber_1P].Draw(rc);
		}
		if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
		{
			if (m_player2P->CharGetSkillCoolTimer() != playerCoolTime[enPlayerNumber_2P])
			{
				m_skillFont[enPlayerNumber_2P].Draw(rc);
			}
		}

		//制限時間
		m_TimeAndPointRender.Draw(rc);
		m_minutes.Draw(rc);
		m_seconds.Draw(rc);
		m_coron.Draw(rc);

		if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
		{
			m_experienceBarBack[enPlayerNumber_2P].Draw(rc);
			if (m_mathExp[enPlayerNumber_2P] != 0) {
				m_experienceBarFlont[enPlayerNumber_2P].Draw(rc);
			}
			m_ExperienceFlame[enPlayerNumber_2P].Draw(rc);

			m_hpBarBack[enPlayerNumber_2P].Draw(rc);
			m_HpBar_White[enPlayerNumber_2P].Draw(rc);
			m_hpBar[enPlayerNumber_2P].Draw(rc);
			m_HPFrame[enPlayerNumber_2P].Draw(rc);

			m_SkillRenderIN[enPlayerNumber_2P].Draw(rc);
			m_SkillRenderOUT[enPlayerNumber_2P].Draw(rc);

			m_UltRenderIN[enPlayerNumber_2P].Draw(rc);
			m_UltRenderOUT[enPlayerNumber_2P].Draw(rc);

			m_HpFont[enPlayerNumber_2P].Draw(rc);
			m_Lv[enPlayerNumber_2P].Draw(rc);
			m_LvNumber_back[enPlayerNumber_2P].Draw(rc);
			m_LvNumber[enPlayerNumber_2P].Draw(rc);
			m_MaxLv[enPlayerNumber_2P].Draw(rc);
		}

		m_experienceBarBack[enPlayerNumber_1P].Draw(rc);
		if (m_mathExp[enPlayerNumber_1P] != 0) {
			m_experienceBarFlont[enPlayerNumber_1P].Draw(rc);
		}
		m_ExperienceFlame[enPlayerNumber_1P].Draw(rc);
		m_hpBarBack[enPlayerNumber_1P].Draw(rc);
		m_HpBar_White[enPlayerNumber_1P].Draw(rc);
		m_hpBar[enPlayerNumber_1P].Draw(rc);
		m_HPFrame[enPlayerNumber_1P].Draw(rc);

		m_SkillRenderIN[enPlayerNumber_1P].Draw(rc);
		m_SkillRenderOUT[enPlayerNumber_1P].Draw(rc);

		m_UltRenderIN[enPlayerNumber_1P].Draw(rc);
		m_UltRenderOUT[enPlayerNumber_1P].Draw(rc);

		m_HpFont[enPlayerNumber_1P].Draw(rc);
		m_Lv[enPlayerNumber_1P].Draw(rc);
		m_LvNumber_back[enPlayerNumber_1P].Draw(rc);
		m_LvNumber[enPlayerNumber_1P].Draw(rc);
		m_MaxLv[enPlayerNumber_1P].Draw(rc);

		//ポイントを描画
		//左のフレームの色々を描画
		int num = 0;
		int enemyNum = 0;
		for (auto actor : m_Actors)
		{
			m_pointFlame[num].Draw(rc);
			m_PointFont[num].Draw(rc);
			m_charIcon[num].Draw(rc);
			if (num >= 1)
			{
				m_LevelFont[enemyNum].Draw(rc);
				enemyNum++;
			}
			num++;
		}

		//finishの画像
		if (m_game->GetMinutesTimer() < 1 && m_game->GetSecondsTimer() <= 0)
		{
			m_FinishCountNumber.Draw(rc);
		}
	}
	else
	{
		//カウントダウンの表示
		if (m_game->NowGameState() == 0 && m_game->CountDownMinutes() <= 3) {
			m_CountNumper.Draw(rc);
		}
	}
}
