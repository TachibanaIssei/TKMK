#include "stdafx.h"
#include "Game.h"

#include "BackGround.h"
#include "Rezult.h"
#include "GameCamera.h"
#include "KnightBase.h"
#include "Actor.h"

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
	DeleteGO(m_knightbase);
}

bool Game::Start()
{
	//ディレクションライトの設定
	Vector3 directionLightDir = Vector3{ 1.0f,-1.0f,-1.0f };
	Vector3 directionLightColor = Vector3{ 0.5f,0.5f,0.5f };
	g_renderingEngine->SetDirectionLight(directionLightDir,directionLightColor);

	//レベル
	m_level3DRender.Init("Assets/level3D/stadiumLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.EqualObjectName(L"stadium02") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		return false;
	});

	m_respawnLevel.Init("Assets/level3D/respawnLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Knight") == true) {
			if (objData.number == 0){
				Actor* m_actor;
				m_actor->SetRespawnPos(objData.position,objData.number);
				return true;
			}
			if (objData.number == 1) {
				Actor* m_actor;
				m_actor->SetRespawnPos(objData.position, objData.number);
				return true;
			}
			if (objData.number == 2) {
				Actor* m_actor;
				m_actor->SetRespawnPos(objData.position, objData.number);
				return true;
			}
			if (objData.number == 3) {
				Actor* m_actor;
				m_actor->SetRespawnPos(objData.position, objData.number);
				return true;
			}
		}
		return true;
	});

	//剣士の作成
	m_knightbase = NewGO<KnightBase>(0, "knightbase");
	m_knightbase->SetSGame(this);
	//ゲームカメラの生成
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	m_gamecamera->SetKnight(m_knightbase);

	

	m_spriteRender.Init("Assets/sprite/magicball.DDS", 256.0f, 256.0f);
	m_spriteRender.SetPosition(100.0f, 100.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	m_fontRender.SetText(L"hello");
	m_fontRender.SetPosition(-500.0f, 200.0f);
	m_fontRender.SetScale(3.0f);
	m_fontRender.SetRotation(90.0f);
	m_fontRender.SetShadowParam(true, 2.0f, g_vec4Black);


	//当たり判定を有効化する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		Rezult* rezult = NewGO<Rezult>(0, "rezult");
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
