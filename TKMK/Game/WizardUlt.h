#pragma once

class Player;
class KnightAI;
class Actor;
class Game;
class GameCamera;

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
	void Move();

	//雷を落とす
	void FallThunder();

	void Damege();

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

	void CreatCollision()
	{
		UltCollision->CreateSphere(m_position, Quaternion::Identity, 20.0f);
		UltCollision->SetName("Wizard_UltSkill");
		//懲り所オブジェクトが自動で削除されないようにする。
		UltCollision->SetIsEnableAutoDelete(false);
	}
	
	/// <summary>
	/// アクターのオブジェクト名を取得する
	/// </summary>
	/// <param name="character">オブジェクトの名前</param>
	const void SetActor(const char* character)
	{
		m_targrtName = character;
	}

	/// <summary>
	/// アクターのオブジェクト名を返す
	/// </summary>
	/// <returns>オブジェクトの名前</returns>
	const char* SetTargetActorName()
	{
		return m_targrtName;
	}

	void SetGame(Game* game)
	{
		m_game = game;
	}

	void ChangeUltEndFlag(bool flag)
	{
		UltEndFlag = flag;
	}

	/// <summary>
	/// 自身を生成したのがプレイヤーかどうか
	/// trueでplayer
	/// </summary>
	/// <param name="flag"></param>
	void SetThisCreatCharcter(bool flag)
	{
		m_ThisCreatPlayerFlag = flag;
	}

	/// <summary>
	/// 必殺技のダメージを決める
	/// </summary>
	/// <param name="level"></param>
	void SetAboutUlt(int level)
	{
		//レベル×50=必殺技のダメージ
		m_UltDamege = 50 * level;

		//レベル7以上なら
		if (level >= 7) {
			m_CharLevel7AndAdoveFlag = true;
		}
	}

	bool GetFallTunderFlag()
	{
		return FallTunderFlag;
	}

	bool GetCameraShakeFlag()
	{
		return m_cameraShakeFlag;
	}

private:
	Player* player = nullptr;
	KnightAI* knightAI = nullptr;
	Game* m_game = nullptr;
	Actor* m_targetActor = nullptr;
	Actor* m_CreatMeActor = nullptr;
	Actor* m_GivePointActor = nullptr;
	GameCamera* gameCamera = nullptr;

	EffectEmitter* Thunder;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation;
	Vector3 m_scale;
	Vector3	m_moveSpeed;
	CollisionObject* UltCollision;

	float m_timer = 0.0f;
	const char* m_targrtName = nullptr;

	//雷を地上にいるキャラ全てに打ち終わったら
	bool UltEndFlag = false;
	//攻撃する時にtureにする
	bool FallTunderFlag = false;
	//自身を生成したのがプレイヤーならtrueにする
	bool m_ThisCreatPlayerFlag = false;
	//必殺技を打ったキャラがレベル6以上かどうかの判定
	bool m_CharLevel7AndAdoveFlag = false;

	bool m_cameraShakeFlag = true;

	//必殺技の初期ダメージ
	int m_UltDamege = 0;
};

