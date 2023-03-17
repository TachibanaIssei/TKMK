#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;
class KnightPlayer;

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
	Game* m_game;//ゲーム
	KnightPlayer* m_knightPlayer;//剣士プレイヤーvoid Rotation();
	void SearchPlayer();
	void Rotation();
	Quaternion				m_rotation;
	Vector3					m_position;
	Vector3					m_forward;
	bool					m_isSearchPlayer = false;
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;							//コライダー。
	RigidBody				m_rigidBody;						//剛体。	
};

