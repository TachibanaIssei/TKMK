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
	DeleteGO(m_Neutral_Enemy);
	DeleteGO(m_gameUI);
}

bool Game::Start()
{
	//
	g_renderingEngine->UnUseHemiLight();

	//�f�B���N�V�������C�g�̐ݒ�
	Vector3 directionLightDir = Vector3{ 1.0f,-1.0f,-1.0f };
	directionLightDir.Normalize();
	Vector3 directionLightColor = Vector3{ 1.0f,1.0f,1.0f };
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor);
	g_renderingEngine->SetAmbient({ 0.4f,0.4f,0.4f });

	//�X�^�W�A���̃��x���̐ݒ�
	m_level3DRender.Init("Assets/level3D/stadiumLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.EqualObjectName(L"stadium04") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}
		return false;
	});
	//���m�̐���
	/*m_knightbase = NewGO<KnightBase>(0, "knightbase");
	m_knightbase->SetSGame(this);*/
	m_knightplayer = NewGO<KnightPlayer>(0, "m_knightplayer");
	m_knightplayer->SetSGame(this);
	

	//�Q�[���J�����̐���
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	m_gamecamera->SetKnight(m_knightplayer);

	//�����̓G�̐���
	m_Neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
	m_Neutral_Enemy->SetNeutral_EnemyGame(this);

	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	
	//ポーズ画面の背景の設定
	m_Pause_Back.Init("Assets/sprite/pause_back.DDS", 1920.0f, 1080.0f);
	m_Pause_Back.SetPosition(g_vec3Zero);
	m_Pause_Back.SetScale(1.0f, 1.0f, 1.0f);
	m_Pause_Back.SetRotation(m_sRotation);
	m_Pause_Back.SetMulColor(Vector4(1.0f,1.0f,1.0f,0.5f));
	m_Pause_Back.Update();

	m_Pause_Front.Init("Assets/sprite/pause.DDS", 1920.0f, 1080.0f);
	m_Pause_Front.SetPosition(g_vec3Zero);
	m_Pause_Front.SetScale(1.0f, 1.0f, 1.0f);
	m_Pause_Front.SetRotation(m_sRotation);
	m_Pause_Front.Update();

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
	//リザルト画面への遷移
	//Yボタンが押されたら。
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		Result* result =NewGO<Result>(0, "Result");
		DeleteGO(this);
	}
	//ポーズ画面への遷移
	//スタートボタンが押されたら。
	if (g_pad[0]->IsTrigger(enButtonStart)) {

		bool temp;
		temp = PauseOpenFlag;
		PauseOpenFlag = PauseCloseFlag;
		PauseCloseFlag = temp;
	}


	//if(PauseFlag==true)TestPlayer();

	//m_spriteAlpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	//m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0, fabsf(sinf(m_spriteAlpha))));

	m_modelRender.Update();
	//m_Pause_Back.Update();
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

	if (PauseOpenFlag == true)
	{
		m_Pause_Back.Draw(rc);
		m_Pause_Front.Draw(rc);
	}
	//m_fontRender.Draw(rc);
	
}
