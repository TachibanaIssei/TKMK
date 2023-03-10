#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"

#include "KnightPlayer.h"
namespace
{
	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//ゲージのピボット
	const Vector2 HPGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//HPゲージのピボット
	const Vector3 STATUS_BAR_POS = Vector3(-450.0f, -500.0f, 0.0f);	//ステータスバーポジション
	const Vector3 TIME_AND_POINT = Vector3(-580.0,-390.0f, 0.0f);	//プレイヤーの顔の枠のポジション
	const Vector3 HP_BAR_POS = Vector3(-500.0f, -480.0f, 0.0f);	//HPバーポジション

	const float HP_BAR_WIDTH = 1400.0f;     //HPバーの長さ
	const float HP_BAR_HIGHT = 200.0f;      //HPバーの高さ
	const Vector3 HP_BAR_SIZE = Vector3(HP_BAR_WIDTH, HP_BAR_HIGHT, 0.0f);  //HPバーのサイズ

	const Vector3 Skill_Pos = Vector3(830.0f, -430.0f, 0.0f);   //スキルアイコンポジション
	const Vector3 Ult_Pos = Vector3(830.0f, -240.0f, 0.0f);     //必殺技アイコンポジション

	const Vector3 LvNumberPos = Vector3(-850.0f, -440.0f, 0.0f);
	const Vector3 LvPos = Vector3(-920.0f, -480.0f, 0.0f);

	const Vector3 FlamePos = Vector3(-640.0f,-490.0f,0.0f);
}
GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	//m_knightplayer->SetGameUI(this);
	m_knightplayer = FindGO<KnightPlayer>("m_knightplayer");

	//m_game = FindGO<Game>("m_game");

	//Level
	/*m_LevelFont.SetPosition(-850.0f, -460.0f, 0.0f);
	m_LevelFont.SetScale(2.0f);
	m_LevelFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LevelFont.SetRotation(0.0f);
	m_LevelFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_LevelNameFont.SetText(L"Lv");
	m_LevelNameFont.SetPosition(-950.0f, -460.0f,0.0f);
	m_LevelNameFont.SetScale(2.0f);
	m_LevelNameFont.SetRotation(0.0f);
	m_LevelNameFont.SetShadowParam(true, 2.0f, g_vec4Black);*/

	//HP
	m_HpFont.SetPosition(-540.0f, -465.0f, 0.0f);
	m_HpFont.SetScale(1.0f);
	m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_HpFont.SetRotation(0.0f);
	m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

	/*m_HpNameFont.SetText(L"HP");
	m_HpNameFont.SetPosition(-900.0f, -280.0f, 0.0f);
	m_HpNameFont.SetScale(2.0f);
	m_HpNameFont.SetRotation(0.0f);
	m_HpNameFont.SetShadowParam(true, 2.0f, g_vec4Black);*/

	//Lvの画像を読み込む
	m_Lv.Init("Assets/sprite/Lv.DDS", 200.0f, 150.0f);
	m_Lv.SetPosition(LvPos);
	m_Lv.SetScale(0.3, 0.3, 1.0);

	//Lv1の画像を読み込む
	m_LvNumber.Init("Assets/sprite/Lv1.DDS", 320.0f, 150.0f);
	m_LvNumber.SetPosition(LvNumberPos);
	m_LvNumber.SetScale(0.4, 1.0, 1.0);

	//HPゲージ裏の画像を読み込む
	m_statusBar.Init("Assets/sprite/HP_bar_ura.DDS", 600.0f, 85.0f);
	m_statusBar.SetPosition(HP_BAR_POS);
	m_statusBar.SetScale(1.05,1.2,1.0);

	//HPゲージのフレームの画像を読み込む
	m_playerFaceFrame.Init("Assets/sprite/HP_flame.DDS", 600.0f, 85.0f);
	m_playerFaceFrame.SetPosition(HP_BAR_POS);
	m_playerFaceFrame.SetScale(1.05, 1.2, 1.0);

	//
	m_Flame.Init("Assets/sprite/Flame.DDS", 700.0f, 400.0f);
	m_Flame.SetPosition(FlamePos);
	m_Flame.SetScale(1.5, 1.0, 1.0);
	m_Flame.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.8f));

	////制限時間と獲得ポイント
	m_TimeAndPointRender.Init("Assets/sprite/time_point.DDS", 500.0f, 140.0f);
	m_TimeAndPointRender.SetPosition(TIME_AND_POINT);
	m_TimeAndPointRender.SetScale(0.8,0.9,1.0);

	//スキルのアイコン
	m_SkillRender.Init("Assets/sprite/skill_flame.DDS", 162, 162);
	m_SkillRender.SetPosition(Skill_Pos);
	m_SkillRender.SetScale(1.3, 1.3);
	//必殺技のアイコン
	m_UltRender.Init("Assets/sprite/ult_flame.DDs", 162, 162);
	m_UltRender.SetPosition(Ult_Pos);
	m_UltRender.SetScale(1.3, 1.3);

	//HPゲージ表の画像読み込み
	m_hpBar.Init("Assets/sprite/HP_bar.DDS", 557.0f, 58.0f);
	//ピボットを設定する
	m_hpBar.SetPivot(HPGAUGE_PIVOT);
	m_hpBar.SetPosition({ -777.0f, -481.0f, 0.0f });
	//m_hpBar.SetScale(0.4f, 0.4f, 1.0f);
	//更新処理
	m_statusBar.Update();
	m_hpBar.Update();
	//m_hp = PLAYER_MAXHP;
	m_playerFaceFrame.Update();
	m_TimeAndPointRender.Update();
	m_SkillRender.Update();
	m_UltRender.Update();
	m_Lv.Update();
	m_LvNumber.Update();
	m_Flame.Update();

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
	//wchar_t Lv[255];
	//swprintf_s(Lv, 255, L"%d", LEVEL);
	//m_LevelFont.SetText(Lv);

	//0分でないなら
	if (MinutesTimer != 0) {
		SecondsTimer -= g_gameTime->GetFrameDeltaTime();
		//0秒以下なら
		if (SecondsTimer <= 0) {
			//1分減らす
			MinutesTimer--;
			//60秒に戻す
			SecondsTimer = 60.0f;
		}
	}
	//制限時間の表示
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d:%d", int(MinutesTimer),int(SecondsTimer));
	
	//表示するテキストを設定。
	m_time_left.SetText(wcsbuf);
	//フォントの設定。
	m_time_left.SetPosition(Vector3(-755.0f, -361.0f, 0.0f));
	//フォントの大きさを設定。
	m_time_left.SetScale(1.5f);
	//フォントの色を設定。
	m_time_left.SetColor({ 1.0f,1.0f,1.0f,1.0f });
	m_time_left.SetShadowParam(true, 2.0f, g_vec4Black);


	HPBar();
	
	//回避バー
	
}

void GameUI::HPBar()
{
	int HP = m_knightplayer->SetHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d", HP);
	m_HpFont.SetText(hp);

	Vector3 HpScale = Vector3::One;
	//HPバーの減っていく割合。
	HpScale.x = (float)m_knightplayer->SetHp() / (float)m_knightplayer->SetMaxHp();
	m_hpBar.SetScale(HpScale);

	m_hpBar.Update();
}
void GameUI::Render(RenderContext& rc)
{
	//m_Flame.Draw(rc);
	/*m_LevelFont.Draw(rc);
	m_LevelNameFont.Draw(rc);*/
	m_HpNameFont.Draw(rc);

	m_TimeAndPointRender.Draw(rc);

	m_time_left.Draw(rc);

	/*m_AtkFont.Draw(rc);
	m_SpeedFont.Draw(rc);*/
	m_statusBar.Draw(rc);
	m_hpBar.Draw(rc);
	//m_playerFaceBack.Draw(rc);
	m_playerFaceFrame.Draw(rc);
	m_SkillRender.Draw(rc);
	m_UltRender.Draw(rc);

	m_HpFont.Draw(rc);
	m_Lv.Draw(rc);
	m_LvNumber.Draw(rc);
}
