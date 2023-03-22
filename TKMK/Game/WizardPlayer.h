#pragma once
#include "WizardBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;

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

	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //�����̓G

	//�X�L�����g�������̈ړ����x
	float SkillSpeed = 270.0f;
	//������g�������̈ړ����x
	float AvoidanceSpeed = 170.0f;

	bool m_spriteFlag = true;

	int exp = 5;

};

