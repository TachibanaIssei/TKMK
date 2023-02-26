#include "stdafx.h"
#include "Game.h"

#include "BackGround.h"

Game::Game()
{
	
}

Game::~Game()
{
	if (m_backGround != nullptr)
	{
		DeleteGO(m_backGround);
	}
}

bool Game::Start()
{
	//プレイヤー
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_modelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_modelRender.SetScale(Vector3(1.0f, 1.0f, 1.0f));

	m_rotation.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rotation);


	//レベル
	m_level3DRender.Init("Assets/level3D/testLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.EqualObjectName(L"testBackGround") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		return false;
	});

	m_spriteRender.Init("Assets/sprite/magicball.DDS", 256.0f, 256.0f);
	m_spriteRender.SetPosition(100.0f, 100.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();


	m_fontRender.SetText(L"hello");
	m_fontRender.SetPosition(-500.0f, 200.0f);
	m_fontRender.SetScale(3.0f);
	m_fontRender.SetRotation(Math::DegToRad(90.0f));
	m_fontRender.SetShadowParam(true, 2.0f, g_vec4Black);


	return true;
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

	m_spriteAlpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0, fabsf(sinf(m_spriteAlpha))));

	m_modelRender.Update();
	m_spriteRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_spriteRender.Draw(rc);
	m_fontRender.Draw(rc);
}
