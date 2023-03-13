#pragma once
#include "KnightBase.h"

#include "Status.h"

class Game;
//class GameUI;

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

	/*void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}*/
	/*Game* GetSGame()
	{
		return m_game;
	}*/

	

	/// <summary>
	/// �摜�\���t���O��擾����
	/// </summary>
	/// <returns>false�Ȃ�\�����Ȃ�</returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

private:
	Game* m_game=nullptr;
	//GameUI* m_gameUI = nullptr;
	
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

	bool m_spriteFlag = true;

	bool m_spriteFlag = true;
	//Status m_Status;                           //ステータス

	//Vector3 m_position = Vector3::Zero;
	////初期ステータス 最大HP、HP、攻撃力、スピード
	//Status status = { 150,150,35,150.0f };
	////レベルアップ時に増加するステータス
	//LvUpStatus LvUpStatus = { 30,10,30.0f };
};

