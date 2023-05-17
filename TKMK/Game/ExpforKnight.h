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
	//�x�W�F�Ȑ�
	void Bezier();
	void SetPosition(Vector3 pos)
	{
		m_EnemyPos = pos;
	}

	//�E�T�M�p�o���l�摜�֐؂�ւ���
	void SetIsRabbitExp() {
		isRabbit = true;
	}

private:
	SpriteRender m_NormalExp;
	Vector3 m_EnemyPos = Vector3::Zero;
	//�x�W�F�Ȑ���p
	Vector2 m_EFKSCPOS = Vector2::Zero;                  //�G�t�F�N�g�̃|�W�V����
	Vector2 center;
	Vector2 MovePos;
	Vector2 MovePos2;
	Vector2 MovePos3;
	int Randamu = 0;

	float m_Timer = 0.0f;
	float LeftTimer = 0.0f;
	bool isRabbit = false;
};

