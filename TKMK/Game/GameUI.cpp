#include "stdafx.h"
#include "GameUI.h"

#include "KnightBase.h"

GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_knightbase = FindGO<KnightBase>("m_knightbase");

	m_LevelFont.SetPosition(400.0f, 500.0f, 0.0f);
	m_LevelFont.SetScale(6.0f);
	m_LevelFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_LevelFont.SetRotation(0.0f);

	return true;
}

void GameUI::Update()
{
	m_knightbase->SetLevel(LEVEL);

	wchar_t Lv[255];
	swprintf_s(Lv, 255, L"%d", LEVEL);
	m_LevelFont.SetText(Lv);
}

void GameUI::Render(RenderContext& rc)
{
	m_LevelFont.Draw(rc);
	m_HpFont.Draw(rc);
	m_AtkFont.Draw(rc);
	m_SpeedFont.Draw(rc);
}
