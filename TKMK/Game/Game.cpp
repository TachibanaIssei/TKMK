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
	/*DeleteGO(m_knightplayer);
	DeleteGO(wizardPlayer);*/
	
	DeleteGO(m_gameUI);
	DeleteGO(m_Map);
	//DeleteGO(m_KnightAI);
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

	//�X�^�W�A���̐���
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


	//GameUI�̐���
	m_gameUI = NewGO<GameUI>(0, "m_gameUI");
	m_gameUI->SetSGame(this);

	//�v���C���[�̐���
	player = NewGO<Player>(0, "player");
	//��������L�����I��

	player->CharSelect(SelectCharNumber);
	player->CreaetPlayer();
	m_Actors.push_back(player->GetPlayerActor());

	//�Q�[���J�����̐���
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	//m_gamecamera->SetKnight(m_knightplayer);

	//�����̓G�̐���
	m_Enemylevel.Init("Assets/level3D/enemyRespawnPos.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Pos") == true) {
			//����̍�W
			if (objData.number == 0) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				return true;
			}
			//�E��̍�W
			if (objData.number == 1) {

				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				CreateEnemy(objData.position, objData.rotation);
				return true;
			}
			if (objData.number == 2) {
				//���X�|�[����W�̐ݒ�
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				return true;
			}
			//�E���̍�W
			if (objData.number == 3) {
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				enemyNumber++;
				ENEMY_AMOUNT;
				CreateEnemy(objData.position, objData.rotation);
				return true;
			}
			if (objData.number == 4) {
				//���X�|�[����W�̐ݒ�
				SetRespawnPosition(objData.position, objData.rotation, objData.number);
				return true;
			}
			//�����̍�W
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

	/*m_Neutral_Enemy = NewGO<Neutral_Enemy>(0, "Neutral_Enemy");
	m_Neutral_Enemy->SetNeutral_EnemyGame(this);
	m_Neutral_Enemy->SetKnightPlayer(m_knightplayer);*/
	//m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");

	//�Ƃ肠�����R�����g�A�E�g
	m_KnightAI = NewGO<KnightAI>(0, "KnightAI");
	m_KnightAI->SetGame(this);
	m_Actors.push_back(m_KnightAI);
	
	//�}�b�v�̐���
	m_Map = NewGO<Map>(2, "map");

	//�|�[�Y��ʂ̔w�i�̐ݒ�
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

	//�Q�[���̃X�e�[�g��Q�[���ɂ���
	m_GameState = enGameState_Battle;

	//BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/gameBGM/SentouBGM1.wav");
	//se
	//player
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
	//enemy
	//�U���̐�
	g_soundEngine->ResistWaveFileBank(21, "Assets/sound/enemySE/enemyKoe.wav");
	
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(2);
	m_bgm->Play(true);
	m_bgm->SetVolume(musicVolume);


	//�����蔻��̉���
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

//�o�g���X�e�[�g���̏���
void Game::Battle()
{
	if (m_GameState == enGameState_Battle) {
		//���U���g��ʂւ̑J��
		//CTRL�������ꂽ��B
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			m_GameState = enGameState_Rezult;
		}
	}

	//�|�[�Y��ʂւ̑J��
	//�X�^�[�g�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		//�Q�[����ʂ���|�[�Y��ʂɑJ�ڂ��鎞�̏���
			m_GameState = enGameState_Pause;
			//�v���C���[�AAI�̃X�e�[�g��|�[�Y��ʗp�̃X�e�[�g�ɕύX
			for (auto character : m_Actors)
			{
				character->ChangeGameState(character->enPause);
			}
			//UI�̃X�e�[�g��|�[�Y��ʗp�̃X�e�[�g�ɕύX
			m_gameUI->SetGameUIState(m_gameUI->m_PauseState);
			//�J�����̃X�e�[�g��|�[�Y��ʗp�̃X�e�[�g�ɕύX
			m_gamecamera->SetCameraState(m_gamecamera->enPauseState);
			//�����̓G��|�[�Y��ʗp�̃X�e�[�g�ɕύX
			////�z��̃T�C�Y�𒲂ׂ�for���ŉ�
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

//�|�[�Y��ʂ̏���
void Game::Pause()
{
	//�X�^�[�g�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		//�|�[�Y��ʂ���Q�[����ʂɖ߂鎞�̏���
			m_GameState = enGameState_Battle;
			//�v���C���[�̃X�e�[�g��|�[�Y��ʗp�̃X�e�[�g�ł͂Ȃ��悤�ɂ���
			// //�v���C���[�AAI�̃X�e�[�g��|�[�Y��ʗp�̃X�e�[�g�ɕύX
			for (auto character : m_Actors)
			{
				character->ChangeGameState(character->enGame);
			}
			//UI�̃X�e�[�g��Q�[���̃X�e�[�g�ɕύX
			m_gameUI->SetGameUIState(m_gameUI->m_GameState);
			//�J�����̃X�e�[�g��Q�[���̃X�e�[�g�ɕύX
			m_gamecamera->SetCameraState(m_gamecamera->enGameState);
			//�����̓G��|�[�Y��ʗp�̃X�e�[�g�ɕύX
			////�z��̃T�C�Y�𒲂ׂ�for���ŉ�
			for (auto seutral_Enemy : m_neutral_Enemys)
			{
				seutral_Enemy->SetNeutral_EnemyState(seutral_Enemy->enNeutral_Enemy_Idle);
			}
	}

	//���ʂ�グ��
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		if(musicVolume<4.0f)
		musicVolume += 0.1f;
		//���ʒ���
		m_bgm->SetVolume(musicVolume);
	}
	//���ʂ����
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		if(musicVolume>0)
		musicVolume -= 0.1f;
		//���ʒ���
		m_bgm->SetVolume(musicVolume);
	}
	


	//�^�C�g����ʂւ̑J��
	//A�{�^���������
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle*m_tittle = NewGO<Tittle>(0,"m_tittle");
		DeleteGO(this);
	}
}

//�o�g���I�����̏���
void Game::End()
{

}

/// <summary>
/// �����̓G�̃��X�|�[���̏���
/// </summary>
void Game::Respawn()
{
	for (int count = 0; count < 10; count++)
	{
		EnemyRespawnFlag[count] = false;
	}

	//�����̓G�̑����Ɛ�������Ă��钆���̓G�̐����Ⴄ�Ȃ�
	if (ENEMY_AMOUNT != m_neutral_Enemys.size()) {
		//����Ă��Ȃ�����v�Z����
		int spawnAmount = ENEMY_AMOUNT - m_neutral_Enemys.size();
		for (int generate = 0; generate < spawnAmount; generate++) {
			//�����_���Ƀ��X�|�[�������W�̔ԍ���߂�
			RandamRespawnPosNumber = rand() % 8 + 1;

			//���X�|�[�������W��߂�
			SetEnemyRespawnPos();
			//�����̓G����
			CreateEnemy(EnemyRespawnPosition[SearchRespawnPosNumber], EnemyReapawnPot[SearchRespawnPosNumber]);
		}
	}
}

/// <summary>
/// �����̓G�̃��X�|�[�������W��߂�
/// </summary>
/// <returns></returns>
void Game::SetEnemyRespawnPos()
{
	//�����_���ɑI�񂾔ԍ�����
	SearchRespawnPosNumber = RandamRespawnPosNumber;
	//�����ԍ��̍ő�l
	int MaxSearchNumber = SearchRespawnPosNumber - 1;
	//�I�񂾔ԍ���0�Ȃ�
	if(SearchRespawnPosNumber==0)
	{
		MaxSearchNumber = 8;
	}

	while (SearchRespawnPosNumber!= MaxSearchNumber)
	{
		if (EnemyRespawnFlag[SearchRespawnPosNumber] == false)
		{
			//�A�N�^�[���狗�������ȏ㗣��Ă��邩�̃J�E���^�[
			int distanceCounter = 0;
			for (auto actorPos : m_Actors)
			{
				Vector3 CharPos = actorPos->GetPosition();
				//���X�|�[�������W����L�����̍�W�ւ̃x�N�g����v�Z����
				Vector3 diff = EnemyRespawnPosition[SearchRespawnPosNumber] - CharPos;
				//�x�N�g���̒�����700�ȏ�Ȃ�
				if (diff.Length() > 600)
				{
					//�J�E���^�[�̒l+1
					distanceCounter++;
				}

			}
			//�L�����̐��Ƌ����J�E���g�������Ȃ甲���o��
			if (distanceCounter == m_Actors.size())
			{
				//
				EnemyRespawnFlag[SearchRespawnPosNumber] = true;
				//���̒i�K��SearchRespawnPosNumber�����X�|�[�������W�̔ԍ��ɂȂ�
				return;
			}
		}
		
		//���X�|�[�������W�̔ԍ���8�ł͂Ȃ��Ȃ�
		if (SearchRespawnPosNumber < 8)
		{
			SearchRespawnPosNumber++;
		}
		else  //���X�|�[�������W�̔ԍ����Ō�(8)�܂ł������
		{
			SearchRespawnPosNumber = 0;
		}
		
		
	}
	//�S�Ă̍�W�Ń��X�|�[�������W��ݒ�ł��Ȃ������
	SearchRespawnPosNumber= rand() % 8 + 1;
	return;
}

//�Q�[���X�e�[�g�̊Ǘ�
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
		//���U���g��ʂ̐����A�Q�[���̍폜
		Result* result = NewGO<Result>(0, "Result");
		DeleteGO(this);
		break;

	/*default:
		break;*/
	}
}

void Game::CreateEnemy(Vector3 pos, Quaternion rot) {

	enemyNumber++;
	ENEMY_AMOUNT;

	Neutral_Enemy* neutral_Enemy = NewGO<Neutral_Enemy>(0, CreateEnemyName());
	neutral_Enemy->SetNeutral_EnemyGame(this);
	neutral_Enemy->SetPlayerActor(player->GetPlayerActor());
	neutral_Enemy->SetPosition(pos);
	neutral_Enemy->SetRotation(rot);

	m_neutral_Enemys.push_back(neutral_Enemy);
	//m_neutral_Enemys.erase(m_neutral_Enemys.);
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
