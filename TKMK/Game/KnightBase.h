#pragma once

#include "Actor.h"


//class Status;
class Game;
class GameUI;

class KnightBase:public Actor
{
public:
	KnightBase();
	virtual ~KnightBase();

	enum PlayerState {
		enKnightState_Idle,
		enKnightState_Walk,
		enKnightState_Run,
		enKnightState_ChainAtk,
		enKnightState_Damege,
		enKnightState_Death,
		enKnightState_Skill,
		enKnightState_UltimateSkill,
		enKnightState_Avoidance,
		enKnightState_Num,
		enKnightState_Pause,        //ゲームの状態を受け取る
	};

	/// <summary>
	/// モデルのInit、キャラコンの初期化
	/// </summary>
	/// <param name="Model"></param>
	/// <param name="charCon"></param>
	/// bool Start()
	void SetModel();

	/// <summary>
	/// 中立の敵を倒したときの経験値の処理
	/// </summary>
	/// <param name="GetExp">中立の敵の経験値</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack()=0;

	/// <summary>
	/// 自身の当たり判定
	/// </summary>
	void Collition();

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
	/// スキルを使用したときの処理
	/// </summary>
	//void Skill(Vector3& right,Vector3& forward);

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
	void AtkCollisiton();

	/// <summary>
	/// 必殺技の当たり判定の処理
	/// </summary>
	void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// アニメーションのステートの管理
	/// </summary>
	void ManageState();

	/// <summary>
	/// アニメーション再生時の移動方向、移動速度を決める
	/// </summary>
	/// <param name="moveSpeed">スティックの移動量と乗算させたいスピードの値</param>
	void AnimationMove(float moveSpeed);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="right"></param>
	/// <param name="forward"></param>
	void MoveStraight(Vector3& right, Vector3& forward);

	/// <summary>
	/// アニメーションイベント
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)=0;

	/// <summary>
	/// 座標のセット
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// 剣士の座標を返り値として返す
	/// </summary>
	virtual Vector3 GetPosition() = 0;

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	//
	void SetGameUI(GameUI* gameUI)
	{
		m_gameUI = gameUI;
	}
	GameUI* GetSGameUI()
	{
		return m_gameUI;
	}

	/// <summary>
	/// 特定のアニメーションが再生中ならfalseを返す
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_playerState != enKnightState_ChainAtk &&
			m_playerState != enKnightState_UltimateSkill &&
			m_playerState != enKnightState_Skill &&
			m_playerState != enKnightState_Avoidance &&
			m_playerState != enKnightState_Damege&&
			m_playerState != enKnightState_Death;
	}

	/// <summary>
	/// 現在のレベルを返す
	/// </summary>
	/// <returns>現在のレベル</returns>
	int& SetLevel(){
		return Lv;
	}

	/// <summary>
	/// 現在のヒットポイントを返す
	/// </summary>
	/// <returns>現在のHP</returns>
	int& SetHp() {
		return m_Status.Hp;
	}

	/// <summary>
		/// プレイヤーの前方向を取得。
		/// </summary>
	const Vector3& GetForward() const
	{
		return m_forward;
	}
	Quaternion& GetRot()
	{
		return m_rot;
  }


	/// <summary>
	/// 現在のマックスヒットポイントを返す
	/// </summary>
	/// <returns></returns>
	int& SetMaxHp() {
		return m_Status.MaxHp;
	}

	/// <summary>
	/// プレイヤーのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetPlayerState(PlayerState gamescene) {
		m_playerState = gamescene;

	}

protected:
	void PlayAnimation();
	//共通のステートの遷移処理
	void OnProcessCommonStateTransition();
	//待機のステートの遷移処理
	void OnProcessIdleStateTransition();
	//歩きのステートの遷移処理
	void OnProcessRunStateTransition();
	//チェインアタックのステートの遷移処理
	void OnProcessChainAtkStateTransition();
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

	//enum PlayerState {
	//	enKnightState_Idle,
	//	enKnightState_Run,
	//	enKnightState_ChainAtk,
	//	enKnightState_Damege,
	//	enKnightState_Death,
	//	enKnightState_Skill,
	//	enKnightState_UltimateSkill,
	//	enKnightState_Avoidance,
	//	enKnightState_Num,
	//	enKnightState_GameScene,        //ゲームの状態を受け取る
	//};
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_ChainAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Num,
	};

	Game* m_game=nullptr;
	GameUI* m_gameUI = nullptr;

	//初期ステータス 最大HP、HP、攻撃力、スピード
	Status m_Status;

	Vector3 firstposition;                                //最初の座標
	Vector3 OldPosition = Vector3::Zero;                  //前のフレームの座標
	Vector3 m_position = Vector3::Zero;                   //座標
	float m_position_YUp = 33.0f;                         //モデルの軸が腰にあるのでY座標を50.0f上げる
	Vector3 m_forward = Vector3::AxisZ;                   //正面ベクトル
	Vector3 collisionRot= Vector3::Zero;                  //必殺技
	CollisionObject* collisionObject;                     //コリジョン
	Vector3 UltCollisionPos= Vector3::Zero;               //必殺技の当たり判定の座標
	Vector3 m_Skill_Right = Vector3::Zero;                 //カメラの右方向
	Vector3 m_Skill_Forward = Vector3::Zero;               //カメラの前方向
	CharacterController m_charCon;                        //キャラクターコントロール
	Quaternion m_rot = Quaternion::Identity;              //回転
	ModelRender m_modelRender;                            //モデルレンダー
	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ
	PlayerState m_playerState/* = enKnightState_Num*/;
	
	//レベルアップ時に増加するステータス
	LvUpStatus LvUpStatus = { 30,10,30.0f };
	PlayerState m_animState = enKnightState_Idle;
	//現在のコンボ
	int ComboState = 0;
	//コンボが継続する時間を記録する
	float ComboTimer = 0;
	//ボタンが押されたかの判定
	bool pushFlag = false;
	//一段目のアタックをしたかの判定
	bool AtkState = false;
	//スキルのアニメーション再生が終わったかの判定
	bool SkillEndFlag = false;
	//回避アニメーションを再生したかの判定
	bool AvoidanceFlag = false;
	//
	bool AvoidanceEndFlag = false;
	//「」ボーンのID
	int m_swordBoneId = -1;
	//攻撃アニメーションイベント再生時の剣士の座標を取得する
	int AtkEndPosId= -1;
	//スキルのクールタイムを計算するタイマー
	float SkillTimer = 0;
	//回避のクールタイムを計算するタイマー
	float AvoidanceTimer = 0;

	//獲得した経験値仮
	int exp=5;
	//Newtral_Enemyの攻撃力
	int Enemy_atk = 10;
	//必殺技使用のフラグ
	bool UltCollisionSetFlag = false;
	//攻撃時の剣のコリジョンを表示するかのフラグ
	bool AtkCollistionFlag = false;

};

