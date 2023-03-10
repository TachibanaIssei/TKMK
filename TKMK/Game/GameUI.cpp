#include "stdafx.h"
#include "GameUI.h"

#include "KnightPlayer.h"
namespace
{
	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//ゲージのピボット
	const Vector2 HPGAUGE_PIVOT = Vector2(0.5f, 0.5f);				//HPゲージのピボット
	const Vector3 STATUS_BAR_POS = Vector3(-400.0f, -450.0f, 0.0f);	//ステータスバーポジション
	const Vector3 PLAYER_FACE_BAR_POS = Vector3(-557.0,-400.0f, 0.0f);	//プレイヤーの顔の枠のポジション
	const Vector3 HP_BAR_POS = Vector3(-450.0f, -420.0f, 0.0f);	//HPバーポジション
	const Vector3 Skill_Pos = Vector3(700.0f, -225.0f, 0.0f);   //スキルアイコンのポジション
	const Vector3 Ult_Pos = Vector3(700.0f, -370.0f, 0.0f);     //必殺技のアイコンのポジション

}
GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_knightplayer = FindGO<KnightPlayer>("m_knightplayer");

	//Level
	m_LevelFont.SetPosition(-800.0f, -200.0f, 0.0f);
	m_LevelFont.SetScale(2.0f);
	m_LevelFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LevelFont.SetRotation(0.0f);
	m_LevelFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_LevelNameFont.SetText(L"Lv");
	m_LevelNameFont.SetPosition(-900.0f, -200.0f,0.0f);
	m_LevelNameFont.SetScale(2.0f);
	m_LevelNameFont.SetRotation(0.0f);
	m_LevelNameFont.SetShadowParam(true, 2.0f, g_vec4Black);

	//HP
	/*m_HpFont.SetPosition(-800.0f, -280.0f, 0.0f);
	m_HpFont.SetScale(2.0f);
	m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_HpFont.SetRotation(0.0f);
	m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_HpNameFont.SetText(L"HP");
	m_HpNameFont.SetPosition(-900.0f, -280.0f, 0.0f);
	m_HpNameFont.SetScale(2.0f);
	m_HpNameFont.SetRotation(0.0f);
	m_HpNameFont.SetShadowParam(true, 2.0f, g_vec4Black);*/

	//HPゲージの画像を読み込む
	m_statusBar.Init("Assets/sprite/HP_gauge.DDS", 631, 207);
	m_statusBar.SetPosition(STATUS_BAR_POS);
	m_statusBar.SetScale(0.87,0.87,1.0);
	//顔アイコンの外枠
	m_playerFaceFrame.Init("Assets/sprite/HP_window.DDS", 602, 197);
	m_playerFaceFrame.SetPosition(PLAYER_FACE_BAR_POS);
	m_playerFaceFrame.SetScale(0.8,0.8,1.0);
	//顔のアイコン
	m_playerFaceBack.Init("Assets/sprite/HP_windowBack.DDS", 615, 201);
	m_playerFaceBack.SetPosition(PLAYER_FACE_BAR_POS);
	m_playerFaceBack.SetScale(0.82,0.82,1.0);
	//スキルアイコン
	m_SkillRender.Init("Assets/sprite/skill_flame.DDS", 162, 162);
	m_SkillRender.SetPosition(Skill_Pos);
	m_SkillRender.SetScale(1, 1);
    //必殺技アイコン
	m_UltRender.Init("Assets/sprite/ult_flame.DDs", 162, 162);
	m_UltRender.SetPosition(Ult_Pos);
	m_UltRender.SetScale(1, 1);
	//HPゲージの画像読み込み
	m_hpBar.Init("Assets/sprite/uf_bar_b_health.DDS", 492.0f, 40.0f);
	//ピボットを設定する
	m_hpBar.SetPivot(HPGAUGE_PIVOT);
	m_hpBar.SetPosition(HP_BAR_POS);
	//更新処理
	m_statusBar.Update();
	m_hpBar.Update();
	//m_hp = PLAYER_MAXHP;
	m_playerFaceFrame.Update();
	m_playerFaceBack.Update();
	m_SkillRender.Update();
	m_UltRender.Update();

    return true;
}

void GameUI::Update()
{
	int LEVEL=m_knightplayer->SetLevel();
	wchar_t Lv[255];
	swprintf_s(Lv, 255, L"%d", LEVEL);
	m_LevelFont.SetText(Lv);


	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"残り時間%d", int(m_timer));
	m_timer-= g_gameTime->GetFrameDeltaTime();
	//表示するテキストを設定。
	m_time_left.SetText(wcsbuf);
	//フォントの設定。
	m_time_left.SetPosition(Vector3(200.0f, 200.0f, 0.0f));
	//フォントの大きさを設定。
	m_time_left.SetScale(2.0f);
	//フォントの色を設定。
	m_time_left.SetColor({ 1.0f,0.0f,0.0f,1.0f });
}

void GameUI::HPBar()
{
	
}
void GameUI::Render(RenderContext& rc)
{
	m_LevelFont.Draw(rc);
	m_LevelNameFont.Draw(rc);
	m_HpNameFont.Draw(rc);
	m_time_left.Draw(rc);

	/*m_AtkFont.Draw(rc);
	m_SpeedFont.Draw(rc);*/
	m_statusBar.Draw(rc);
	m_hpBar.Draw(rc);
	m_playerFaceBack.Draw(rc);
	m_playerFaceFrame.Draw(rc);
	m_SkillRender.Draw(rc);
	m_UltRender.Draw(rc);

}
