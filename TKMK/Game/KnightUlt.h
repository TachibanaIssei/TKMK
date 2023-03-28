#pragma once
class KnightUlt:public IGameObject
{
public:
	KnightUlt();
	~KnightUlt();

	//マジックボールを撃ったキャラ。
	enum EnUltSkill {
		enUltSkill_None,
		enUltSkill_Player,			//プレイヤー。
		enUltSkill_Knight,			//kenn。
	};
	EnUltSkill m_enUltSkill;

	bool Start();
	void Update();

	/// <summary>
		/// 座標を設定する。
		/// </summary>
		/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 術者を設定する。
	/// </summary>
	/// <param name="enMagician">術者。</param>
	void SetEnUlt(const EnUltSkill enMagician)
	{
		m_enUltSkill = enMagician;
	}

private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* UltCollision;

	float m_timer = 0.0f;

};

