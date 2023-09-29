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
class Map;
class ExpforKnight;
class ChaseEFK;

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
	void DeathEfk();
	//中立の敵のステート
	enum EnNEState {
		enNeutral_Enemy_Idle,					//待機。
		enNeutral_Enemy_Chase,					//追跡（ウサギなら逃げ）
		enNeutral_Enemy_Attack,			        //攻撃
		enNeutral_Enemy_ReceiveDamage,			//被ダメージ。
		enNeutral_Enemy_Death,					//ダウン。
		enNeutral_Enemy_Pause,                  //ポーズ画面
		enNeutral_Enemy_Patrol,                 //巡回
		enNeutral_Enemy_BackPatrol
	};

	enum EnEnemyKinds {
		enEnemyKinds_White,						//白
		enEnemyKinds_Red,						//赤
		enEnemyKinds_Green,						//緑
		enEnemyKinds_Rabbit,                    //ウサギ
		enEnemyKinds_Num,						//敵の種類
	};

	enum EnAnimationClip {                      //アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Death,					//ダウンアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};

	enum EnJumpFlag {
		enJumpStart,
		enJumpEnd,
		enJumpnull,
		enJumpNum
	};
	void SetNeutral_EnemyGame(Game* NEgame)
	{
		m_game = NEgame;
	}
	Game* GetNeutral_EnemyGame()
	{
		return m_game;
	}

	void ChangeRabbit()
	{
		m_enemyKinds = enEnemyKinds_Rabbit;
	}

	void SetRabbitLifeFlag(bool rabbit)
	{
		rabbitLife = rabbit;
	}

	void HPreductionbytime();
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

	EnNEState GetEnemyState() {
		return m_Neutral_EnemyState;
	}

	EnEnemyKinds GetenemyColor()
	{
		return m_enemyKinds;
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

	bool RabbitSearch();

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
	/// 巡回に戻れステート遷移処理
	/// </summary>
	void ProcessBackPatrolStateTransition();
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
	/// ウサギが逃げる距離
	/// </summary>
	/// <returns></returns>
    void EscapeSearch();
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
	void SetPlayerActor(Actor* actor0 = nullptr, Actor* actor1 = nullptr, Actor* actor2 = nullptr, Actor* actor3 = nullptr) {
		m_player[0] = actor0;
		m_player[1] = actor1;
		m_player[2] = actor2;
		m_player[3] = actor3;
	}

	/// <summary>
	/// アクターの情報を追加する
	/// </summary>
	/// <param name="actor">加えたいアクター</param>
	void AddActorFromList(Actor* actor) {
		be_target.push_back(actor);
	}

	/// <summary>
	/// アクターの情報をリストから削除する
	/// </summary>
	/// <param name="actor">消したいアクター</param>
	void RemoveActorFromList(Actor* actor)
	{
		std::vector<Actor*>::iterator it = std::find(
			be_target.begin(), // アクターのリストの最初
			be_target.end(),   // アクターのリストの最後
			actor                     // 消したいアクター
		);

		if (it != be_target.end()) {
			be_target.erase(it);
		}
	}

	//自分を狙っている敵を返す
	int GetBetargetCount()
	{
		return (int)be_target.size();
	}
	void modelUpdate();

	float SoundSet(Player* player, float Max, float Min);

	/// <summary>
	/// 効果音を上げたり下げたりする
	/// </summary>
	void SetSE(float addSE)
	{
		//SEVolume += addSE;
		MaxVolume += addSE;
	}

	void GetSE(float nowSEsixe)
	{
		MaxVolume = nowSEsixe;
	}

	void MapMove();

	void EnemyMap(RenderContext& rc)
	{
		if (m_isMapImage == true)
		{
			m_enemyMapSprite.Draw(rc);
		}
	}


private:
	static const int m_maxPlayerCount = 4;					   //プレイヤーの最大人数
	AnimationClip m_animationClips[enAnimationClip_Num];       //アニメーションクリップ
	ModelRender   m_modelRender;                               //モデルレンダー
	Vector3       m_position = Vector3::Zero;;                                 //座標
	Vector3       m_moveSpeed;                                 //移動速度
	Vector3       m_forward = Vector3::AxisZ;                  //正面のベクトル
	Quaternion    m_rot;                                       //クォータニオン
	Vector3       m_scale = Vector3{0.2f,0.2f,0.2f};           //大きさ
	CharacterController m_charaCon;                            //キャラコン
	EnNEState m_Neutral_EnemyState = enNeutral_Enemy_Idle;     //中立の敵のステート。
	EnEnemyKinds m_enemyKinds;								   //中立の敵の種類
	Vector3   RadiusPos;
	SoundSource* m_se = nullptr;

	Actor* m_targetActor = nullptr;
	std::array<Actor*, m_maxPlayerCount> m_player = { nullptr,nullptr,nullptr,nullptr };
	Actor* m_lastAttackActor = nullptr;		// 最後に自分を攻撃したやつ

	Game* m_game = nullptr;                               

	GameCamera* m_gameCamera = nullptr;
	Player* player = nullptr;
	MagicBall* magicBall = nullptr;
	KnightAI* m_knightAI = nullptr;
	Map* map = nullptr;

	Level3DRender m_EnemyPoslevel;      //エネミーのポジションレベル
	Status m_Status;                    //ステータス
	SpriteRender		m_enemyMapSprite;

	FontRender              m_Name;
	SphereCollider			m_sphereCollider;
	RigidBody				m_rigidBody;
	Vector3                 m_patrolPos[41];
	Vector3 nowPos = Vector3::Zero;
	Vector3 m_hagikiPower;
	
	bool CallBezier = false;

	bool m_UnderAttack = false;              //攻撃判定
	int m_AttackBoneId = 1;                  //頭のボーンのID
	//中立の敵
	float	m_chaseTimer = 0.0f;			//追跡タイマー。
	float	m_idleTimer = 0.0f;		        //待機タイマー。
	float   m_stopTimer = 1.0f;             //止まってしまったタイマー

	int enemyColorRam = 0;					//エネミーのカラーを決める
	int AtkPass = 30;						//渡す攻撃力の量
	

	//攻撃を受けたときに相手の攻撃力を格納する変数
	int GetAtk=0;
	//やられたかのフラグ
	bool Deathflag = false;

	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	//中立の敵がやられたときに渡す経験値
	const int Exp = 5;

	//std::vector<Neutral_Enemy*>::iterator m_number;
	int P = -1;
	int randam;
	bool m_backPatrol = false;
	bool m_backPatrolFarst = false;

	//自分をターゲットしてるアクターのリスト
	std::vector<Actor*> be_target;

	bool isStart = false;

	//マップに映すかどうかの判定
	bool m_isMapImage = false;

	//効果音
	float SEVolume = 0.0f;
	float MaxVolume = 0.7f;
	float MinVolume = 0.0f;

	// ウサギ専用
	bool isPatrolRandom = false;
	float isPatrolTimer = 0.0f;
	float HPreductionbyTimer = 0.0f;
	bool rabbitLife = false;
	EffectEmitter* Rabbit_kirakira = nullptr;
	Vector3 m_Rabbit_Pos = Vector3::Zero;
	EnJumpFlag enJump = enJumpNum;
};

