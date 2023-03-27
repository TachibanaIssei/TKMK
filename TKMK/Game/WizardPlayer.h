#pragma once
#include "WizardBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class MagicBall;

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
	void UltimateSkill();

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

	//void Death()
	//{
	//	////死亡ステート
	//	//m_playerState = enKnightState_Death;
	//	//レベルを１下げる
	//	levelDown(LvUpStatus, m_Status, Lv, 1);
	//	//HPを最大にする
	//	m_Status.Hp = m_Status.MaxHp;
	//	//経験値をリセット
	//	ExpReset(Lv, GetExp);
	//	//一つ下のレベルの経験値テーブルにする
	//	ExpTableChamge(Lv, ExpTable);

	//	//レベルに合わせてレベルの画像を変更する
	//	m_gameUI->LevelFontChange(Lv);
	//}

	//void AvoidanceSprite();





private:
	Game* m_game = nullptr;
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
	float AvoidanceSpeed = 170.0f;

	bool m_spriteFlag = true;

	int exp = 5;
	int dddd = 10;
};

