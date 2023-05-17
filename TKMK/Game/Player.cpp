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
	//DeleteGO(knightPlayer);
	//DeleteGO(wizardPlayer);
}

void Player::CreaetPlayer()
{
	game = FindGO<Game>("game");
	gameUI = FindGO<GameUI>("m_gameUI");

	//選択されたキャラを生成する
	switch (m_selectCharctar)
	{
	case enKnight:
		//剣士プレイヤーを生成
		knightPlayer = NewGO<KnightPlayer>(0, "knightplayer");
		knightPlayer->SetSGame(game);
		knightPlayer->SetGameUI(gameUI);
		knightPlayer->SetAIorPlayer(Actor::EnAIorPlayer::enPlayer);
		knightPlayer->SetKnightColor(KnightBase::enKnightKinds_Blue);
		player_name = knightPlayer->GetName();
		playerActor = knightPlayer;
		break;

	case enWizard:
		//魔法使いプレイヤーの生成
		wizardPlayer = NewGO<WizardPlayer>(0, "wizardPlayer");
		wizardPlayer->SetSGame(game);
		wizardPlayer->SetGameUI(gameUI);
		wizardPlayer->SetAIorPlayer(Actor::EnAIorPlayer::enPlayer);
		playerActor = wizardPlayer;
		break;

	default:
		break;
	}
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
/// キャラの座標を取得カメラで使う
/// </summary>
/// <returns>選択されているキャラの座標</returns>
Vector3 Player::GetCharPosition()const
{
	return playerActor->GetPosition();
	/*switch (m_selectCharctar)
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
	}*/
}

/// <summary>
/// キャラの現在のレベルを返す
/// </summary>
/// <returns>選択されたキャラの現在のレベルを返す関数</returns>
int Player::CharSetLevel()const
{
	return playerActor->GetLevel();
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
/// キャラの前方向を取得するカメラで使う
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

/// <summary>
/// キャラのスプライトフラグを返す
/// </summary>
/// <returns>選択されたキャラのスプライトフラグを返す関数</returns>
bool Player::CharSetSpriteFlag()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->GetSpriteFlag();
		break;

	case enWizard:
		return wizardPlayer->GetSpriteFlag();
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
/// プレイヤーのポイントを返す
/// </summary>
/// <returns>プレイヤーの現在のポイント</returns>
int Player::CharSetPoint()const
{
	return playerActor->GetPoint();
}

/// <summary>
/// プレイヤーの現在の経験値を取得
/// </summary>
/// <returns>プレイヤーの現在の経験値の量</returns>
int Player::CharSetEXP() const
{
	return playerActor->GetExperience();
}

/// <summary>
/// プレイヤーの現在の経験値テーブルを取得
/// </summary>
/// <returns>プレイヤーの現在の経験値テーブル</returns>
int Player::CharSetEXPTable() const
{
	return playerActor->GetExpTable();
}

/// <summary>
/// プレイヤーの前のレベルの経験値テーブルを取得
/// </summary>
/// <returns></returns>
int Player::CharSetOldEXPTable() const
{
	return playerActor->GetOldExpTable();
}

float Player::CharGetRespawnTime() const
{
	return playerActor->GetRespawnTimer();
}