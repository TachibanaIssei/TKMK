#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"

#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"

namespace
{
	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//ゲージのピボット
	const Vector2 HPGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//HPゲージのピボット
	const Vector2 EXPERIENCEGAUGE_PIVOT = Vector2(1.0f, 1.0f);				//経験値ゲージのピボット

	const Vector3 STATUS_BAR_POS = Vector3(-450.0f, -500.0f, 0.0f);	//ステータスバーポジション
	const Vector3 TIME_POS = Vector3(0.0,470.0f, 0.0f);	//制限時間の座標
	const Vector3 TIME_FONT_POS = Vector3(-80.0, 500.0f, 0.0f);	//制限時間の座標

	const Vector3 HP_BAR_POS = Vector3(-670.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_FLONT_POS = Vector3(-960.0f, -480.0f, 0.0f);	//HPバーの表のポジション

	const float HP_BAR_WIDTH = 1400.0f;     //HPバーの長さ
	const float HP_BAR_HIGHT = 200.0f;      //HPバーの高さ
	const Vector3 HP_BAR_SIZE = Vector3(HP_BAR_WIDTH, HP_BAR_HIGHT, 0.0f);  //HPバーのサイズ

	const Vector3 Skill_Pos = Vector3(520.0f, -280.0f, 0.0f);   //スキルアイコンポジション
	const Vector3 Ult_Pos = Vector3(470.0f, -445.0f, 0.0f);     //必殺技アイコンポジション

	const Vector3 LV_NUBER_POS = Vector3(750.0f, -360.0f, 0.0f);
	const Vector3 LvPos = Vector3(640.0f, -310.0f, 0.0f);       //Lv

	const Vector3 FLAME_POS = Vector3(920.0f,-480.0f,0.0f);    //レベルや経験値のフレーム

	const Vector3 PointPos = Vector3(-850.0f, -260.0f, 0.0f);  //ポイント

	const Vector3 EXPERIENCE_POS = Vector3(750.0f, -500.0f, 0.0f);  //ポイント
}
GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	player = FindGO<Player>("player");

	//Level
	m_LevelFont.SetPosition(LV_NUBER_POS);
	m_LevelFont.SetScale(2.0f);
	m_LevelFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LevelFont.SetRotation(0.0f);
	m_LevelFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_PointFont.SetPosition(PointPos);
	m_PointFont.SetScale(2.0f);
	m_PointFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointFont.SetRotation(0.0f);
	m_PointFont.SetShadowParam(true, 2.0f, g_vec4Black);

	

	//右下のフレーム
	{
		//レベルや経験値のフレーム
		m_Flame.Init("Assets/sprite/gameUI/LevelBar.DDS", 1200.0f, 500.0f);
		m_Flame.SetPosition(FLAME_POS);
		m_Flame.SetScale(1.0, 1.0, 1.0);
		
		//経験値のフレーム
		m_ExperienceFlame.Init("Assets/sprite/gameUI/ExperienceBar.DDS", 600.0f, 120.0f);
		m_ExperienceFlame.SetPosition(EXPERIENCE_POS);
		m_ExperienceFlame.SetScale(0.5, 0.5, 1.0);

		//経験値バーの表ピボットにするtodo
		m_ExperienceBar_flont.Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", 600.0f, 120.0f);
		m_ExperienceBar_flont.SetPosition(EXPERIENCE_POS);
		m_ExperienceBar_flont.SetPivot(EXPERIENCEGAUGE_PIVOT);
		m_ExperienceBar_flont.SetScale(0.5, 0.5, 1.0);

		//Lvの画像を読み込む
		m_Lv.Init("Assets/sprite/Lv.DDS", 200.0f, 150.0f);
		m_Lv.SetPosition(LvPos);
		m_Lv.SetScale(0.4, 0.4, 1.0);

		//Lv1の画像を読み込む
		m_LvNumber.Init("Assets/sprite/Lv1.DDS", 320.0f, 150.0f);
		m_LvNumber.SetPosition(LV_NUBER_POS);
		m_LvNumber.SetScale(0.4, 1.0, 1.0);

		//スキルのアイコン
		m_SkillRender.Init("Assets/sprite/skill_flame.DDS", 162, 162);
		m_SkillRender.SetPosition(Skill_Pos);
		m_SkillRender.SetScale(1.1, 1.1);
		//必殺技のアイコン
		m_UltRender.Init("Assets/sprite/ult_flame.DDs", 162, 162);
		m_UltRender.SetPosition(Ult_Pos);
		m_UltRender.SetScale(1.2, 1.2);

		m_Lv.Update();
		m_LvNumber.Update();
		m_Flame.Update();
		m_ExperienceFlame.Update();
		m_ExperienceBar_flont.Update();
		m_SkillRender.Update();
		m_UltRender.Update();
	}

	//HP関連
	{
		//HPのフォント
		m_HpFont.SetPosition(-540.0f, -465.0f, 0.0f);
		m_HpFont.SetScale(1.0f);
		m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_HpFont.SetRotation(0.0f);
		m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//HPゲージ裏の画像を読み込む
		m_statusBar.Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", 600.0f, 120.0f);
		m_statusBar.SetPosition(HP_BAR_POS);
		m_statusBar.SetScale(1.0, 1.0, 1.0);

		//HPゲージの表の画像を読み込む
		m_hpBar.Init("Assets/sprite/gameUI/HPBar_HP.DDS", 580.0f, 90.0f);
		//ピボットを設定する
		m_hpBar.SetPivot(HPGAUGE_PIVOT);
		m_hpBar.SetPosition(HP_BAR_FLONT_POS);

		//HPゲージのフレームの画像を読み込む
		m_HPFrame.Init("Assets/sprite/gameUI/HPBar_flame.DDS", 600.0f, 120.0f);
		m_HPFrame.SetPosition(HP_BAR_POS);
		m_HPFrame.SetScale(1.0, 1.0, 1.0);

		//更新処理
		m_statusBar.Update();
		m_HPFrame.Update();
		m_hpBar.Update();
	}

	//制限時間と獲得ポイント
	{
		m_TimeAndPointRender.Init("Assets/sprite/gameUI/timer.DDS", 1100.0f, 400.0f);
		m_TimeAndPointRender.SetPosition(TIME_POS);
		m_TimeAndPointRender.SetScale(0.35, 0.3, 0.3);

		//フォントの設定。
		m_time_left.SetPosition(TIME_FONT_POS);
		//フォントの大きさを設定。
		m_time_left.SetScale(1.6f);
		//フォントの色を設定。
		m_time_left.SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_time_left.SetShadowParam(true, 2.0f, g_vec4Black);

		m_TimeAndPointRender.Update();
	}
	



	
	//m_hp = PLAYER_MAXHP;
	
	
	
	

	m_GameUIState = m_GameState;

	return true;
}

void GameUI::Update()
{
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState== m_PauseState) {
		return;
	}

	//レベルの表示
	//int LEVEL=m_knightplayer->SetLevel();
	int LEVEL = player->CharSetLevel();
	wchar_t Lv[255];
	swprintf_s(Lv, 255, L"%d", LEVEL);
	m_LevelFont.SetText(Lv);

	//ポイントの表示
	int POINT = player->CharSetPoint();
	wchar_t P[255];
	swprintf_s(P, 255, L"%dポイント", POINT);
	m_PointFont.SetText(P);
	
	//時間切れではないなら
	if (GameEndFlag == false) {
		Timer();
	}
	//制限時間の表示
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d:%02d", int(MinutesTimer),int(SecondsTimer));
	//表示するテキストを設定。
	m_time_left.SetText(wcsbuf);
	
	//経験値の表示
	Vector3 EXPScale = Vector3::One;
	//HPバーの減っていく割合。
	EXPScale.x = (float)player->CharSetEXP() / (float)player->CharSetEXPTable();
	m_ExperienceBar_flont.SetScale(EXPScale);
	m_ExperienceBar_flont.Update();

	HPBar();
	
	//回避バー
	
}

void GameUI::HPBar()
{
	//int HP = m_knightplayer->SetHp();
	int HP = player->CharSetHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d", HP);
	m_HpFont.SetText(hp);

	Vector3 HpScale = Vector3::One;
	//HPバーの減っていく割合。
	HpScale.x = (float)player->CharSetHp() / (float)player->CharSetMaxHp();
	m_hpBar.SetScale(HpScale);

	m_hpBar.Update();
}

void GameUI::Timer()
{
	//0分でないなら

		//0秒以下なら
		if (SecondsTimer <= 0) {
			//1分減らす
			MinutesTimer--;
			//もし0分なら、秒も0にする
			if (MinutesTimer < 0) {
				SecondsTimer = 0.0f;
				MinutesTimer = 0.0f;
				GameEndFlag = true;
			}
			//60秒に戻す
			else SecondsTimer = 60.0f;
		}
		else SecondsTimer -= g_gameTime->GetFrameDeltaTime();
	
}

void GameUI::Render(RenderContext& rc)
{
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState != m_PauseState) {
		//レベルや経験値のフレーム
		m_Flame.Draw(rc);
		//経験値
		m_ExperienceFlame.Draw(rc);
		m_ExperienceBar_flont.Draw(rc);

	//m_LevelNameFont.Draw(rc);
		m_HpNameFont.Draw(rc);

		m_TimeAndPointRender.Draw(rc);

		m_time_left.Draw(rc);

		/*m_AtkFont.Draw(rc);
		m_SpeedFont.Draw(rc);*/
		m_statusBar.Draw(rc);
		m_hpBar.Draw(rc);
		//m_playerFaceBack.Draw(rc);
		m_HPFrame.Draw(rc);
		m_SkillRender.Draw(rc);
		m_UltRender.Draw(rc);

		m_HpFont.Draw(rc);
		m_Lv.Draw(rc);
		m_LvNumber.Draw(rc);
		m_LevelFont.Draw(rc);

		m_PointFont.Draw(rc);
	}
	
}
