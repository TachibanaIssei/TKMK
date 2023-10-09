#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "GameUI.h"

Player::Player()
{

}

Player::~Player()
{
}

void Player::CreatePlayer()
{
	m_game = FindGO<Game>("game");
	m_gameUI = FindGO<GameUI>("m_gameUI");

	//選択されたキャラを生成する
	switch (m_selectCharctar)
	{
	case enKnight:
		//剣士プレイヤーを生成
		if (IGameObject::m_name == "player2")
		{
			m_knightPlayer = NewGO<KnightPlayer>(0, "knightplayer2");
			m_knightPlayer->SetPlayerNumber(1);
			m_knightPlayer->SetKnightColor(KnightBase::enKnightKinds_Red);
		}
		else {
			m_knightPlayer = NewGO<KnightPlayer>(0, "knightplayer");
			m_knightPlayer->SetKnightColor(KnightBase::enKnightKinds_Blue);

		}
		
		m_knightPlayer->SetSGame(m_game);
		m_knightPlayer->SetGameUI(m_gameUI);
		m_knightPlayer->SetAIorPlayer(Actor::EnAIorPlayer::enPlayer);
		m_playerName = m_knightPlayer->GetName();

		m_playerActor = m_knightPlayer;
		break;

	case enWizard:
		//魔法使いプレイヤーの生成
		m_wizardPlayer = NewGO<WizardPlayer>(0, "wizardPlayer");
		m_wizardPlayer->SetSGame(m_game);
		m_wizardPlayer->SetGameUI(m_gameUI);
		m_wizardPlayer->SetAIorPlayer(Actor::EnAIorPlayer::enPlayer);
		m_playerActor = m_wizardPlayer;
		break;

	default:
		break;
	}
}

/// <summary>
	/// 生成するキャラを選ぶ
	/// </summary>
	/// <param name="number">キャラの番号</param>
void Player::SelectCharcter(int number)
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
Vector3 Player::GetCharcterPosition()const
{
	return m_playerActor->GetPosition();
}

/// <summary>
/// キャラの現在のレベルを返す
/// </summary>
/// <returns>選択されたキャラの現在のレベルを返す関数</returns>
int Player::GetCharacterLevel()const
{
	return m_playerActor->GetLevel();
}

/// <summary>
/// キャラの現在のヒットポイントを返す
/// </summary>
/// <returns>選択されたキャラの現在のヒットポイントを返す関数</returns>
int Player::GetCharacterHp()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return m_knightPlayer->GetHitPoint();
		break;

	case enWizard:
		return m_wizardPlayer->GetHitPoint();
		break;

	case enZombie:
		return -1;
		break;

	case enMonster:
		return -1;
		break;


	default:
		return -1;
		break;
	}
}

/// <summary>
/// キャラの最大ヒットポイントを返す
/// </summary>
/// <returns>選択されたキャラの最大ヒットポイントを返す関数</returns>
int Player::GetCharcterMaxHp()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return m_knightPlayer->GetMaxHp();
		break;

	case enWizard:
		return m_wizardPlayer->GetMaxHp();
		break;

	case enZombie:
		return -1;

		break;

	case enMonster:
		return -1;

		break;


	default:
		return -1;

		break;
	}
}

/// <summary>
/// キャラの回転量を返す
/// </summary>
/// <returns>選択されたキャラの回転量を返す関数</returns>
Quaternion Player::GetCharcterRotation()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return m_knightPlayer->GetRotation();
		break;

	case enWizard:
		return m_wizardPlayer->GetRotation();
		break;

	case enZombie:
		return Quaternion::Identity;
		break;

	case enMonster:
		return Quaternion::Identity;

		break;


	default:
		return Quaternion::Identity;

		break;
	}
}

/// <summary>
/// キャラの前方向を取得するカメラで使う
/// </summary>
/// <returns>選択されたキャラの前方向を取得する関数</returns>
Vector3 Player::GetCharcterForward()const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return m_knightPlayer->GetForward();
		break;

	case enWizard:
		return m_wizardPlayer->GetForward();
		break;
	}
}

/// <summary>
/// プレイヤーのポイントを返す
/// </summary>
/// <returns>プレイヤーの現在のポイント</returns>
int Player::CharSetPoint()const
{
	return m_playerActor->GetPoint();
}

/// <summary>
/// プレイヤーの現在の経験値を取得
/// </summary>
/// <returns>プレイヤーの現在の経験値の量</returns>
int Player::CharGetEXP() const
{
	return m_playerActor->GetExperience();
}

/// <summary>
/// プレイヤーの現在の経験値テーブルを取得
/// </summary>
/// <returns>プレイヤーの現在の経験値テーブル</returns>
int Player::CharSetEXPTable() const
{
	return m_playerActor->GetExpTable();
}

int Player::CharGetEXPTableForLevel(int Level) const
{
	return m_playerActor->GetExpTableForLevel(Level);
}

//セーブしている経験値を取得
int Player::CharGetSaveEXP() const
{
	return m_playerActor->GetSaveEXP();
}

//セーブした経験値をリセット
void Player::CharResatSaveEXP(int num) const
{
	m_playerActor->SetSaveEXP(num);
}

float Player::CharGetSkillCoolTimer() const
{
	return m_playerActor->GetSkillTimer();
}

float Player::CharGetRespawnTime() const
{
	return m_playerActor->GetRespawnTimer();
}

