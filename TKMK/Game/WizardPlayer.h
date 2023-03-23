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

	/// <summary>
	/// �}�W�b�N�{�[���̐���
	/// </summary>
	void MakeMagicBall();

	


	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //�����̓G
	//MagicBall* magicBall = nullptr;

	CollisionObject* AtkCollision;                   //�A�^�b�N�̃R���W����
	Vector3 AtkCollisionPos = Vector3::Zero;
	Vector3 AtkCollisionVec = Vector3::Zero;
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

};

