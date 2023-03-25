#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "GameUI.h"
//#include "KnightPlayer.h"
//#include "WizardPlayer.h"

Player::Player()
{

}

Player::~Player()
{
	DeleteGO(knightPlayer);
	DeleteGO(wizardPlayer);
}

bool Player::Start()
{

	game = FindGO<Game>("game");
	gameUI = FindGO<GameUI>("m_gameUI");

	//選択されたキャラを生成する
	switch (m_selectCharctar)
	{
	case enKnight:
		//剣士プレイヤーを生成
		knightPlayer = NewGO<KnightPlayer>(0, "m_knightplayer");
		knightPlayer->SetSGame(game);
		knightPlayer->SetGameUI(gameUI);
		break;

	case enWizard:
		//魔法使いプレイヤーの生成
		wizardPlayer = NewGO<WizardPlayer>(0, "wizardPlayer");
		wizardPlayer->SetSGame(game);
		wizardPlayer->SetGameUI(gameUI);
		break;

	default:
		break;
	}


	
	//int a = 0;
	return true;
}

/// <summary>
	/// 生成するキャラを選ぶ
	/// </summary>
	/// <param name="number">キャラの番号</param>
void Player::CharSelect(int number)
{
	switch (number)
	{
	case 0:
		m_selectCharctar = enKnight;
		break;
	case 1:
		m_selectCharctar = enWizard;
		break;
	case 2:
		m_selectCharctar = enZombie;
		break;
	case 3:
		m_selectCharctar = enMonster;
		break;

	default:
		break;
	}
}

/// <summary>
/// キャラの座標を取得
/// </summary>
/// <returns>選択されているキャラの座標</returns>
Vector3 Player::GetCharPosition()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->GetPosition();
		break;

	case enWizard:
		return wizardPlayer->GetPosition();
		break;

	case enZombie:
		break;

	case enMonster:
		break;

	default:
		break;
	}
}

/// <summary>
/// キャラの現在のレベルを返す
/// </summary>
/// <returns>選択されたキャラの現在のレベルを返す関数</returns>
int Player::CharSetLevel()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->SetLevel();
		break;
	case enWizard:
		return wizardPlayer->SetLevel();
		break;
	case enZombie:
		break;
	case enMonster:
		break;

	default:
		break;
	}
}

/// <summary>
/// キャラの現在のヒットポイントを返す
/// </summary>
/// <returns>選択されたキャラの現在のヒットポイントを返す関数</returns>
int Player::CharSetHp()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->SetHp();
		break;

	case enWizard:
		return wizardPlayer->SetHp();
		break;

	case enZombie:
		break;

	case enMonster:
		break;


	default:
		break;
	}
}

/// <summary>
/// キャラの最大ヒットポイントを返す
/// </summary>
/// <returns>選択されたキャラの最大ヒットポイントを返す関数</returns>
int Player::CharSetMaxHp()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->SetMaxHp();
		break;

	case enWizard:
		return wizardPlayer->SetMaxHp();
		break;

	case enZombie:
		break;

	case enMonster:
		break;


	default:
		break;
	}
}

/// <summary>
/// キャラの回転量を返す
/// </summary>
/// <returns>選択されたキャラの回転量を返す関数</returns>
Quaternion Player::CharSetRot()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->GetRot();
		break;

	case enWizard:
		return wizardPlayer->GetRot();
		break;

	case enZombie:
		break;

	case enMonster:
		break;


	default:
		break;
	}
}

/// <summary>
/// キャラのステートを変更する
/// </summary>
/// <returns>選択されたキャラのステートを変更する関数</returns>
void Player::CharSetState(EnPlayerStateChange gameState)const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		//ゲームのステートがポーズなら
		if (gameState == enPause)knightPlayer->SetPlayerState(KnightPlayer::enKnightState_Pause);
		//ゲームのステートがゲームなら
		if (gameState == enGame)knightPlayer->SetPlayerState(KnightPlayer::enKnightState_Idle);
		break;

	case enWizard:

		//ゲームのステートがポーズなら
		if (gameState == enPause)wizardPlayer->SetPlayerState(WizardPlayer::enWizardState_Pause);
		//ゲームのステートがゲームなら
		if (gameState == enGame)wizardPlayer->SetPlayerState(WizardPlayer::enWizardState_Idle);
		break;

	case enZombie:
		break;

	case enMonster:
		break;


	default:
		break;
	}
}

/// <summary>
/// キャラの前方向を取得する
/// </summary>
/// <returns>選択されたキャラの前方向を取得する関数</returns>
Vector3 Player::CharSetForward()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->GetForward();
		break;

	case enWizard:
		return wizardPlayer->GetForward();
		break;

	case enZombie:
		break;

	case enMonster:
		break;


	default:
		break;
	}
}


