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

	//�v���C���[�̃X�e�[�g��
	enum EnPlayerStateChange
	{
		enPause,
		enGame,
	};
	EnPlayerStateChange m_playerStateChange;
	
	bool Start();

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
	/// �L�����̃X�e�[�g��ύX����
	/// </summary>
	/// <returns>�I�����ꂽ�L�����̃X�e�[�g��ύX����֐�</returns>
	void CharSetState(EnPlayerStateChange gameState)const;
	
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

private:

	Game* game = nullptr;
	GameUI* gameUI = nullptr;
	KnightPlayer* knightPlayer = nullptr;
	WizardPlayer* wizardPlayer = nullptr;

	

};

