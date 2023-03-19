#pragma once
#include "Actor.h"

class Game;
class GameUI;

class WizardBase:public Actor
{
public:
	WizardBase();
	virtual ~WizardBase();

	enum WizardState
	{
		enWizardState_Idle,
		enWizardState_Walk,
		enWizardState_Run,
		enWizardState_Attack,
		enWizardState_Damege,
		enWizardState_Death,
		enWizardState_Skill,
		enWizardState_UltimateSkill,
		enWizardState_Avoidance,
		enWizardState_Run,
		enWizardState_Pause,
		enWizardState_Num,
	};

	void SetModel();

	/// <summary>
	/// �����̓G��|�����Ƃ��̌o���l�̏���
	/// </summary>
	/// <param name="GetExp">�����̓G�̌o���l</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack() = 0;

	/// <summary>
	/// �_���[�W���󂯂��Ƃ��̏���
	/// </summary>
	/// <param name="damege">�G�̍U����</param>
	void Dameged(int damege);

	/// <summary>
	/// ���g���|���ꂽ�Ƃ��̏���
	/// </summary>
	void Death();

	/// <summary>
	/// �K�E�Z�𔭓������Ƃ��̏���
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// ���X�|�[��������W�̃Z�b�g
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �ʏ�U���̓����蔻��̏���
	/// </summary>
	void AtkCollisiton();

	/// <summary>
	/// �K�E�Z�̓����蔻��̏���
	/// </summary>
	void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// �A�j���[�V�����̃X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// �A�j���[�V�����C�x���g
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName) = 0;

	/// <summary>
	/// ���W�̃Z�b�g
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// ���@�g���̍��W��Ԃ�l�Ƃ��ĕԂ�
	/// </summary>
	virtual Vector3 GetPosition() = 0;

	/// <summary>
	/// ����̃A�j���[�V�������Đ����Ȃ�false��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return;
	}

	/// <summary>
	/// ���݂̃q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>���݂�HP</returns>
	int& SetHp() {
		return m_Status.Hp;
	}



	void SetSGame(Game* Cgame)
	{
		game = Cgame;
	}
	Game* GetSGame()
	{
		return game;
	}
	//
	void SetGameUI(GameUI* gameUI)
	{
		gameUI = gameUI;
	}
	GameUI* GetSGameUI()
	{
		return gameUI;
	}

private:

	//�����X�e�[�^�X �ő�HP�AHP�A�U���́A�X�s�[�h
	Status m_Status;


	Game* game = nullptr;
	GameUI* gameUI = nullptr;

	Vector3 m_position = Vector3::Zero;
};

