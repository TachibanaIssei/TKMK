#pragma once
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
		enNEState_Idle,					//待機。
		enNEState_Chase,					//追跡。
		enNEState_Attack,			    //攻撃
		enNEState_ReceiveDamage,			//被ダメージ。
		enNEState_Death,					//ダウン。
	};
	void SetNEGame(Game* NEgame)
	{
		m_game = NEgame;
	}
	Game* GetNEGame()
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
	Vector3 m_scale = Vector3::One;          //大きさ
	//CharacterController m_charaCon;          //キャラコン
	EnNEState m_NEState = enNEState_Idle;    //中立の敵のステート。
	bool m_UnderAttack = false;              //攻撃判定
	int m_hp = 0;                            //HP
	Game* m_game = nullptr;                  //ゲーム
	KnightPlayer* m_knightPlayer;                        //剣士
	float					m_chaseTimer = 0.0f;						//追跡タイマー。
	float					m_idleTimer = 0.0f;		                    //待機タイマー。

};

