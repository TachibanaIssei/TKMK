#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;

class KnightAI:public KnightBase
{
public:
	KnightAI();
	~KnightAI();
	void Update();
	void Attack();
	void Render(RenderContext& rc);
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
	Game* m_game;//ÉQÅ[ÉÄ
};

