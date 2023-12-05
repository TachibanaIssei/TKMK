#pragma once
class Neutral_Enemy;
class Actor;
class Game;

class ExpforKnight : public IGameObject
{
public:
	ExpforKnight();
	~ExpforKnight();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Init(Vector3& pos,Actor* playerActor, const bool rabbitFlag);

	/// <summary>
	/// ベジェ曲線
	/// </summary>
	void Bezier();
	
	

private:
	void SetPosition(Vector3& pos)
	{
		m_enemyPos = pos;
	}
	//ウサギ用経験値画像へ切り替える
	void SetIsRabbitFlag(const bool flag) {
		m_isRabbit = flag;
	}
	void SetPlayerNumber();
	Vector2 SetExperienceBarPosition();

private:
	Actor* m_playerActor = nullptr;
	Game* m_game = nullptr;
	SpriteRender m_NormalExp;
	Vector3 m_enemyPos = Vector3::Zero;
	Vector2 m_experienceBarPosition = Vector2::Zero;

	//ベジェ曲線用
	Vector2 m_effectScreenPosition = Vector2::Zero;	//エフェクトのポジション
	Vector2 m_center = Vector2::Zero;
	Vector2 m_movePos = Vector2::Zero;
	Vector2 m_movePos2 = Vector2::Zero;
	Vector2 m_movePos3 = Vector2::Zero;

	int m_playerActorNumber = 0;
	int m_random = 0;
	float m_timer = 0.0f;
	float m_leftTimer = 0.0f;
	bool m_isRabbit = false;
};

