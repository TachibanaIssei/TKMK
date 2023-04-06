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

		playerActor = knightPlayer;
		break;

	case enWizard:
		//���@�g���v���C���[�̐���
		wizardPlayer = NewGO<WizardPlayer>(0, "wizardPlayer");
		wizardPlayer->SetSGame(game);
		wizardPlayer->SetGameUI(gameUI);

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
/// �L�����̌��݂̃��x����Ԃ�
/// </summary>
/// <returns>�I�����ꂽ�L�����̌��݂̃��x����Ԃ��֐�</returns>
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
		break;

	case enMonster:
		break;


	default:
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
		break;

	case enMonster:
		break;


	default:
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
		break;

	case enMonster:
		break;


	default:
		break;
	}
}

/// <summary>
/// �L�����̃X�e�[�g��ύX����
/// </summary>
/// <returns>�I�����ꂽ�L�����̃X�e�[�g��ύX����֐�</returns>
void Player::CharSetState(EnPlayerStateChange gameState)const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		//�Q�[���̃X�e�[�g���|�[�Y�Ȃ�
		if (gameState == enPause)knightPlayer->SetPlayerState(KnightPlayer::enKnightState_Pause);
		//�Q�[���̃X�e�[�g���Q�[���Ȃ�
		if (gameState == enGame)knightPlayer->SetPlayerState(KnightPlayer::enKnightState_Idle);
		break;

	case enWizard:
		//�Q�[���̃X�e�[�g���|�[�Y�Ȃ�
		if (gameState == enPause)wizardPlayer->SetPlayerState(WizardPlayer::enWizardState_Pause);
		//�Q�[���̃X�e�[�g���Q�[���Ȃ�
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
/// ?L??????U???????
/// </summary>
/// <returns>?I????L??????U??????????</returns>
//int Player::CharSetAttack()const
//{
//	//switch (m_selectCharctar)
//	//{
//	//case enKnight:
//	//	return knightPlayer->SetKnightAtk();
//	//	break;
//
//	////???@?g????}?W?b?N?{?[????U??????????K?v???
//
//	//case enZombie:
//	//	break;
//
//	//case enMonster:
//	//	break;
//
//
//	//default:
//	//	break;
//	//}
//}

/// <summary>
/// ?L??????exp?????
/// </summary>
/// <param name="exp">??????G??o???l</param>
void Player::CharSetExpProcess(int exp)const
{
	switch (m_selectCharctar)
	{
	case enKnight:
		return knightPlayer->ExpProcess(exp);
		break;

	case enWizard:
		return wizardPlayer->ExpProcess(exp);
		break;

	case enZombie:
		break;

	case enMonster:
		break;


	default:
		break;
	}
}

