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
	// 
	

	bool UltimateSkillFlag = false;
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;
	SpriteRender m_AtkUpIcon_Render;

	//bool m_atkUpSpriteFlag = false;

	//スキルを使った時の移動速度
	float SkillSpeed = 270.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;

	int oldLv;

	int dddd = 20;

	//std::vector<Neutral_Enemy*> m_neutral_Enemys;
	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵

	//タワーの画像出すかどうか
	bool TowerSpriteFlag = false;
	//攻撃の画像を出すかどうか
	bool AttackSpriteFlag = false;
};

