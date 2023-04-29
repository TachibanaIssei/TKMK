#pragma once
class Game;
class GameCamera;
class Actor;

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
	void Render(RenderContext& rc);

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
	void SetCollision();

	/// <summary>
	/// モデル、エフェクトの生成
	/// </summary>
	void MakeUlt();

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// プレイヤーの前方向を取得。
	/// </summary>
	const Vector3& GetForward() const
	{
		return m_forward;
	}

	/// <summary>
	/// 当たり判定が生成できるか調べる
	/// </summary>
	/// <returns></returns>
	bool MakeCheck();

	/// <summary>
	/// キャラのレベルを取得する
	/// </summary>
	/// <param name="level">必殺技を打ったキャラのレベル</param>
	void GetCharLevel(int level)
	{
		CharLevel = level;
	}

	void SetDeletetime()
	{
		if (CharLevel < 6)
		{
			DeleteTime = 1;
		}
		//レベルが7以下なら
		//必殺技一段階強化
		else if (CharLevel < 8)
		{
			DeleteTime = 1;
		}
		//レベルが10以下なら
		//必殺技二段階強化
		else if (CharLevel <= 10)
		{
			DeleteTime = 1;
		}
	}

	void SetGame(Game* game)
	{
		m_game = game;
	}
	
	/// <summary>
	/// 生成者を設定
	/// </summary>
	void SetActor(Actor* actor) {
		m_actor = actor;
	}

private:
	ModelRender model;

	Vector3 m_firstposition = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_Checkposition = Vector3::Zero;

	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	Vector3 m_forward;
	EffectEmitter* Ulteffect;
	//エフェクトの当たり判定
	CollisionObject* UltCollision;
	//この当たり判定が壁に当たったら消す
	CollisionObject* UltDeleteJudgeCollision;

	Game* m_game;
	RigidBody				m_rigidBody;						//剛体。
	SphereCollider			m_sphereCollider;							//コライダー。

	//char m_collisionName;

	float m_timer = 0.0f;

	//当たり判定を消す時間
	float DeleteTime;

	//生成したキャラのレベル
	int CharLevel = 0;
	//bool Ultflag = true;

	// 生成者
	Actor* m_actor = nullptr;
	// ゲームカメラ
	GameCamera* m_gameCamera = nullptr;
};

