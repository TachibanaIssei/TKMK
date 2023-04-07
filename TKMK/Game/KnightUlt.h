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

	/// <summary>
	/// このクラスを作ったオブジェクトの名前
	/// </summary>
	/// <param name="collisionname">名前</param>
	void SetCreatorName(const char* creatorname)
	{
		//コリジョンオブジェクトを作成する。
		UltCollision = NewGO<CollisionObject>(0);
		//このクラスを作ったオブジェクトの名前
		UltCollision->SetCreatorName(creatorname);
	}

	/// <summary>
	/// 当たり判定の設定
	/// </summary>
	void SetCollision(Vector3 collisionsize)
	{
		
		//球状のコリジョンを作成する。
		UltCollision->CreateBox(m_position, m_rotation, collisionsize);
		//コリジョンの名前
		UltCollision->SetName("player_UltimateSkill");

		//懲り所オブジェクトが自動で削除されないようにする。
		UltCollision->SetIsEnableAutoDelete(false);
	}

private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* UltCollision;

	//char m_collisionName;

	float m_timer = 0.0f;

};

