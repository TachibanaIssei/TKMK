#pragma once
#include "KnightPlayer.h"
#include "WizardPlayer.h"


#include "Status.h"

class Status;
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

	//�v���C���[�̃X�e�[�g��
	enum EnPlayerStateChange
	{
		enPause,
		enGame,
	};
	EnPlayerStateChange m_playerStateChange;
	
	/// <summary>
	/// �v���C���[�𐶐�����
	/// </summary>
	void CreaetPlayer();

	/// <summary>
	/// ��������L������I��
	/// </summary>
	/// <param name="number">�L�����̔ԍ�</param>
	void CharSelect(int number);

	/// <summary>
	/// �L�����̍��W���擾
	/// </summary>
	/// <returns>�I������Ă���L�����̍��W</returns>
	Vector3 GetCharPosition()const;

	/// <summary>
	/// �L�����̌��݂̃��x����Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̌��݂̃��x����Ԃ��֐�</returns>
	int CharSetLevel()const;

	/// <summary>
	/// �L�����̌��݂̃q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̌��݂̃q�b�g�|�C���g��Ԃ��֐�</returns>
	int CharSetHp()const;

	/// <summary>
	/// �L�����̍ő�q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̍ő�q�b�g�|�C���g��Ԃ��֐�</returns>
	int CharSetMaxHp()const;

	/// <summary>
	/// �L�����̉�]�ʂ�Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̉�]�ʂ�Ԃ��֐�</returns>
	Quaternion CharSetRot()const;
	
	/// <summary>
	/// �L�����̑O������Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̑O������Ԃ��֐�	</returns>
	Vector3 CharSetForward()const;

	/// <summary>
	/// �L�����̃X�v���C�g�t���O��Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̃X�v���C�g�t���O��Ԃ��֐�</returns>
	bool CharSetSpriteFlag()const;

	/// <summary>
	/// �v���C���[�̃|�C���g��Ԃ�
	/// </summary>
	/// <returns>�v���C���[�̌��݂̃|�C���g</returns>
	int CharSetPoint()const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	float CharGetRespawnTime()const;

	Actor* GetPlayerActor() {
		return playerActor;
	}

	int CharSetEXP() const;

	int CharSetEXPTable() const;

	//�v���C���[�̃X�e�[�g���擾
	//auto CharGetState() const;
	auto CharGetState() const
	{
		return playerActor->NowCharState();
	}
private:

	Game* game = nullptr;
	GameUI* gameUI = nullptr;
	KnightPlayer* knightPlayer = nullptr;
	WizardPlayer* wizardPlayer = nullptr;
	Status m_Status;

	Actor* playerActor = nullptr;

};

