#pragma once
class WizardUlt:public IGameObject
{
public:
	WizardUlt();
	~WizardUlt();

	//必殺技を撃ったキャラ。
	enum EnUltimateSkillian {
		enMagician_None,
		enMagician_Player,			//プレイヤー。
		enMagician_Wizard,			//魔法使い。
	};
	EnUltimateSkillian m_ultSkillian;

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
	void SetEnMagician(const EnUltimateSkillian enMagician)
	{
		m_ultSkillian = enMagician;
	}

private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* BallCollision;

	float m_timer = 0.0f;

};

