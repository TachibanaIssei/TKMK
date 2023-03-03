#pragma once

#include "Actor.h"
class Game;

class KnightBase:public Actor
{
public:
	KnightBase();
	~KnightBase();
	bool Start();
	void Update();
	void Move();
	void Render(RenderContext& rc);
	/// <summary>
	/// 中立の敵を倒したときの経験値の処理
	/// </summary>
	/// <param name="GetExp">中立の敵の経験値</param>
	void ExpProcess(int Exp);
	void Attack();
	void Dameged(int damege);
	void Skill();
	void UltimateSkill();
	void SetRespawn();
	void Death();
	void Rotation();
	//ステート管理。
	void ManageState();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	inline void SetPosition(Vector3 PS) { m_position = PS; }
	inline Vector3 GetPosition() { return m_position; }
	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	bool IsEnableMove() const
	{
		return m_animState != enKnightState_FirstAtk &&
			m_animState != enKnightState_SecondAtk//&&
			/*m_animState!= enKnightState_Damege*/;
	}


private:
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
	//HPが0になったときの処理
	void OnProcessDeathStateTransition();

	enum AnimationState {
		enKnightState_Idle,
		enKnightState_Run,
		enKnightState_FirstAtk,
		enKnightState_SecondAtk,
		enKnightState_Damege,
		enKnightState_Death,
	};
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_FirstAtk,
		enAnimationClip_SecondAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
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

	//初期ステータス
	Status status = { 150,35,150.0f };
	//レベルアップ時に増加するステータス
	LvUpStatus LvUpStatus = { 30,10,30.0f };
	AnimationState m_animState = enKnightState_Idle;
	//現在のコンボ
	int ComboState = 0;
	//コンボが継続する時間を記録する
	float ComboTimer = 0;


	//獲得した経験値仮
	int exp=5;
	//受けたダメージ仮
	int dddd = 50;

};

