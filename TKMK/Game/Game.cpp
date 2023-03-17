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
//#include <vector>
//#include <algorithm>

namespace {
	int ENEMY_AMOUNT = 5;
}

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

	//Neutral_Enemyを配列で消す
	auto seutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	//配列のサイズを調べてfor文で回す
	for (auto seutral_Enemy : seutral_Enemys)
	{
		DeleteGO(seutral_Enemy);
	}

	DeleteGO(m_knightplayer);
	
	DeleteGO(m_gameUI);
	DeleteGO(m_Map);
	DeleteGO(m_KnightAI);
	DeleteGO(m_bgm);
}

bool Game::Start()
{
	g_renderingEngine->UnUseHemiLight();

	//�P�B���N�V�������C�g�̐ݒ�
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

	m_Enemylevel.Init("Assets/level3D/enemy_respawnLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Neutral_Enemy") == true) {
			//左上の座標
			if (objData.number == 0) {
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy*neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				neutral_Enemy->SetKnightPlayer(m_knightplayer);
				//RespawnNumberBox[enemyNumber]=true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy0->SetScale(objData.scale);
				return true;
			}
			//右上の座標
			if (objData.number == 1) {
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				neutral_Enemy->SetKnightPlayer(m_knightplayer);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy1->SetScale(objData.scale);
				return true;
			}
			//右下の座標
			if (objData.number == 2) {
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				neutral_Enemy->SetKnightPlayer(m_knightplayer);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy2->SetScale(objData.scale);
				return true;
			}
			//左下の座標
			if (objData.number == 3) {
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				neutral_Enemy->SetKnightPlayer(m_knightplayer);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy3->SetScale(objData.scale);
				return true;
			}
			if (objData.number == 4) {
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				neutral_Enemy->SetKnightPlayer(m_knightplayer);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy4->SetScale(objData.scale);
				return true;
			}
		}
		return true;
	});

	/*m_Neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
	m_Neutral_Enemy->SetNeutral_EnemyGame(this);
	m_Neutral_Enemy->SetKnightPlayer(m_knightplayer);*/

	//m_KnightAI = NewGO<KnightAI>(0, "KnightAI");
	//m_KnightAI->SetGame(this);
	
	//GameUIの生成
	m_Map = NewGO<Map>(2, "map");

	
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
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

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
			//中立の敵をポーズ画面用のステートに変更
			auto seutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
			//配列のサイズを調べてfor文で回す
			for (auto seutral_Enemy : seutral_Enemys)
			{
				seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Pause);
			}

			
		}
			
		//ポーズ画面からゲーム画面に戻る時の処理
		else if (m_GameState == enGameState_Pause) {
			m_GameState = enGameState_Battle;
			//プレイヤーのステートをポーズ画面用のステートではないようにする
			m_knightplayer->SetPlayerState(m_knightplayer->enKnightState_Idle);
			//UIのステートをゲームのステートに変更
			m_gameUI->SetGameUIState(m_gameUI->m_GameState);
			//中立の敵をポーズ画面用のステートに変更
			auto seutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
			//配列のサイズを調べてfor文で回す
			for (auto seutral_Enemy : seutral_Enemys)
			{
				seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Idle);
			}
		}
	}

	GameState();
	
	m_Timer += g_gameTime->GetFrameDeltaTime();
	if (m_Timer>=15) {
		Respawn();
		m_Timer = 0.0f;
	}
	
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

void Game::Respawn()
{
	if (ENEMY_AMOUNT != enemyNumber) {
		int spawnAmount = ENEMY_AMOUNT - enemyNumber;
		for (int generate = 0; generate < spawnAmount; generate++) {
			enemyNumber++;
			ENEMY_AMOUNT;
			Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
			neutral_Enemy->SetNeutral_EnemyGame(this);
			neutral_Enemy->SetPosition(Vector3(0.0f,0.0f,0.0f));
			//neutral_Enemy->SetRotation(objData.rotation);
			neutral_Enemy->SetKnightPlayer(m_knightplayer);
		}
	}

	//マップのFindGO関数を呼び出しエネミーの数を把握する
	m_Map->FindEnemys();

	//Neutral_Enemyを配列で消す
	//auto seutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
	//存在するエネミーの数がスタジアムのエネミーの総数より小さいなら
	//if (ENEMY_AMOUNT != m_enemyCounter.size()) {
	//	//生成するエネミーの数
	//	int spawnAmount = ENEMY_AMOUNT - m_enemyCounter.size();
	//	//
	//	for (int generate = 0; generate < spawnAmount; generate++) {


	//		Neutral_Enemy* m_Neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");

	//		m_Neutral_Enemy->SetNeutral_EnemyGame(this);
	//		m_Neutral_Enemy->SetPosition(Vector3(0.0f,0.0f,0.0f));
	//		//m_Neutral_Enemy->SetRotation(objData.rotation);
	//		m_Neutral_Enemy->SetKnightPlayer(m_knightplayer);
	//		m_enemyCounter.push_back(neutral_Enemy);

	//	}
	//}
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
