#pragma once
#include "KnightPlayer.h"
#include "WizardPlayer.h"

class Game;
class GameUI;
class KnightPlayer;
class WizardPlayer;

class Player:public IGameObject/*,public KnightPlayer*/
{
public:
	Player();
	~Player();

	//�L�����I��
	enum SelectCharctar
	{
		enKnight,
		enWizard,
		enZombie,
		enMonster,
		enNum,
	};
	SelectCharctar m_selectCharctar;
	
	bool Start();

	/// <summary>
	/// ��������L������I��
	/// </summary>
	/// <param name="number">�L�����̔ԍ�</param>
	void CharSelect(int number)
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
	/// �L�����̍��W���擾
	/// </summary>
	/// <returns>�I������Ă���L�����̍��W</returns>
	Vector3 GetCharPosition()const
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
	int CharSetLevel()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->SetLevel();
			break;
		case enWizard:
			//wizardPlayer->Set
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
	int CharSetHp()const
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
	int CharSetMaxHp()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->SetMaxHp();
			break;

		case enWizard:
			//return wizardPlayer->SetMaxHp();
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
	Quaternion CharSetRot()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->GetRot();
			break;

		case enWizard:
			//return wizardPlayer->GetRot();
			break;

		case enZombie:
			break;

		case enMonster:
			break;


		default:
			break;
		}
	}




	Game* game = nullptr;
	GameUI* gameUI = nullptr;
	KnightPlayer* knightPlayer = nullptr;
	WizardPlayer* wizardPlayer = nullptr;

	

};

