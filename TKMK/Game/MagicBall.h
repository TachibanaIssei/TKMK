#pragma once
#include "Status.h"
class Status;

class MagicBall:public IGameObject
{
public:
	MagicBall();
	~MagicBall();

	//マジックボールを撃ったキャラ。
	enum EnMagician {
		enMagician_None,
		enMagician_Player,			//プレイヤー。
		enMagician_Wizard,			//魔法使い。
	};
	EnMagician m_enMagician;

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
	void SetEnMagician(const EnMagician enMagician)
	{
		m_enMagician = enMagician;
	}


	/// <summary>
	/// 魔法使いの攻撃力を取得
	/// </summary>
	/// <param name="status">魔法使いの攻撃力</param>
	void GetWizardAttack(Status status)
	{
		m_Status.Atk = status.Atk;
	}

	/// <summary>
	/// 魔法使いの攻撃力を返す
	/// </summary>
	/// <returns>魔法使いの攻撃力</returns>
	int SetWizardAttack()const
	{
		return m_Status.Atk;
	}

	/// <summary>
	/// このクラスを作ったオブジェクトの名前
	/// </summary>
	/// <param name="collisionname">名前</param>
	void SetCreatorName(const char* creatorname)
	{
		//コリジョンオブジェクトを作成する。
		BallCollision = NewGO<CollisionObject>(0);
		//このクラスを作ったオブジェクトの名前
		BallCollision->SetCreatorName(creatorname);
	}

	/// <summary>
	/// 当たり判定の設定
	/// </summary>
	void SetCollision()
	{
		//球状のコリジョンを作成する。
		BallCollision->CreateSphere(m_position, Quaternion::Identity, 20.0f);
		//名前をplayer_fireballにする。
		BallCollision->SetName("Wizard_MagicBall");
		//懲り所オブジェクトが自動で削除されないようにする。
		BallCollision->SetIsEnableAutoDelete(false);
	}

private:
	Status m_Status;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* BallCollision;

	float m_timer = 0.0f;

	int WizardAtk = 0;

};

