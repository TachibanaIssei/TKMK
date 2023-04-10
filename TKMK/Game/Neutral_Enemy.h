#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include "Status.h"
#include "Level3DRender.h"
//クラス宣言
class KnightPlayer;
class Game;
class GameCamera;
class Neutral_Enemy;
class Patrolnumb;
class KnightAI;
class Player;
class MagicBall;
class Actor;

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
	void HPBar();

	//中立の敵のステート
	enum EnNEState {
		enNeutral_Enemy_Idle,					//待機。
		enNeutral_Enemy_Chase,					//追跡。
		enNeutral_Enemy_Attack,			        //攻撃
		enNeutral_Enemy_ReceiveDamage,			//被ダメージ。
		enNeutral_Enemy_Death,					//ダウン。
		enNeutral_Enemy_Pause,                  //ポーズ画面
		enNEutral_Enemy_Patrol,                 //巡回
	};

		


	void SetNeutral_EnemyGame(Game* NEgame)
	{
		m_game = NEgame;
	}
	Game* GetNeutral_EnemyGame()
	{
		return m_game;
	}



	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
	void SetRotation(Quaternion rotation)
	{
		m_rot = rotation;
	}

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	


	void Move();
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
	/// 追いかける対象を探す
	/// 見つかったらtrueを返す
	/// </summary>
	bool Search();

	void SetKnightPlayer(Actor* knightPlayer)
	{
		m_targetActor = knightPlayer;
	}
	Actor* GetKnightPlayer()
	{
		return m_targetActor;
	}

	/// <summary>
	/// 攻撃用の当たり判定
	/// </summary>
	void MakeAttackCollision();

	/// <summary>
	/// リスポーン
	/// </summary>
	//void Respawn();

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

	/// <summary>
	/// 
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
	/// 巡回ステートの遷移処理
	/// </summary>
	void ProcessPatrolStateTransition();
	/// <summary>
	/// 攻撃できるならtrue
	/// </summary>
	/// <returns></returns>
	const bool CanAttack() const;

	/// <summary>
	/// ゲージを左寄せする処理
	/// </summary>
	/// <param name="size">画像の元の大きさ</param>
	/// <param name="scale">現在のスケール倍率</param>
	/// <returns>変換前と変換後の差</returns>
	Vector3 HPBerSend(Vector3 size, Vector3 scale);

	/// <summary>
	/// HPゲージの描画フラグ
	/// </summary>
	/// <returns>描画できる範囲にあるときtrue</returns>
	bool DrawHP();

	/// <summary>
	/// プレイヤーのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetNeutral_EnemyState(EnNEState gamescene) {
		m_Neutral_EnemyState = gamescene;
	}
	/// <summary>
	/// 巡回する座標を設定する
	/// </summary>
	/// <param name="pos">巡回する座標</param>
	/// <param name="number">座標をセットする番号</param>
	void SetPatrolPos(Vector3 pos, int number) {
		m_patrolPos[number] = pos;
	};

	/// <summary>
	/// プレイヤーのアクターを設定する
	/// </summary>
	void SetPlayerActor(Actor* actor) {
		m_player = actor;
	}

	enum EnAnimationClip {                      //アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Death,					//ダウンアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};

private:
	AnimationClip m_animationClips[enAnimationClip_Num];       //アニメーションクリップ
	ModelRender   m_modelRender;                               //モデルレンダー
	Vector2		  m_HPBerPos = Vector2::Zero;				   //HPバーのポジション
	Vector2	   	  m_HPWindowPos = Vector2::Zero;			   //HP枠のポジション
	Vector2		  m_HPBackPos = Vector2::Zero;			       //HP背景のポジション
	Vector3       m_position;                                  //座標
	Vector3       m_moveSpeed;                                 //移動速度
	Vector3       m_forward = Vector3::AxisZ;                  //正面のベクトル
	Quaternion    m_rot;                                       //クォータニオン
	Vector3       m_scale = Vector3{0.2f,0.2f,0.2f};           //大きさ
	CharacterController m_charaCon;                            //キャラコン
	EnNEState m_Neutral_EnemyState = enNeutral_Enemy_Idle;     //中立の敵のステート。
	Vector3   RadiusPos;
	SoundSource* m_se = nullptr;

	Actor* m_targetActor = nullptr;
	Actor* m_player = nullptr;
	Actor* m_lastAttackActor = nullptr;		// 最後に自分を攻撃したやつ

	Game* m_game = nullptr;                               
	Neutral_Enemy* m_Neutral_Enemy=nullptr; 


	GameCamera* m_gameCamera = nullptr;
	Player* player = nullptr;
	MagicBall* magicBall = nullptr;
	KnightAI* m_knightAI = nullptr;

	Level3DRender m_EnemyPoslevel;      //エネミーのポジションレベル
	Status m_Status;                    //ステータス
	SpriteRender		m_HPBar;		//HPバー画像
	SpriteRender		m_HPFrame;		//HP枠画像
	SpriteRender		m_HPBack;		//HP背景画像	
	
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;
	RigidBody				m_rigidBody;
	Vector3                 m_inRespawnPosition[12];
	Vector3                 m_patrolPos[9];
	Vector3 nowPos = Vector3::Zero;
	Vector3 m_hagikiPower;
	bool m_UnderAttack = false;              //攻撃判定
	int m_AttackBoneId = 1;                  //頭のボーンのID
	//中立の敵
	float	m_chaseTimer = 0.0f;			//追跡タイマー。
	float	m_idleTimer = 0.0f;		        //待機タイマー。
	float   m_stopTimer = 1.0f;             //止まってしまったタイマー

	int enemyColorRam = 0;					//エネミーのカラーを決める
	int AtkPass = 10;						//渡す攻撃力の量
	int HpPass = 30;						//回復量

	//攻撃を受けたときに相手の攻撃力を格納する変数
	int GetAtk=0;
	//やられたかのフラグ
	bool Deathflag = false;
	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	int Exp = 5;

	//std::vector<Neutral_Enemy*>::iterator m_number;
	int P = -1;
	
};

