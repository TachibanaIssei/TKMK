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
	/*int LEVEL=m_knightbase->*/


}

void GameUI::Render(RenderContext& rc)
{
	m_LevelFont.Draw(rc);
	m_HpFont.Draw(rc);
	m_AtkFont.Draw(rc);
	m_SpeedFont.Draw(rc);
}
