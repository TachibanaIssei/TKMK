#pragma once
#include "KnightPlayer.h"
#include "WizardPlayer.h"


#include "Status.h"

struct Status;
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
	// TODO:public�ɂȂ��Ă���̂�private��
	SelectCharctar m_selectCharctar;

	//�v���C���[�̃X�e�[�g��
	enum EnPlayerStateChange
	{
		enPause,
		enGame,
	};
	// TODO:public�ɂȂ��Ă���̂�private��
	EnPlayerStateChange m_playerStateChange;
	
	/// <summary>
	/// �v���C���[�𐶐�����
	/// </summary>
	void CreatePlayer();

	/// <summary>
	/// ��������L������I��
	/// </summary>
	/// <param name="number">�L�����̔ԍ�</param>
	void SelectCharcter(int number);

	/// <summary>
	/// �L�����̍��W���擾
	/// </summary>
	/// <returns>�I������Ă���L�����̍��W</returns>
	Vector3 GetCharcterPosition()const;

	/// <summary>
	/// �L�����̌��݂̃��x����Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̌��݂̃��x����Ԃ��֐�</returns>
	int GetCharacterLevel()const;

	/// <summary>
	/// �L�����̌��݂̃q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̌��݂̃q�b�g�|�C���g��Ԃ��֐�</returns>
	int GetCharacterHp()const;

	/// <summary>
	/// �L�����̍ő�q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̍ő�q�b�g�|�C���g��Ԃ��֐�</returns>
	int GetCharcterMaxHp()const;

	/// <summary>
	/// �L�����̉�]�ʂ�Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̉�]�ʂ�Ԃ��֐�</returns>
	Quaternion GetCharcterRotation()const;
	
	/// <summary>
	/// �L�����̑O������Ԃ�
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̑O������Ԃ��֐�	</returns>
	Vector3 GetCharcterForward()const;

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
		return m_playerActor;
	}

	/// <summary>
	/// ���m�v���C���[�̃|�C���^��Ԃ�
	/// </summary>
	KnightPlayer* GetKnightPlayer()
	{
		return m_knightPlayer;
	}

	int CharGetEXP() const;

	int CharSetEXPTable() const;

	int CharGetEXPTableForLevel(int Level) const;

	int CharGetSaveEXP() const;

	void CharResatSaveEXP(int num) const;

	float CharGetSkillCoolTimer() const;

	//�v���C���[�̃X�e�[�g���擾
	//auto CharGetState() const;
	auto CharGetState() const
	{
		return m_playerActor->NowCharState();
	}

	/// <summary>
	/// �v���C���[�̖��O���擾����
	/// </summary>
	/// <returns></returns>
	const char* GetName() {
		return m_playerName;
	}
private:

	Game* m_game = nullptr;
	GameUI* m_gameUI = nullptr;
	KnightPlayer* m_knightPlayer = nullptr;
	WizardPlayer* m_wizardPlayer = nullptr;
	Status m_status;

	Actor* m_playerActor = nullptr;
	const char* m_playerName = nullptr;
};

