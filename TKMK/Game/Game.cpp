#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Result.h"
#include "Tittle.h"
#include "GameCamera.h"
#include "KnightBase.h"
#include "Actor.h"
#include "GameUI.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Map.h"
#include "KnightAI.h"

Game::Game()
{
	//sound設定
	//1-5 Title/Game中BGM
	//6-10 選択音
	//11-20 playerのスキルなどの音
	//21-30 enemyの音
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
	DeleteGO(m_Map);
	DeleteGO(m_KnightAI);
	DeleteGO(m_bgm);
}

bool Game::Start()
{
	g_renderingEngine->UnUseHemiLight();

	//�f�B���N�V�������C�g�̐ݒ�
	Vector3 directionLightDir = Vector3{ 1.0f,-1.0f,-1.0f };
	directionLightDir.Normalize();
	Vector4 directionLightColor = Vector4{ 1.0f,1.0f,1.0f, 1.0f };
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor);
	g_renderingEngine->SetAmbient({ 0.4f,0.4f,0.4f,1.0f });

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

	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "m_gameUI");
	m_gameUI->SetSGame(this);

	//���m�̐���
	m_knightplayer = NewGO<KnightPlayer>(0, "m_knightplayer");
	m_knightplayer->SetSGame(this);
	m_knightplayer->SetGameUI(m_gameUI);
	

	//�Q�[���J�����̐���
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	m_gamecamera->SetKnight(m_knightplayer);

	//�����̓G�̐���
	m_Neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
	m_Neutral_Enemy->SetNeutral_EnemyGame(this);

	//m_KnightAI = NewGO<KnightAI>(0, "KnightAI");
	//m_KnightAI->SetGame(this);
	//GameUIの生成
	m_Map = NewGO<Map>(2, "map");

	/*m_fontRender.SetText(L"hello");
	m_fontRender.SetPosition(-500.0f, 200.0f);
	m_fontRender.SetScale(3.0f);
	m_fontRender.SetRotation(90.0f);
	m_fontRender.SetShadowParam(true, 2.0f, g_vec4Black);*/
	
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

	m_GameState = enGameState_Battle;

	//BGMの設定
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/gameBGM/SentouBGM1.wav");
	
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(2);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);



	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	if (m_GameState == enGameState_Battle) {
		//リザルト画面への遷移
	    //Yボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_GameState = enGameState_Rezult;
		}
	}
	

	//ポーズ画面への遷移
	//スタートボタンが押されたら。
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		//ゲーム画面からポーズ画面に遷移する時の処理
		if (m_GameState == enGameState_Battle) {
			m_GameState = enGameState_Pause;
			//プレイヤーのステートをポーズ画面用のステートに変更
			m_knightplayer->SetPlayerState(m_knightplayer->enKnightState_Pause);
			//UIのステートをポーズ画面用のステートに変更
			m_gameUI->SetGameUIState(m_gameUI->m_PauseState);
		}
			
		//ポーズ画面からゲーム画面に戻る時の処理
		else if (m_GameState == enGameState_Pause) {
			m_GameState = enGameState_Battle;
			//プレイヤーのステートをポーズ画面用のステートではないようにする
			m_knightplayer->SetPlayerState(m_knightplayer->enKnightState_Idle);
			//UIのステートをゲームのステートに変更
			m_gameUI->SetGameUIState(m_gameUI->m_GameState);
		}
	}

	GameState();
	
	//m_spriteAlpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	//m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0, fabsf(sinf(m_spriteAlpha))));

	m_modelRender.Update();
	//m_Pause_Back.Update();
}

//ポーズ画面の処理
void Game::Pause()
{
	//タイトル画面への遷移
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle*m_tittle = NewGO<Tittle>(0,"m_tittle");
		DeleteGO(this);
	}
}

//ゲームステートの管理
void Game::GameState()
{
	switch (m_GameState)
	{
	case enGameState_Start:

		break;

	case enGameState_Battle:

		break;

	case enGameState_Pause:
		Pause();
		break;

	case enGamestate_End:

		break;

	case enGameState_Rezult:
		//リザルト画面の生成、ゲームの削除
		Result* result = NewGO<Result>(0, "Result");
		DeleteGO(this);
		break;

	/*default:
		break;*/
	}
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	if (m_GameState == enGameState_Pause)
	{
		m_Pause_Back.Draw(rc);
		m_Pause_Front.Draw(rc);
	}
	//m_fontRender.Draw(rc);
	
}
