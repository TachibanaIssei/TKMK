#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	m_modelRender.Init("Assets/modelData/unityChan.tkm",m_animationClips,enAnimationClip_Num,enModelUpAxisY);
	m_modelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));

	m_rotation.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rotation);
}

Game::~Game()
{
}

void Game::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
	}
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
	}

	m_modelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
