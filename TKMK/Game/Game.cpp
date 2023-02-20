#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_modelRender.SetPosition(Vector3(100.0f, 0.0f, 0.0f));
	m_modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));

	m_rotation.SetRotationY(50.0f);
	m_modelRender.SetRotation(m_rotation);
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
