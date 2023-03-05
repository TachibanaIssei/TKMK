#pragma once

#include "Actor.h"



class Game;

class KnightBase:public Actor
{
public:
	KnightBase();
	virtual ~KnightBase();

	/// <summary>
	/// モデルのInit、キャラコンの初期化
	/// </summary>
	/// <param name="Model"></param>
	/// <param name="charCon"></param>
	/// bool Start()
	void SetModel();

	//void Update();
	void Move();
	//void Render(RenderContext& rc);

	/// <summary>
	/// 中立の敵を倒したときの経験値の処理
	/// </summary>
	/// <param name="GetExp">中立の敵の経験値</param>
	void ExpProcess(int Exp);

	void Attack();

	/// <summary>
	/// ダメージを受けたときの処理
	/// </summary>
	/// <param name="damege">敵の攻撃力</param>
	void Dameged(int damege);

	/// <summary>
	/// スキルを使用したときの処理
	/// </summary>
	void Skill();

	/// <summary>
	/// 
	/// </summary>
	void UltimateSkill();

	/// <summary>
	/// 
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// 
	/// </summary>
	void Death();

	/// <summary>
	/// 
	/// </summary>
	void Rotation();

	/// <summary>
	/// 当たり判定の処理
	/// </summary>
	void AtkCollisiton();

	/// <summary>
	/// アニメーションのステートの管理
	/// </summary>
	void ManageState();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	//inline Vector3 GetPosition() { return m_position; }
	virtual Vector3 GetPosition() = 0;

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}

	/// <summary>
	/// 特定のアニメーションが再生中ならfalseを返す
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_animState != enKnightState_FirstAtk &&
			m_animState != enKnightState_SecondAtk&&
			m_animState!= enKnightState_Damege&&
			m_animState != enKnightState_Death;
	}

	/// <summary>
	/// 現在のレベルをint型の変数に代入する
	/// </summary>
	/// <param name="level"></param>
	int SetLevel(int& level){
		level = Lv;
		return level;
	}


protected:
	void PlayAnimation();
	//共通のステートの遷移処理
	void OnProcessCommonStateTransition();
	//待機のステートの遷移処理
	void OnProcessIdleStateTransition();
	//歩きのステートの遷移処理
	void OnProcessRunStateTransition();
	//一段目のアタックのステートの遷移処理
	void OnProcessFirstAtkStateTransition();
	//二段目のアタックのステートの遷移処理
	void OnProcessSecondAtkStateTransition();
	//ダメージを受けたときのステートの遷移処理
	void OnProcessDamegeStateTransition();
	//HPが0になったときのステートの遷移処理
	void OnProcessDeathStateTransition();

	enum PlayerState {
		enKnightState_Idle,
		enKnightState_Run,
		enKnightState_FirstAtk,
		enKnightState_SecondAtk,
		enKnightState_Damege,
		enKnightState_Death,
		enKnightState_Skill,
		enKnightState_UltimateSkill,
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_FirstAtk,
		enAnimationClip_SecondAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		/*enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,*/
		enAnimationClip_Num,
	};
	Game* m_game;
	Vector3 firstposition;                                //最初の座標
	Vector3 m_position = Vector3::Zero;                   //座標
	Vector3 m_forward = Vector3::AxisZ;                   //正面ベクトル
	Vector3 m_moveSpeed;                                  //移動速度
	CharacterController m_charCon;                        //キャラクターコントロール
	Quaternion m_rot = Quaternion::Identity;              //クォータニオン
	ModelRender m_modelRender;                            //モデルレンダー
	AnimationClip m_animationClips[enAnimationClip_Num]; //アニメーションクリップ
	//初期ステータス 最大HP、HP、攻撃力、スピード
	Status status = { 150,150,35,150.0f };
	//レベルアップ時に増加するステータス
	LvUpStatus LvUpStatus = { 30,10,30.0f };
	PlayerState m_animState = enKnightState_Idle;
	//現在のコンボ
	int ComboState = 0;
	//コンボが継続する時間を記録する
	float ComboTimer = 0;
	//一段目のアタックをしたかの判定
	bool FirstAtkState = false;

	//獲得した経験値仮
	int exp=5;
	//受けたダメージ仮
	int dddd = 50;

};

