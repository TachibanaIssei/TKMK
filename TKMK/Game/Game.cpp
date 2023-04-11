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
	int ENEMY_AMOUNT = 5;
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


	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "m_gameUI");
	m_gameUI->SetSGame(this);

	//プレイヤーの生成
	player = NewGO<Player>(0, "player");
	player->CharSelect(SelectCharNumber);
	player->CreaetPlayer();
	m_Actors.push_back(player->GetPlayerActor());

	//カメラの生成
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");

	//中立の敵の生成
	m_Enemylevel.Init("Assets/level3D/enemyRespawnPos.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			if (objData.number == 0) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				return true;
			}
			if (objData.number == 1) {

				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				CreateEnemy(objData.position, objData.rotation);
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

	//剣士AIの生成
	m_KnightAI = NewGO<KnightAI>(0, "KnightAI");
	m_KnightAI->SetGame(this);
	m_Actors.push_back(m_KnightAI);
	
	//マップの生成
	m_Map = NewGO<Map>(2, "map");

	//ポーズ画面の画像
	m_Pause_Back.Init("Assets/sprite/pause_back.DDS", 1920.0f, 1080.0f);
	m_Pause_Back.SetPosition(g_vec3Zero);
	m_Pause_Back.SetScale(1.0f, 1.0f, 1.0f);
	m_Pause_Back.SetRotation(m_sRotation);
	m_Pause_Back.SetMulColor(Vector4(1.0f,1.0f,1.0f,0.5f));
	m_Pause_Back.Update();

	m_Pause_Front.Init("Assets/sprite/Pause2_flame.DDS", 1920.0f, 1080.0f);
	m_Pause_Front.SetPosition(g_vec3Zero);
	m_Pause_Front.SetScale(1.0f, 1.0f, 1.0f);
	m_Pause_Front.SetRotation(m_sRotation);
	m_Pause_Front.Update();

	//ゲームの状態をゲームステートにする
	m_GameState = enGameState_Battle;

	//ゲーム中に再生される音を読み込む
	SetMusic();
	
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
	GameState();

	m_modelRender.Update();
	//m_Pause_Back.Update();
}

void Game::BattleStart()
{

}

//バトルステートの処理
void Game::Battle()
{
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

	m_Timer += g_gameTime->GetFrameDeltaTime();
	if (m_Timer >= 20) {
		Respawn();
		m_Timer = 0.0f;
	}
}

//ポーズ時の処理
void Game::Pause()
{
	//Bボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		//ステートをバトルステートに戻す
			m_GameState = enGameState_Battle;
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
	}

	//右
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		if(BGMVolume <4.0f)
			BGMVolume += 0.1f;
		//音量を上げる
		m_bgm->SetVolume(BGMVolume);
	}
	//左
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		if(BGMVolume >0)
			BGMVolume -= 0.1f;
		//音量を下げる
		m_bgm->SetVolume(BGMVolume);
	}
	


	//タイトルへの遷移
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle*m_tittle = NewGO<Tittle>(0,"m_tittle");
		DeleteGO(this);
	}
}

//タイムアップになったあとの処理
void Game::End()
{

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
		Result* result = NewGO<Result>(0, "Result");
		DeleteGO(this);
		break;

	/*default:
		break;*/
	}
}

//中立の敵の生成処理
void Game::CreateEnemy(Vector3 pos, Quaternion rot) {

	enemyNumber++;
	ENEMY_AMOUNT;

	Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, CreateEnemyName());
	neutral_Enemy->SetNeutral_EnemyGame(this);
	neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
	neutral_Enemy->SetPosition(pos);
	neutral_Enemy->SetRotation(rot);

	m_neutral_Enemys.push_back(neutral_Enemy);
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
