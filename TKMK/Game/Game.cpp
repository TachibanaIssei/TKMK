#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Result.h"
#include "GameCamera.h"
#include "KnightBase.h"
#include "Actor.h"
#include "GameUI.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"

Game::Game()
{
	
}

Game::~Game()
{
	if (m_backGround != nullptr)
	{
		DeleteGO(m_backGround);
	}

	DeleteGO(m_gamecamera);
	DeleteGO(m_knightplayer);
	DeleteGO(m_NE);
	DeleteGO(m_gameUI);
}

bool Game::Start()
{
	//ディレクションライトの設定
	Vector3 directionLightDir = Vector3{ 1.0f,-1.0f,-1.0f };
	directionLightDir.Normalize();
	Vector3 directionLightColor = Vector3{ 1.0f,1.0f,1.0f };
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor);
	g_renderingEngine->SetAmbient({ 0.4f,0.4f,0.4f });

	//スタジアムのレベルの設定
	m_level3DRender.Init("Assets/level3D/stadiumLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.EqualObjectName(L"stadium03") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		return false;
	});
	//剣士の生成
	/*m_knightbase = NewGO<KnightBase>(0, "knightbase");
	m_knightbase->SetSGame(this);*/
	m_knightplayer = NewGO<KnightPlayer>(0, "m_knightplayer");
	m_knightplayer->SetSGame(this);
	

	//ゲームカメラの生成
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	m_gamecamera->SetKnight(m_knightplayer);

	//中立の敵の生成
	m_NE = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
	m_NE->SetNEGame(this);

	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	
	m_spriteRender.Init("Assets/sprite/magicball.DDS", 256.0f, 256.0f);
	m_spriteRender.SetPosition(100.0f, 100.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	/*m_fontRender.SetText(L"hello");
	m_fontRender.SetPosition(-500.0f, 200.0f);
	m_fontRender.SetScale(3.0f);
	m_fontRender.SetRotation(90.0f);
	m_fontRender.SetShadowParam(true, 2.0f, g_vec4Black);*/


	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		Result* result =NewGO<Result>(0, "Result");
		DeleteGO(this);
	}


	TestPlayer();

	m_spriteAlpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0, fabsf(sinf(m_spriteAlpha))));

	m_modelRender.Update();
	m_spriteRender.Update();
}

void Game::TestPlayer()
{
	
}

void Game::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//m_spriteRender.Draw(rc);
	//m_fontRender.Draw(rc);
}
