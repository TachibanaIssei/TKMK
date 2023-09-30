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

	//�I�����ꂽ�L�����𐶐�����
	switch (m_selectCharctar)
	{
	case enKnight:
		//���m�v���C���[�𐶐�
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
		//���@�g���v���C���[�̐���
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
	/// ��������L������I��
	/// </summary>
	/// <param name="number">�L�����̔ԍ�</param>
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
/// �L�����̍��W���擾�J�����Ŏg��
/// </summary>
/// <returns>�I������Ă���L�����̍��W</returns>
Vector3 Player::GetCharcterPosition()const
{
	return m_playerActor->GetPosition();
}

/// <summary>
/// �L�����̌��݂̃��x����Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̌��݂̃��x����Ԃ��֐�</returns>
int Player::GetCharacterLevel()const
{
	return m_playerActor->GetLevel();
}

/// <summary>
/// �L�����̌��݂̃q�b�g�|�C���g��Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̌��݂̃q�b�g�|�C���g��Ԃ��֐�</returns>
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
/// �L�����̍ő�q�b�g�|�C���g��Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̍ő�q�b�g�|�C���g��Ԃ��֐�</returns>
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
/// �L�����̉�]�ʂ�Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̉�]�ʂ�Ԃ��֐�</returns>
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
/// �L�����̑O�������擾����J�����Ŏg��
/// </summary>
/// <returns>�I�����ꂽ�L�����̑O�������擾����֐�</returns>
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
/// �v���C���[�̃|�C���g��Ԃ�
/// </summary>
/// <returns>�v���C���[�̌��݂̃|�C���g</returns>
int Player::CharSetPoint()const
{
	return m_playerActor->GetPoint();
}

/// <summary>
/// �v���C���[�̌��݂̌o���l���擾
/// </summary>
/// <returns>�v���C���[�̌��݂̌o���l�̗�</returns>
int Player::CharGetEXP() const
{
	return m_playerActor->GetExperience();
}

/// <summary>
/// �v���C���[�̌��݂̌o���l�e�[�u�����擾
/// </summary>
/// <returns>�v���C���[�̌��݂̌o���l�e�[�u��</returns>
int Player::CharSetEXPTable() const
{
	return m_playerActor->GetExpTable();
}

int Player::CharGetEXPTableForLevel(int Level) const
{
	return m_playerActor->GetExpTableForLevel(Level);
}

//�Z�[�u���Ă���o���l���擾
int Player::CharGetSaveEXP() const
{
	return m_playerActor->GetSaveEXP();
}

//�Z�[�u�����o���l�����Z�b�g
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

