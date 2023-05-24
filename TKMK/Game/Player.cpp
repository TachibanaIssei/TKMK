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

	//�I�����ꂽ�L�����𐶐�����
	switch (m_selectCharctar)
	{
	case enKnight:
		//���m�v���C���[�𐶐�
		knightPlayer = NewGO<KnightPlayer>(0, "knightplayer");
		knightPlayer->SetSGame(game);
		knightPlayer->SetGameUI(gameUI);
		knightPlayer->SetAIorPlayer(Actor::EnAIorPlayer::enPlayer);
		knightPlayer->SetKnightColor(KnightBase::enKnightKinds_Blue);
		player_name = knightPlayer->GetName();
		playerActor = knightPlayer;
		break;

	case enWizard:
		//���@�g���v���C���[�̐���
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
	/// ��������L������I��
	/// </summary>
	/// <param name="number">�L�����̔ԍ�</param>
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
/// �L�����̍��W���擾�J�����Ŏg��
/// </summary>
/// <returns>�I������Ă���L�����̍��W</returns>
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
/// �L�����̌��݂̃��x����Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̌��݂̃��x����Ԃ��֐�</returns>
int Player::CharSetLevel()const
{
	return playerActor->GetLevel();
}

/// <summary>
/// �L�����̌��݂̃q�b�g�|�C���g��Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̌��݂̃q�b�g�|�C���g��Ԃ��֐�</returns>
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
/// �L�����̃X�v���C�g�t���O��Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̃X�v���C�g�t���O��Ԃ��֐�</returns>
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
/// �v���C���[�̃|�C���g��Ԃ�
/// </summary>
/// <returns>�v���C���[�̌��݂̃|�C���g</returns>
int Player::CharSetPoint()const
{
	return playerActor->GetPoint();
}

/// <summary>
/// �v���C���[�̌��݂̌o���l���擾
/// </summary>
/// <returns>�v���C���[�̌��݂̌o���l�̗�</returns>
int Player::CharSetEXP() const
{
	return playerActor->GetExperience();
}

/// <summary>
/// �v���C���[�̌��݂̌o���l�e�[�u�����擾
/// </summary>
/// <returns>�v���C���[�̌��݂̌o���l�e�[�u��</returns>
int Player::CharSetEXPTable() const
{
	return playerActor->GetExpTable();
}

int Player::CharGetEXPTableForLevel(int Level) const
{
	return playerActor->GetExpTableForLevel(Level);
}

/// <summary>
/// �v���C���[�̑O�̃��x���̌o���l�e�[�u�����擾
/// </summary>
/// <returns></returns>
//int Player::CharSetOldEXPTable() const
//{
//	return 1;
//	//return playerActor->GetOldExpTable();
//}

//�Z�[�u���Ă���o���l���擾
int Player::CharGetSaveEXP() const
{
	return playerActor->GetSaveEXP();
}

//�Z�[�u�����o���l�����Z�b�g
void Player::CharResatSaveEXP(int num) const
{
	playerActor->SetSaveEXP(num);
}

float Player::CharGetSkillCoolTimer() const
{
	return playerActor->GetSkillTimer();
}

float Player::CharGetRespawnTime() const
{
	return playerActor->GetRespawnTimer();
}

