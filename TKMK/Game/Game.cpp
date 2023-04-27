#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Result.h"
#include "Tittle.h"
#include "GameCamera.h"
#include "Actor.h"
#include "GameUI.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"
#include "Map.h"
#include "KnightAI.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Lamp.h"
//#include <vector>
//#include <algorithm>

namespace {
	int ENEMY_AMOUNT = 10;
	const Vector3 Menu_BackPos = Vector3(0.0f, 210.0f, 0.0f);
	const Vector3 Menu_HowToPlayPos = Vector3(0.0f, 60.0f, 0.0f);
	const Vector3 Menu_BGMPos = Vector3(-245.0f, -70.0f, 0.0f);
	const Vector3 Menu_SEPos = Vector3(-245.0f, -185.0f, 0.0f);
	const Vector3 Menu_QuitGamePos = Vector3(0.0f, -320.0f, 0.0f);
	const Vector3 Menu_SelectBar_BGMPos = Vector3(90.0f, -68.0f, 0.0f);
	const Vector3 Menu_SelectBar_SEPos = Vector3(90.0f, -183.0f, 0.0f);
}

Game::Game()
{
	//sound�ݒ�
	//1-5 Title/Game��BGM
	//6-10 �I��
	//11-20 player�̃X�L���Ȃǂ̉�
	//21-30 enemy�̉�
}

Game::~Game()
{
	if (m_backGround != nullptr)
	{
		DeleteGO(m_backGround);
	}
	DeleteGO(m_gamecamera);

	//�����̓G�̃T�C�Y�𒲂ׂ�for���ŉ�
	for (auto seutral_Enemy : m_neutral_Enemys)
	{
		DeleteGO(seutral_Enemy);
	}
	//m_neutral_Enemys.clear();

	for (auto aoctor : m_Actors)
	{
		DeleteGO(aoctor);
	}

	DeleteGO(player);
	
	DeleteGO(m_gameUI);
	DeleteGO(m_Map);
	DeleteGO(m_bgm);
	DeleteGO(lamp);
}

bool Game::Start()
{

	g_renderingEngine->UnUseHemiLight();

	Vector3 directionLightDir = Vector3{ 0.0f,-1.0f,-1.0f };
  
	directionLightDir.Normalize();
	Vector4 directionLightColor = Vector4{ 0.5f, 0.5f, 0.5f, 1.0f };
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor);
	g_renderingEngine->SetAmbient({ 0.6f,0.6f,0.6f,1.0f });

	InitSkyCube();

	//スタジアムの生成
	m_level3DRender.Init("Assets/level3D/stadium05Level.tkl", [&](LevelObjectData& objData) {

		if (objData.EqualObjectName(L"stadium05_ground") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			m_backGround->SetRotation(objData.rotation);
			m_backGround->SetScale(objData.scale);

			return true;
		}

		if (objData.EqualObjectName(L"Lamp") == true) {
			lamp = NewGO<Lamp>(0, "lamp");
			lamp->SetPosition(objData.position);
			lamp->SetRotation(objData.rotation);
			lamp->SetScale(objData.scale);

			return true;
		}
	
		return false;

	});

	//プレイヤーの生成
	player = NewGO<Player>(0, "player");
	player->CharSelect(SelectCharNumber);
	player->CreaetPlayer();
	m_Actors.push_back(player->GetPlayerActor());

	//カメラの生成
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	//中立の敵の生成
	{
		m_Enemylevel.Init("Assets/level3D/enemyRespawnPos.tkl", [&](LevelObjectData& objData) {

			if (objData.ForwardMatchName(L"Pos") == true) {
				if (objData.number == 0) {
					SetRespawnPosition(objData.position, objData.rotation, objData.number);
					return true;
				}
				if (objData.number == 1) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				//enemyNumber++;
				//ENEMY_AMOUNT;
				//CreateEnemy(objData.position, objData.rotation);
				return true;
			}
			if (objData.number == 2) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				return true;
			}
			if (objData.number == 3) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				CreateEnemy(objData.position, objData.rotation);
				return true;
			}
			if (objData.number == 4) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				return true;
			}
			if (objData.number == 5) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);

					enemyNumber++;
					ENEMY_AMOUNT;
					CreateEnemy(objData.position, objData.rotation);
					return true;
				}
				if (objData.number == 6) {
					SetRespawnPosition(objData.position, objData.rotation, objData.number);
					return true;
				}
				if (objData.number == 7) {
					SetRespawnPosition(objData.position, objData.rotation, objData.number);
					enemyNumber++;
					ENEMY_AMOUNT;
					CreateEnemy(objData.position, objData.rotation);
					return true;
				}
				if (objData.number == 8) {
					SetRespawnPosition(objData.position, objData.rotation, objData.number);
					return true;
				}
			}
			return true;
			});
	}

	

	m_AIPos.Init("Assets/level3D/AIPOS2.tkl", [&](LevelObjectData& objData) {

		

			if (objData.ForwardMatchName(L"CharPos") == true) {
				//左上の座標
				if (objData.number == 0) {
					m_KnightAI = NewGO<KnightAI>(0, "KnightAI");
					m_KnightAI->SetGame(this);
					m_Actors.push_back(m_KnightAI);
					m_KnightAI->SetPosition(objData.position);
					m_KnightAI->SetCharaconPosition(objData.position);
					int Number = 0;
					m_KnightAI->SetRespawnNumber(Number);
					return true;
				}
				//右上の座標
				if (objData.number == 1) {
					m_KnightAI1 = NewGO<KnightAI>(0, "KnightAI1");
					m_KnightAI1->SetGame(this);
					m_Actors.push_back(m_KnightAI1);
					m_KnightAI1->SetPosition(objData.position);
					m_KnightAI1->SetCharaconPosition(objData.position);
					int Number = 1;
					m_KnightAI1->SetRespawnNumber(Number);
					return true;
				}
				//左下の座標
				if (objData.number == 3) {
					m_KnightAI2 = NewGO<KnightAI>(0, "KnightAI2");
					m_KnightAI2->SetGame(this);
					m_Actors.push_back(m_KnightAI2);
					m_KnightAI2->SetPosition(objData.position);
					m_KnightAI2->SetCharaconPosition(objData.position);
					int Number = 3;
					m_KnightAI2->SetRespawnNumber(Number);
					return true;
				}
			}
		return true;
		});
	
	
	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "m_gameUI");
	m_gameUI->SetSGame(this);

	//マップの生成
	m_Map = NewGO<Map>(2, "map");

	//ポーズ画面の画像読み込み
	{
		//背景
		m_Pause_Back.Init("Assets/sprite/PauseMenu/pause_back.DDS", 1920.0f, 1080.0f);
		m_Pause_Back.SetPosition(g_vec3Zero);
		m_Pause_Back.SetScale(1.0f, 1.0f, 1.0f);
		m_Pause_Back.SetRotation(m_sRotation);
		m_Pause_Back.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.4f));
		m_Pause_Back.Update();
		//ポーズメイン
		m_Pause_Front.Init("Assets/sprite/PauseMenu/Pause2_flame.DDS", 1920.0f, 1080.0f);
		m_Pause_Front.SetPosition(g_vec3Zero);
		m_Pause_Front.SetScale(1.0f, 1.0f, 1.0f);
		m_Pause_Front.SetRotation(m_sRotation);
		m_Pause_Front.Update();

		//Back
		m_Menu_Back.Init("Assets/sprite/PauseMenu/Pause2_Back_decision.DDS", 300.0f, 100.0f);		m_Menu_Back.SetPosition(Menu_BackPos);
		m_Menu_Back.SetScale(1.0f, 1.0f, 1.0f);
		m_Menu_Back.SetRotation(m_sRotation);
		m_Menu_Back.Update();
		//HowToPlay
		m_Menu_HowToPlay.Init("Assets/sprite/PauseMenu/Pause2_HowToPlay.DDS", 620.0f, 120.0f);
		m_Menu_HowToPlay.SetPosition(Menu_HowToPlayPos);
		m_Menu_HowToPlay.SetScale(1.0f, 1.0f, 1.0f);
		m_Menu_HowToPlay.SetRotation(m_sRotation);
		m_Menu_HowToPlay.Update();
		//BGM
		m_Menu_BGM.Init("Assets/sprite/PauseMenu/Pause2_BGM.DDS", 300.0f, 100.0f);
		m_Menu_BGM.SetPosition(Menu_BGMPos);
		m_Menu_BGM.SetScale(1.0f, 1.0f, 1.0f);
		m_Menu_BGM.SetRotation(m_sRotation);
		m_Menu_BGM.Update();
		//SE
		m_Menu_SE.Init("Assets/sprite/PauseMenu/Pause2_SE.DDS", 300.0f, 100.0f);
		m_Menu_SE.SetPosition(Menu_SEPos);
		m_Menu_SE.SetScale(1.0f, 1.0f, 1.0f);
		m_Menu_SE.SetRotation(m_sRotation);
		m_Menu_SE.Update();
		//QuitGame  
		m_Menu_QuitGame.Init("Assets/sprite/PauseMenu/Pause2_QuitGame.DDS", 620.0f, 120.0f);
		m_Menu_QuitGame.SetPosition(Menu_QuitGamePos);
		m_Menu_QuitGame.SetScale(1.0f, 1.0f, 1.0f);
		m_Menu_QuitGame.SetRotation(m_sRotation);
		m_Menu_QuitGame.Update();
		//m_Menu_SelectBar_BGM
		m_Menu_SelectBar_BGM.Init("Assets/sprite/PauseMenu/Pause_selectBar.DDS", 50.0f, 100.0f);
		m_Menu_SelectBar_BGM.SetPosition(Menu_SelectBar_BGMPos);
		m_Menu_SelectBar_BGM.SetScale(1.0f, 1.0f, 1.0f);
		m_Menu_SelectBar_BGM.SetRotation(m_sRotation);
		m_Menu_SelectBar_BGM.Update();
		//m_Menu_SelectBar_SE
		m_Menu_SelectBar_SE.Init("Assets/sprite/PauseMenu/Pause_selectBar.DDS", 50.0f, 100.0f);
		m_Menu_SelectBar_SE.SetPosition(Menu_SelectBar_SEPos);
		m_Menu_SelectBar_SE.SetScale(1.0f, 1.0f, 1.0f);
		m_Menu_SelectBar_SE.SetRotation(m_sRotation);
		m_Menu_SelectBar_SE.Update();

		SelectBar_BGMPos = Menu_SelectBar_BGMPos;
		SelectBar_SEPos = Menu_SelectBar_SEPos;
	}
	
	//ゲームの状態をゲームステートにする
	m_GameState = enGameState_Start;

	//ゲーム中に再生される音を読み込む
	SetMusic();
	//BGMの再生
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(2);
	m_bgm->Play(true);
	m_bgm->SetVolume(BGMVolume);


	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{
	//制限時間に達したら
	if (GameEndFlag == true)
	{
		m_GameState=enGamestate_End;
		GameState();
	}

	GameState();

	m_modelRender.Update();
}

void Game::BattleStart()
{
	m_StartToGameTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_StartToGameTimer < 0)
	{
		m_GameState = enGameState_Battle;
		//ゲームUIのステートをgameStateにする
		m_gameUI->SetGameUIState(GameUI::m_GameState);
	}
}

//バトルステートの処理
void Game::Battle()
{
	//時間切れではないなら
	if (GameEndFlag == false) {
		CountDown();
	}

	if (m_GameState == enGameState_Battle) {
		//CTRLを押したら
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			m_GameState = enGameState_Rezult;
		}
	}

	//ポーズステートに変える
	//スタートボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		//ゲームのステートをポーズにする
			m_GameState = enGameState_Pause;
			//生成されているキャラのステートをポーズステートに変更
			for (auto character : m_Actors)
			{
				character->ChangeGameState(character->enPause);
			}
			//UIのステートをポーズステートに変更
			m_gameUI->SetGameUIState(m_gameUI->m_PauseState);
			//カメラのステートをポーズステートに変更
			m_gamecamera->SetCameraState(m_gamecamera->enPauseState);
			//生成されている中立の敵のステートをポーズステートに変更
			for (auto seutral_Enemy : m_neutral_Enemys)
			{
				seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Pause);
			}
	}
	if (UltStopFlag == true)
	{
		return;
	}
	m_RespawnTimer += g_gameTime->GetFrameDeltaTime();
	if (m_RespawnTimer >= 5) {
		Respawn();
		m_RespawnTimer = 0.0f;
	}
}

//ポーズ時の処理
void Game::Pause()
{
	//ポーズ時の移動処理
	PauseMove();
	//前のフレームのメニュー番号と今のフレームのメニュー番号が違うなら
	if (MenuNumber_old != MenuNumber) {
		//メニューのステートを選ぶ
		SelectMenu();
	}
	//ステートの管理
	MenuState();

	MenuNumber_old = MenuNumber;
}

//タイムアップになったあとの処理
void Game::End()
{
	m_EndtoResultTimer+= g_gameTime->GetFrameDeltaTime();

	if (m_EndtoResultTimer >= 10)
	{
		m_GameState=enGameState_Rezult;
	}
}

void Game::GoResult()
{
	Result* result = NewGO<Result>(0, "Result");
	//DeleteGO(this);
}

//ポーズステートからゲームステートに移るときの間の処理
void Game::Between()
{
	m_BetweenTimer++;
	if (m_BetweenTimer>=1)
	{
		//生成されているキャラのステートをゲームステートに戻す
		for (auto character : m_Actors)
		{
			character->ChangeGameState(character->enGame);
		}
		//UIのステートをゲームステートに戻す
		m_gameUI->SetGameUIState(m_gameUI->m_GameState);
		//カメラのステートをゲームステートに戻す
		m_gamecamera->SetCameraState(m_gamecamera->enGameState);
		//生成されている中立の敵のステートをゲームステートに戻す
		for (auto seutral_Enemy : m_neutral_Enemys)
		{
			seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Idle);
		}
		m_GameState = enGameState_Battle;
	}
}

/// <summary>
/// 中立の敵のリスポーンの処理
/// </summary>
void Game::Respawn()
{
	for (int count = 0; count < 10; count++)
	{
		EnemyRespawnFlag[count] = false;
	}

	//　乱数を初期化。
	srand((unsigned)time(NULL));

	//中立の敵の総数が生成されている中立の敵と数が違うなら
	if (ENEMY_AMOUNT != m_neutral_Enemys.size()) {
		//足りない中立の敵の数を調べる
		int spawnAmount = ENEMY_AMOUNT - m_neutral_Enemys.size();
		for (int generate = 0; generate < spawnAmount; generate++) {
			//ランダムに番号を決める(0以外)
			RandamRespawnPosNumber = rand() % 8 + 1;

			//中立の敵のリスポーンする座標を決める
			SetEnemyRespawnPos();
			//中立の敵を生成
			CreateEnemy(EnemyRespawnPosition[SearchRespawnPosNumber], EnemyReapawnPot[SearchRespawnPosNumber]);
		}
	}
}

/// <summary>
/// 中立の敵のリスポーンする座標を決める
/// </summary>
void Game::SetEnemyRespawnPos()
{
	//検索する変数にランダムに決めた番号を代入する
	SearchRespawnPosNumber = RandamRespawnPosNumber;
	//ループする最大値を決める
	int MaxSearchNumber = SearchRespawnPosNumber - 1;
	//最大値が0だったら
	if(SearchRespawnPosNumber==0)
	{
		MaxSearchNumber = 8;
	}

	while (SearchRespawnPosNumber!= MaxSearchNumber)
	{
		if (EnemyRespawnFlag[SearchRespawnPosNumber] == false)
		{
			//中立の敵とキャラと一定以上距離が離れていることを記憶する変数
			int distanceCounter = 0;
			for (auto actorPos : m_Actors)
			{
				Vector3 CharPos = actorPos->GetPosition();
				//検索する変数の座標からキャラに向かうベクトルを計算する
				Vector3 diff = EnemyRespawnPosition[SearchRespawnPosNumber] - CharPos;
				//ベクトルの長さが600より大きかったら
				if (diff.Length() > 600)
				{
					//距離が離れている
					distanceCounter++;
				}

			}
			//全てのキャラと距離が離れているなら
			if (distanceCounter == m_Actors.size())
			{
				//生成した番号のフラグをtrueにする
				EnemyRespawnFlag[SearchRespawnPosNumber] = true;
				return;
			}
		}
		
		//番号が8でないなら
		if (SearchRespawnPosNumber < 8)
		{
			SearchRespawnPosNumber++;
		}
		else  //番号が8になったら
		{
			SearchRespawnPosNumber = 0;
		}
		
		
	}
	//ループでどの座標にも生成できなかったらランダムに決める
	SearchRespawnPosNumber= rand() % 8 + 1;
	return;
}

//ゲームのステート管理
void Game::GameState()
{
	switch (m_GameState)
	{
	case enGameState_Start:
		BattleStart();
		break;

	case enGameState_Battle:
		Battle();
		break;

	case enGameState_Pause:
		Pause();
		break;

	case enGamestate_End:
		End();
		break;

	case enGameState_Rezult:
		GoResult();
		break;

	case enGameState_BetweenGameAndPause:
		Between();
		break;
	/*default:
		break;*/
	}
}

void Game::GetActorPoints(int charPoints[])
{
	int count = 0;
	for (auto actor : m_Actors)
	{
		charPoints[count] = actor->GetPoint();
		count++;
	}
}

//中立の敵の生成処理
void Game::CreateEnemy(Vector3 pos, Quaternion rot) {

	enemyNumber++;
	ENEMY_AMOUNT;

	Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(1, CreateEnemyName());
	neutral_Enemy->SetNeutral_EnemyGame(this);
	neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
	neutral_Enemy->SetPosition(pos);
	neutral_Enemy->SetRotation(rot);
	neutral_Enemy->modelUpdate();

	m_neutral_Enemys.push_back(neutral_Enemy);
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->SetScale(600.0f);
}

/// <summary>
/// ゲーム中に再生される音の読み込み
/// </summary>
void Game::SetMusic()
{
	//バトル中のBGM
	{
		g_soundEngine->ResistWaveFileBank(2, "Assets/sound/gameBGM/SentouBGM1.wav");
	}
	
	//効果音
	{
	//剣士
		{
			//���X�L��
			g_soundEngine->ResistWaveFileBank(11, "Assets/sound/playerSE/kenSkill3.wav");
			//�_���[�W�������Ƃ��̔ߖ�
			g_soundEngine->ResistWaveFileBank(12, "Assets/sound/playerSE/playerScream1.wav");
			//���ʏ�U��
			g_soundEngine->ResistWaveFileBank(13, "Assets/sound/kenSE/ken1.wav");
			g_soundEngine->ResistWaveFileBank(14, "Assets/sound/kenSE/ken2.wav");
			g_soundEngine->ResistWaveFileBank(15, "Assets/sound/kenSE/ken3.wav");
			//�A���e�B���b�g
			g_soundEngine->ResistWaveFileBank(16, "Assets/sound/playerSE/kenSkill1.wav");
			//���S�����Ƃ�
			g_soundEngine->ResistWaveFileBank(17, "Assets/sound/playerSE/playerScream3.wav");
		}
	//魔法使い
		{

		}
	//ゾンビ
		{

		}
	//魔物
		{

		}

	//中立の敵
		{
			g_soundEngine->ResistWaveFileBank(21, "Assets/sound/enemySE/enemyKoe.wav");
		}
		
	}
	
}

void Game::PauseMove()
{
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		MenuNumber++;
	}
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		MenuNumber--;
	}

}

/// <summary>
/// 選んだ番号に対応したステートにする処理
/// </summary>
void Game::SelectMenu()
{
	if (MenuNumber == 0)
	{
		m_EnPauseMenu = enPauseMenuState_Back;
		m_Menu_Back.Init("Assets/sprite/PauseMenu/Pause2_Back_decision.DDS", 300.0f, 100.0f);
	}
	else
	{
		m_Menu_Back.Init("Assets/sprite/PauseMenu/Pause2_Back.DDS", 300.0f, 100.0f);
	}

	if (MenuNumber == 1)
	{
		m_EnPauseMenu = enPauseMenuState_HowToPlay;
		m_Menu_HowToPlay.Init("Assets/sprite/PauseMenu/Pause2_HowToPlay_decision.DDS", 620.0f, 120.0f);
	}
	else
	{
		m_Menu_HowToPlay.Init("Assets/sprite/PauseMenu/Pause2_HowToPlay.DDS", 620.0f, 120.0f);
	}

	if (MenuNumber == 2)
	{
		m_EnPauseMenu = enPauseMenuState_BGM;
		m_Menu_BGM.Init("Assets/sprite/PauseMenu/Pause2_BGM_decision.DDS", 300.0f, 100.0f);
	}
	else
	{
		m_Menu_BGM.Init("Assets/sprite/PauseMenu/Pause2_BGM.DDS", 300.0f, 100.0f);
	}

	if (MenuNumber == 3)
	{
		m_EnPauseMenu = enPauseMenuState_SE;
		m_Menu_SE.Init("Assets/sprite/PauseMenu/Pause2_SE_decision.DDS", 300.0f, 100.0f);
	}
	else
	{
		m_Menu_SE.Init("Assets/sprite/PauseMenu/Pause2_SE.DDS", 300.0f, 100.0f);
	}

	if (MenuNumber == 4)
	{
		m_EnPauseMenu = enPauseMenuState_QuitGame;
		m_Menu_QuitGame.Init("Assets/sprite/PauseMenu/Pause2_QuitGame_decision.DDS", 620.0f, 120.0f);
	}
	else
	{
		m_Menu_QuitGame.Init("Assets/sprite/PauseMenu/Pause2_QuitGame.DDS", 620.0f, 120.0f);
	}
}

/// <summary>
/// メニュー時のステートの管理
/// </summary>
void Game::MenuState()
{
	switch (m_EnPauseMenu)
	{
	case enPauseMenuState_Back:
		Menu_Back();
		break;
	case enPauseMenuState_HowToPlay:
		Menu_HowToPlay();
		break;
	case enPauseMenuState_BGM:
		Menu_BGM();
		break;
	case enPauseMenuState_SE:
		Menu_SE();
		break;
	case enPauseMenuState_QuitGame:
		Menu_QuitGame();
		break;
	}
}
//Backの処理
void Game::Menu_Back()
{
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//ステートをバトルステートに戻す
		m_GameState = enGameState_BetweenGameAndPause;
		
	}
}
//HowToPlayの処理
void Game::Menu_HowToPlay()
{

}
//BGMの処理
void Game::Menu_BGM()
{
	//音量を上げる
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		if (BGMVolume < 8.0f)
		{
			BGMVolume += 0.5f;
			m_bgm->SetVolume(BGMVolume);
			SelectBar_BGMPos.x += m_nuwBGMPos;
		}
		
	}
	//音量を下げる
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		if (BGMVolume > 0)
		{
			BGMVolume -= 0.2f;
			m_bgm->SetVolume(BGMVolume);
			SelectBar_BGMPos.x -= m_nuwBGMPos;
		}
	
	}
	//バーの座標を更新
	m_Menu_SelectBar_BGM.SetPosition(SelectBar_BGMPos);
	m_Menu_SelectBar_BGM.Update();
}
//SEの処理
void Game::Menu_SE()
{
	//音量を上げる
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		if (SoundEffectVolume < 4.0f)
		{
			SoundEffectVolume += 0.4f;
			SelectBar_SEPos.x += m_nuwSEPos;
		}
		
	}
	//音量を下げる
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		if (SoundEffectVolume > 0)
		{
			SoundEffectVolume -= 0.1f;
			SelectBar_SEPos.x -= m_nuwSEPos;
		}
		
	}
	//バーの座標を更新
	m_Menu_SelectBar_SE.SetPosition(SelectBar_SEPos);
	m_Menu_SelectBar_SE.Update();
}
//QuitGameの処理
void Game::Menu_QuitGame()
{
	//タイトルへの遷移
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle* m_tittle = NewGO<Tittle>(0, "m_tittle");
		DeleteGO(this);
	}
}

//制限時間の処理
void Game::CountDown()
{
	//0秒以下なら
	if (SecondsTimer <= 0) {
		//1分減らす
		MinutesTimer--;
		//もし0分なら、秒も0にする
		if (MinutesTimer < 0) {
			SecondsTimer = 0.0f;
			MinutesTimer = 0.0f;
			GameEndFlag = true;
		}
		//60秒に戻す
		else SecondsTimer = 60.0f;
	}
	else SecondsTimer -= g_gameTime->GetFrameDeltaTime();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	if (m_GameState == enGameState_Pause)
	{
		m_Pause_Back.Draw(rc);
		m_Pause_Front.Draw(rc);
		m_Menu_Back.Draw(rc);        //Back
		m_Menu_HowToPlay.Draw(rc);   //HowToPlay
		m_Menu_BGM.Draw(rc);         //BGM
		m_Menu_SE.Draw(rc);          //SE
		m_Menu_QuitGame.Draw(rc);    //QuitGame
		m_Menu_SelectBar_BGM.Draw(rc);
		m_Menu_SelectBar_SE.Draw(rc);
	}
	//m_fontRender.Draw(rc);
	
}
