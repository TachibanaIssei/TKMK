#pragma once
class Neutral_Enemy;

class ExpforKnight : public IGameObject
{
public:
	ExpforKnight();
	~ExpforKnight();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//ベジェ曲線
	void Bezier();
	void SetPosition(Vector3 pos)
	{
		m_EnemyPos = pos;
	}

	//ウサギ用経験値画像へ切り替える
	void SetIsRabbitExp() {
		isRabbit = true;
	}

private:
	SpriteRender m_NormalExp;
	Vector3 m_EnemyPos = Vector3::Zero;
	//ベジェ曲線専用
	Vector2 m_EFKSCPOS = Vector2::Zero;                  //エフェクトのポジション
	Vector2 center;
	Vector2 MovePos;
	Vector2 MovePos2;
	Vector2 MovePos3;
	int Randamu = 0;

	float m_Timer = 0.0f;
	float LeftTimer = 0.0f;
	bool isRabbit = false;
};

