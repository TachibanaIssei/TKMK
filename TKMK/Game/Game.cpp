#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_modelRender.SetPosition(Vector3(100.0f, 0.0f, 0.0f));
}

Game::~Game()
{
}

void Game::Update()
{
	m_modelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
