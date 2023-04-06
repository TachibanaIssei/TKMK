#pragma once
#include "KnightBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class KnightUlt;
class GameUI;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();
	void Update();

	void Attack();

	void Avoidance();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();

	/// <summary>
	/// 必殺技の当たり判定生成する
	/// </summary>
	void MakeUltSkill();

	/// <summary>
	/// スキルを発動したときに範囲内で一番近い敵をねらう処理
	/// </summary>
	//void SkillTarget()
	//{
	//	m_neutral_Enemys = FindGOs<Neutral_Enemy>("Neutral_Enemy");

	//	Vector3 nearPos = Vector3::Zero;
	//	//一番近い距離
	//	float Near = nearPos.Length();
	//	for (auto enemy : m_neutral_Enemys)
	//	{
	//		Vector3 toEnemy = enemy->GetPosition() - m_position;
	//		//エネミーとの距離を計算する
	//		float newNear = toEnemy.Length();
	//		//計算した距離が一番近い距離より小さいなら上書き
	//		if (Near > newNear) {
	//			Near = newNear;
	//		}
	//	}
	//	if (Near < 300) {

	//	}
	//}


private:
	Game* m_game=nullptr;
	
	GameUI* m_gameUI = nullptr;
	
	//CollisionObject* collisionObject;                     //コリジョン
	Vector3 AnimEndPos = Vector3::Zero;
	Vector3 OldPos = Vector3::Zero;
	Vector3 UltPos = Vector3::Zero;

	Vector2 Avoidance_FlamePos = Vector2::Zero;
	Vector2 Avoidance_BarPos = Vector2::Zero;

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
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;

	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;

	int oldLv;

	int dddd = 20;

	//std::vector<Neutral_Enemy*> m_neutral_Enemys;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵
};

