#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();
	void Update();

	void Attack();

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	void Render(RenderContext& rc);

private:
	Game* m_game=nullptr;
	
	Vector3 AnimEndPos = Vector3::Zero;
	Vector3 OldPos = Vector3::Zero;
	Vector3 UltPos = Vector3::Zero;
	float UltimateSkillTimer = 0;

	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	AtkTimingState m_AtkTmingState = Num_State;

	//bool AtkCollistionFlag = false;
	
	bool UltimateSkillFlag = false;

	Status m_Status;                           //ステータス
												//プレイヤーの顔
	//Vector3 m_position = Vector3::Zero;
	////初期ステータス 最大HP、HP、攻撃力、スピード
	//Status status = { 150,150,35,150.0f };
	////レベルアップ時に増加するステータス
	//LvUpStatus LvUpStatus = { 30,10,30.0f };
};

