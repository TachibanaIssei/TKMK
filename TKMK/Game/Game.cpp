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
//#include <vector>
//#include <algorithm>
//タワーのエフェクト消すときエラーあり
namespace {
	const int ENEMY_AMOUNT = 10;
	const Vector3 Menu_BackPos = Vector3(0.0f, 210.0f, 0.0f);
	const Vector3 Menu_HowToPlayPos = Vector3(0.0f, 60.0f, 0.0f);
	const Vector3 Menu_BGMPos = Vector3(-245.0f, -70.0f, 0.0f);
	const Vector3 Menu_SEPos = Vector3(-245.0f, -185.0f, 0.0f);
	const Vector3 Menu_QuitGamePos = Vector3(0.0f, -320.0f, 0.0f);
	const Vector3 Menu_SelectBar_BGMPos = Vector3(90.0f, -68.0f, 0.0f);
	const Vector3 Menu_SelectBar_SEPos = Vector3(90.0f, -183.0f, 0.0f);
	const Vector3 TOWEREXPOSITION_POS = Vector3(20.0f, -410.0f, 0.0f);
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
	DeleteGO(m_gamecamera);

	//�����̓G�̃T�C�Y�𒲂ׂ�for���ŉ�
	for (auto seutral_Enemy : m_neutral_Enemys)
	{
		DeleteGO(seutral_Enemy);
	}
	//m_neutral_Enemys.clear();
	if (GameObjectManager::GetInstance()->IsActive()) {
		// ゲームオブジェクトマネージャーが有効なら
		for (auto aoctor : m_Actors)
		{
			//たまにエラーあり
			aoctor->ChaseEffectDelete();
			DeleteGO(aoctor);
		}
	}

	DeleteGO(player);
  
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
	//DeleteGO(lamp);
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
	fade->StartFadeOut(1.0f);
	

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
	m_gamecamera = NewGO<GameCamera>(1, "gamecamera");

	

	



	m_AIPos.Init("Assets/level3D/AIPOS3.tkl", [&](LevelObjectData& objData) {		
			if (objData.ForwardMatchName(L"CharPos") == true) {
				//左上の座標

				if (objData.number == 0) {
					m_KnightAI = NewGO<KnightAI>(0, "KnightAI");
					m_KnightAI->SetGame(this);
					m_Actors.push_back(m_KnightAI);
					m_KnightAI->SetPosition(objData.position);
					m_KnightAI->SetCharaconPosition(objData.position);
					m_KnightAI->SetPlayerActor(player->GetPlayerActor());
					int Number = 0;
					m_KnightAI->SetRespawnNumber(Number);
					m_KnightAI->SetKnightColor(KnightBase::enKnightKinds_Red);

					return true;
				}
				//右上の座標
				if (objData.number == 1) {
					m_KnightAI1 = NewGO<KnightAI>(0, "KnightAI1");
					m_KnightAI1->SetGame(this);
					m_Actors.push_back(m_KnightAI1);
					m_KnightAI1->SetPosition(objData.position);
					m_KnightAI1->SetCharaconPosition(objData.position);
					m_KnightAI1->SetPlayerActor(player->GetPlayerActor());
					int Number = 1;
					m_KnightAI1->SetRespawnNumber(Number);
					m_KnightAI1->SetKnightColor(KnightBase::enKnightKinds_Green);

					return true;
				}
				//左下の座標
				if (objData.number == 3) {
					m_KnightAI2 = NewGO<KnightAI>(0, "KnightAI2");
					m_KnightAI2->SetGame(this);
					m_Actors.push_back(m_KnightAI2);
					m_KnightAI2->SetPosition(objData.position);
					m_KnightAI2->SetCharaconPosition(objData.position);
					m_KnightAI2->SetPlayerActor(player->GetPlayerActor());
					int Number = 3;
					m_KnightAI2->SetRespawnNumber(Number);
					m_KnightAI2->SetKnightColor(KnightBase::enKnightKinds_Yellow);

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

		m_operationPic.Init("Assets/sprite/Controller.DDS", 1920.0f, 1080.0f);
		m_operationPic.SetPosition(Vector3::Zero);
		m_operationPic.SetScale(g_vec3One);
		m_operationPic.Update();


		m_underSprite.Init("Assets/sprite/TowerDown.DDS", 886.0f, 255.0f);
		m_underSprite.SetPosition(TOWEREXPOSITION_POS);
		m_underSprite.SetScale(SpriteScale);
		m_underSprite.Update();

		m_RabbitSprite.Init("Assets/sprite/rabbit.DDS", 886.0f, 255.0f);
		m_RabbitSprite.SetPosition(RABBIT_POS);
		m_RabbitSprite.SetScale(RabbitSpriteScale);
		m_RabbitSprite.Update();
	}


	//ゲームの状態をゲームステートにする
	m_GameState = enGameState_Start;

	//ゲーム中に再生される音を読み込む
	SetMusic();

	/*m_boxCollider.Create(Vector3(1.0f,1.0f,1.0f));*/

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
	if (player->GetCharPosition().y <= 10 && m_underSprite_TowerDown == false)
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
		se->SetVolume(1.0f);
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
			m_gamecamera->SetCameraState(m_gamecamera->enPauseState);
			//生成されている中立の敵のステートをポーズステートに変更
			for (auto seutral_Enemy : m_neutral_Enemys)
			{
				seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Pause);
			}
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
	if (m_RabbitRespawnTimer >= 5.0f)
	{
		RabbitRespawn();
		m_RabbitRespawnTimer = 0.0f;
	}

	if (UltStopFlag == true)
	{
		UltCanUseTimer -= g_gameTime->GetFrameDeltaTime();
		if (UltCanUseTimer <= 0)
		{
			UltCanUseFlag = false;
			UltCanUseTimer = 0.0f;
		}
	}
}

//ポーズ時の処理
void Game::Pause()
{
	if (HowToPlaySpriteFlag == false)
	{
		//ポーズ時の移動処理
		PauseMove();
	}
	
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
			RandamRespawnPosNumber = rand() % 19 + 1;

			//中立の敵のリスポーンする座標を決める
			SetEnemyRespawnPos();
			//中立の敵を生成
			CreateEnemy(EnemyRespawnPosition[SearchRespawnPosNumber], EnemyReapawnPot[SearchRespawnPosNumber],false);
			
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
	CreateEnemy(EnemyRespawnPosition[SearchRespawnPosNumber], EnemyReapawnPot[SearchRespawnPosNumber],true);
	
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
		MaxSearchNumber = 19;
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
	SearchRespawnPosNumber= rand() % 19 + 1;
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
	Quaternion Efk_Rot= Quaternion::Identity;
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
void Game::CreateEnemy(Vector3 pos, Quaternion rot, bool isRabiit) {
	
	enemyNumber++;

	ENEMY_AMOUNT;
	
	Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(1, CreateEnemyName());
	neutral_Enemy->SetNeutral_EnemyGame(this);
	neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
	neutral_Enemy->SetPosition(pos);
	neutral_Enemy->SetRotation(rot);
	if (isRabiit == true)
	{
		neutral_Enemy->ChangeRabbit();
		neutral_Enemy->SetRabbitLifeFlag(true);
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Rabbit_Spawn);
		se->SetVolume(1.0f);
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
		g_soundEngine->ResistWaveFileBank(enSound_PlayBattle, "Assets/sound/gameBGM/SentouBGM1.wav");
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
			g_soundEngine->ResistWaveFileBank(enSound_Hand, "Assets/sound/playerSE/kenSkill1.wav");
			//雷の落下音
			g_soundEngine->ResistWaveFileBank(enSound_Sword_Ult, "Assets/sound/playerSE/thunder/thunder2.wav");
			//被ダメの声
			g_soundEngine->ResistWaveFileBank(enSound_Knight_Receiving_Damage, "Assets/sound/playerSE/playerScream1.wav");
			//通常攻撃のコンボ音
			g_soundEngine->ResistWaveFileBank(enSound_ComboONE, "Assets/sound/kenSE/ken1.wav");
			g_soundEngine->ResistWaveFileBank(enSound_ComboTwo, "Assets/sound/kenSE/ken2.wav");
			g_soundEngine->ResistWaveFileBank(enSound_ComboThree, "Assets/sound/kenSE/ken3.wav");
			//必殺チャージ音
			g_soundEngine->ResistWaveFileBank(enSound_Knight_Charge_Power, "Assets/sound/playerSE/kenSkill1.wav");
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
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Knight_LevelDown,u"Assets/effect/knight/LevelDown.efk");
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

void Game::PauseMove()
{
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Title_Choise);
		se->SetVolume(1.0f);
		se->Play(false);
		MenuNumber++;
	}

	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Title_Choise);
		se->SetVolume(1.0f);
		se->Play(false);
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
		Push_OK();
		//ステートをバトルステートに戻す
		m_GameState = enGameState_BetweenGameAndPause;
	}
}
//HowToPlayの処理
void Game::Menu_HowToPlay()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Push_OK();
		//フラグ反転
		HowToPlaySpriteFlag = !HowToPlaySpriteFlag;
	}
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
void Game::Push_OK()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(enSound_OK);
	se->SetVolume(1.0f);
	se->Play(false);
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
		Push_OK();
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

void Game::Render(RenderContext& rc)
{
	
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
		
		if (HowToPlaySpriteFlag == true)
		{
			m_operationPic.Draw(rc);
		}
	}
	
	if (UltStopFlag == true) {
		return;
	}

	if (RabbitFlag == true && m_GameState == enGameState_Battle)
	{
		m_RabbitSprite.Draw(rc);

	}

	if (m_underSprite_Ult == false && m_GameState == enGameState_Battle) {

		if (m_underSprite_Attack && m_underSprite_Skill && m_underSprite_Level == false) {
			return;
		}

		m_underSprite.Draw(rc);
	}
	
	//m_fontRender.Draw(rc);
	
}
