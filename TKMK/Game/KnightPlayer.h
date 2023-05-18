#pragma once
#include "KnightBase.h"

#include "Status.h"

class Game;
class Neutral_Enemy;
class KnightUlt;
class GameUI;
class GameCamera;
class WizardUlt;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();
	bool Start();
	void Update();

	void Attack();

	bool UltimaitSkillTime();

	void Avoidance();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();

	/// <summary>
	/// 必殺技の当たり判定生成する
	/// </summary>
	void MakeUltSkill();


private:
	void CoolTimeProcess();
	/// <summary>
	/// UIをグレースケールにする処理
	/// </summary>
	void GrayScaleUI();



private:
	Game* m_game=nullptr;
	//GameCamera* gameCamera = nullptr;
	GameUI* m_gameUI = nullptr;
	
	//CollisionObject* collisionObject;                     //コリジョン
	Vector3 AnimEndPos = Vector3::Zero;
	Vector3 OldPos = Vector3::Zero;
	Vector3 UltPos = Vector3::Zero;

	Vector2 Avoidance_FlamePos = Vector2::Zero;
	Vector2 Avoidance_BarPos = Vector2::Zero;

	EffectEmitter* EffectKnightSkill;
	bool m_swordEffectFlag = false;

	float UltimateSkillTimer = 0;
  
	//bool AtkCollistionFlag = false;
	ChaseEFK* m_FootSmoke = nullptr;

	bool UltimateSkillFlag = false;
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;
	SpriteRender m_AtkUpIcon_Render;

	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;

	int oldLv = 1;

	int dddd = 20;

	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵

	//タワーの画像出すかどうか
	bool TowerSpriteFlag = false;
	//攻撃の画像を出すかどうか
	bool AttackSpriteFlag = false;

	ChaseEFK* LevelUp = nullptr;
};

