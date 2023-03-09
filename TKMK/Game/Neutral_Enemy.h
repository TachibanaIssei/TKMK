#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include "Status.h"
//クラス宣言
class KnightPlayer;
class Game;
/// <summary>
/// 中立の敵
/// </summary>
class Neutral_Enemy : public IGameObject
{

public:
	Neutral_Enemy();
	~Neutral_Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//中立の敵のステート
	enum EnNEState {
		enNeutral_Enemy_Idle,					//待機。
		enNeutral_Enemy_Chase,					//追跡。
		enNeutral_Enemy_Attack,			        //攻撃
		enNeutral_Enemy_ReceiveDamage,			//被ダメージ。
		enNeutral_Enemy_Death,					//ダウン。
	};

	void SetNeutral_EnemyGame(Game* NEgame)
	{
		m_game = NEgame;
	}
	Game* GetNeutral_EnemyGame()
	{
		return m_game;
	}
private:
	/// <summary>
	/// 追跡
	/// </summary>
	void Chase();
	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	/// <summary>
	/// 当たり判定
	/// </summary>
	void Collision();
	/// <summary>
	/// プレイヤーが見つかったら
	/// </summary>
	const bool SearchEnemy()const;
	/// <summary>
	/// 攻撃用の当たり判定
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// ステート
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// アニメーションイベント用の関数。
	/// </summary>
	/// <param name="clipName">アニメーションの名前。</param>
	/// <param name="eventName">アニメーションイベントのキーの名前。</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理。
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 走りステートの遷移処理。
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// 追跡ステートの背遷移処理。
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理。
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理。
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理。
	/// </summary>
	void ProcessDeathStateTransition();
	/// <summary>
	/// 攻撃できるならtrue
	/// </summary>
	/// <returns></returns>
	const bool CanAttack() const;

	

	enum EnAnimationClip {                      //アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Death,					//ダウンアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};

	AnimationClip m_animationClips[enAnimationClip_Num];//アニメーションクリップ
	ModelRender   m_modelRender;              //モデルレンダー
	Vector3 m_position;                       //座標
	Vector3 m_moveSpeed;                      //移動速度
	Vector3 m_forward = Vector3::AxisZ;      //正面のベクトル
	Quaternion m_rot;                        //クォータニオン
	Vector3 m_scale = Vector3{0.2f,0.2f,0.2f};          //大きさ
	CharacterController m_charaCon;          //キャラコン
	EnNEState m_Neutral_EnemyState = enNeutral_Enemy_Idle;    //中立の敵のステート。
	bool m_UnderAttack = false;              //攻撃判定
	int m_AttackBoneId = 1;                  //頭のボーンのID
	Game* m_game = nullptr;                  //ゲーム
	KnightPlayer* m_knightPlayer;                        //剣士
	float					m_chaseTimer = 0.0f;						//追跡タイマー。
	float					m_idleTimer = 0.0f;		                    //待機タイマー。
	Status m_Status;                           //ステータス
	//ナビゲーションメッシュ
	TknFile m_tknFile;
	PhysicsStaticObject m_bgObject;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	Vector3 m_targetPointPosition;
};

