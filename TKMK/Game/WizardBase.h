#pragma once
#include "Actor.h"

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
		enWizardState_Num,
	};

	void SetModel();

	/// <summary>
	/// 中立の敵を倒したときの経験値の処理
	/// </summary>
	/// <param name="GetExp">中立の敵の経験値</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack() = 0;

	/// <summary>
	/// ダメージを受けたときの処理
	/// </summary>
	/// <param name="damege">敵の攻撃力</param>
	void Dameged(int damege);

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
	//void AtkCollisiton();

	/// <summary>
	/// 必殺技の当たり判定の処理
	/// </summary>
	//void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// アニメーションのステートの管理
	/// </summary>
	void ManageState();

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
		return m_wizardState != enWizardState_Death;
	}

	/// <summary>
	/// 現在のヒットポイントを返す
	/// </summary>
	/// <returns>現在のHP</returns>
	int& SetHp() {
		return m_Status.Hp;
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
	void SetGameUI(GameUI* gameUI)
	{
		gameUI = gameUI;
	}
	GameUI* GetSGameUI()
	{
		return gameUI;
	}

	/// <summary>
	/// 魔法使いのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetWizardState(WizardState gamescene) {
		m_wizardState = gamescene;

	}

protected:
	void PlayAnimation();
	//共通のステートの遷移処理
	void OnProcessCommonStateTransition();
	//待機のステートの遷移処理
	void OnProcessIdleStateTransition();
	//歩きのステートの遷移処理
	void OnProcessRunStateTransition();
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
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ

	//初期ステータス 最大HP、HP、攻撃力、スピード
	Status m_Status;
	//レベルアップ時に増加するステータス
	LvUpStatus LvUpStatus = { 30,5,10.0f };

	WizardState m_wizardState=enWizardState_Idle;

	Game* game = nullptr;
	GameUI* gameUI = nullptr;

	Vector3 m_position = Vector3::Zero;
	float m_position_YUp = 33.0f;                         //モデルの軸が腰にあるのでY座標を50.0f上げる
	Vector3 m_forward = Vector3::AxisZ;                   //正面ベクトル
	Vector3 collisionRot = Vector3::Zero;                  //必殺技
	CollisionObject* collisionObject;                     //コリジョン
	Vector3 UltCollisionPos = Vector3::Zero;               //必殺技の当たり判定の座標
	Vector3 m_Skill_Right = Vector3::Zero;                 //カメラの右方向
	Vector3 m_Skill_Forward = Vector3::Zero;               //カメラの前方向
	CharacterController m_charCon;                        //キャラクターコントロール
	Quaternion m_rot = Quaternion::Identity;              //回転
	ModelRender m_modelRender;                            //モデルレンダー

	//スキルのクールタイムを計算するタイマー
	float SkillTimer = 0;
	//回避のクールタイムを計算するタイマー
	float AvoidanceTimer = 0;
};

