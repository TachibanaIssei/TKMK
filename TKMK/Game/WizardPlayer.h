#pragma once
#include "WizardBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class MagicBall;

class WizardPlayer:public WizardBase
{
public:
	WizardPlayer();
	~WizardPlayer();

	void Update();
	void Attack();
	void Avoidance();
	void Render(RenderContext& rc);

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();

	/// <summary>
	/// �}�W�b�N�{�[���̐���
	/// </summary>
	void MakeMagicBall();

	/// <summary>
	/// �K�E�Z�̗��̐���
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// �v���C���[�̃X�e�[�g��ύX
	/// </summary>
	/// <param name="gamescene">�ύX�������X�e�[�g�̖��O</param>
	void SetPlayerState(WizardState gamescene) {
		m_wizardState = gamescene;

	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}

	//void Death()
	//{
	//	////���S�X�e�[�g
	//	//m_playerState = enKnightState_Death;
	//	//���x�����P������
	//	levelDown(LvUpStatus, m_Status, Lv, 1);
	//	//HP���ő�ɂ���
	//	m_Status.Hp = m_Status.MaxHp;
	//	//�o���l�����Z�b�g
	//	ExpReset(Lv, GetExp);
	//	//����̃��x���̌o���l�e�[�u���ɂ���
	//	ExpTableChamge(Lv, ExpTable);

	//	//���x���ɍ��킹�ă��x���̉摜��ύX����
	//	m_gameUI->LevelFontChange(Lv);
	//}

	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //�����̓G
	//MagicBall* magicBall = nullptr;

	CollisionObject* AtkCollision;                   //�A�^�b�N�̃R���W����
	Vector3 AtkCollisionPos = Vector3::Zero;
	Vector3 AtkCollisionVec = Vector3::Zero;
	
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;

	//�A�^�b�N�̃R���W�������쐬�������̔���
	bool AtkCollisionSetFlag = false;
	bool AttackFlag = false;
	float AttackTimer = 0.0f;


	//�X�L�����g�������̈ړ����x
	float SkillSpeed = 270.0f;
	//������g�������̈ړ����x
	float AvoidanceSpeed = 170.0f;

	bool m_spriteFlag = true;

	int exp = 5;
	int dddd = 10;
};

