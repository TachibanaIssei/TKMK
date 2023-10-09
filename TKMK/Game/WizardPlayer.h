#pragma once
#include "WizardBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class MagicBall;
class WizardUlt;
class GameUI;

class WizardPlayer:public WizardBase
{
public:
	WizardPlayer();
	~WizardPlayer();

	void Update();
	void Attack();
	void Avoidance();
	void Render(RenderContext& rc);

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();

	/// <summary>
	/// マジックボールの生成
	/// </summary>
	void MakeMagicBall();

	/// <summary>
	/// 必殺技の雷の生成
	/// </summary>
	void MakeUltimateSkill();

	/// <summary>
	/// プレイヤーのステートを変更
	/// </summary>
	/// <param name="gamescene">変更したいステートの名前</param>
	void SetPlayerState(WizardState gamescene) {
		m_wizardState = gamescene;

	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}

	/// <summary>
	/// プレイヤーの番号を設定する
	/// </summary>
	void SetPlayerNumber(const int number)
	{
		m_playerNumber = number;
	}

	int GetPlayerNumber()
	{
		return m_playerNumber;
	}

	//void Death()
	//{
	//	////死亡ステート
	//	//m_charState = enKnightState_Death;
	//	//レベルを１下げる
	//	levelDown(LvUpStatus, m_status, Lv, 1);
	//	//HPを最大にする
	//	m_status.m_hp = m_status.m_maxHp;
	//	//経験値をリセット
	//	ExpReset(Lv, GetExp);
	//	//一つ下のレベルの経験値テーブルにする
	//	ExpTableChamge(Lv, ExpTable);

	//	//レベルに合わせてレベルの画像を変更する
	//	m_gameUI->LevelSpriteChange(Lv);
	//}

	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
	GameUI* gameUI = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵
	//MagicBall* magicBall = nullptr;

	CollisionObject* AtkCollision;                   //アタックのコリジョン
	Vector3 AtkCollisionPos = Vector3::Zero;
	Vector3 AtkCollisionVec = Vector3::Zero;
	
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;

	//アタックのコリジョンを作成したかの判定
	bool AtkCollisionSetFlag = false;
	bool AttackFlag = false;
	float AttackTimer = 0.0f;


	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 150.0f;

	bool m_spriteFlag = true;

	int exp = 5;
	int dddd = 10;

	int oldLv = 0;

	int m_playerNumber = 0;
};

