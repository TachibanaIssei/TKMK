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
	/// マジックボールの生成
	/// </summary>
	void MakeMagicBall();

	


	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵
	//MagicBall* magicBall = nullptr;

	CollisionObject* AtkCollision;                   //アタックのコリジョン
	Vector3 AtkCollisionPos = Vector3::Zero;
	Vector3 AtkCollisionVec = Vector3::Zero;
	//アタックのコリジョンを作成したかの判定
	bool AtkCollisionSetFlag = false;
	bool AttackFlag = false;
	float AttackTimer = 0.0f;


	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;

	bool m_spriteFlag = true;

	int exp = 5;

};

