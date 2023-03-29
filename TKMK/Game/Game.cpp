#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Result.h"
#include "Tittle.h"
#include "GameCamera.h"
//#include "KnightBase.h"
#include "Actor.h"
#include "GameUI.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Map.h"
#include "KnightAI.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "CharUltFlag.h"
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

	DeleteGO(player);
	DeleteGO(m_knightplayer);
	DeleteGO(wizardPlayer);
	
	DeleteGO(m_gameUI);
	DeleteGO(m_Map);
	DeleteGO(m_KnightAI);
	DeleteGO(m_bgm);
	DeleteGO(charUltFlag);
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

	//スタジアムの生成
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

	//必殺技フラグを生成
	charUltFlag = NewGO<CharUltFlag>(0, "charUltFlag");

	//プレイヤーの生成
	player = NewGO<Player>(0, "player");
	//生成するキャラ選択
	player->CharSelect(SelectCharNumber);
	player->CreaetPlayer();
	m_Actors.push_back(player->GetPlayerActor());

	//ゲームカメラの生成
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	//m_gamecamera->SetKnight(m_knightplayer);

	//中立の敵の生成
	m_Enemylevel.Init("Assets/level3D/enemy_respawn.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			//左上の座標
			if (objData.number == 0) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				//enemyNumber++;
				//ENEMY_AMOUNT;
				//Neutral_Enemy*neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				//neutral_Enemy->SetNeutral_EnemyGame(this);
				//neutral_Enemy->SetPlayerActor(player->playerActor);
				//neutral_Enemy->SetPosition(objData.position);
				//neutral_Enemy->SetRotation(objData.rotation);
				//neutral_Enemy->SetKnightPlayer(m_knightplayer);

				//enemyNumber++;
				//ENEMY_AMOUNT;
				//Neutral_Enemy*neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				//neutral_Enemy->SetNeutral_EnemyGame(this);
				//neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
				//neutral_Enemy->SetPosition(objData.position);
				//neutral_Enemy->SetRotation(objData.rotation);
				//RespawnNumberBox[enemyNumber]=true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy0->SetScale(objData.scale);
				return true;
			}
			//右上の座標
			if (objData.number == 1) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy1->SetScale(objData.scale);
				return true;
			}
			if (objData.number == 2) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				return true;
			}
			//右下の座標
			if (objData.number == 3) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy2->SetScale(objData.scale);
				return true;
			}
			if (objData.number == 4) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				return true;
			}
			//左下の座標
			if (objData.number == 5) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy3->SetScale(objData.scale);
				return true;
			}
			if (objData.number == 6) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				return true;
			}
			if (objData.number == 7) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
				neutral_Enemy->SetNeutral_EnemyGame(this);
				neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
				neutral_Enemy->SetPosition(objData.position);
				neutral_Enemy->SetRotation(objData.rotation);
				//RespawnNumberBox[enemyNumber] = true;
				//m_enemyCounter.push_back(neutral_Enemy);
				//m_Neutral_Enemy4->SetScale(objData.scale);
				return true;
			}
			if (objData.number == 8) {
				//リスポーン座標の設定
				SetRespawnPosition(objData.position, objData.number);
				return true;
			}
		}
		return true;
	});

	/*m_Neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
	m_Neutral_Enemy->SetNeutral_EnemyGame(this);
	m_Neutral_Enemy->SetKnightPlayer(m_knightplayer);*/
	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");

	//m_KnightAI = NewGO<KnightAI>(0, "KnightAI");
	//m_KnightAI->SetGame(this);
	//m_Actors.push_back(m_KnightAI);
	
	//マップの生成
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
	//se
	//player
	//剣スキル
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/playerSE/kenSkill3.wav");
	//ダメージくらったときの悲鳴
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/playerSE/playerScream1.wav");
	//剣通常攻撃
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/kenSE/ken1.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/kenSE/ken2.wav");
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/kenSE/ken3.wav");
	//アルティメット
	g_soundEngine->ResistWaveFileBank(16, "Assets/sound/playerSE/kenSkill1.wav");
	//死亡したとき
	g_soundEngine->ResistWaveFileBank(17, "Assets/sound/playerSE/playerScream3.wav");
	//enemy
	//攻撃の声
	g_soundEngine->ResistWaveFileBank(21, "Assets/sound/enemySE/enemyKoe.wav");
	
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(2);
	m_bgm->Play(true);
	m_bgm->SetVolume(musicVolume);


	//当たり判定の可視化
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	if (m_GameState == enGameState_Battle) {
		//リザルト画面への遷移
	    //Yボタンが押されたら。
		/*if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_GameState = enGameState_Rezult;
		}*/
	}
	

	//ポーズ画面への遷移
	//スタートボタンが押されたら。
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		//ゲーム画面からポーズ画面に遷移する時の処理
		if (m_GameState == enGameState_Battle) {
			m_GameState = enGameState_Pause;
			//プレイヤーのステートをポーズ画面用のステートに変更
			player->CharSetState(Player::enPause);
			//UIのステートをポーズ画面用のステートに変更
			m_gameUI->SetGameUIState(m_gameUI->m_PauseState);
			//カメラのステートをポーズ画面用のステートに変更
			m_gamecamera->SetCameraState(m_gamecamera->enPauseState);
			//中立の敵をポーズ画面用のステートに変更
			//auto seutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
			////配列のサイズを調べてfor文で回す
			//for (auto seutral_Enemy : seutral_Enemys)
			//{
			//	seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Pause);
			//}

			
		}
			
		//ポーズ画面からゲーム画面に戻る時の処理
		else if (m_GameState == enGameState_Pause) {
			m_GameState = enGameState_Battle;
			//プレイヤーのステートをポーズ画面用のステートではないようにする
			player->CharSetState(Player::enGame);
			//UIのステートをゲームのステートに変更
			m_gameUI->SetGameUIState(m_gameUI->m_GameState);
			//カメラのステートをゲームのステートに変更
			m_gamecamera->SetCameraState(m_gamecamera->enGameState);
			//中立の敵をポーズ画面用のステートに変更
			//auto seutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");
			////配列のサイズを調べてfor文で回す
			//for (auto seutral_Enemy : seutral_Enemys)
			//{
			//	seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Idle);
			//}
		}
	}

	GameState();
	
	m_Timer += g_gameTime->GetFrameDeltaTime();
	if (m_Timer>=15) {
		//Respawn();
		m_Timer = 0.0f;
	}
	
	m_modelRender.Update();
	//m_Pause_Back.Update();
}

//ポーズ画面の処理
void Game::Pause()
{
	//音量を上げる
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		if(musicVolume<4.0f)
		musicVolume += 0.1f;
		//音量調整
		m_bgm->SetVolume(musicVolume);
	}
	//音量を下げる
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		if(musicVolume>0)
		musicVolume -= 0.1f;
		//音量調整
		m_bgm->SetVolume(musicVolume);
	}
	


	//タイトル画面への遷移
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle*m_tittle = NewGO<Tittle>(0,"m_tittle");
		DeleteGO(this);
	}
}

/// <summary>
/// 中立の敵のリスポーンの処理
/// </summary>
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
			//リスポーンする座標
			neutral_Enemy->SetPosition(SetEnemyRespawnPos());
			//neutral_Enemy->SetRotation(objData.rotation);
			neutral_Enemy->SetPlayerActor(player->GetPlayerActor());

		}
	}

	//マップのFindGO関数を呼び出しエネミーの数を把握する
	m_Map->FindEnemys();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
Vector3 Game::SetEnemyRespawnPos()
{
	for (int i = 0; i < 9; i++)
	{
		int a;
		for (auto actorPos : m_Actors)
		{
			//キャラの座標
			Vector3 CharPos = actorPos->GetPosition();
			Vector3 diff = EnemyRespawnPosition[i] - CharPos;
			if (diff.Length() < 200)
			{
				a++;
			}
			
		}
		//もしリスポーンする座標が全てのキャラの座標から200離れていたら
		if (a == m_Actors.max_size())
		{
			//その座標をリスポーンする座標とする
			return EnemyRespawnPosition[i];
		}
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
