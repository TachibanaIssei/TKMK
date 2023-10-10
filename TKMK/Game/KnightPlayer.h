#pragma once
#include "KnightBase.h"
#include "Status.h"

class Game;
class Neutral_Enemy;
class KnightUlt;
class GameUI;
class GameCamera;
class WizardUlt;
class EnemyHpBar;
class GameCamera;

class KnightPlayer:public KnightBase
{
public:
	KnightPlayer();
	~KnightPlayer();
	bool Start();
	void Update();

	void Attack();

	bool UltimaitSkillTime();
	void TowerJumpEFK();
	void Avoidance();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// ラストアタックの移動計算
	/// </summary>
	void LastAttackMove() {}

	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();

	/// <summary>
	/// 必殺技の当たり判定生成する
	/// </summary>
	void MakeUltSkill();

	void SetAndPlaySoundSource(EnSound soundNumber);

	/// <summary>
	/// プレイヤーの番号を設定する。
	/// 最大2人プレイの予定のため0か1になるようにしている
	/// </summary>
	/// <param name="number"></param>
	void SetPlayerNumber(const int number)
	{
		m_playerNumber = number;
	}
	/// <summary>
	/// プレイヤーの番号を取得する
	/// </summary>
	/// <returns>プレイヤーの番号</returns>
	int GetPlayerNumber()
	{
		return m_playerNumber;
	}

private:
	void CoolTimeProcess();
	/// <summary>
	/// UIをグレースケールにする処理
	/// </summary>
	void GrayScaleUI();

	void IsLevelEffect(int oldlevel, int nowlevel);

	/// <summary>
	/// キャラクターの頭上のHPバー処理
	/// </summary>
	void CharacterUpperHpBar();

private:
	Game* m_game=nullptr;
	GameUI* m_gameUI = nullptr;
	GameCamera* m_gameCamera = nullptr;
	
	Vector3 AnimEndPos = Vector3::Zero;
	Vector3 OldPos = Vector3::Zero;
	Vector3 UltPos = Vector3::Zero;

	Vector2 Avoidance_FlamePos = Vector2::Zero;
	Vector2 Avoidance_BarPos = Vector2::Zero;

	float UltimateSkillTimer = 0;

	bool UltimateSkillFlag = false;
	FontRender Skillfont;
	SpriteRender m_Avoidance_flameRender;
	SpriteRender m_Avoidance_barRender;

	//スキルを使った時の移動速度
	float SkillSpeed = 200.0f;
	//回避を使った時の移動速度
	float AvoidanceSpeed = 170.0f;

	int m_playerNumber = 0;	//プレイヤーの番号(ゲームパッドの番号などで使用)

	Neutral_Enemy* m_Neutral_Enemy = nullptr; //中立の敵

	std::array<EnemyHpBar*, enPlayerNumber_Num - 1>	m_enemyHpBar = { nullptr,nullptr,nullptr};		//敵の上部に描画されるHPバー
};

