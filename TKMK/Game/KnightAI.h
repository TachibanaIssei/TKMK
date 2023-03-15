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
	void Rotation();
	void SearchPlayer();
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
	
	Quaternion				m_rotation;                    //回転
	Vector3					m_position ;                    //ポジション
	Vector3					m_forward;                      //前方向
	KnightPlayer* m_KnightPlayer = nullptr;                    //剣士プレイヤー
	bool					m_isSearchPlayer = false;          //サーチプレイヤー
	FontRender				m_fontRender;                       //文字
	SphereCollider			m_sphereCollider;							//コライダー。
	RigidBody				m_rigidBody;						//剛体。	
};

