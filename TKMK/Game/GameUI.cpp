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
