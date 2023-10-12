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
#include "KnightAI.h"

namespace
{
	const int Characters = 4;

	const float FLAME_WIDTH = 1200.0f;
	const float FLAME_HEIGHT = 500.0f;
	const Vector3 FLAME_POS_SOLO = Vector3(920.0f, -480.0f, 0.0f);    //レベルや経験値のフレーム
	const Vector3 FLAME_DUO_POS_1P = Vector3(-200.0f, -435.0f, 0.0f);
	const Vector3 FLAME_DUO_POS_2P = Vector3(255.0f, -435.0f, 0.0f);
	const Vector3 FLAME_QUARTET_POS_1P = Vector3(-115.5f, 70.0f, 0.0f);
	const Vector3 FLAME_QUARTET_POS_2P = Vector3(844.0f, 70.0f, 0.0f);
	const Vector3 FLAME_QUARTET_POS_3P = Vector3(-115.5f, -470.0f, 0.0f);
	const Vector3 FLAME_QUARTET_POS_4P = Vector3(844.0f, -470.0f, 0.0f);
	const Vector3 FLAME_SCALE_DUO = Vector3(0.43f, 0.5f, 1.0f);
	const Vector3 FLAME_SCALE_QUARTET = Vector3(0.25f, 0.3f, 1.0f);

	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//ゲージのピボット
	const Vector2 HPGAUGE_PIVOT_LEFT = Vector2(0.0f, 0.5f);				//HPゲージのピボット
	const Vector2 HPGAUGE_PIVOT_RIGHT = Vector2(1.0f, 0.5f);				//右のHPゲージのピボット
	const Vector2 EXPERIENCEGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//経験値ゲージのピボット

	const Vector3 STATUS_BAR_POS = Vector3(-450.0f, -500.0f, 0.0f);	//ステータスバーポジション
	const Vector3 TIME_POS = Vector3(0.0f, 470.0f, 0.0f);	//制限時間の座標
	const Vector3 TIME_SCALE = Vector3(0.35f, 0.3f, 1.0f);
	const Vector3 TIME_SCALE_QUARTET = Vector3(0.25f, 0.2f, 1.0f);
	const Vector3 MINUTES_FONT_POS = Vector3(-75.0f, 500.0f, 0.0f);	//制限時間の座標(分)
	const Vector3 CORON_FONT_POS = MINUTES_FONT_POS + Vector3(70.0f, 10.0f, 0.0f);//制限時間の座標(:コロン)
	const Vector3 SECONDS_FONT_POS = CORON_FONT_POS + Vector3(35.0f, -10.0f, 0.0f);//制限時間の座標(秒)
	const Vector3 MINUTES_FONT_POS_QUARTET = Vector3(-55.0f, 490.0f, 0.0f);	//制限時間の座標(分)
	const Vector3 CORON_FONT_POS_QUARTET = MINUTES_FONT_POS_QUARTET + Vector3(55.0f, 7.0f, 0.0f);//制限時間の座標(:コロン)
	const Vector3 SECONDS_FONT_POS_QUARTET = CORON_FONT_POS_QUARTET + Vector3(30.0f, -7.0f, 0.0f);//制限時間の座標(秒)

	const float HP_BAR_BACK_WIDTH = 600.0f;
	const float HP_BAR_BACK_HEIGHT = 120.0f;
	const float HP_BAR_FOWARD_WIDTH = 580.0f;
	const float HP_BAR_FOWARD_HEIGHT = 80.0f;
	const float HP_BAR_WIDTH_DUO = 380.0f;
	const float HP_BAR_HEIGHT_DUO = 80.0f;
	const float HP_BAR_BACK_WIDTH_QUARTET = 600.0f;
	const float HP_BAR_BACK_HEIGHT_QUARTET = 120.0f;
	const float HP_BAR_FRONT_WIDTH_QUARTET = 203.0f;
	const float HP_BAR_FRONT_HEIGHT_QUARTET = 90.0f;

	const Vector3 HP_BAR_POS = Vector3(-670.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_POS_RIGHT = Vector3(775.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_POS_LEFT = Vector3(-775.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_POS_QUARTET_LEFT_UP = Vector3(-855.0f, 30.0f, 0.0f);
	const Vector3 HP_BAR_POS_QUARTET_RIGHT_UP = Vector3(105.0f, 30.0f, 0.0f);
	const Vector3 HP_BAR_POS_QUARTET_LEFT_DOWN = Vector3(-855.0f, -520.0f, 0.0f);
	const Vector3 HP_BAR_POS_QUARTET_RIGHT_DOWN = Vector3(105.0f, -520.0f, 0.0f);

	const Vector3 HP_BAR_FLONT_POS = Vector3(-960.0f, -480.0f, 0.0f);	//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_POS_LEFT = Vector3(-965.0f, -480.0f, 0.0f);	//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_POS_RIGHT = Vector3(965.0f, -480.0f, 0.0f);	//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_QUARTET_LEFT_UP = Vector3(-956.0, 30.0f, 0.0f);			//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_QUARTET_RIGHT_UP = Vector3(4.0, 30.0f, 0.0f);		//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_QUARTET_LEFT_DOWN = Vector3(-956.0, -520.0f, 0.0f);		//HPバーの表のポジション
	const Vector3 HP_BAR_FLONT_QUARTET_RIGHT_DOWN = Vector3(4.0f, -520.0f, 0.0f);	//HPバーの表のポジション
	const Vector3 HP_BAR_QUARTET_SCALE = Vector3(1.0f, 0.38f, 1.0f);
	const Vector3 HP_BAR_BACK_QUARTET_SCALE = Vector3(0.35, 0.35f, 1.0f);

	const Vector3 HP_FONT_POS = Vector3(-650.0f, -445.0f, 0.0f);
	const Vector3 HP_FONT_POS_LEFT = Vector3(-780.0f, -455.0f, 0.0f);
	const Vector3 HP_FONT_POS_RIGHT = Vector3(610.0f, -455.0f, 0.0f);
	const Vector3 ADD_HP_FONT_POS = Vector3(10.0f, 13.0f, 0.0f);
	const Vector3 HP_FONT_QUARTET_POS_1P = HP_BAR_POS_QUARTET_LEFT_UP + ADD_HP_FONT_POS;
	const Vector3 HP_FONT_QUARTET_POS_2P = HP_BAR_POS_QUARTET_RIGHT_UP + ADD_HP_FONT_POS;
	const Vector3 HP_FONT_QUARTET_POS_3P = HP_BAR_POS_QUARTET_LEFT_DOWN + ADD_HP_FONT_POS;
	const Vector3 HP_FONT_QUARTET_POS_4P = HP_BAR_POS_QUARTET_RIGHT_DOWN + ADD_HP_FONT_POS;

	//スキルアイコンの座標や拡大率など
	const float SKILL_AND_ULT_ICON_RESOLUTION_SOLO = 162.0f;
	const float SKILL_AND_ULT_ICON_RESOLUTION = 1080.0f;
	const Vector3 SKILL_POS_SOLO = Vector3(522.0f, -275.0f, 0.0f);  //スキルアイコンポジション
	const Vector3 SKILL_POS_1P = Vector3(-400.0f, -360.0f, 0.0f);	//スキルアイコンポジション
	const Vector3 SKILL_POS_2P = Vector3(400.0f, -360.0f, 0.0f);	//スキルアイコンポジション
	const Vector3 ADD_SKILL_QUARTET_POS = Vector3(-106.5f, 26.0f, 0.0f);
	const Vector3 SKILL_QUARTET_POS_1P = FLAME_QUARTET_POS_1P + ADD_SKILL_QUARTET_POS;
	const Vector3 SKILL_QUARTET_POS_2P = FLAME_QUARTET_POS_2P + ADD_SKILL_QUARTET_POS;
	const Vector3 SKILL_QUARTET_POS_3P = FLAME_QUARTET_POS_3P + ADD_SKILL_QUARTET_POS;
	const Vector3 SKILL_QUARTET_POS_4P = FLAME_QUARTET_POS_4P + ADD_SKILL_QUARTET_POS;
	const Vector3 SKILL_SCALE_SOLO = Vector3(0.14f, 0.14f, 1.0f);
	const Vector3 SKILL_SCALE_DUO = Vector3(0.11f, 0.11f, 1.0f);
	const Vector3 SKILL_SCALE_QUARTET = Vector3(0.05f, 0.05f, 1.0f);

	//スキルクールタイムの文字の座標や拡大率など
	const Vector3 SKILL_COOLTIME_FONT_POS = Vector3(479.0f, -220.0f, 0.0f);
	const Vector3 SKILL_COOLTIME_FONT_POS_1P = Vector3(-425.0f, -320.0f, 0.0f);
	const Vector3 SKILL_COOLTIME_FONT_POS_2P = Vector3(375.0f, -320.0f, 0.0f);
	const Vector3 ADD_SKILL_COOLTIME_FONT = Vector3(-18.0f, 26.0f, 0.0f);
	const Vector3 SKILL_COOLTIME_FONT_POS_QUARTET_1P = SKILL_QUARTET_POS_1P + ADD_SKILL_COOLTIME_FONT;
	const Vector3 SKILL_COOLTIME_FONT_POS_QUARTET_2P = SKILL_QUARTET_POS_2P + ADD_SKILL_COOLTIME_FONT;
	const Vector3 SKILL_COOLTIME_FONT_POS_QUARTET_3P = SKILL_QUARTET_POS_3P + ADD_SKILL_COOLTIME_FONT;
	const Vector3 SKILL_COOLTIME_FONT_POS_QUARTET_4P = SKILL_QUARTET_POS_4P + ADD_SKILL_COOLTIME_FONT;
	const float SKILL_COOLTIME_FONT_SHADOW_OFFSET = 2.0f;
	const float SKILL_COOLTIME_FONT_SCALE = 1.7f;
	const float SKILL_COOLTIME_FONT_SCALE_DUO = 1.0f;
	const float SKILL_COOLTIME_FONT_SCALE_QUARTET = 0.7f;

	//必殺技アイコンの座標や拡大率など
	const Vector3 ULT_POS_SOLO = Vector3(470.0f, -445.0f, 0.0f);    //必殺技アイコンポジション
	const Vector3 ULT_POS_1P = Vector3(-440.0f, -475.0f, 0.0f);     //必殺技アイコンポジション
	const Vector3 ULT_POS_2P = Vector3(440.0f, -475.0f, 0.0f);		//必殺技アイコンポジション
	const Vector3 ADD_ULT_QUARTET_POS = Vector3(-130.5f, -28.0f, 0.0f);
	const Vector3 ULT_QUARTET_POS_1P = FLAME_QUARTET_POS_1P + ADD_ULT_QUARTET_POS;
	const Vector3 ULT_QUARTET_POS_2P = FLAME_QUARTET_POS_2P + ADD_ULT_QUARTET_POS;
	const Vector3 ULT_QUARTET_POS_3P = FLAME_QUARTET_POS_3P + ADD_ULT_QUARTET_POS;
	const Vector3 ULT_QUARTET_POS_4P = FLAME_QUARTET_POS_4P + ADD_ULT_QUARTET_POS;
	const Vector3 ULT_ICON_SCALE_SOLO = Vector3(0.15f, 0.15f, 1.0f);
	const Vector3 ULT_ICON_SCALE_DUO = Vector3(0.11f, 0.11f, 1.0f);
	const Vector3 ULT_ICON_SCALE_QUARTET = Vector3(0.05f, 0.05f, 1.0f);

	const Vector3 LEVEL_SPRITE_POS_SOLO = Vector3(640.0f, -310.0f, 0.0f);       //Lv
	const Vector3 LEVEL_SPRITE_POS_1P = Vector3(-300.0f, -360.0f, 0.0f);
	const Vector3 LEVEL_SPRITE_POS_2P = Vector3(70.0f, -360.0f, 0.0f);
	const Vector3 ADD_LEVEL_SPRITE_QUARTET = Vector3(-60.0f, 40.0f, 0.0f);
	const Vector3 LEVEL_SPRITE_QUARTET_POS_1P = FLAME_QUARTET_POS_1P + ADD_LEVEL_SPRITE_QUARTET;
	const Vector3 LEVEL_SPRITE_QUARTET_POS_2P = FLAME_QUARTET_POS_2P + ADD_LEVEL_SPRITE_QUARTET;
	const Vector3 LEVEL_SPRITE_QUARTET_POS_3P = FLAME_QUARTET_POS_3P + ADD_LEVEL_SPRITE_QUARTET;
	const Vector3 LEVEL_SPRITE_QUARTET_POS_4P = FLAME_QUARTET_POS_4P + ADD_LEVEL_SPRITE_QUARTET;
	const Vector3 LEVEL_SPRITE_SCALE_DUO = Vector3(0.4f, 0.4f, 1.0f);
	const Vector3 LEVEL_SPRITE_SCALE_QUARTET = Vector3(0.2f, 0.2f, 1.0f);

	const float LEVEL_NUMBER_RESOLUTION = 150.0f;
	const float LEVEL_MAX_RESOLUTION = 196.0f;
	const Vector3 LEVEL_NUMBER_POS_SOLO = Vector3(780.0f, -360.0f, 0.0f);
	const Vector3 LEVEL_NUMBER_POS_1P = Vector3(-180.0f, -400.0f, 0.0f);
	const Vector3 LEVEL_NUMBER_POS_2P = Vector3(185.0f, -400.0f, 0.0f);
	const Vector3 ADD_LEVEL_NUMBER_SPRITE_QUARTET = Vector3(5.5f, 10.0f, 0.0f);
	const Vector3 LEVEL_NUMBER_QUARTET_POS_1P = FLAME_QUARTET_POS_1P + ADD_LEVEL_NUMBER_SPRITE_QUARTET;
	const Vector3 LEVEL_NUMBER_QUARTET_POS_2P = FLAME_QUARTET_POS_2P + ADD_LEVEL_NUMBER_SPRITE_QUARTET;
	const Vector3 LEVEL_NUMBER_QUARTET_POS_3P = FLAME_QUARTET_POS_3P + ADD_LEVEL_NUMBER_SPRITE_QUARTET;
	const Vector3 LEVEL_NUMBER_QUARTET_POS_4P = FLAME_QUARTET_POS_4P + ADD_LEVEL_NUMBER_SPRITE_QUARTET;
	const Vector3 LEVEL_NUMBER_SCALE_SOLO = Vector3(1.4f, 1.4f, 1.0f);
	const Vector3 LEVEL_NUMBER_SCALE_QUARTET = Vector3(0.65f, 0.65f, 1.0f);

	const Vector3 MAX_LEVEL_POS_SOLO = Vector3(920.0f, -400.0f, 0.0f);       //10
	const Vector3 MAX_LEVEL_POS_1P = Vector3(-60.0f, -420.0f, 0.0f);
	const Vector3 MAX_LEVEL_POS_2P = Vector3(310.0f, -420.0f, 0.0f);
	const Vector3 ADD_MAX_LEVEL_SPRITE_QUARTET = Vector3(80.5, -5.0f, 0.0f);
	const Vector3 MAX_LEVEL_QUARTET_POS_1P = FLAME_QUARTET_POS_1P + ADD_MAX_LEVEL_SPRITE_QUARTET;
	const Vector3 MAX_LEVEL_QUARTET_POS_2P = FLAME_QUARTET_POS_2P + ADD_MAX_LEVEL_SPRITE_QUARTET;
	const Vector3 MAX_LEVEL_QUARTET_POS_3P = FLAME_QUARTET_POS_3P + ADD_MAX_LEVEL_SPRITE_QUARTET;
	const Vector3 MAX_LEVEL_QUARTET_POS_4P = FLAME_QUARTET_POS_4P + ADD_MAX_LEVEL_SPRITE_QUARTET;
	const Vector3 MAX_LEVEL_SCALE = Vector3(0.5f, 0.5f, 1.0f);
	const Vector3 MAX_LEVEL_SCALE_QUARTET = Vector3(0.3f, 0.3f, 1.0f);

	const float LEVEL_SPRITE_WIDTH = 196.0f;
	const float LEVEL_SPRITE_HEIGHT = 150.0f;

	const float DownPointPosY = 100.0f;

	const Vector3 EXPERIENCE_FLAME_POS = Vector3(750.0f, -500.0f, 0.0f);  //経験値テーブル
	const Vector3 EXPERIENCE_FLAME_POS_DUO_1P = Vector3(-200.0f, -500.0f, 0.0f);
	const Vector3 EXPERIENCE_FLAME_POS_DUO_2P = Vector3(200.0f, -500.0f, 0.0f);
	const Vector3 ADD_EXPERIENCE_FLAME_POS = Vector3(0.0f, -40.0f, 0.0f);
	const Vector3 EXPERIENCE_FLAME_POS_QUARTET_1P = FLAME_QUARTET_POS_1P + ADD_EXPERIENCE_FLAME_POS;
	const Vector3 EXPERIENCE_FLAME_POS_QUARTET_2P = FLAME_QUARTET_POS_2P + ADD_EXPERIENCE_FLAME_POS;
	const Vector3 EXPERIENCE_FLAME_POS_QUARTET_3P = FLAME_QUARTET_POS_3P + ADD_EXPERIENCE_FLAME_POS;
	const Vector3 EXPERIENCE_FLAME_POS_QUARTET_4P = FLAME_QUARTET_POS_4P + ADD_EXPERIENCE_FLAME_POS;
	const Vector3 EXPERIENCE_SCALE = Vector3(0.5f, 0.5f, 1.0f);
	const Vector3 EXPERIENCE_SCALE_QUARTET = Vector3(0.25f, 0.25f, 1.0f);

	const float EXPBAR_WIDTH = 300.0f;
	const float EXPBAR_HEIGHT = 70.0f;
	const float EXPBAR_FLAME_WIDTH = 600.0f;
	const float EXPBAR_FLAME_HEIGHT = 120.0f;
	const Vector3 EXPERIENCE_BAR_POS = Vector3(600.0f, -500.0f, 0.0f);	//経験値バーの座標
	const Vector3 EXPERIENCE_BAR_POS_DUO_1P = Vector3(-350.0f, -500.0f, 0.0f);	//経験値バーの座標
	const Vector3 EXPERIENCE_BAR_POS_DUO_2P = Vector3(48.0f, -500.0f, 0.0f);	//経験値バーの座標
	const Vector3 ADD_EXPERIENCE_BAR_POS = Vector3(-75.0f, -40.0f, 0.0f);
	const Vector3 EXPERIENCE_BAR_POS_QUARTET_1P = FLAME_QUARTET_POS_1P + ADD_EXPERIENCE_BAR_POS;
	const Vector3 EXPERIENCE_BAR_POS_QUARTET_2P = FLAME_QUARTET_POS_2P + ADD_EXPERIENCE_BAR_POS;
	const Vector3 EXPERIENCE_BAR_POS_QUARTET_3P = FLAME_QUARTET_POS_3P + ADD_EXPERIENCE_BAR_POS;
	const Vector3 EXPERIENCE_BAR_POS_QUARTET_4P = FLAME_QUARTET_POS_4P + ADD_EXPERIENCE_BAR_POS;
	const Vector3 EXPERIENCE_BAR_SCALE_QUARTET = Vector3(1.0f, 0.2f, 1.0f);
	const Vector3 EXPBAR_SIZE = Vector3(EXPBAR_WIDTH, EXPBAR_HEIGHT, 0.0f);	//経験値バーのサイズ

	const Vector3 UPTOLEVEL_POS = Vector3(820.0f, -480.0f, 0.0f);		//レベルアップまでに必要な経験値の量

	const Vector3 SMALLSCALE = Vector3(0.1f, 0.1f, 0.0f);

	const Vector3 FIGHT_SMALL_SCALE = Vector3(0.5f, 0.5f, 0.0f);

	const Vector3 RESPAWN_COUNT_POS = Vector3(0.0f, -200.0f, 0.0f);		//リスポーンした後のカウント
	const Vector3 RESPAWN_COUNT_DUOPLAY_LEFT_POS = Vector3(-480.0f, 0.0f, 0.0f);
	const Vector3 RESPAWN_COUNT_DUOPLAY_RIGHT_POS = Vector3(480.0f, 0.0f, 0.0f);
	const Vector3 RESPAWN_COUNT_QUARTER_LEFT_UP_POS = Vector3(-480.0f, 220.0f, 0.0f);
	const Vector3 RESPAWN_COUNT_QUARTER_LEFT_DOWN_POS = Vector3(-480.0f, -320.0f, 0.0f);
	const Vector3 RESPAWN_COUNT_QUARTER_RIGHT_UP_POS = Vector3(480.0f, 220.0f, 0.0f);
	const Vector3 RESPAWN_COUNT_QUARTER_RIGHT_DOWN_POS = Vector3(480.0f, -320.0f, 0.0f);

	const Vector3 RESPAWN_IN_POS = Vector3(0.0f, 300.0f, 0.0f);		//Respawn inの座標

	const Vector3 RESPAWN_IN_DUOPLAY_LEFT_POS = Vector3(-480.0f, 300.0f, 0.0f);
	const Vector3 RESPAWN_IN_DUOPLAY_RIGHT_POS = Vector3(480.0f, 300.0f, 0.0f);

	const Vector3 RESPAWN_IN_QUARTER_LEFT_UP_POS = Vector3(-480.0f, 390.0f, 0.0f);
	const Vector3 RESPAWN_IN_QUARTER_LEFT_DOWN_POS = Vector3(-480.0f, -90.0f, 0.0f);
	const Vector3 RESPAWN_IN_QUARTER_RIGHT_UP_POS = Vector3(480.0f, 390.0f, 0.0f);
	const Vector3 RESPAWN_IN_QUARTER_RIGHT_DOWN_POS = Vector3(480.0f, -90.0f, 0.0f);

	const Vector3 RESPAWN_IN_DUOPLAY_SCALE = Vector3(0.5f, 0.5f, 1.0f);
	const Vector3 RESPAWN_IN_QUATER_SCALE = Vector3(0.25f, 0.25f, 1.0f);

	const Vector3 RESPAWN_DUOPLAY_SCALE = Vector3(0.5f, 1.0f, 1.0f);
	const Vector3 RESPAWN_QUARTER_SCALE = Vector3(0.5f, 0.5f, 1.0f);
	const Vector3 RESPAWN_DUOPLAY_LEFT_POS = Vector3(-480.0f, 0.0f, 0.0f);
	const Vector3 RESPAWN_DUOPLAY_RIGHT_POS = Vector3(480.0f, 0.0f, 0.0f);
	const Vector3 RESPAWN_QUARTER_LEFT_UP_POS = Vector3(-480.0f, 270.0f, 0.0f);
	const Vector3 RESPAWN_QUARTER_LEFT_DOWN_POS = Vector3(-480.0f, -270.0f, 0.0f);
	const Vector3 RESPAWN_QUARTER_RIGHT_UP_POS = Vector3(480.0f, 270.0f, 0.0f);
	const Vector3 RESPAWN_QUARTER_RIGHT_DOWN_POS = Vector3(480.0f, -270.0f, 0.0f);

	const float WHITEHP_WAIT = 0.2f;

	const float CHAR_ICON_SIZE = 74.0f;

	const Vector3 CHAR_ICON_MAXSIZE = Vector3(1.2f, 1.2f, 1.0f);

	const float TIMERSCALE = 1.65f;
	const float TIMERSCALE_QUARTET = 1.1f;

	const float LEVEL_FONT_SCALE = 0.6f;

	const float NAME_SPRITE_WIDTH = 203.0f;
	const float NAME_SPRITE_HEIGHT = 64.0f;
	const Vector3 NAME_SPRITE_SCALE = Vector3(0.55f, 0.55f, 1.0f);
	const Vector3 NAME_SPRITE_SCALE_QUARTET = Vector3(0.275f, 0.275f, 1.0f);
	const Vector3 POINT_FLAME_SCALE_QUARTET = Vector3(0.45f, 0.5f, 1.0f);
	const Vector3 CHARACTER_ICON_SCALE_QUARTET = Vector3(0.5f, 0.5f, 1.0f);

	const std::array<Vector3, 4> POINT_FLAME_POS = {
		TIME_POS + Vector3(-700,0.0f,0.0f) ,
		TIME_POS + Vector3(-370,0.0f,0.0f),
		TIME_POS + Vector3(370,0.0f,0.0f),
		TIME_POS + Vector3(700,0.0f,0.0f),
	};															//ポイントのフレーム

	const std::array<Vector3, 4> POINT_POS = {
		POINT_FLAME_POS[0] + Vector3(-10.0f, 46.0f, 0.0f),
		POINT_FLAME_POS[1] + Vector3(-10.0f, 46.0f, 0.0f),
		POINT_FLAME_POS[2] + Vector3(-10.0f, 46.0f, 0.0f),
		POINT_FLAME_POS[3] + Vector3(-10.0f, 46.0f, 0.0f),
	};															//ポイント

	const std::array<Vector3, 4> CHARACTOR_ICON_POS = {
		POINT_FLAME_POS[0] + Vector3(-60.0f, 10.0f, 0.0f),
		POINT_FLAME_POS[1] + Vector3(-60.0f, 10.0f, 0.0f),
		POINT_FLAME_POS[2] + Vector3(-60.0f, 10.0f, 0.0f),
		POINT_FLAME_POS[3] + Vector3(-60.0f, 10.0f, 0.0f),
	};															//アイコン

	const std::array<Vector3, 4> LEVEL_POS = {
		CHARACTOR_ICON_POS[0] + Vector3(-43.0f, -10.0f, 0.0f),
		CHARACTOR_ICON_POS[1] + Vector3(-43.0f, -10.0f, 0.0f),
		CHARACTOR_ICON_POS[2] + Vector3(-43.0f, -10.0f, 0.0f),
		CHARACTOR_ICON_POS[3] + Vector3(-43.0f, -10.0f, 0.0f)
	};															//レベル

	const std::array<Vector3, 4> NAME_POS =
	{
		CHARACTOR_ICON_POS[0] + Vector3(0.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS[1] + Vector3(0.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS[2] + Vector3(0.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS[3] + Vector3(0.0f, 0.0f, 0.0f),
	};

	const std::array<Vector3, 4> POINT_FLAME_POS_QUARTET = {
		HP_BAR_POS_QUARTET_LEFT_UP + Vector3(-20.0f,45.0f,0.0f) ,
		HP_BAR_POS_QUARTET_RIGHT_UP + Vector3(-20.0f,45.0f,0.0f),
		HP_BAR_POS_QUARTET_LEFT_DOWN + Vector3(-20.0f,45.0f,0.0f),
		HP_BAR_POS_QUARTET_RIGHT_DOWN + Vector3(-20.0f,45.0f,0.0f),
	};															//ポイントのフレーム4画面
	const std::array<Vector3, 4> POINT_POS_QUARTET = {
		POINT_FLAME_POS_QUARTET[0] + Vector3(-10.0f, 20.0f, 0.0f),
		POINT_FLAME_POS_QUARTET[1] + Vector3(-10.0f, 20.0f, 0.0f),
		POINT_FLAME_POS_QUARTET[2] + Vector3(-10.0f, 20.0f, 0.0f),
		POINT_FLAME_POS_QUARTET[3] + Vector3(-10.0f, 20.0f, 0.0f),
	};															//ポイント
	const std::array<Vector3, 4> CHARACTOR_ICON_POS_QUARTET = {
		POINT_FLAME_POS_QUARTET[0] + Vector3(-50.0f, 0.0f, 0.0f),
		POINT_FLAME_POS_QUARTET[1] + Vector3(-50.0f, 0.0f, 0.0f),
		POINT_FLAME_POS_QUARTET[2] + Vector3(-50.0f, 0.0f, 0.0f),
		POINT_FLAME_POS_QUARTET[3] + Vector3(-50.0f, 0.0f, 0.0f),
	};															//アイコン
	const std::array<Vector3, 4> LEVEL_POS_QUARTET = {
		CHARACTOR_ICON_POS_QUARTET[0] + Vector3(-43.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS_QUARTET[1] + Vector3(-43.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS_QUARTET[2] + Vector3(-43.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS_QUARTET[3] + Vector3(-43.0f, 0.0f, 0.0f)
	};															//レベル
	const std::array<Vector3, 4> NAME_POS_QUARTET =
	{
		CHARACTOR_ICON_POS_QUARTET[0] + Vector3(0.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS_QUARTET[1] + Vector3(0.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS_QUARTET[2] + Vector3(0.0f, 0.0f, 0.0f),
		CHARACTOR_ICON_POS_QUARTET[3] + Vector3(0.0f, 0.0f, 0.0f),
	};
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
	m_fade = FindGO<Fade>("fade");

	m_gameMode = g_renderingEngine->GetGameMode();

	//マルチプレイなら
	if (m_gameMode != RenderingEngine::enGameMode_SoloPlay)
	{
		m_player2P = FindGO<Player>("player2");
		//3人以上でプレイの場合
		if (m_gameMode != RenderingEngine::enGameMode_DuoPlay)
		{
			m_player3P = FindGO<Player>("player3");
			//4人でプレイの場合
			if (m_gameMode != RenderingEngine::enGameMode_TrioPlay)
			{
				m_player4P = FindGO<Player>("player4");
			}
			else
			{
				m_knightAI = FindGO<KnightAI>("KnightAI1");
			}
		}
	}

	InitAssets();

	m_GameUIState = m_GameStartState;

	return true;
}

void GameUI::InitPointUI()
{
	//ポイント関連
	//キャラのポイントを表示
	m_Actors = m_game->GetActors();
	int num = 0;
	for (auto actor : m_Actors)
	{
		//ポイントを表示
		if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
		{
			m_PointFont[num].SetPosition(POINT_POS_QUARTET[num]);
			m_PointFont[num].SetScale(0.45f);
		}
		else
		{
			m_PointFont[num].SetPosition(POINT_POS[num]);
		}
		m_PointFont[num].SetColor(g_vec4White);
		m_PointFont[num].SetShadowParam(true, 2.0f, g_vec4Black);
		//レベル
		m_LevelFont[num].SetPosition(LEVEL_POS[num]);
		m_LevelFont[num].SetScale(LEVEL_FONT_SCALE);
		m_LevelFont[num].SetColor(g_vec4White);
		m_LevelFont[num].SetShadowParam(true, 1.0f, g_vec4Black);

		//プレイヤーが剣士なら
		if (actor->IsMatchName(knightname))
		{
			//アイコンを剣士にする(ブルー)
			//ブルー
			m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Blue.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
			m_pointFlame[num].Init("Assets/sprite/gameUI/NewPointFlame.DDS", 300.0f, 100.0f);
			m_playerCpuName[num].Init("Assets/sprite/gameUI/P1.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);

			if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS_QUARTET[0]);
				m_playerCpuName[num].SetPosition(NAME_POS_QUARTET[0]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS_QUARTET[0]);
			}
			else
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS[0]);
				m_playerCpuName[num].SetPosition(NAME_POS[0]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS[0]);
			}
		}

		//赤の剣士なら
		if (actor->IsMatchName(KnightAI_Red) || actor->IsMatchName(knightname2))
		{
			//レッド
			m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Red.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
			m_pointFlame[num].Init("Assets/sprite/gameUI/NewPointFlame.DDS", 300.0f, 100.0f);

			if (g_renderingEngine->GetGameMode() != RenderingEngine::enGameMode_SoloPlay)
			{
				m_playerCpuName[num].Init("Assets/sprite/gameUI/P2.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
			}
			else
			{
				m_playerCpuName[num].Init("Assets/sprite/gameUI/CPU1.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
			}

			if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS_QUARTET[1]);
				m_playerCpuName[num].SetPosition(NAME_POS_QUARTET[1]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS_QUARTET[1]);
			}
			else
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS[1]);
				m_playerCpuName[num].SetPosition(NAME_POS[1]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS[1]);
			}
		}

		if (actor->IsMatchName(KnightAI_Yellow) || actor->IsMatchName(knightname3))
		{
			//イエロー
			m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Yellow.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
			m_pointFlame[num].Init("Assets/sprite/gameUI/NewPointFlame.DDS", 300.0f, 100.0f);

			if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay ||
				g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
			{
				m_playerCpuName[num].Init("Assets/sprite/gameUI/P3.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
			}
			else
			{
				if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
				{
					m_playerCpuName[num].Init("Assets/sprite/gameUI/CPU1.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
				}
				else
				{
					m_playerCpuName[num].Init("Assets/sprite/gameUI/CPU2.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
				}
			}

			if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS_QUARTET[2]);
				m_playerCpuName[num].SetPosition(NAME_POS_QUARTET[2]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS_QUARTET[2]);
			}
			else
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS[2]);
				m_playerCpuName[num].SetPosition(NAME_POS[2]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS[2]);
			}
		}

		if (actor->IsMatchName(KnightAI_Green) || actor->IsMatchName(knightname4))
		{
			//グリーン
			m_charIcon[num].Init("Assets/sprite/gameUI/Knight_Green.DDS", CHAR_ICON_SIZE, CHAR_ICON_SIZE);
			m_pointFlame[num].Init("Assets/sprite/gameUI/NewPointFlame.DDS", 300.0f, 100.0f);

			if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
			{
				m_playerCpuName[num].Init("Assets/sprite/gameUI/P4.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
			}
			else
			{
				if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
				{
					m_playerCpuName[num].Init("Assets/sprite/gameUI/CPU2.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
				}
				else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay)
				{
					m_playerCpuName[num].Init("Assets/sprite/gameUI/CPU1.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
				}
				else
				{
					m_playerCpuName[num].Init("Assets/sprite/gameUI/CPU3.DDS", NAME_SPRITE_WIDTH, NAME_SPRITE_HEIGHT);
				}
			}
			if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS_QUARTET[3]);
				m_playerCpuName[num].SetPosition(NAME_POS_QUARTET[3]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS_QUARTET[3]);
			}
			else
			{
				m_charIcon[num].SetPosition(CHARACTOR_ICON_POS[3]);
				m_playerCpuName[num].SetPosition(NAME_POS[3]);
				m_pointFlame[num].SetPosition(POINT_FLAME_POS[3]);
			}
		}

		if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
		{
			m_playerCpuName[num].SetScale(NAME_SPRITE_SCALE_QUARTET);
			m_pointFlame[num].SetScale(POINT_FLAME_SCALE_QUARTET);
			m_charIcon[num].SetScale(CHARACTER_ICON_SCALE_QUARTET);
		}
		else
		{
			m_playerCpuName[num].SetScale(NAME_SPRITE_SCALE);
			m_pointFlame[num].SetScale(g_vec3One);
		}
		m_charIcon[num].Update();
		m_playerCpuName[num].Update();
		m_pointFlame[num].Update();

		num++;
	}
}

void GameUI::InitAssets()
{
	InitPointUI();
	InitRespawnUI();

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

	InitExpelienceUI();
	InitHpUI();
	InitTimerUI();
}

void GameUI::SkillCoolTimeFont()
{
	int SkillCoolTime = m_player1P->CharGetSkillCoolTimer();
	wchar_t Skill[255];
	swprintf_s(Skill, 255, L"%d", SkillCoolTime);
	m_skillFont[enPlayerNumber_1P].SetText(Skill);

	if (m_gameMode != RenderingEngine::enGameMode_SoloPlay)
	{
		SkillCoolTime = m_player2P->CharGetSkillCoolTimer();
		swprintf_s(Skill, 255, L"%d", SkillCoolTime);
		m_skillFont[enPlayerNumber_2P].SetText(Skill);

		if (m_gameMode != RenderingEngine::enGameMode_DuoPlay)
		{
			SkillCoolTime = m_player3P->CharGetSkillCoolTimer();
			swprintf_s(Skill, 255, L"%d", SkillCoolTime);
			m_skillFont[enPlayerNumber_3P].SetText(Skill);

			if (m_gameMode != RenderingEngine::enGameMode_TrioPlay)
			{
				SkillCoolTime = m_player4P->CharGetSkillCoolTimer();
				swprintf_s(Skill, 255, L"%d", SkillCoolTime);
				m_skillFont[enPlayerNumber_4P].SetText(Skill);
			}
		}
	}
}

void GameUI::InitRespawnUI()
{
	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
	{
		for (int i = 0; i < g_renderingEngine->GetGameMode(); i++)
		{
			m_respawnBack[i].Init("Assets/sprite/gameUI/Respawn_back.DDS", 1920.0f, 1080.0f);
			m_respawnBack[i].SetScale(RESPAWN_DUOPLAY_SCALE);
			m_respawnIn[i].Init("Assets/sprite/gameUI/RespawnIn.DDS", 900.0f, 200.0f);
			m_respawnIn[i].SetScale(RESPAWN_IN_DUOPLAY_SCALE);
			m_respawnCountNumber[i].Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
		}
		m_respawnBack[enPlayerNumber_1P].SetPosition(RESPAWN_DUOPLAY_LEFT_POS);
		m_respawnBack[enPlayerNumber_2P].SetPosition(RESPAWN_DUOPLAY_RIGHT_POS);

		m_respawnIn[enPlayerNumber_1P].SetPosition(RESPAWN_IN_DUOPLAY_LEFT_POS);
		m_respawnIn[enPlayerNumber_2P].SetPosition(RESPAWN_IN_DUOPLAY_RIGHT_POS);

		//リスポーンのカウントダウンの画像
		m_respawnCountNumber[enPlayerNumber_1P].SetPosition(RESPAWN_COUNT_DUOPLAY_LEFT_POS);
		m_respawnCountNumber[enPlayerNumber_2P].SetPosition(RESPAWN_COUNT_DUOPLAY_RIGHT_POS);

	}
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay || g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
	{
		for (int i = 0; i < g_renderingEngine->GetGameMode(); i++)
		{
			m_respawnBack[i].Init("Assets/sprite/gameUI/Respawn_back.DDS", 1920.0f, 1080.0f);
			m_respawnBack[i].SetScale(RESPAWN_QUARTER_SCALE);
			m_respawnIn[i].Init("Assets/sprite/gameUI/RespawnIn.DDS", 900.0f, 200.0f);
			m_respawnIn[i].SetScale(RESPAWN_QUARTER_SCALE);
			m_respawnCountNumber[i].Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300.0f, 500.0f);
			m_respawnCountNumber[i].SetScale(RESPAWN_QUARTER_SCALE);
		}
		m_respawnBack[enPlayerNumber_1P].SetPosition(RESPAWN_QUARTER_LEFT_UP_POS);
		m_respawnBack[enPlayerNumber_2P].SetPosition(RESPAWN_QUARTER_RIGHT_UP_POS);
		m_respawnBack[enPlayerNumber_3P].SetPosition(RESPAWN_QUARTER_LEFT_DOWN_POS);
		m_respawnBack[enPlayerNumber_4P].SetPosition(RESPAWN_QUARTER_RIGHT_DOWN_POS);

		m_respawnIn[enPlayerNumber_1P].SetPosition(RESPAWN_IN_QUARTER_LEFT_UP_POS);
		m_respawnIn[enPlayerNumber_2P].SetPosition(RESPAWN_IN_QUARTER_RIGHT_UP_POS);
		m_respawnIn[enPlayerNumber_3P].SetPosition(RESPAWN_IN_QUARTER_LEFT_DOWN_POS);
		m_respawnIn[enPlayerNumber_4P].SetPosition(RESPAWN_IN_QUARTER_RIGHT_DOWN_POS);

		//リスポーンのカウントダウンの画像
		m_respawnCountNumber[enPlayerNumber_1P].SetPosition(RESPAWN_COUNT_QUARTER_LEFT_UP_POS);
		m_respawnCountNumber[enPlayerNumber_2P].SetPosition(RESPAWN_COUNT_QUARTER_RIGHT_UP_POS);
		m_respawnCountNumber[enPlayerNumber_3P].SetPosition(RESPAWN_COUNT_QUARTER_LEFT_DOWN_POS);
		m_respawnCountNumber[enPlayerNumber_4P].SetPosition(RESPAWN_COUNT_QUARTER_RIGHT_DOWN_POS);
	}
	else
	{
		//Respawn inの画像
		m_respawnIn[enPlayerNumber_1P].Init("Assets/sprite/gameUI/RespawnIn.DDS", 900.0f, 200.0f);
		m_respawnIn[enPlayerNumber_1P].SetPosition(RESPAWN_IN_POS);

		//リスポーンの背景の画像
		m_respawnBack[enPlayerNumber_1P].Init("Assets/sprite/gameUI/Respawn_back.DDS", 1920, 1080.0f);

		//リスポーンのカウントダウンの画像
		m_respawnCountNumber[enPlayerNumber_1P].Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
		m_respawnCountNumber[enPlayerNumber_1P].SetPosition(RESPAWN_COUNT_POS);
	}

	for (int i = 0; i < enPlayerNumber_Num; i++)
	{
		m_respawnIn[i].Update();
		m_respawnBack[i].Update();
		m_respawnCountNumber[i].Update();
	}
}

void GameUI::InitExpelienceUI()
{
	for (int i = 0; i < enPlayerNumber_Num; i++)
	{
		m_Lv[i].Init("Assets/sprite/gameUI/Lv.DDS", LEVEL_SPRITE_WIDTH, LEVEL_SPRITE_HEIGHT);
		m_LvNumber[i].Init("Assets/sprite/gameUI/Lv1.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_LvNumber_back[i].Init("Assets/sprite/gameUI/Lv1_back.DDS", LEVEL_NUMBER_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_MaxLv[i].Init("Assets/sprite/gameUI/maxLv.DDS", LEVEL_MAX_RESOLUTION, LEVEL_NUMBER_RESOLUTION);
		m_ExperienceFlame[i].Init("Assets/sprite/gameUI/ExperienceBar.DDS", EXPBAR_FLAME_WIDTH, EXPBAR_FLAME_HEIGHT);
		m_experienceBarBack[i].Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", EXPBAR_FLAME_WIDTH, EXPBAR_FLAME_HEIGHT);
		m_UltRenderIN[i].Init("Assets/sprite/gameUI/Ult_Thunder_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION, SKILL_AND_ULT_ICON_RESOLUTION);
		m_UltRenderOUT[i].Init("Assets/sprite/gameUI/ULT_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION, SKILL_AND_ULT_ICON_RESOLUTION);
		m_SkillRenderIN[i].Init("Assets/sprite/gameUI/Skill_Icon_IN.DDS", SKILL_AND_ULT_ICON_RESOLUTION, SKILL_AND_ULT_ICON_RESOLUTION);
		m_SkillRenderOUT[i].Init("Assets/sprite/gameUI/Skill_Icon_OUT.DDS", SKILL_AND_ULT_ICON_RESOLUTION, SKILL_AND_ULT_ICON_RESOLUTION);
		m_experienceBarFlont[i].SetPivot(EXPERIENCEGAUGE_PIVOT);
		m_skillFont[i].SetColor(g_vec4Red);
		m_skillFont[i].SetShadowParam(true, SKILL_COOLTIME_FONT_SHADOW_OFFSET, g_vec4Black);

		if (m_gameMode == RenderingEngine::enGameMode_TrioPlay || m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
		{
			m_experienceBarFlont[i].Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_FLAME_WIDTH * EXPERIENCE_SCALE_QUARTET.x, EXPBAR_FLAME_HEIGHT);
		}
		else
		{
			m_experienceBarFlont[i].Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_WIDTH, EXPBAR_HEIGHT);
		}
	}

	if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
	{
		for (int i = 0; i < enPlayerNumber_3P; i++)
		{
			m_Flame[i].SetScale(FLAME_SCALE_DUO);
			m_UltRenderIN[i].SetScale(ULT_ICON_SCALE_DUO);
			m_UltRenderOUT[i].SetScale(ULT_ICON_SCALE_DUO);
			m_SkillRenderIN[i].SetScale(SKILL_SCALE_DUO);
			m_SkillRenderOUT[i].SetScale(SKILL_SCALE_DUO);
			m_skillFont[i].SetScale(SKILL_COOLTIME_FONT_SCALE);
			m_Lv[i].SetScale(LEVEL_SPRITE_SCALE_DUO);
			m_MaxLv[i].SetScale(MAX_LEVEL_SCALE);
			m_ExperienceFlame[i].SetScale(EXPERIENCE_SCALE);
			m_experienceBarBack[i].SetScale(EXPERIENCE_SCALE);
			m_experienceBarFlont[i].SetPivot(EXPERIENCEGAUGE_PIVOT);
			m_experienceBarFlont[i].SetScale(EXPERIENCE_SCALE);
		}

		m_Flame[enPlayerNumber_1P].Init("Assets/sprite/gameUI/LevelBar.DDS", FLAME_WIDTH, FLAME_HEIGHT);
		m_Flame[enPlayerNumber_2P].Init("Assets/sprite/gameUI/LevelBar2P.DDS", FLAME_WIDTH, FLAME_HEIGHT);
		m_Flame[enPlayerNumber_1P].SetPosition(FLAME_DUO_POS_1P);
		m_Flame[enPlayerNumber_2P].SetPosition(FLAME_DUO_POS_2P);

		//必殺技のアイコン
		m_UltRenderIN[enPlayerNumber_1P].SetPosition(ULT_POS_1P);
		m_UltRenderIN[enPlayerNumber_2P].SetPosition(ULT_POS_2P);

		//必殺のアイコンフレーム
		m_UltRenderOUT[enPlayerNumber_1P].SetPosition(ULT_POS_1P);
		m_UltRenderOUT[enPlayerNumber_2P].SetPosition(ULT_POS_2P);

		//スキルのアイコン
		m_SkillRenderIN[enPlayerNumber_1P].SetPosition(SKILL_POS_1P);
		m_SkillRenderIN[enPlayerNumber_2P].SetPosition(SKILL_POS_2P);

		//スキルのアイコンフレーム
		m_SkillRenderOUT[enPlayerNumber_1P].SetPosition(SKILL_POS_1P);
		m_SkillRenderOUT[enPlayerNumber_2P].SetPosition(SKILL_POS_2P);

		//スキルのクールタイムを表示するフォントの設定
		m_skillFont[enPlayerNumber_1P].SetPosition(SKILL_COOLTIME_FONT_POS_1P);
		m_skillFont[enPlayerNumber_2P].SetPosition(SKILL_COOLTIME_FONT_POS_2P);
		playerCoolTime[enPlayerNumber_1P] = m_player1P->CharGetSkillCoolTimer();
		playerCoolTime[enPlayerNumber_2P] = m_player2P->CharGetSkillCoolTimer();

		//Lvの画像を読み込む
		m_Lv[enPlayerNumber_1P].SetPosition(LEVEL_SPRITE_POS_1P);
		m_Lv[enPlayerNumber_2P].SetPosition(LEVEL_SPRITE_POS_2P);

		//Lv1の画像を読み込む
		m_LvNumber[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_1P);
		m_LvNumber[enPlayerNumber_2P].SetPosition(LEVEL_NUMBER_POS_2P);

		//Lv1の裏の画像の読み込み
		m_LvNumber_back[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_1P);
		m_LvNumber_back[enPlayerNumber_2P].SetPosition(LEVEL_NUMBER_POS_2P);

		//10の画像を読み込む
		m_MaxLv[enPlayerNumber_1P].SetPosition(MAX_LEVEL_POS_1P);
		m_MaxLv[enPlayerNumber_2P].SetPosition(MAX_LEVEL_POS_2P);

		//経験値テーブルと初期経験値
		m_expTable[enPlayerNumber_2P] = m_player2P->CharSetEXPTable();
		m_mathExp[enPlayerNumber_2P] = m_player2P->CharGetEXP();

		//経験値のフレーム
		m_ExperienceFlame[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS_DUO_1P);
		m_ExperienceFlame[enPlayerNumber_2P].SetPosition(EXPERIENCE_FLAME_POS_DUO_2P);

		//経験値バーの裏
		m_experienceBarBack[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS_DUO_1P);
		m_experienceBarBack[enPlayerNumber_2P].SetPosition(EXPERIENCE_FLAME_POS_DUO_2P);

		//経験値バー
		m_experienceBarFlont[enPlayerNumber_1P].SetPosition(EXPERIENCE_BAR_POS_DUO_1P);
		m_experienceBarFlont[enPlayerNumber_2P].SetPosition(EXPERIENCE_BAR_POS_DUO_2P);
	}

	else if (m_gameMode == RenderingEngine::enGameMode_TrioPlay || m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
	{
		for (int i = 0; i < enPlayerNumber_Num; i++)
		{
			m_Flame[i].Init("Assets/sprite/gameUI/LevelBar.DDS", FLAME_WIDTH, FLAME_HEIGHT);
			m_Flame[i].SetScale(FLAME_SCALE_QUARTET);
			m_UltRenderIN[i].SetScale(ULT_ICON_SCALE_QUARTET);
			m_UltRenderOUT[i].SetScale(ULT_ICON_SCALE_QUARTET);
			m_SkillRenderIN[i].SetScale(SKILL_SCALE_QUARTET);
			m_SkillRenderOUT[i].SetScale(SKILL_SCALE_QUARTET);
			m_skillFont[i].SetScale(SKILL_COOLTIME_FONT_SCALE_QUARTET);
			m_Lv[i].SetScale(LEVEL_SPRITE_SCALE_QUARTET);
			m_MaxLv[i].SetScale(MAX_LEVEL_SCALE_QUARTET);
			m_LvNumber[i].SetScale(LEVEL_NUMBER_SCALE_QUARTET);
			m_LvNumber_back[i].SetScale(LEVEL_NUMBER_SCALE_QUARTET);
			m_ExperienceFlame[i].SetScale(EXPERIENCE_SCALE_QUARTET);
			m_experienceBarBack[i].SetScale(EXPERIENCE_SCALE_QUARTET);
			m_experienceBarFlont[i].SetScale(EXPERIENCE_BAR_SCALE_QUARTET);
		}
		m_Flame[enPlayerNumber_1P].SetPosition(FLAME_QUARTET_POS_1P);
		m_Flame[enPlayerNumber_2P].SetPosition(FLAME_QUARTET_POS_2P);
		m_Flame[enPlayerNumber_3P].SetPosition(FLAME_QUARTET_POS_3P);
		m_Flame[enPlayerNumber_4P].SetPosition(FLAME_QUARTET_POS_4P);

		//必殺技のアイコン
		m_UltRenderIN[enPlayerNumber_1P].SetPosition(ULT_QUARTET_POS_1P);
		m_UltRenderIN[enPlayerNumber_2P].SetPosition(ULT_QUARTET_POS_2P);
		m_UltRenderIN[enPlayerNumber_3P].SetPosition(ULT_QUARTET_POS_3P);
		m_UltRenderIN[enPlayerNumber_4P].SetPosition(ULT_QUARTET_POS_4P);

		//必殺のアイコンフレーム
		m_UltRenderOUT[enPlayerNumber_1P].SetPosition(ULT_QUARTET_POS_1P);
		m_UltRenderOUT[enPlayerNumber_2P].SetPosition(ULT_QUARTET_POS_2P);
		m_UltRenderOUT[enPlayerNumber_3P].SetPosition(ULT_QUARTET_POS_3P);
		m_UltRenderOUT[enPlayerNumber_4P].SetPosition(ULT_QUARTET_POS_4P);

		//スキルのアイコン
		m_SkillRenderIN[enPlayerNumber_1P].SetPosition(SKILL_QUARTET_POS_1P);
		m_SkillRenderIN[enPlayerNumber_2P].SetPosition(SKILL_QUARTET_POS_2P);
		m_SkillRenderIN[enPlayerNumber_3P].SetPosition(SKILL_QUARTET_POS_3P);
		m_SkillRenderIN[enPlayerNumber_4P].SetPosition(SKILL_QUARTET_POS_4P);
		//スキルのアイコンフレーム
		m_SkillRenderOUT[enPlayerNumber_1P].SetPosition(SKILL_QUARTET_POS_1P);
		m_SkillRenderOUT[enPlayerNumber_2P].SetPosition(SKILL_QUARTET_POS_2P);
		m_SkillRenderOUT[enPlayerNumber_3P].SetPosition(SKILL_QUARTET_POS_3P);
		m_SkillRenderOUT[enPlayerNumber_4P].SetPosition(SKILL_QUARTET_POS_4P);

		//スキルのクールタイムを表示するフォントの設定
		m_skillFont[enPlayerNumber_1P].SetPosition(SKILL_COOLTIME_FONT_POS_QUARTET_1P);
		m_skillFont[enPlayerNumber_2P].SetPosition(SKILL_COOLTIME_FONT_POS_QUARTET_2P);
		m_skillFont[enPlayerNumber_3P].SetPosition(SKILL_COOLTIME_FONT_POS_QUARTET_3P);
		m_skillFont[enPlayerNumber_4P].SetPosition(SKILL_COOLTIME_FONT_POS_QUARTET_4P);

		playerCoolTime[enPlayerNumber_1P] = m_player1P->CharGetSkillCoolTimer();
		playerCoolTime[enPlayerNumber_2P] = m_player2P->CharGetSkillCoolTimer();
		playerCoolTime[enPlayerNumber_3P] = m_player3P->CharGetSkillCoolTimer();
		if (m_gameMode == RenderingEngine::enGameMode_TrioPlay)
		{
			playerCoolTime[enPlayerNumber_4P] = m_knightAI->GetSkillTimer();
		}
		else
		{
			playerCoolTime[enPlayerNumber_4P] = m_player4P->CharGetSkillCoolTimer();
		}

		//Lvの画像を読み込む
		m_Lv[enPlayerNumber_1P].SetPosition(LEVEL_SPRITE_QUARTET_POS_1P);
		m_Lv[enPlayerNumber_2P].SetPosition(LEVEL_SPRITE_QUARTET_POS_2P);
		m_Lv[enPlayerNumber_3P].SetPosition(LEVEL_SPRITE_QUARTET_POS_3P);
		m_Lv[enPlayerNumber_4P].SetPosition(LEVEL_SPRITE_QUARTET_POS_4P);

		//Lv1の画像を読み込む
		m_LvNumber[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_QUARTET_POS_1P);
		m_LvNumber[enPlayerNumber_2P].SetPosition(LEVEL_NUMBER_QUARTET_POS_2P);
		m_LvNumber[enPlayerNumber_3P].SetPosition(LEVEL_NUMBER_QUARTET_POS_3P);
		m_LvNumber[enPlayerNumber_4P].SetPosition(LEVEL_NUMBER_QUARTET_POS_4P);

		//Lv1の裏の画像の読み込み
		m_LvNumber_back[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_QUARTET_POS_1P);
		m_LvNumber_back[enPlayerNumber_2P].SetPosition(LEVEL_NUMBER_QUARTET_POS_2P);
		m_LvNumber_back[enPlayerNumber_3P].SetPosition(LEVEL_NUMBER_QUARTET_POS_3P);
		m_LvNumber_back[enPlayerNumber_4P].SetPosition(LEVEL_NUMBER_QUARTET_POS_4P);

		//10の画像を読み込む
		m_MaxLv[enPlayerNumber_1P].SetPosition(MAX_LEVEL_QUARTET_POS_1P);
		m_MaxLv[enPlayerNumber_2P].SetPosition(MAX_LEVEL_QUARTET_POS_2P);
		m_MaxLv[enPlayerNumber_3P].SetPosition(MAX_LEVEL_QUARTET_POS_3P);
		m_MaxLv[enPlayerNumber_4P].SetPosition(MAX_LEVEL_QUARTET_POS_4P);

		//経験値テーブルと初期経験値
		m_expTable[enPlayerNumber_2P] = m_player2P->CharSetEXPTable();
		m_mathExp[enPlayerNumber_2P] = m_player2P->CharGetEXP();
		m_expTable[enPlayerNumber_3P] = m_player3P->CharSetEXPTable();
		m_mathExp[enPlayerNumber_3P] = m_player3P->CharGetEXP();
		if (m_gameMode == RenderingEngine::enGameMode_TrioPlay)
		{
			m_expTable[enPlayerNumber_4P] = m_knightAI->GetExpTable();
			m_mathExp[enPlayerNumber_4P] = m_knightAI->GetExperience();
		}
		else
		{
			m_expTable[enPlayerNumber_4P] = m_player4P->CharSetEXPTable();
			m_mathExp[enPlayerNumber_4P] = m_player4P->CharGetEXP();
		}


		//経験値のフレーム
		m_ExperienceFlame[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_1P);
		m_ExperienceFlame[enPlayerNumber_2P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_2P);
		m_ExperienceFlame[enPlayerNumber_3P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_3P);
		m_ExperienceFlame[enPlayerNumber_4P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_4P);

		//経験値バーの裏
		m_experienceBarBack[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_1P);
		m_experienceBarBack[enPlayerNumber_2P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_2P);
		m_experienceBarBack[enPlayerNumber_3P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_3P);
		m_experienceBarBack[enPlayerNumber_4P].SetPosition(EXPERIENCE_FLAME_POS_QUARTET_4P);

		//経験値バー
		m_experienceBarFlont[enPlayerNumber_1P].SetPosition(EXPERIENCE_BAR_POS_QUARTET_1P);
		m_experienceBarFlont[enPlayerNumber_2P].SetPosition(EXPERIENCE_BAR_POS_QUARTET_2P);
		m_experienceBarFlont[enPlayerNumber_3P].SetPosition(EXPERIENCE_BAR_POS_QUARTET_3P);
		m_experienceBarFlont[enPlayerNumber_4P].SetPosition(EXPERIENCE_BAR_POS_QUARTET_4P);
	}
	else
	{
		//レベルや経験値のフレーム
		m_Flame[enPlayerNumber_1P].Init("Assets/sprite/gameUI/LevelBar.DDS", FLAME_WIDTH, FLAME_HEIGHT);
		m_Flame[enPlayerNumber_1P].SetPosition(FLAME_POS_SOLO);

		//必殺技のアイコン
		m_UltRenderIN[enPlayerNumber_1P].SetPosition(ULT_POS_SOLO);
		m_UltRenderIN[enPlayerNumber_1P].SetScale(ULT_ICON_SCALE_SOLO);
		//必殺のアイコンフレーム
		m_UltRenderOUT[enPlayerNumber_1P].SetPosition(ULT_POS_SOLO);
		m_UltRenderOUT[enPlayerNumber_1P].SetScale(ULT_ICON_SCALE_SOLO);
		//スキルのアイコン
		m_SkillRenderIN[enPlayerNumber_1P].SetPosition(SKILL_POS_SOLO);
		m_SkillRenderIN[enPlayerNumber_1P].SetScale(SKILL_SCALE_SOLO);
		//スキルのアイコンフレーム
		m_SkillRenderOUT[enPlayerNumber_1P].SetPosition(SKILL_POS_SOLO);
		m_SkillRenderOUT[enPlayerNumber_1P].SetScale(SKILL_SCALE_SOLO);

		//スキルのクールタイムを表示するフォントの設定
		m_skillFont[enPlayerNumber_1P].SetPosition(SKILL_COOLTIME_FONT_POS);
		m_skillFont[enPlayerNumber_1P].SetScale(SKILL_COOLTIME_FONT_SCALE);
		m_skillFont[enPlayerNumber_1P].SetColor(g_vec4Red);
		m_skillFont[enPlayerNumber_1P].SetShadowParam(true, SKILL_COOLTIME_FONT_SHADOW_OFFSET, g_vec4Black);
		playerCoolTime[enPlayerNumber_1P] = m_player1P->CharGetSkillCoolTimer();

		//Lvの画像を読み込む
		m_Lv[enPlayerNumber_1P].SetPosition(LEVEL_SPRITE_POS_SOLO);
		m_Lv[enPlayerNumber_1P].SetScale(LEVEL_SPRITE_SCALE_DUO);

		//Lv1の画像を読み込む
		m_LvNumber[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_SOLO);
		m_LvNumber[enPlayerNumber_1P].SetScale(LEVEL_NUMBER_SCALE_SOLO);

		//Lv1の裏の画像の読み込み
		m_LvNumber_back[enPlayerNumber_1P].SetPosition(LEVEL_NUMBER_POS_SOLO);
		m_LvNumber_back[enPlayerNumber_1P].SetScale(LEVEL_NUMBER_SCALE_SOLO);

		//10の画像を読み込む
		m_MaxLv[enPlayerNumber_1P].SetPosition(MAX_LEVEL_POS_SOLO);
		m_MaxLv[enPlayerNumber_1P].SetScale(MAX_LEVEL_SCALE);

		//経験値のフレーム
		m_ExperienceFlame[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS);
		m_ExperienceFlame[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);

		//経験値バーの裏
		m_experienceBarBack[enPlayerNumber_1P].SetPosition(EXPERIENCE_FLAME_POS);
		m_experienceBarBack[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);

		//経験値バー
		m_experienceBarFlont[enPlayerNumber_1P].SetPosition(EXPERIENCE_BAR_POS);
		m_experienceBarFlont[enPlayerNumber_1P].SetPivot(EXPERIENCEGAUGE_PIVOT);
		m_experienceBarFlont[enPlayerNumber_1P].SetScale(EXPERIENCE_SCALE);
	}

	//経験値テーブルと初期経験値
	m_expTable[enPlayerNumber_1P] = m_player1P->CharSetEXPTable();
	m_mathExp[enPlayerNumber_1P] = m_player1P->CharGetEXP();

	//レベルアップまでに必要な経験値の量
	m_ExpFont.SetPosition(UPTOLEVEL_POS);
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

void GameUI::InitHpUI()
{
	for (int i = 0; i < enPlayerNumber_Num; i++)
	{
		//HPのフォント
		m_HpFont[i].SetColor(g_vec4White);
		m_HpFont[i].SetShadowParam(true, 2.0f, g_vec4Black);

		if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
		{
			//HPゲージ裏の画像を読み込む
			m_hpBarBack[i].Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", HP_BAR_WIDTH_DUO, HP_BAR_BACK_HEIGHT);
			//HPゲージの画像を読み込む
			m_hpBar[i].Init("Assets/sprite/gameUI/HPBar_HP.DDS", HP_BAR_WIDTH_DUO, HP_BAR_HEIGHT_DUO);
			//HPバーの白い部分
			m_HpBar_White[i].Init("Assets/sprite/gameUI/HPBar_backwhite.DDS", HP_BAR_WIDTH_DUO, HP_BAR_HEIGHT_DUO);
			//HPゲージのフレームの画像を読み込む
			m_HPFrame[i].Init("Assets/sprite/gameUI/HPBar_flame.DDS", HP_BAR_WIDTH_DUO, HP_BAR_BACK_HEIGHT);
		}
		else if (m_gameMode == RenderingEngine::enGameMode_TrioPlay || m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
		{
			//HPゲージ裏の画像を読み込む
			m_hpBarBack[i].Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", HP_BAR_BACK_WIDTH_QUARTET, HP_BAR_BACK_HEIGHT_QUARTET);
			//HPゲージの画像を読み込む
			m_hpBar[i].Init("Assets/sprite/gameUI/HPBar_HP.DDS", HP_BAR_FRONT_WIDTH_QUARTET, HP_BAR_FRONT_HEIGHT_QUARTET);
			//HPバーの白い部分
			m_HpBar_White[i].Init("Assets/sprite/gameUI/HPBar_backwhite.DDS", HP_BAR_FRONT_WIDTH_QUARTET, HP_BAR_FRONT_HEIGHT_QUARTET);
			//HPゲージのフレームの画像を読み込む
			m_HPFrame[i].Init("Assets/sprite/gameUI/HPBar_flame.DDS", HP_BAR_BACK_WIDTH_QUARTET, HP_BAR_BACK_HEIGHT_QUARTET);
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
	if (m_player2P != nullptr)
	{
		White_BackHp[enPlayerNumber_2P] = m_player2P->GetCharacterHp();
		WhiteHp_Timer[enPlayerNumber_2P] = WHITEHP_WAIT;
		BackUPLV[enPlayerNumber_2P] = m_player2P->GetCharacterLevel();
	}
	if (m_player3P != nullptr)
	{
		White_BackHp[enPlayerNumber_3P] = m_player3P->GetCharacterHp();
		WhiteHp_Timer[enPlayerNumber_3P] = WHITEHP_WAIT;
		BackUPLV[enPlayerNumber_3P] = m_player3P->GetCharacterLevel();
	}
	if (m_player4P != nullptr)
	{
		White_BackHp[enPlayerNumber_4P] = m_player4P->GetCharacterHp();
		WhiteHp_Timer[enPlayerNumber_4P] = WHITEHP_WAIT;
		BackUPLV[enPlayerNumber_4P] = m_player4P->GetCharacterLevel();
	}

	if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
	{
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
	else if (m_gameMode == RenderingEngine::enGameMode_TrioPlay || m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
	{
		//1P用画像
		m_hpBar[enPlayerNumber_1P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_HpBar_White[enPlayerNumber_1P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_hpBar[enPlayerNumber_1P].SetScale(HP_BAR_QUARTET_SCALE);
		m_HpBar_White[enPlayerNumber_1P].SetScale(HP_BAR_QUARTET_SCALE);

		m_hpBarBack[enPlayerNumber_1P].SetPosition(HP_BAR_POS_QUARTET_LEFT_UP);
		m_hpBarBack[enPlayerNumber_1P].SetScale(HP_BAR_BACK_QUARTET_SCALE);
		m_hpBar[enPlayerNumber_1P].SetPosition(HP_BAR_FLONT_QUARTET_LEFT_UP);
		m_HpBar_White[enPlayerNumber_1P].SetPosition(HP_BAR_FLONT_QUARTET_LEFT_UP);
		m_HPFrame[enPlayerNumber_1P].SetPosition(HP_BAR_POS_QUARTET_LEFT_UP);
		m_HPFrame[enPlayerNumber_1P].SetScale(HP_BAR_BACK_QUARTET_SCALE);

		//2P用画像
		m_hpBar[enPlayerNumber_2P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_HpBar_White[enPlayerNumber_2P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_hpBar[enPlayerNumber_2P].SetScale(HP_BAR_QUARTET_SCALE);
		m_HpBar_White[enPlayerNumber_2P].SetScale(HP_BAR_QUARTET_SCALE);

		m_HPFrame[enPlayerNumber_2P].SetPosition(HP_BAR_POS_QUARTET_RIGHT_UP);
		m_hpBarBack[enPlayerNumber_2P].SetPosition(HP_BAR_POS_QUARTET_RIGHT_UP);
		m_hpBarBack[enPlayerNumber_2P].SetScale(HP_BAR_BACK_QUARTET_SCALE);
		m_hpBar[enPlayerNumber_2P].SetPosition(HP_BAR_FLONT_QUARTET_RIGHT_UP);
		m_HpBar_White[enPlayerNumber_2P].SetPosition(HP_BAR_FLONT_QUARTET_RIGHT_UP);
		m_HPFrame[enPlayerNumber_2P].SetScale(HP_BAR_BACK_QUARTET_SCALE);

		//3P用画像
		m_hpBar[enPlayerNumber_3P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_HpBar_White[enPlayerNumber_3P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_hpBar[enPlayerNumber_3P].SetScale(HP_BAR_QUARTET_SCALE);
		m_HpBar_White[enPlayerNumber_3P].SetScale(HP_BAR_QUARTET_SCALE);

		m_HPFrame[enPlayerNumber_3P].SetPosition(HP_BAR_POS_QUARTET_LEFT_DOWN);
		m_hpBarBack[enPlayerNumber_3P].SetPosition(HP_BAR_POS_QUARTET_LEFT_DOWN);
		m_hpBarBack[enPlayerNumber_3P].SetScale(HP_BAR_BACK_QUARTET_SCALE);
		m_hpBar[enPlayerNumber_3P].SetPosition(HP_BAR_FLONT_QUARTET_LEFT_DOWN);
		m_HpBar_White[enPlayerNumber_3P].SetPosition(HP_BAR_FLONT_QUARTET_LEFT_DOWN);
		m_HPFrame[enPlayerNumber_3P].SetScale(HP_BAR_BACK_QUARTET_SCALE);

		//4P用画像
		m_hpBar[enPlayerNumber_4P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_HpBar_White[enPlayerNumber_4P].SetPivot(HPGAUGE_PIVOT_LEFT);
		m_hpBar[enPlayerNumber_4P].SetScale(HP_BAR_QUARTET_SCALE);
		m_HpBar_White[enPlayerNumber_4P].SetScale(HP_BAR_QUARTET_SCALE);

		m_HPFrame[enPlayerNumber_4P].SetPosition(HP_BAR_POS_QUARTET_RIGHT_DOWN);
		m_hpBarBack[enPlayerNumber_4P].SetPosition(HP_BAR_POS_QUARTET_RIGHT_DOWN);
		m_hpBarBack[enPlayerNumber_4P].SetScale(HP_BAR_BACK_QUARTET_SCALE);
		m_hpBar[enPlayerNumber_4P].SetPosition(HP_BAR_FLONT_QUARTET_RIGHT_DOWN);
		m_HpBar_White[enPlayerNumber_4P].SetPosition(HP_BAR_FLONT_QUARTET_RIGHT_DOWN);
		m_HPFrame[enPlayerNumber_4P].SetScale(HP_BAR_BACK_QUARTET_SCALE);

		//HPフォント
		m_HpFont[enPlayerNumber_1P].SetPosition(HP_FONT_QUARTET_POS_1P);
		m_HpFont[enPlayerNumber_1P].SetScale(0.35f);
		m_HpFont[enPlayerNumber_1P].SetShadowParam(true, 0.7f, g_vec4Black);
		m_HpFont[enPlayerNumber_2P].SetPosition(HP_FONT_QUARTET_POS_2P);
		m_HpFont[enPlayerNumber_2P].SetScale(0.35f);
		m_HpFont[enPlayerNumber_2P].SetShadowParam(true, 0.7f, g_vec4Black);
		m_HpFont[enPlayerNumber_3P].SetPosition(HP_FONT_QUARTET_POS_3P);
		m_HpFont[enPlayerNumber_3P].SetScale(0.35f);
		m_HpFont[enPlayerNumber_3P].SetShadowParam(true, 0.7f, g_vec4Black);
		m_HpFont[enPlayerNumber_4P].SetPosition(HP_FONT_QUARTET_POS_4P);
		m_HpFont[enPlayerNumber_4P].SetScale(0.35f);
		m_HpFont[enPlayerNumber_4P].SetShadowParam(true, 0.7f, g_vec4Black);
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

void GameUI::InitTimerUI()
{
	m_TimeAndPointRender.Init("Assets/sprite/gameUI/timer.DDS", 1100.0f, 400.0f);

	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		m_TimeAndPointRender.SetPosition(TIME_POS);
		m_TimeAndPointRender.SetScale(TIME_SCALE_QUARTET);

		//フォントの大きさを設定。
		m_minutes.SetScale(TIMERSCALE_QUARTET);
		m_seconds.SetScale(TIMERSCALE_QUARTET);
		m_coron.SetScale(TIMERSCALE_QUARTET);

		//座標の設定
		m_minutes.SetPosition(MINUTES_FONT_POS_QUARTET);
		m_seconds.SetPosition(SECONDS_FONT_POS_QUARTET);
		m_coron.SetPosition(CORON_FONT_POS_QUARTET);
	}
	else
	{
		m_TimeAndPointRender.SetPosition(TIME_POS);
		m_TimeAndPointRender.SetScale(TIME_SCALE);

		//フォントの大きさを設定。
		m_minutes.SetScale(TIMERSCALE);
		m_seconds.SetScale(TIMERSCALE);
		m_coron.SetScale(TIMERSCALE);

		//座標の設定
		m_minutes.SetPosition(MINUTES_FONT_POS);
		m_seconds.SetPosition(SECONDS_FONT_POS);
		m_coron.SetPosition(CORON_FONT_POS);
	}
	
	Vector2 pivotOffSet = { 30.0f, -20.0f };
	//オフセットの設定
	m_minutes.SetPivotOffSet(pivotOffSet);
	m_seconds.SetPivotOffSet(pivotOffSet);
	m_coron.SetPivotOffSet(pivotOffSet);

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
	if (m_gameMode >= RenderingEngine::enGameMode_DuoPlay && m_player2P->CharGetRespawnTime() > 0.0f)
	{
		RespawnCountDown(enPlayerNumber_2P);
	}
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay && m_player3P->CharGetRespawnTime() > 0.0f)
	{
		RespawnCountDown(enPlayerNumber_3P);
	}
	if (m_gameMode == RenderingEngine::enGameMode_QuartetPlay && m_player4P->CharGetRespawnTime() > 0.0f)
	{
		RespawnCountDown(enPlayerNumber_4P);
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
		m_LvNumber_back[enPlayerNumber_3P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
		m_LvNumber_back[enPlayerNumber_4P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
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
		m_LvNumber_back[enPlayerNumber_3P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
		m_LvNumber_back[enPlayerNumber_4P].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_LvNumberColor));
	}
	m_LvNumber_back[enPlayerNumber_1P].Update();
	m_LvNumber_back[enPlayerNumber_2P].Update();
	m_LvNumber_back[enPlayerNumber_3P].Update();
	m_LvNumber_back[enPlayerNumber_4P].Update();

	ExpState(m_player1P);
	HPBar(m_player1P);
	if (m_gameMode >= RenderingEngine::enGameMode_DuoPlay)
	{
		ExpState(m_player2P);
		HPBar(m_player2P);
	}
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		ExpState(m_player3P);
		HPBar(m_player3P);
	}
	if (m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
	{
		ExpState(m_player4P);
		HPBar(m_player4P);
	}
	if (m_gameMode == RenderingEngine::enGameMode_TrioPlay)
	{
		CpuExpState(m_knightAI);
		CpuHpBar(m_knightAI);
	}
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
		m_gameCountScale += SMALLSCALE;
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
			m_gameCountScale -= FIGHT_SMALL_SCALE;

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
	else if (playerNumber == enPlayerNumber_2P) {
		respornCountDown = (int)m_player2P->CharGetRespawnTime();
	}
	else if (playerNumber == enPlayerNumber_3P)
	{
		respornCountDown = (int)m_player3P->CharGetRespawnTime();
	}
	else
	{
		respornCountDown = (int)m_player4P->CharGetRespawnTime();
	}

	if (oldRespawnCount[playerNumber] != respornCountDown)
	{
		switch (respornCountDown)
		{
		case 0:
			m_respawnCountNumber[playerNumber].Init("Assets/sprite/gameUI/RespawnConut0.DDS", 300, 500.0f);
			//画面を暗くしてゆく
			if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
			{
				if (playerNumber == enPlayerNumber_1P) {
					m_fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_Left);
				}
				else {
					m_fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_Right);
				}
				break;
			}
			if (m_gameMode == RenderingEngine::enGameMode_TrioPlay || m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
			{
				if (playerNumber == enPlayerNumber_1P) {
					m_fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_LeftUp);
				}
				else if (playerNumber == enPlayerNumber_2P)
				{
					m_fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_RightUp);
				}
				else if (playerNumber == enPlayerNumber_3P)
				{
					m_fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_LeftDown);
				}
				else if (playerNumber == enPlayerNumber_4P)
				{
					m_fade->StartFadeIn(2.0f, Fade::enFadeSpriteType_RightDown);
				}
				break;
			}
			//ソロプレイ時
			m_fade->StartFadeIn(2.0f);
			break;
		case 1:
			m_respawnCountNumber[playerNumber].Init("Assets/sprite/gameUI/RespawnConut1.DDS", 300, 500.0f);
			break;
		case 2:
			m_respawnCountNumber[playerNumber].Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
			break;
		default:
			break;
		}
	}

	oldRespawnCount[playerNumber] = respornCountDown;

	m_respawnCountNumber[playerNumber].Update();
}

//プレイヤーのHPの表示の処理
void GameUI::HPBar(const Player* player)
{
	EnPlayerNumber playerNumber = enPlayerNumber_1P;
	if (player == m_player2P)
	{
		playerNumber = enPlayerNumber_2P;
	}
	else if (player == m_player3P)
	{
		playerNumber = enPlayerNumber_3P;
	}
	else if (player == m_player4P)
	{
		playerNumber = enPlayerNumber_4P;
	}

	int HP = player->GetCharacterHp();
	int MaxHP = player->GetCharcterMaxHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%3d/%3d", HP, MaxHP);
	m_HpFont[playerNumber].SetText(hp);

	Vector3 HpScale = Vector3::One;
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		HpScale = HP_BAR_QUARTET_SCALE;
	}

	//HPバーの減っていく割合。
	HpScale.x = (float)player->GetCharacterHp() / (float)player->GetCharcterMaxHp();
	m_hpBar[playerNumber].SetScale(HpScale);

	m_hpBar[playerNumber].Update();

	//レベルが下がった時の処理
	if (BackUPLV[playerNumber] > player->GetCharacterLevel())
	{
		White_BackHp[playerNumber] = HP;
	}
	BackUPLV[playerNumber] = player->GetCharacterLevel();

	//Hp削られたら白い部分も減らす
	if (HP < White_BackHp[playerNumber])
	{
		if (WhiteHp_Timer[playerNumber] > 0.0f)
		{
			WhiteHp_Timer[playerNumber] -= g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			White_BackHp[playerNumber] -= 2;

			//HPバーの減っていく割合。
			HpScale.x = (float)White_BackHp[playerNumber] / (float)MaxHP;
			m_HpBar_White[playerNumber].SetScale(HpScale);

			if (White_BackHp[playerNumber] <= HP)
			{
				White_BackHp[playerNumber] = HP;
				WhiteHp_Timer[playerNumber] = WHITEHP_WAIT;
			}
		}
	}
	else if (HP > White_BackHp[playerNumber])
	{
		White_BackHp[playerNumber] = HP;
	}
	m_HpBar_White[playerNumber].Update();
}

void GameUI::CpuHpBar(KnightAI* character)
{
	EnPlayerNumber playerNumber = enPlayerNumber_4P;

	int HP = character->GetHitPoint();
	int MaxHP = character->GetMaxHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%3d/%3d", HP, MaxHP);
	m_HpFont[playerNumber].SetText(hp);

	Vector3 HpScale = Vector3::One;
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		HpScale = HP_BAR_QUARTET_SCALE;
	}

	//HPバーの減っていく割合。
	HpScale.x = (float)character->GetHitPoint() / (float)character->GetMaxHp();
	m_hpBar[playerNumber].SetScale(HpScale);

	m_hpBar[playerNumber].Update();

	//レベルが下がった時の処理
	if (BackUPLV[playerNumber] > character->GetLevel())
	{
		White_BackHp[playerNumber] = HP;
	}
	BackUPLV[playerNumber] = character->GetLevel();

	//Hp削られたら白い部分も減らす
	if (HP < White_BackHp[playerNumber])
	{
		if (WhiteHp_Timer[playerNumber] > 0.0f)
		{
			WhiteHp_Timer[playerNumber] -= g_gameTime->GetFrameDeltaTime();
		}
		else
		{
			White_BackHp[playerNumber] -= 2;

			//HPバーの減っていく割合。
			HpScale.x = (float)White_BackHp[playerNumber] / (float)MaxHP;
			m_HpBar_White[playerNumber].SetScale(HpScale);

			if (White_BackHp[playerNumber] <= HP)
			{
				White_BackHp[playerNumber] = HP;
				WhiteHp_Timer[playerNumber] = WHITEHP_WAIT;
			}
		}
	}
	else if (HP > White_BackHp[playerNumber])
	{
		White_BackHp[playerNumber] = HP;
	}
	m_HpBar_White[playerNumber].Update();
}

//AIのレベルの表示
void GameUI::Level()
{
	int num = 0;
	for (auto actor : m_Actors)
	{
		int Lv = actor->GetLevel();
		wchar_t AILv[255];
		swprintf_s(AILv, 255, L"Lv%2d", Lv);
		m_LevelFont[num].SetText(AILv);
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

	float bigTimeScale = 0.0f;
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		bigTimeScale = TIMERSCALE_QUARTET + 1.15f;
	}
	else
	{
		bigTimeScale = TIMERSCALE + 1.15f;
	}

	//ゲームタイマーが1秒より大きく、10秒より少ないとき
	if (m_game->GetMinutesTimer() < 1 && m_game->GetSecondsTimer() < 10 && m_game->GetSecondsTimer() > 1)
	{
		if (m_timerScaleFlag == false)
		{
			if (m_timerScale < bigTimeScale)
			{
				m_timerScale += 1.9f * g_gameTime->GetFrameDeltaTime();
			}
			else
			{
				m_timerScale = bigTimeScale;
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
			if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
			{
				if (m_timerScale > TIMERSCALE_QUARTET)
				{
					m_timerScale -= 3.4f * g_gameTime->GetFrameDeltaTime();
				}
				else
				{
					m_timerScale = TIMERSCALE_QUARTET;
					m_timerScaleFlag = false;
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
		if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
		{
			m_minutes.SetScale(TIMERSCALE_QUARTET);
			m_seconds.SetScale(TIMERSCALE_QUARTET);
			m_coron.SetScale(TIMERSCALE_QUARTET);
		}
		else
		{
			m_minutes.SetScale(TIMERSCALE);
			m_seconds.SetScale(TIMERSCALE);
			m_coron.SetScale(TIMERSCALE);
		}
	}
}

void GameUI::ExpState(const Player* player)
{
	EnPlayerNumber enPlayerNumber = enPlayerNumber_1P;
	if (player == m_player2P)
	{
		enPlayerNumber = enPlayerNumber_2P;
	}
	else if (player == m_player3P)
	{
		enPlayerNumber = enPlayerNumber_3P;
	}
	else if (player == m_player4P)
	{
		enPlayerNumber = enPlayerNumber_4P;
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
		UpExp(enPlayerNumber);
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

	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		EXPScale.y = EXPERIENCE_BAR_SCALE_QUARTET.y;
	}

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

void GameUI::UpExp(const EnPlayerNumber playerNumber)
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

void GameUI::CpuExpState(KnightAI* knight)
{
	EnPlayerNumber enPlayerNumber = enPlayerNumber_4P;

	//レベルが下がったら
	if (m_playerLevel[enPlayerNumber] > knight->GetLevel()) {
		m_expUpFlag[enPlayerNumber] = true;
		m_enExpProcessState[enPlayerNumber] = enLevelDownState;
	}

	if (m_expUpFlag[enPlayerNumber] == false) {
		return;
	}

	switch (m_enExpProcessState[enPlayerNumber])
	{
	case GameUI::enChackExpState:
		CpuChackExp(knight, enPlayerNumber);
		break;
	case GameUI::enUpExpState:
		UpExp(enPlayerNumber);
		break;
	case GameUI::enDownExpState:
		DownExp(enPlayerNumber);
		break;
	case GameUI::enLevelUpState:
		CpuLevelUp(knight, enPlayerNumber);
		break;
	case GameUI::enLevelDownState:
		CpuLevelDown(knight, enPlayerNumber);
		break;
	default:
		break;
	}

	m_oldSaveExp[enPlayerNumber] = m_saveExp[enPlayerNumber];

	//経験値の表示
	Vector3 EXPScale = Vector3::One;

	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		EXPScale.y = EXPERIENCE_BAR_SCALE_QUARTET.y;
	}

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

void GameUI::CpuChackExp(KnightAI* knight, const EnPlayerNumber playerNumber)
{
	//レベルが下がったら
	if (m_playerLevel[playerNumber] > knight->GetLevel()) {
		m_enExpProcessState[playerNumber] = enLevelDownState;
		return;
	}

	//セーブした経験値が前フレームのセーブした経験値と違うなら
	if (knight->GetSaveEXP() != m_oldSaveExp[playerNumber]) {
		m_saveExp[playerNumber] = knight->GetSaveEXP();

		m_enExpProcessState[playerNumber] = enUpExpState;
	}
}

void GameUI::CpuLevelUp(KnightAI* knight, const EnPlayerNumber playerNumber)
{
	m_saveExp[playerNumber] -= m_expTable[playerNumber];

	m_mathExp[playerNumber] = 0;

	//レベルアップの処理
	if (m_playerLevel[playerNumber] < knight->GetLevel()) {
		m_playerLevel[playerNumber]++;
	}
	//レベルに応じた経験値テーブルにする
	m_expTable[playerNumber] = knight->GetExpTableForLevel(m_playerLevel[playerNumber]);

	//レベル画像変更
	LevelSpriteChange(m_playerLevel[playerNumber], playerNumber);

	if (m_playerLevel[playerNumber] == 10) {
		m_saveExp[playerNumber] = 10;
		knight->SetSaveEXP(m_saveExp[playerNumber]);
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
		if (knight->GetSaveEXP() > 0) {
			//セーブした経験値が変わらない
			knight->SetSaveEXP(m_saveExp[playerNumber]);
		}

		m_oldSaveExp[playerNumber] = knight->GetSaveEXP();
		m_enExpProcessState[playerNumber] = enUpExpState;
	}
	//もうレベルアップの処理が終わりなら
	else if (m_saveExp[playerNumber] <= 0) {

		m_expUpFlag[playerNumber] = false;

		//レベルアップの処理の間に中立の敵を倒していたなら
		if (knight->GetExperience() > 0) {
			knight->SetSaveEXP(knight->GetExperience());
			m_saveExp[playerNumber] = knight->GetSaveEXP();
			m_oldSaveExp[playerNumber] = m_saveExp[playerNumber];
			//経験値の処理にいく
			m_enExpProcessState[playerNumber] = enUpExpState;
		}
		else
		{
			//セーブした経験値をリセット
			knight->SetSaveEXP(0);
			m_saveExp[playerNumber] = knight->GetSaveEXP();
			m_oldSaveExp[playerNumber] = m_saveExp[playerNumber];

			m_enExpProcessState[playerNumber] = enChackExpState;
		}
	}
}

void GameUI::CpuLevelDown(KnightAI* knight, const EnPlayerNumber playerNumber)
{
	m_playerLevel[playerNumber]--;
	//レベルに応じた経験値テーブルにする
	m_expTable[playerNumber] = knight->GetExpTableForLevel(m_playerLevel[playerNumber]);

	LevelSpriteChange(m_playerLevel[playerNumber], playerNumber);

	if (m_playerLevel[playerNumber] <= knight->GetLevel()) {
		m_expUpFlag[playerNumber] = false;
		//レベルダウンの処理を終わる
		m_enExpProcessState[playerNumber] = enChackExpState;
		m_saveExp[playerNumber] = 0;
		m_oldSaveExp[playerNumber] = m_saveExp[playerNumber];
		m_mathExp[playerNumber] = 0;
		//セーブした経験値をリセット
		knight->SetSaveEXP(0);
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
			wchar_t text[] = L"?p";
			swprintf_s(P, 255, L"%3s", text);
			m_PointFont[num].SetText(P);
			num++;
			continue;
		}

		charPoint[num] = actor->GetPoint();

		//ポイントの表示
		int POINT = charPoint[num];
		wchar_t P[255];
		swprintf_s(P, 255, L"%2dp", POINT);
		m_PointFont[num].SetText(P);

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

void GameUI::RenderDeathPlayerSprite(RenderContext& rc)
{
	//リスポーンするまでの時間
	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
	{
		if (m_player1P->CharGetRespawnTime() > 0 || m_player2P->CharGetRespawnTime() > 0)
		{
			//制限時間
			m_TimeAndPointRender.Draw(rc);
			m_minutes.Draw(rc);
			m_seconds.Draw(rc);
			m_coron.Draw(rc);

			if (m_player1P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_1P].Draw(rc);
				m_respawnIn[enPlayerNumber_1P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_1P].Draw(rc);
			}
			if (m_player2P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_2P].Draw(rc);
				m_respawnIn[enPlayerNumber_2P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_2P].Draw(rc);
			}
		}
	}
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay)
	{
		if (m_player1P->CharGetRespawnTime() > 0 ||
			m_player2P->CharGetRespawnTime() > 0 ||
			m_player3P->CharGetRespawnTime() > 0)
		{
			//制限時間
			m_TimeAndPointRender.Draw(rc);
			m_minutes.Draw(rc);
			m_seconds.Draw(rc);
			m_coron.Draw(rc);

			if (m_player1P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_1P].Draw(rc);
				m_respawnIn[enPlayerNumber_1P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_1P].Draw(rc);
			}
			if (m_player2P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_2P].Draw(rc);
				m_respawnIn[enPlayerNumber_2P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_2P].Draw(rc);
			}
			if (m_player3P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_3P].Draw(rc);
				m_respawnIn[enPlayerNumber_3P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_3P].Draw(rc);
			}
		}
	}
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
	{
		if (m_player1P->CharGetRespawnTime() > 0 ||
			m_player2P->CharGetRespawnTime() > 0 ||
			m_player3P->CharGetRespawnTime() > 0 ||
			m_player4P->CharGetRespawnTime() > 0)
		{
			//制限時間
			m_TimeAndPointRender.Draw(rc);
			m_minutes.Draw(rc);
			m_seconds.Draw(rc);
			m_coron.Draw(rc);

			if (m_player1P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_1P].Draw(rc);
				m_respawnIn[enPlayerNumber_1P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_1P].Draw(rc);
			}
			if (m_player2P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_2P].Draw(rc);
				m_respawnIn[enPlayerNumber_2P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_2P].Draw(rc);
			}
			if (m_player3P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_3P].Draw(rc);
				m_respawnIn[enPlayerNumber_3P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_3P].Draw(rc);
			}
			if (m_player4P->CharGetRespawnTime() > 0)
			{
				m_respawnBack[enPlayerNumber_4P].Draw(rc);
				m_respawnIn[enPlayerNumber_4P].Draw(rc);
				m_respawnCountNumber[enPlayerNumber_4P].Draw(rc);
			}
		}
	}
	else
	{
		if (m_player1P->CharGetRespawnTime() > 0)
		{
			//制限時間
			m_TimeAndPointRender.Draw(rc);
			m_minutes.Draw(rc);
			m_seconds.Draw(rc);
			m_coron.Draw(rc);

			m_respawnBack[enPlayerNumber_1P].Draw(rc);

			m_respawnIn[enPlayerNumber_1P].Draw(rc);
			m_respawnCountNumber[enPlayerNumber_1P].Draw(rc);

		}
	}
	return;
}

void GameUI::RenderCoolTimeFont(RenderContext& rc)
{
	//スキルのクールタイムとタイマーが違う時だけ表示
	if (m_player1P->CharGetSkillCoolTimer() != playerCoolTime[enPlayerNumber_1P])
	{
		m_skillFont[enPlayerNumber_1P].Draw(rc);
	}
	//2人プレイ以上
	if (m_gameMode >= RenderingEngine::enGameMode_DuoPlay)
	{
		if (m_player2P->CharGetSkillCoolTimer() != playerCoolTime[enPlayerNumber_2P])
		{
			m_skillFont[enPlayerNumber_2P].Draw(rc);
		}
		//3人プレイ以上
		if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
		{
			if (m_player3P->CharGetSkillCoolTimer() != playerCoolTime[enPlayerNumber_3P])
			{
				m_skillFont[enPlayerNumber_3P].Draw(rc);
			}
			//4人プレイのとき
			if (m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
			{
				if (m_player4P->CharGetSkillCoolTimer() != playerCoolTime[enPlayerNumber_4P])
				{
					m_skillFont[enPlayerNumber_4P].Draw(rc);
				}
			}
			else
			{
				if (m_knightAI->GetSkillTimer() != playerCoolTime[enPlayerNumber_4P])
				{
					m_skillFont[enPlayerNumber_4P].Draw(rc);
				}
			}
		}
	}
}

void GameUI::RenderTimeLimit(RenderContext& rc)
{
	//制限時間
	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
	{
		m_TimeAndPointRender.Draw(rc);
		m_minutes.Draw(rc);
		m_seconds.Draw(rc);
		m_coron.Draw(rc);
	}
	else
	{
		m_TimeAndPointRender.DrawFront(rc);
		m_minutes.Draw(rc, true);
		m_seconds.Draw(rc, true);
		m_coron.Draw(rc, true);
	}
}

void GameUI::RenderPoint(RenderContext& rc)
{
	//ポイントを描画
	for (int i = 0; i < enPlayerNumber_Num; i++)
	{
		if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
		{
			m_pointFlame[i].DrawFront(rc);
			m_PointFont[i].Draw(rc, true);
			m_charIcon[i].DrawFront(rc);
			m_playerCpuName[i].DrawFront(rc);
			m_LevelFont[i].Draw(rc, true);
		}
		else
		{
			m_pointFlame[i].Draw(rc);
			m_charIcon[i].Draw(rc);
			m_PointFont[i].Draw(rc);
			m_playerCpuName[i].Draw(rc);
			if (m_gameMode == RenderingEngine::enGameMode_SoloPlay)
			{
				m_LevelFont[i].Draw(rc);
			}
		}
	}
}

void GameUI::RenderExpelience(RenderContext& rc)
{
	int loopCount = static_cast<int>(m_gameMode);
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		loopCount = enPlayerNumber_Num;
	}
	for (int i = 0; i < loopCount; i++)
	{
		m_experienceBarBack[i].Draw(rc);
		if (m_mathExp[i] != 0) {
			m_experienceBarFlont[i].Draw(rc);
		}
		m_ExperienceFlame[i].Draw(rc);

		m_Lv[i].Draw(rc);
		m_LvNumber_back[i].Draw(rc);
		m_LvNumber[i].Draw(rc);
		m_MaxLv[i].Draw(rc);
	}
}

void GameUI::RenderHp(RenderContext& rc)
{
	int loopCount = static_cast<int>(m_gameMode);
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		loopCount = enPlayerNumber_Num;
	}
	for (int i = 0; i < loopCount; i++)
	{
		m_hpBarBack[i].Draw(rc);
		m_HpBar_White[i].Draw(rc);
		m_hpBar[i].Draw(rc);
		m_HPFrame[i].Draw(rc);
		m_HpFont[i].Draw(rc);
	}
}

void GameUI::RenderIcon(RenderContext& rc)
{
	int loopCount = static_cast<int>(m_gameMode);
	if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
	{
		loopCount = enPlayerNumber_Num;
	}
	for (int i = 0; i < loopCount; i++)
	{
		m_SkillRenderIN[i].Draw(rc);
		m_SkillRenderOUT[i].Draw(rc);

		m_UltRenderIN[i].Draw(rc);
		m_UltRenderOUT[i].Draw(rc);
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

	RenderDeathPlayerSprite(rc);

	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState != m_PauseState && m_GameUIState != m_GameStartState) {
		int loopCount = static_cast<int>(m_gameMode);
		if (m_gameMode >= RenderingEngine::enGameMode_TrioPlay)
		{
			loopCount = enPlayerNumber_Num;
		}
		//レベルや経験値のフレーム
		for (int i = 0; i < loopCount; i++)
		{
			m_Flame[i].Draw(rc);
		}

		RenderCoolTimeFont(rc);
		RenderTimeLimit(rc);

		RenderExpelience(rc);
		RenderHp(rc);
		RenderIcon(rc);

		RenderPoint(rc);
	}
	else
	{
		//カウントダウンの表示
		if (m_game->NowGameState() == 0 && m_game->CountDownMinutes() <= 3) {
			m_CountNumper.Draw(rc);
		}
	}
}