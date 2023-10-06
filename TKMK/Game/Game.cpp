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
#include "Fade.h"
#include "ExpforKnight.h"
#include "Pause.h"

namespace {
	const int ENEMY_AMOUNT = 14;
	const Vector3 Menu_BackPos = Vector3(0.0f, 210.0f, 0.0f);
	const Vector3 Menu_HowToPlayPos = Vector3(0.0f, 60.0f, 0.0f);
	const Vector3 Menu_BGMPos = Vector3(-245.0f, -70.0f, 0.0f);
	const Vector3 Menu_SEPos = Vector3(-245.0f, -185.0f, 0.0f);
	const Vector3 Menu_QuitGamePos = Vector3(0.0f, -320.0f, 0.0f);
	const Vector3 Menu_SelectBar_BGMPos = Vector3(90.0f, -68.0f, 0.0f);
	const Vector3 Menu_SelectBar_SEPos = Vector3(90.0f, -183.0f, 0.0f);
	const Vector3 TOWER_X_POS = Vector3(20.0f, -410.0f, 0.0f);
	const Vector3 RABBIT_POS = Vector3(650.0f, 400.0f, 0.0f);
	const Vector3 RabbitSpriteScale = Vector3(0.75f, 0.75f, 0.0f);
	const Vector3 SpriteScale = Vector3::One;
	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.5f, 0.5f, 0.5f);//ディレクションライトのカラー
	const Vector3 AMBIENT_COLOR = Vector3(0.6f, 0.6f, 0.6f);//環境光のカラー

	//const Vector3 DARKNESS_DIRECTION = Vector3(0.4f, 0.4f, 0.4f);//必殺技発動時のディレクションライトのカラー
	const float DARKNESS_DIRECTION = 0.4f;
	const Vector3 DARKNESS_AMBIENT = Vector3(0.55f, 0.5f, 0.6f);//必殺技発動時の環境光のカラー

	const float SKYCOLOR = 1.0f;
	const float DARKNESS_SKY_COLOR = 0.2f;
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

	for (auto seutral_Enemy : m_neutral_Enemys)
	{
		DeleteGO(seutral_Enemy);
	}
	if (GameObjectManager::GetInstance()->IsActive()) {
		// ゲームオブジェクトマネージャーが有効なら
		for (auto aoctor : m_Actors)
		{
			aoctor->ChaseEffectDelete();
			DeleteGO(aoctor);
		}
	}

	for (int i = 0; i < 2; i++) {
		DeleteGO(m_gamecamera[i]);

		if (player[i] != nullptr)
		{
			DeleteGO(player[i]);
		}
	}

	if (GameObjectManager::GetInstance()->IsActive()) {
		TowerDown->Stop();
	}

	DeleteGO(m_gameUI);
	DeleteGO(m_Map);
	DeleteGO(m_bgm);

	QueryGOs<Lamp>("lamp", [&](Lamp* lamp_) {
		DeleteGO(lamp_);
		return true;
		});
	DeleteGO(m_skyCube);

}

bool Game::Start()
{
	//エフェクト読み込み
	SetEffects();

	Vector3 directionLightDir = Vector3{ 0.0f,-1.0f,-1.0f };
	directionLightDir.Normalize();
	Vector3 directionLightColor = DIRECTION_RIGHT_COLOR;
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor);
	g_renderingEngine->SetAmbient(AMBIENT_COLOR);
	//暗いときの画面のカラー
	//ディレクションライト
	m_FluctuateDirectionColor = DIRECTION_RIGHT_COLOR.x;
	//環境光
	m_FluctuateAmbientColor = AMBIENT_COLOR;


	InitSkyCube();

	fade = FindGO<Fade>("fade");
	//フェードインしているのでフェードアウトする
	//画面を明るくする
	fade->StartFadeOut(1.0f, Fade::enFadeSpriteType_Full, Fade::enFadeSpriteCategory_Tip);


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

	//ソロプレイだったら
	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
	{
		InitSoloPlay();
	}
	//複数人プレイだったら
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
	{
		InitDuoPlay();
	}
	SetKnightPlayerActor();

	//ゲームの状態をゲームステートにする
	m_GameState = enGameState_Start;

	//ゲーム中に再生される音を読み込む
	SetMusic();

	//当たり判定の可視化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}

void Game::Update()
{
	//制限時間に達したら
	if (GameEndFlag == true)
	{
		m_GameState = enGamestate_End;
		GameState();
	}

	GameState();
}

void Game::BattleStart()
{
	if (fade->GetCurrentAlpha() > 0.0f)
	{
		return;
	}

	m_StartToGameTimer -= g_gameTime->GetFrameDeltaTime();

	if (m_StartToGameTimer < 0)
	{
		//中立の敵の生成
		{
			m_Enemylevel.Init("Assets/level3D/RabbitPatrolPos.tkl", [&](LevelObjectData& objData) {
				//座標を記憶する
				if (objData.ForwardMatchName(L"Pos") == true) {
					SetRespawnPosition(objData.position, objData.rotation, objData.number);
				}

				//リスポーン番号1～8まで生成
				//中立の敵の生成
				if (objData.number != 0 && objData.number <= 8) {
					CreateEnemy(objData.position, objData.rotation);
					return true;
				}

				return true;
				});
		}

		//マップの生成
		m_Map = NewGO<Map>(2, "map");
		//BGMの再生
		m_bgm = NewGO<SoundSource>(0);
		m_bgm->Init(enSound_PlayBattle);
		m_bgm->Play(true);
		m_bgm->SetVolume(BGMVolume);
		m_GameState = enGameState_Battle;
		//ゲームUIのステートをgameStateにする
		m_gameUI->SetGameUIState(GameUI::m_GameState);
		TowerEFK();
	}
}

//バトルステートの処理
void Game::Battle()
{
	//↑の表示の処理
	if (player[0]->GetCharcterPosition().y <= 10 && m_underSprite_TowerDown == false)
	{
		m_underSprite_TowerDown = true;
		UnderSpriteUpdate();
	}

	//foractor


	//誰かが必殺技の溜め状態なら
	if (UltTimeSkyFlag == true) {
		UltTimeSkyDarkness();
	}
	else
	{
		LightReset();
	}

	//ポーズステートに変える
	//スタートボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Pause_Screen);
		se->SetVolume(SoundEffectVolume);
		se->Play(false);
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
		m_gamecamera[g_renderingEngine->EnCameraDrawing::enCameraDrawing_Left]->SetCameraState(m_gamecamera[g_renderingEngine->EnCameraDrawing::enCameraDrawing_Left]->enPauseState);
		if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
		{
			m_gamecamera[g_renderingEngine->EnCameraDrawing::enCameraDrawing_Right]->SetCameraState(m_gamecamera[g_renderingEngine->EnCameraDrawing::enCameraDrawing_Right]->enPauseState);
		}
		//生成されている中立の敵のステートをポーズステートに変更
		for (auto seutral_Enemy : m_neutral_Enemys)
		{
			seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Pause);
		}

		pause = NewGO<Pause>(0, "pause");
	}

	//誰かが必殺技を使ったら処理を止める
	if (UltStopFlag == true)
	{
		return;
	}

	//時間切れではないなら
	if (GameEndFlag == false) {
		CountDown();
	}

	m_RespawnTimer += g_gameTime->GetFrameDeltaTime();
	if (m_RespawnTimer >= 5.0f) {
		Respawn();
		m_RespawnTimer = 0.0f;
	}
	m_RabbitRespawnTimer += g_gameTime->GetFrameDeltaTime();
	if (m_RabbitRespawnTimer >= 30.0f)
	{
		RabbitRespawn();
		m_RabbitRespawnTimer = 0.0f;
	}

	if (UltCanUseFlag == true)
	{
		UltCanUseTimer -= g_gameTime->GetFrameDeltaTime();

		if (UltCanUseTimer <= 0.0f)
		{
			UltCanUseFlag = false;
			UltCanUseTimer = 0.0f;
		}
	}

}

//タイムアップになったあとの処理
void Game::End()
{
	m_EndtoResultTimer += g_gameTime->GetFrameDeltaTime();

	if (m_EndtoResultTimer >= 5.0f)
	{
		m_GameState = enGameState_Rezult;
		fade->StartFadeIn(1.0f);
	}
}

void Game::GoResult()
{
	if (fade->GetCurrentAlpha() >= 1.0f)
		Result* result = NewGO<Result>(0, "Result");
	g_renderingEngine->SetGameModeToRenderingEngine(RenderingEngine::enGameMode_SoloPlay);
	//DeleteGO(this);
}

//ポーズステートからゲームステートに移るときの間の処理
void Game::Between()
{
	m_BetweenTimer++;
	if (m_BetweenTimer >= 1)
	{
		//生成されているキャラのステートをゲームステートに戻す
		for (auto character : m_Actors)
		{
			character->ChangeGameState(character->enGame);
		}
		//UIのステートをゲームステートに戻す
		m_gameUI->SetGameUIState(m_gameUI->m_GameState);
		//カメラのステートをゲームステートに戻す
		if (g_renderingEngine->GetGameMode() != RenderingEngine::enGameMode_SoloPlay)
		{
			for (int i = 0; i < RenderingEngine::enGameMode_Num - 1; i++) {
				m_gamecamera[i]->SetCameraState(m_gamecamera[i]->enGameState);
			}
		}
		else {
			m_gamecamera[RenderingEngine::enGameMode_SoloPlay - 1]->SetCameraState(m_gamecamera[RenderingEngine::enGameMode_SoloPlay - 1]->enGameState);
		}
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
		int spawnAmount = ENEMY_AMOUNT - (int)m_neutral_Enemys.size();
		for (int generate = 0; generate < spawnAmount; generate++) {
			//ランダムに番号を決める(0以外)
			RandamRespawnPosNumber = rand() % 19 + 1;

			//中立の敵のリスポーンする座標を決める
			SetEnemyRespawnPos();
			//中立の敵を生成
			CreateEnemy(EnemyRespawnPosition[SearchRespawnPosNumber], EnemyReapawnPot[SearchRespawnPosNumber], false);

		}
	}
}

void Game::RabbitRespawn()
{
	if (RabbitFlag == true) {
		return;
	}

	//　乱数を初期化。
	srand((unsigned)time(NULL));
	RabbitFlag = true;

	//ランダムに番号を決める(0以外)
	RandamRespawnPosNumber = rand() % 19 + 1;	//中立の敵のリスポーンする座標を決める
	SetEnemyRespawnPos();
	//中立の敵を生成
	CreateEnemy(EnemyRespawnPosition[SearchRespawnPosNumber], EnemyReapawnPot[SearchRespawnPosNumber], true);

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
	if (SearchRespawnPosNumber == 0)
	{
		MaxSearchNumber = 19;
	}

	while (SearchRespawnPosNumber != MaxSearchNumber)
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
		if (SearchRespawnPosNumber < 19)
		{
			SearchRespawnPosNumber++;
		}
		else  //番号が8になったら
		{
			SearchRespawnPosNumber = 0;
		}


	}
	//ループでどの座標にも生成できなかったらランダムに決める
	SearchRespawnPosNumber = rand() % 19 + 1;
	return;
}

void Game::TowerEFK()
{
	TowerDown = NewGO <EffectEmitter>(0);
	TowerDown->Init(EnEFK::enEffect_TowerDown);
	TowerDown->SetScale(Vector3::One * 30.0f);
	TowerDown->Play();
	Vector3 EFKPOS = m_EFK_Pos;
	TowerDown->SetPosition(EFKPOS);
	Quaternion Efk_Rot = Quaternion::Identity;
	Efk_Rot.AddRotationDegY(255.0f);
	TowerDown->SetRotation(Efk_Rot);
	TowerDown->Update();
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
		PauseTime();
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

void Game::InitSoloPlay()
{
	//画面分割をしない
	g_renderingEngine->SetGameModeToRenderingEngine(RenderingEngine::enGameMode_SoloPlay);

	//プレイヤーの生成
	player[0] = NewGO<Player>(0, "player");
	player[0]->SelectCharcter(SelectCharNumber);
	player[0]->CreatePlayer();
	m_Actors.push_back(player[0]->GetPlayerActor());

	//カメラの生成
	m_gamecamera[0] = NewGO<GameCamera>(1, "gamecamera");
	m_gamecamera[0]->SetSplitCameraLR(m_gamecamera[0]->enSplitCamera_Left);

	m_AIPos.Init("Assets/level3D/AIPOS3.tkl", [&](LevelObjectData& objData) {
		if (objData.ForwardMatchName(L"CharPos") == true) {
			//左上の座標
			if (objData.number == 0) {
				m_KnightAI[0] = NewGO<KnightAI>(0, "KnightAI");
				m_KnightAI[0]->SetGame(this);
				m_Actors.push_back(m_KnightAI[0]);
				m_KnightAI[0]->SetPosition(objData.position);
				m_KnightAI[0]->SetCharaconPosition(objData.position);
				int Number = 0;
				m_KnightAI[0]->SetRespawnNumber(Number);
				m_KnightAI[0]->SetKnightColor(KnightBase::enKnightKinds_Red);

				return true;
			}
			//右上の座標
			if (objData.number == 1) {
				m_KnightAI[1] = NewGO<KnightAI>(0, "KnightAI1");
				m_KnightAI[1]->SetGame(this);
				m_Actors.push_back(m_KnightAI[1]);
				m_KnightAI[1]->SetPosition(objData.position);
				m_KnightAI[1]->SetCharaconPosition(objData.position);
				int Number = 1;
				m_KnightAI[1]->SetRespawnNumber(Number);
				m_KnightAI[1]->SetKnightColor(KnightBase::enKnightKinds_Green);

				return true;
			}
			//左下の座標
			if (objData.number == 3) {
				m_KnightAI[2] = NewGO<KnightAI>(0, "KnightAI2");
				m_KnightAI[2]->SetGame(this);
				m_Actors.push_back(m_KnightAI[2]);
				m_KnightAI[2]->SetPosition(objData.position);
				m_KnightAI[2]->SetCharaconPosition(objData.position);
				int Number = 3;
				m_KnightAI[2]->SetRespawnNumber(Number);
				m_KnightAI[2]->SetKnightColor(KnightBase::enKnightKinds_Yellow);

				return true;
			}
			if (objData.number == 4)
			{
				m_EFK_Pos = objData.position;
			}
			return true;
		}
		return true;
		});


	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "m_gameUI");
	m_gameUI->SetSGame(this);


	m_underSprite.Init("Assets/sprite/TowerDown.DDS", 886.0f, 255.0f);
	m_underSprite.SetPosition(TOWER_X_POS);
	m_underSprite.SetScale(SpriteScale);
	m_underSprite.Update();

	m_RabbitSprite.Init("Assets/sprite/rabbit.DDS", 886.0f, 255.0f);
	m_RabbitSprite.SetPosition(RABBIT_POS);
	m_RabbitSprite.SetScale(RabbitSpriteScale);
	m_RabbitSprite.Update();
}

void Game::InitDuoPlay()
{
	//画面分割をする
	g_renderingEngine->SetGameModeToRenderingEngine(RenderingEngine::enGameMode_DuoPlay);

	//プレイヤーの生成
	player[0] = NewGO<Player>(0, "player");
	player[0]->SelectCharcter(SelectCharNumber);
	player[0]->CreatePlayer();
	m_Actors.push_back(player[0]->GetPlayerActor());

	player[1] = NewGO<Player>(0, "player2");
	player[1]->SelectCharcter(SelectCharNumber);
	player[1]->CreatePlayer();
	m_Actors.push_back(player[1]->GetPlayerActor());

	//カメラの生成
	m_gamecamera[0] = NewGO<GameCamera>(1, "gamecamera");
	m_gamecamera[0]->SetSplitCameraLR(m_gamecamera[0]->enSplitCamera_Left);
	m_gamecamera[1] = NewGO<GameCamera>(1, "gamecamera2P");
	m_gamecamera[1]->SetSplitCameraLR(m_gamecamera[1]->enSplitCamera_Right);

	m_AIPos.Init("Assets/level3D/AIPOS3.tkl", [&](LevelObjectData& objData) {
		if (objData.ForwardMatchName(L"CharPos") == true) {
			//左上の座標
			if (objData.number == 0) {

				return true;
			}
			//右上の座標
			if (objData.number == 1) {
				m_KnightAI[0] = NewGO<KnightAI>(0, "KnightAI1");
				m_KnightAI[0]->SetGame(this);
				m_Actors.push_back(m_KnightAI[0]);
				m_KnightAI[0]->SetPosition(objData.position);
				m_KnightAI[0]->SetCharaconPosition(objData.position);
				int Number = 1;
				m_KnightAI[0]->SetRespawnNumber(Number);
				m_KnightAI[0]->SetKnightColor(KnightBase::enKnightKinds_Green);

				return true;
			}
			//左下の座標
			if (objData.number == 3) {
				m_KnightAI[1] = NewGO<KnightAI>(0, "KnightAI2");
				m_KnightAI[1]->SetGame(this);
				m_Actors.push_back(m_KnightAI[1]);
				m_KnightAI[1]->SetPosition(objData.position);
				m_KnightAI[1]->SetCharaconPosition(objData.position);
				int Number = 3;
				m_KnightAI[1]->SetRespawnNumber(Number);
				m_KnightAI[1]->SetKnightColor(KnightBase::enKnightKinds_Yellow);

				return true;
			}
			if (objData.number == 4)
			{
				m_EFK_Pos = objData.position;
			}
			return true;
		}
		return true;
		});


	//GameUIの生成
	m_gameUI = NewGO<GameUI>(0, "m_gameUI");
	m_gameUI->SetSGame(this);


	m_underSprite.Init("Assets/sprite/TowerDown.DDS", 886.0f, 255.0f);
	m_underSprite.SetPosition(TOWER_X_POS);
	m_underSprite.SetScale(SpriteScale);
	m_underSprite.Update();

	m_RabbitSprite.Init("Assets/sprite/rabbit.DDS", 886.0f, 255.0f);
	m_RabbitSprite.SetPosition(RABBIT_POS);
	m_RabbitSprite.SetScale(RabbitSpriteScale);
	m_RabbitSprite.Update();
}

void Game::SetKnightPlayerActor()
{
	for (int i = 0; i < m_KnightAI.size(); i++)
	{
		if (m_KnightAI[i] == nullptr)
		{
			continue;
		}

		if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
		{
			m_KnightAI[i]->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
		}
		else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
		{
			m_KnightAI[i]->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
			m_KnightAI[i]->SetKnightPlayerActor(player[1]->GetPlayerActor(), Actor::enPlayerNumber_2P);
		}
		else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay)
		{
			m_KnightAI[i]->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
			m_KnightAI[i]->SetKnightPlayerActor(player[1]->GetPlayerActor(), Actor::enPlayerNumber_2P);
			m_KnightAI[i]->SetKnightPlayerActor(player[2]->GetPlayerActor(), Actor::enPlayerNumber_3P);
		}
		else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
		{
			m_KnightAI[i]->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
			m_KnightAI[i]->SetKnightPlayerActor(player[1]->GetPlayerActor(), Actor::enPlayerNumber_2P);
			m_KnightAI[i]->SetKnightPlayerActor(player[2]->GetPlayerActor(), Actor::enPlayerNumber_3P);
			m_KnightAI[i]->SetKnightPlayerActor(player[3]->GetPlayerActor(), Actor::enPlayerNumber_4P);
		}
	}

	for (int i = 0; i < player.size(); i++)
	{
		if (player[i] == nullptr)
		{
			continue;
		}

		if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
		{
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
		}
		else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
		{
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[1]->GetPlayerActor(), Actor::enPlayerNumber_2P);
		}
		else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay)
		{
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[1]->GetPlayerActor(), Actor::enPlayerNumber_2P);
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[2]->GetPlayerActor(), Actor::enPlayerNumber_3P);
		}
		else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
		{
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[0]->GetPlayerActor(), Actor::enPlayerNumber_1P);
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[1]->GetPlayerActor(), Actor::enPlayerNumber_2P);
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[2]->GetPlayerActor(), Actor::enPlayerNumber_3P);
			player[i]->GetKnightPlayer()->SetKnightPlayerActor(player[3]->GetPlayerActor(), Actor::enPlayerNumber_4P);
		}
	}
}

//中立の敵の生成処理
void Game::CreateEnemy(Vector3 pos, Quaternion rot, bool isRabiit) {

	enemyNumber++;

	ENEMY_AMOUNT;

	Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(1, CreateEnemyName());
	neutral_Enemy->SetNeutral_EnemyGame(this);

	//ゲームモードによって渡すプレイヤーの情報を変更する
	//1人プレイのとき
	if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
	{
		neutral_Enemy->SetPlayerActor(player[Actor::EnPlayerNumber::enPlayerNumber_1P]->GetPlayerActor());
	}
	//2人プレイのとき
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
	{
		neutral_Enemy->SetPlayerActor(player[Actor::EnPlayerNumber::enPlayerNumber_1P]->GetPlayerActor(),
			player[Actor::EnPlayerNumber::enPlayerNumber_2P]->GetPlayerActor());
	}
	//3人プレイのとき
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay)
	{
		neutral_Enemy->SetPlayerActor(player[Actor::EnPlayerNumber::enPlayerNumber_1P]->GetPlayerActor(),
			player[Actor::EnPlayerNumber::enPlayerNumber_2P]->GetPlayerActor(),
			player[Actor::EnPlayerNumber::enPlayerNumber_3P]->GetPlayerActor());
	}
	//4人プレイのとき
	else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
	{
		neutral_Enemy->SetPlayerActor(player[Actor::EnPlayerNumber::enPlayerNumber_1P]->GetPlayerActor(),
			player[Actor::EnPlayerNumber::enPlayerNumber_2P]->GetPlayerActor(),
			player[Actor::EnPlayerNumber::enPlayerNumber_3P]->GetPlayerActor(),
			player[Actor::EnPlayerNumber::enPlayerNumber_4P]->GetPlayerActor());
	}

	neutral_Enemy->SetPosition(pos);
	neutral_Enemy->SetRotation(rot);
	if (isRabiit == true)
	{
		neutral_Enemy->ChangeRabbit();
		neutral_Enemy->SetRabbitLifeFlag(true);
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Rabbit_Spawn);
		se->SetVolume(SoundEffectVolume);
		se->Play(false);
	}
	neutral_Enemy->modelUpdate();

	m_neutral_Enemys.push_back(neutral_Enemy);
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->SetScale(600.0f);
	m_skyCube->SetLuminance(SKYCOLOR);
	m_FluctuateSkyColor = SKYCOLOR;
	DarknessSkyColor = DARKNESS_SKY_COLOR;
}

/// <summary>
/// ゲーム中に再生される音の読み込み
/// </summary>
void Game::SetMusic()
{
	//バトル中のBGM
	{
		//ゲームBGM
		g_soundEngine->ResistWaveFileBank(enSound_PlayBattle, "Assets/sound/gameBGM/SentouBGM2.wav");
		//ポーズ音
		g_soundEngine->ResistWaveFileBank(enSound_Pause_Screen, "Assets/sound/menu/game_stop/gameStop1.wav");
		//ゲーム終了
		g_soundEngine->ResistWaveFileBank(enSound_GameEnd, "Assets/sound/playerSE/CountEnd/GameEnd1.wav");

	}
	//試合開始前
	{
		//カウントダウン3
		g_soundEngine->ResistWaveFileBank(enSound_CountDown3, "Assets/sound/playerSE/CountStart/CountVoice3.wav");
		//カウントダウン2
		g_soundEngine->ResistWaveFileBank(enSound_CountDown2, "Assets/sound/playerSE/CountStart/CountVoice2.wav");
		//カウントダウン2
		g_soundEngine->ResistWaveFileBank(enSound_CountDown1, "Assets/sound/playerSE/CountStart/CountVoice1.wav");
		//ファイト
		g_soundEngine->ResistWaveFileBank(enSound_CountFight, "Assets/sound/playerSE/CountStart/Fight3.wav");
	}
	//効果音
	{
		//剣士
		{
			//スキルの音
			g_soundEngine->ResistWaveFileBank(enSound_Sword_Skill, "Assets/sound/playerSE/kenSkill3.wav");
			//手を上に上げるときの音
			g_soundEngine->ResistWaveFileBank(enSound_Hand, "Assets/sound/playerSE/handup2.wav");
			//雷の落下音
			g_soundEngine->ResistWaveFileBank(enSound_Sword_Ult, "Assets/sound/playerSE/thunder/thunder5.wav");
			//被ダメの声
			g_soundEngine->ResistWaveFileBank(enSound_Knight_Receiving_Damage, "Assets/sound/playerSE/playerScream1.wav");
			//通常攻撃のコンボ音
			g_soundEngine->ResistWaveFileBank(enSound_ComboONE, "Assets/sound/kenSE/ken1.wav");
			g_soundEngine->ResistWaveFileBank(enSound_ComboTwo, "Assets/sound/kenSE/ken2.wav");
			g_soundEngine->ResistWaveFileBank(enSound_ComboThree, "Assets/sound/kenSE/ken3.wav");
			//必殺チャージ音
			g_soundEngine->ResistWaveFileBank(enSound_Knight_Charge_Power, "Assets/sound/playerSE/ult2.wav");
			//死んだときの声
			g_soundEngine->ResistWaveFileBank(enSound_Knight_Death, "Assets/sound/playerSE/playerScream3.wav");
			//地面についたときの音
			g_soundEngine->ResistWaveFileBank(enSound_Metal_Falling, "Assets/sound/playerSE/fall/playerfall1.wav");
			//レベルアップの音
			g_soundEngine->ResistWaveFileBank(enSound_Level_UP, "Assets/sound/playerSE/Level_Up/playerLevelup1.wav");
			//レベルダウンの音
			g_soundEngine->ResistWaveFileBank(enSound_Level_Down, "Assets/sound/playerSE/Level_Down/LevelDown2.wav");
			//ヒール音
			g_soundEngine->ResistWaveFileBank(enSound_Healing, "Assets/sound/playerSE/heal/playerheal1.wav");

		}

		//中立の敵
		{
			//中立の敵の鳴き声
			g_soundEngine->ResistWaveFileBank(enSound_Enemy_Death, "Assets/sound/enemySE/enemyKoe.wav");
			//中立の敵が死んだときの音
			g_soundEngine->ResistWaveFileBank(enSound_Enemy_Voice, "Assets/sound/enemySE/rabbit/Die/rabbitDie1.wav");
			//ウサギが死んだときの音
			g_soundEngine->ResistWaveFileBank(enSound_Rabbit_Death, "Assets/sound/enemySE/rabbit/Die/rabbitDie2.wav");
			//ウサギの足音
			g_soundEngine->ResistWaveFileBank(enSound_Rabbit_FootSteps, "Assets/sound/enemySE/rabbit/Jump/rabbitJump1.wav");
			//ウサギが出現した時の音
			g_soundEngine->ResistWaveFileBank(enSound_Rabbit_Spawn, "Assets/sound/enemySE/rabbit/Spawn/rabbitspawn1.wav");
		}

	}

}

void Game::SetEffects()
{
	//魔法陣のエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_MasicCircle, u"Assets/effect/Knight/Knight_Ult_MagicCircle.efk");

	//剣士のレベル変動する時のエフェクト
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_LevelUp, u"Assets/effect/Knight/LevelUp.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_LevelDown, u"Assets/effect/knight/LevelDown.efk");
	//剣士の死んだときのエフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Death, u"Assets/effect/Knight/DeathTrue.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_LevelUp, u"Assets/effect/Knight/LevelUp.efk");

	//剣士が死んで倒れた時のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Death_Blue, u"Assets/effect/Knight/Knight_Death_Blue.efk");

	//剣士の必殺技エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Thunder, u"Assets/effect/Knight/Knight_Thunder.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Thunder_Charge, u"Assets/effect/Knight/Knight_Ult_Thunder_charge.efk");

	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Ult_Delete_Blue, u"Assets/effect/Knight/Knight_Ult_full.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Ult_Blue, u"Assets/effect/Knight/Knight_Ultimate.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Ult_Red, u"Assets/effect/Knight/Knight_Ultimate_Red.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Ult_Green, u"Assets/effect/Knight/Knight_Ultimate_Green.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Ult_Yellow, u"Assets/effect/Knight/Knight_Ultimate_Yellow.efk");

	//剣士の攻撃エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Attack, u"Assets/effect/Knight/Knight_Attack_default.efk");
	/*EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Atk_Blue, u"Assets/effect/Knight/Knight_Attack_blue.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Atk_Red, u"Assets/effect/Knight/Knight_Attack_Red.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Atk_Green, u"Assets/effect/Knight/Knight_Attack_green.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Atk_Yellow, u"Assets/effect/Knight/Knight_Attack_yellow.efk");*/

	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_TowerJump, u"Assets/effect/Knight/TowerJump.efk");
	//剣士のスキル使用時のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Skill, u"Assets/effect/Knight/Knight_Skill_Effect.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_SkillGround, u"Assets/effect/Knight/Knight_SkillGround_Effect.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_FootSmoke, u"Assets/effect/Knight/footsmoke.efk");
	//剣士の必殺技発動時のオーラエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_Ult_Aura, u"Assets/effect/Knight/knight_ULT_swordEffect.efk");

	//アタックチェックエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_AttackChack, u"Assets/effect/Knight/Knight_Attack_Check.efk");

	//中立の敵を倒すとき得るもののエフェクト
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_GetHoimi, u"Assets/effect/Knight/Knight_GetHoimi.efk");

	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_PowerUP, u"Assets/effect/Knight/Knight_PowerUp2.efk");
	//エフェクトを読み込む。
	//中立の敵の攻撃、死亡時エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Neutral_Enemy_head_butt, u"Assets/effect/Neutral_Enemy/head-butt1.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Neutral_Enemy_Death, u"Assets/effect/Neutral_Enemy/death.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Neutral_Enemy_WhiteMagic, u"Assets/effect/Neutral_enemy/white_magic.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Neutral_Enemy_GreenMagic, u"Assets/effect/Neutral_enemy/green_magic.efk");
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Rabbit_Magic, u"Assets/effect/Neutral_enemy/rabbit_magic.efk");
	//タワーから降りるを示す↑のエフェクト
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_TowerDown, u"Assets/effect/Tower/TowerDown.efk");
	//ウサギがキラキラするエフェクト
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Rabbit_kirakira, u"Assets/effect/Neutral_Enemy/kirakira.efk");
	g_renderingEngine->UnUseHemiLight();
}

//ポーズ中の処理
void Game::PauseTime()
{
	if (pause == nullptr)
	{
		pause = FindGO<Pause>("pause");
	}

	if (pause->IsDeleteGame() == true)
	{
		//Push_OK();
		Tittle* m_tittle = NewGO<Tittle>(0, "m_tittle");

		g_renderingEngine->SetGameModeToRenderingEngine(RenderingEngine::enGameMode_SoloPlay);

		DeleteGO(this);
	}
}

void Game::Push_OK()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(enSound_OK);
	se->SetVolume(SoundEffectVolume);
	se->Play(false);
}

//制限時間の処理
void Game::CountDown()
{
	//0秒以下なら
	if (m_secondsTimer <= 0) {
		//1分減らす
		m_minutesTimer--;
		//もし0分なら、秒も0にする
		if (m_minutesTimer < 0) {
			m_secondsTimer = 0.0f;
			m_minutesTimer = 0.0f;
			GameEndFlag = true;
		}
		//60秒に戻す
		else m_secondsTimer = 60.0f;
	}
	else m_secondsTimer -= g_gameTime->GetFrameDeltaTime();
}

void Game::UltTimeSkyDarkness()
{
	//ディレクションライトを徐々に暗くする
	if (DARKNESS_DIRECTION < m_FluctuateDirectionColor) {
		m_FluctuateDirectionColor -= 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		m_FluctuateDirectionColor = DARKNESS_DIRECTION;
	}

	//環境光を徐々に暗くする
	if (DARKNESS_AMBIENT.x < m_FluctuateAmbientColor.x) {
		m_FluctuateAmbientColor.x -= 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateAmbientColor.x = DARKNESS_AMBIENT.x;
	}

	if (DARKNESS_AMBIENT.y < m_FluctuateAmbientColor.y) {
		m_FluctuateAmbientColor.y -= 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateAmbientColor.y = DARKNESS_AMBIENT.y;
	}

	if (DARKNESS_AMBIENT.z < m_FluctuateAmbientColor.z) {
		m_FluctuateAmbientColor.z -= 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateAmbientColor.z = DARKNESS_AMBIENT.z;
	}

	//スカイキューブのカラーを暗くする
	if (DARKNESS_SKY_COLOR < m_FluctuateSkyColor) {
		m_FluctuateSkyColor -= 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateSkyColor = DARKNESS_SKY_COLOR;
	}


	m_skyCube->SetLuminance(m_FluctuateSkyColor);
	Vector3 directionLightDir = Vector3{ 0.0f,-1.0f,-1.0f };

	directionLightDir.Normalize();
	directionLightColor2 = Vector3::Zero;
	directionLightColor2.x += m_FluctuateDirectionColor;
	directionLightColor2.y += m_FluctuateDirectionColor;
	directionLightColor2.z += m_FluctuateDirectionColor;
	//Vector3 directionLightColor = Vector3::One* m_FluctuateDirectionColor;
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor2);
	g_renderingEngine->SetAmbient(m_FluctuateAmbientColor);
}

void Game::LightReset()
{
	//ディレクションライトを徐々に明るくする
	if (DIRECTION_RIGHT_COLOR.x > m_FluctuateDirectionColor) {
		m_FluctuateDirectionColor += 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else
	{
		m_FluctuateDirectionColor = DIRECTION_RIGHT_COLOR.x;
	}

	//環境光を徐々に明るくする
	if (AMBIENT_COLOR.x > m_FluctuateAmbientColor.x) {
		m_FluctuateAmbientColor.x += 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateAmbientColor.x = AMBIENT_COLOR.x;
	}

	if (AMBIENT_COLOR.y > m_FluctuateAmbientColor.y) {
		m_FluctuateAmbientColor.y += 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateAmbientColor.y = AMBIENT_COLOR.y;
	}

	if (AMBIENT_COLOR.z > m_FluctuateAmbientColor.z) {
		m_FluctuateAmbientColor.z += 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateAmbientColor.z = AMBIENT_COLOR.z;
	}

	//スカイキューブのカラーを明るくする
	if (SKYCOLOR > m_FluctuateSkyColor) {
		m_FluctuateSkyColor += 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_FluctuateSkyColor = SKYCOLOR;
	}

	m_skyCube->SetLuminance(m_FluctuateSkyColor);
	Vector3 directionLightDir = Vector3{ 0.0f,-1.0f,-1.0f };

	directionLightDir.Normalize();
	directionLightColor2 = Vector3::Zero;
	directionLightColor2.x += m_FluctuateDirectionColor;
	directionLightColor2.y += m_FluctuateDirectionColor;
	directionLightColor2.z += m_FluctuateDirectionColor;
	g_renderingEngine->SetDirectionLight(0, directionLightDir, directionLightColor2);
	g_renderingEngine->SetAmbient(m_FluctuateAmbientColor);
}

void Game::ToggleObjectActive(bool IsUltFlag, Actor* targetActor)
{
	//for (auto actor : m_Actors) {
	//	//必殺技発動時のカメラのターゲットでないなら
	//	if (actor != targetActor) {
	//		//必殺技発動中なら描画しない
	//		if(IsUltFlag==true) actor->SetDarwFlag(false);

	//		else actor->SetDarwFlag(true);
	//	}
	//}
	for (auto enemy : m_neutral_Enemys) {
		//必殺技発動中なら
		if (IsUltFlag == true) enemy->Deactivate();
		//必殺技終了なら
		else enemy->Activate();
	}
}

int Game::GetNowRank(Actor* actor)
{
	int rank = 1;
	for (int i = 0; i < m_Actors.size(); i++)
	{
		if (m_Actors[i] == actor) {
			continue;
		}
		if (m_Actors[i]->GetPoint() > actor->GetPoint())
		{
			rank++;
		}
	}
	return rank;
}

void Game::Render(RenderContext& rc)
{
	if (UltStopFlag == true) {
		return;
	}

	if (player[0]->CharGetRespawnTime() <= 0) {
		if (RabbitFlag == true && m_GameState == enGameState_Battle)
		{
			if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
			{
				m_RabbitSprite.Draw(rc);
			}

		}

		if (m_underSprite_Ult == false && m_GameState == enGameState_Battle) {

			if (m_underSprite_Attack && m_underSprite_Skill && m_underSprite_Level == false) {
				return;
			}
			if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_SoloPlay)
			{
				m_underSprite.Draw(rc);
			}
		}
	}


}
