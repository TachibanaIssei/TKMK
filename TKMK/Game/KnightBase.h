#pragma once

#include "Actor.h"


//class Status;
class Game;
class GameUI;
class Player;

class KnightBase:public Actor
{
public:
	KnightBase();
	virtual ~KnightBase();

	enum EnKnightKinds
	{
		enKnightKinds_Red,
		enKnightKinds_Green,
		enKnightKinds_Blue,
		enKnightKinds_Yellow,
		enKnightKinds_Num
	};
	EnKnightKinds KnightKinds = enKnightKinds_Num;
	void SetKnightColor(EnKnightKinds color)
	{
		KnightKinds = color;
	}
	/// <summary>
	/// モデルのInit、キャラコンの初期化
	/// </summary>
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
	/// <param name="playerGivePoints">ポイントを与えるキャラ</param>
	void Dameged(int damege,Actor* CharGivePoints);

	/// <summary>
	/// 自身が倒されたときの処理
	/// </summary>
	void Death();

	/// <summary>
	/// スキルを使用したときの処理
	/// </summary>
	//void Skill(Vector3& right,Vector3& forward);

	/////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 必殺技を発動したときの処理
	/// </summary>
	void UltimateSkill();
	//////////////////////////////////////////////////////////////////////////////////////////////
	
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
	void AnimationMove(float Speed);

	/// <summary>
	/// 直線移動
	/// </summary>
	void MoveStraight();

	/// <summary>
	/// アニメーションイベント
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)=0;

	/// <summary>
	/// 必殺技を打っている間の処理
	/// </summary>
	virtual bool UltimaitSkillTime() = 0;

	/// <summary>
	/// ラストアタックの移動計算
	/// </summary>
	virtual void LastAttackMove() = 0;

	/// <summary>
	/// 座標のセット
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }
	inline void SetCharaconPosition(Vector3 PS) { m_charCon.SetPosition(PS); }

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
		return m_charState != enCharState_Attack &&
			m_charState != enCharState_SecondAttack &&
			m_charState != enCharState_UltimateSkill &&
			m_charState != enCharState_Skill &&
			m_charState != enCharState_Avoidance &&
			m_charState != enCharState_Damege &&
			m_charState != enCharState_Ult_liberation&&
			m_charState != enCharState_Death;
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
	int& SetHp() 
	{
		return m_Status.Hp;
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
	/// リスポーンする番号を決める
	/// </summary>
	void SetRespawnNumber(int number)
	{
		respawnNumber = number;
	}
	int GetRespawnNumber() const
	{
		return respawnNumber;
	}

	/// <summary>
	/// HPが0なら実行する
	/// </summary>
	/// <param name="status"></param>
	void IsDead(Status& status)
	{
		if (status.Hp <= 0)
		{
			m_charState = enCharState_Death;

		}
	}

	/// <summary>
	/// 追尾エフェクトのポインタをリセット
	/// </summary>
	void EffectNullptr() {

		if (GetHoimi != nullptr) {
			if (GetHoimi->GetEffect()->IsPlay() == false) {
				GetHoimi = nullptr;
			}
		}

		if (EffectKnightSkill != nullptr) {
			if (EffectKnightSkill->GetEffect()->IsPlay() == false) {
				EffectKnightSkill = nullptr;
			}
		}

		if (EffectKnightSkillGround != nullptr) {
			if (EffectKnightSkillGround->GetEffect()->IsPlay() == false) {
				EffectKnightSkillGround = nullptr;
			}
		}

		if (FootSmoke != nullptr) {
			if (FootSmoke->GetEffect()->IsPlay() == false) {
				FootSmoke = nullptr;
			}
		}

		if (LevelUp_efk != nullptr) {
			if (LevelUp_efk->GetEffect()->IsPlay() == false) {
				LevelUp_efk = nullptr;
			}
		}

		if (LevelDown_efk != nullptr) {
			if (LevelDown_efk->GetEffect()->IsPlay() == false) {
				LevelDown_efk = nullptr;
			}
		}
	}
	
	// 追尾エフェクトの削除
	void ChaseEffectDelete() {
		if (GetHoimi != nullptr) {
			GetHoimi->ResetTarget();
		}

		if (EffectKnightSkill != nullptr) {
			EffectKnightSkill->ResetTarget();
		}

		if (EffectKnightSkillGround != nullptr) {
			EffectKnightSkillGround->ResetTarget();
		}

		if (FootSmoke != nullptr) {
			FootSmoke->ResetTarget();
		}

		if (LevelUp_efk != nullptr) {
			LevelUp_efk->ResetTarget();
		}

		if (LevelDown_efk != nullptr) {
			LevelDown_efk->ResetTarget();
		}
	}

	void CreatMagicCircle();

	virtual void IsLevelEffect(int oldlevel,int nowlevel) = 0;

	virtual void SetAndPlaySoundSource(EnSound soundNumber) = 0;

protected:
	/// <summary>
	///無敵時間用
	/// </summary>
	void Invincible();
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
	//チェインアタックのステートの遷移処理
	void OnProcessChainAtkStateTransition();
	//二段目アタックのステートの遷移処理
	void OnProcessSecondAtkStateTransition();
	//三段目アタックのステートの遷移処理
	void OnProcessLastAtkStateTransition();
	//スキルのステートの遷移処理
	void OnProcessSkillAtkStateTransition();
	//回避のステートの遷移処理
	void OnProcessAvoidanceStateTransition();
	//必殺技の溜めのステートの遷移処理
	void OnProcessUlt_liberationStateTransition();
	//必殺技のステートの遷移処理
	void OnProcessUltimateSkillAtkStateTransition();
	//ダメージを受けたときのステートの遷移処理
	void OnProcessDamegeStateTransition();
	//HPが0になったときのステートの遷移処理
	void OnProcessDeathStateTransition();
	//必殺技の終了
	void UltEnd();

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_ChainAtk,
		enAnimationClip_SecondAtk,
		enAnimationClip_lastAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_Ult_liberation,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Jump,
		enAnimationClip_Fall,
		enAnimationClip_Num,
	};

	Game* m_game=nullptr;
	GameUI* m_gameUI = nullptr;
	Player* player = nullptr;

	//初期ステータス 最大HP、HP、攻撃力、スピード
	


	Vector3 firstposition;                                //最初の座標
	Vector3 OldPosition = Vector3::Zero;                  //前のフレームの座標
	float m_position_YUp = 33.0f;                         //モデルの軸が腰にあるのでY座標を50.0f上げる
	Vector3 collisionRot= Vector3::Zero;                  //必殺技
	CollisionObject* collisionObject;                     //コリジョン
	Vector3 UltCollisionPos= Vector3::Zero;               //必殺技の当たり判定の座標
	Vector3 m_Skill_MoveSpeed = Vector3::Zero;

	CharacterController m_charCon;                        //キャラクターコントロール

	EffectEmitter* Ult_Swordeffect = nullptr;				//必殺技の溜めエフェクト
	EffectEmitter* MagicCircle = nullptr;					//必殺技の魔法陣のエフェクト

	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ

	Actor* m_lastAttackActor = nullptr;		// 最後に自分を攻撃したやつ
	Actor* m_Neutral_enemy = nullptr;       //中立の敵用のダメージを受けたときに使うインスタンス。nullptrのままにする

	

	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	AtkTimingState m_AtkTmingState = Num_State;

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
	
	//前フレームのレベル
	int oldLv = 1;

	//獲得した経験値仮
	int exp=1;
	//Newtral_Enemyの攻撃力
	int Enemy_atk = 10;
	//必殺技使用のフラグ
	bool UltCollisionSetFlag = false;
	//必殺技を打ったらしばらく止めるタイマー
	float UltStopTimer = 1.0f;
	

	//攻撃時の剣のコリジョンを表示するかのフラグ
	bool AtkCollistionFlag = false;

	//bool jampAccumulateflag = false;

	//プレイヤーとの内積を求めて線形補間で音量調整
	float SEVolume = 0;

	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;
	//落下音のフラグ
	bool FallSoundFlag = false;
	
	//ラストアタックのアニメーションで切った後に動けないようにする
	//tureの時は動けなくなる
	bool CantMove = false;

	// 追尾エフェクト
	ChaseEFK* EffectKnightSkill = nullptr;
	ChaseEFK* EffectKnightSkillGround = nullptr;
	ChaseEFK* FootSmoke = nullptr;
};

