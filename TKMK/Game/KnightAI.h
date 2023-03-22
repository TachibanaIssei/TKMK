#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;
class KnightPlayer;
class Neutral_Enemy;

class KnightAI:public KnightBase
{
public:
	KnightAI();
	~KnightAI();
	void Update();
	void Attack();
	void Render(RenderContext& rc);
	void ChaseAI();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();
	void SetGame(Game* game)
	{
		m_game = game;
	}
	Game* GetGame()
	{
		return m_game;
	}

private:
	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	AtkTimingState m_AtkTmingState = Num_State;
	Game* m_game;//ゲーム
	KnightPlayer* m_knightPlayer;//剣士プレイヤーvoid Rotation();
	void SearchEnemy();
	void Rotation();
	Quaternion				m_rotation;
	//Vector3					m_position;
	Vector3                 m_moveSpeed;
	Vector3					m_forward;
	bool					m_isSearchEnemy = false;
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;							//コライダー。
	RigidBody				m_rigidBody;						//剛体。	
	Neutral_Enemy*          m_Neutral_Enemy = nullptr;
	bool UltimateSkillFlag = false;
	float SkillSpeed = 270.0f;
	float UltimateSkillTimer = 0;

};

