#pragma once
#include "Actor.h"
#include "CameraCollisionSolver.h"

class Game;
class GameUI;

class WizardBase:public Actor
{
public:
	WizardBase();
	virtual ~WizardBase();

	enum WizardState
	{
		enWizardState_Idle,
		enWizardState_Walk,
		enWizardState_Run,
		enWizardState_Attack,
		enWizardState_Damege,
		enWizardState_Death,
		enWizardState_Skill,
		enWizardState_UltimateSkill,
		enWizardState_Avoidance,
		enWizardState_Pause,
		enWizardState_Jump,
		enWizardState_Fall,
		enWizardState_Num,
	};

	/// <summary>
	/// モデルのInit、キャラコンの初期化
	/// </summary>
	void SetModel();

	/// <summary>
	/// 中立の敵を倒したときの経験値の処理
	/// </summary>
	/// <param name="m_getExp">中立の敵の経験値</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack() = 0;

	/// <summary>
	/// ダメージを受けたときの処理
	/// </summary>
	/// <param name="damege">敵の攻撃力</param>
	void Dameged(int damege, Actor* CharGivePoints);

	/// <summary>
	/// 自身が倒されたときの処理
	/// </summary>
	void Death();

	/// <summary>
	/// 必殺技を発動したときの処理
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// リスポーンする座標のセット
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();

	/// <summary>
	/// 通常攻撃の当たり判定の処理
	/// </summary>
	void Collision();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	void Skill(Vector3& position,Quaternion& rotation,CharacterController& charCon);

	/// <summary>
	/// 必殺技の当たり判定の処理
	/// </summary>
	//void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// アニメーションのステートの管理
	/// </summary>
	void ManageState();

	/// <summary>
	/// アニメーション再生時の移動方向、移動速度を決める
	/// </summary>
	/// <param name="moveSpeed">スティックの移動量と乗算させたいスピードの値</param>
	/// <param name="stickL">スティックの移動の入力量</param>
	void AnimationMove(float moveSpeed, Vector3 stickL);

	/// <summary>
	/// 直線移動
	/// </summary>
	/// <param name="right"></param>
	/// <param name="forward"></param>
	void MoveStraight(Vector3& right, Vector3& forward);

	/// <summary>
	/// アニメーションイベント
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName) = 0;

	/// <summary>
	/// 座標のセット
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// 魔法使いの座標を返り値として返す
	/// </summary>
	virtual Vector3 GetPosition() = 0;

	/// <summary>
	/// 特定のアニメーションが再生中ならfalseを返す
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_wizardState != enWizardState_Attack &&
			m_wizardState != enWizardState_Damege &&
			m_wizardState != enWizardState_Death &&
			m_wizardState != enWizardState_Avoidance &&
			m_wizardState != enWizardState_Skill &&
			m_wizardState != enWizardState_UltimateSkill
			;
	}

	/// <summary>
	/// 現在のレベルを返す
	/// </summary>
	/// <returns>現在のレベル</returns>
	int& SetLevel() {
		return m_lv;
	}

	/// <summary>
	/// 現在のヒットポイントを返す
	/// </summary>
	/// <returns>現在のHP</returns>
	const int& GetHitPoint() {
		return m_status.GetHp();
	}

	/// <summary>
	/// 現在のマックスヒットポイントを返す
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() {
		return m_status.GetMaxHp();
	}

	void SetSGame(Game* Cgame)
	{
		game = Cgame;
	}
	Game* GetSGame()
	{
		return game;
	}
	//
	void SetGameUI(GameUI* mgameUI)
	{
		gameUI = mgameUI;
	}
	GameUI* GetSGameUI()
	{
		return gameUI;
	}

	/// <summary>
	///	プレイヤーの前方向を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_Forward;
	}

	/// <summary>
	/// プレイヤーの回転量を取得
	/// </summary>
	/// <returns></returns>
	Quaternion& GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// 魔法使いのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetWizardState(WizardState gamescene) {
		m_wizardState = gamescene;

	}

	/// <summary>
	/// リスポーンする番号を決める
	/// </summary>
	void SetRespawnNumber(int number)
	{
		m_respawnNumber = number;
	}

	// 追尾エフェクトの削除
	void ChaseEffectDelete() {
		if (GetHoimi != nullptr) {
			GetHoimi->ResetTarget();
		}
	}

protected:
	void PlayAnimation();
	//共通のステートの遷移処理
	void OnProcessCommonStateTransition();
	//待機のステートの遷移処理
	void OnProcessIdleStateTransition();
	//歩きのステートの遷移処理
	void OnProcessRunStateTransition();
	//ジャンプのステートの遷移処理
	void OnProcessJumpStateTransition();
	//落下中のステートの遷移処理
	void OnProcessFallStateTransition();
	//アタックのステートの遷移処理
	void OnProcessAttackStateTransition();
	//スキルのステートの遷移処理
	void OnProcessSkillAtkStateTransition();
	//回避のステートの遷移処理
	void OnProcessAvoidanceStateTransition();
	//必殺技のステートの遷移処理
	void OnProcessUltimateSkillAtkStateTransition();
	//ダメージを受けたときのステートの遷移処理
	void OnProcessDamegeStateTransition();
	//HPが0になったときのステートの遷移処理
	void OnProcessDeathStateTransition();
	//必殺技の終了
	void UltEnd() {
		// とりあえず仮
		int hoge = 0;
	}

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Atk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Jump,
		enAnimationClip_Fall,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ

	//初期ステータス 最大HP、HP、攻撃力、スピード
	//Status m_status;
	//レベルアップ時に増加するステータス
	LvUpStatus LvUpStatus = { 30,5,10.0f };

	WizardState m_wizardState=enWizardState_Idle;

	Game* game = nullptr;
	GameUI* gameUI = nullptr;

	Vector3 oldPosition = Vector3::Zero;         //前フレームの座標
	float m_position_YUp = 34.0f;                         //モデルの軸が腰にあるのでY座標を50.0f上げる
	Vector3 m_forward = Vector3::AxisZ;                   //正面ベクトル
	Vector3 collisionRot = Vector3::Zero;                  //必殺技
	CollisionObject* collisionObject;                     //コリジョン
	Vector3 UltCollisionPos = Vector3::Zero;               //必殺技の当たり判定の座標
	Vector3 m_Skill_Right = Vector3::Zero;                 //カメラの右方向
	Vector3 m_Skill_Forward = Vector3::Zero;               //カメラの前方向
	CharacterController m_charCon;                        //キャラクターコントロール

	RigidBody				m_rigidBody;						//剛体。
	SphereCollider			m_sphereCollider;							//コライダー。

	Actor* m_lastAttackActor = nullptr;		// 最後に自分を攻撃したやつ
	Actor* m_Neutral_enemy = nullptr;       //中立の敵用のダメージを受けたときに使うインスタンス。nullptrのままにする

	//スキルのクールタイムを計算するタイマー
	float m_skillTimer = 0;
	//回避のクールタイムを計算するタイマー
	float m_avoidanceTimer = 0;

	//ボタンが押されたかの判定
	bool pushFlag = false;

	//スキルのアニメーション再生が終わったかの判定
	bool SkillEndFlag = false;
	//回避アニメーションを再生したかの判定
	bool AvoidanceFlag = false;
	//回避アニメーションが終わったかの判定
	bool AvoidanceEndFlag = false;

	//中立の敵から受けるダメージ
	int Enemy_atk = 10;
	//ジャンプしたかのフラグ
	bool jampAccumulateflag = false;

	btCollisionWorld::LocalConvexResult* m_localConvexResult = nullptr;
};

