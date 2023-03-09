#include "stdafx.h"
#include "GameUI.h"

#include "KnightPlayer.h"
namespace
{
	const Vector2 GAUGE_PIVOT = Vector2(0.0f, 0.5f);				//ゲージのピボット
	const Vector3 STATUS_BAR_POS = Vector3(-393.0f, -407.0f, 0.0f);	//ステータスバーポジション
	const Vector3 PLAYER_FACE_BAR_POS = Vector3(-580.0, -418.0f, 0.0f);	//プレイヤーの顔の枠のポジション
	const Vector3 HP_BAR_POS = Vector3(-726.0f, -375.0f, 0.0f);	//HPバーポジション
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
	m_HpFont.SetPosition(-800.0f, -280.0f, 0.0f);
	m_HpFont.SetScale(2.0f);
	m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_HpFont.SetRotation(0.0f);
	m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_HpNameFont.SetText(L"HP");
	m_HpNameFont.SetPosition(-900.0f, -280.0f, 0.0f);
	m_HpNameFont.SetScale(2.0f);
	m_HpNameFont.SetRotation(0.0f);
	m_HpNameFont.SetShadowParam(true, 2.0f, g_vec4Black);

	//HPゲージの画像を読み込む
	m_statusBar.Init("Assets/sprite/HP_gauge.dds", 744, 244);
	m_statusBar.SetPosition(STATUS_BAR_POS);
	m_statusBar.SetScale(g_vec3One);

	m_playerFaceFrame.Init("Assets/sprite/HP_window.dds", 744, 244);
	m_playerFaceFrame.SetPosition(PLAYER_FACE_BAR_POS);
	m_playerFaceFrame.SetScale(g_vec3One);

	m_playerFaceBack.Init("Assets/sprite/HP_windowBack.dds", 744, 244);
	m_playerFaceBack.SetPosition(PLAYER_FACE_BAR_POS);
	m_playerFaceBack.SetScale(g_vec3One);

	//HPゲージの画像読み込み
	m_hpBar.Init("Assets/sprite/uf_bar_b_health.dds", 565, 58.8);
	//ピボットを設定する
	m_hpBar.SetPivot(GAUGE_PIVOT);
	m_hpBar.SetPosition(HP_BAR_POS);
	//更新処理
	m_statusBar.Update();
	m_hpBar.Update();
	//m_hp = PLAYER_MAXHP;
	m_mpBar.Update();
	m_playerFaceFrame.Update();
	m_playerFaceBack.Update();

	return true;
}

void GameUI::Update()
{
	int LEVEL=m_knightplayer->SetLevel();
	wchar_t Lv[255];
	swprintf_s(Lv, 255, L"%d", LEVEL);
	m_LevelFont.SetText(Lv);

	int HP = m_knightplayer->SetHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d", HP);
	m_HpFont.SetText(hp);
}

void GameUI::Render(RenderContext& rc)
{
	m_LevelFont.Draw(rc);
	m_LevelNameFont.Draw(rc);
	m_HpFont.Draw(rc);
	m_HpNameFont.Draw(rc);
	/*m_AtkFont.Draw(rc);
	m_SpeedFont.Draw(rc);*/
}
