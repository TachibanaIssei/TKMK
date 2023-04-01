#pragma once
#include "KnightBase.h"
#include "Status.h"
#include "WizardUlt.h"    //関数を.cppに移したら消すtodo
class Game;
class KnightPlayer;
class Neutral_Enemy;
class CharUltFlag;
class WizardUlt;

class KnightAI:public KnightBase
{
public:

	struct EnemyPositions
	{
		Vector3 AIpos[20];
		bool foundFlag[20];
	};
	EnemyPositions m_enemyPositions;

	KnightAI();
	~KnightAI();
	void Update();
	void Collition();
	void Attack();
	void Render(RenderContext& rc);
	void AtkCollisiton();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	inline Vector3 GetPosition() { return m_position; }

	void AvoidanceSprite();
	void SetGame(Game* game)
	{
		m_game = game;
	}
	Game* GetGame()
	{
		return m_game;
	}
	/// 攻撃できるならtrue
	/// </summary>
	/// <returns></returns>
	const bool CanAttackenemy();
	/// <summary>

	/// </summary>
	/// <returns></returns>
	const bool CanAttackActor();
	//.cppに移す　todo
	void MakeWizardUlt()
	{
		//必殺技の雷の生成
		WizardUlt* wizardUlt = NewGO<WizardUlt>(0, "wizardUlt");
		wizardUlt->SetCreatorName(GetName());

		Vector3 UltPos = Actor::GetPosition();
		UltPos.y += 100.0f;
		wizardUlt->SetPosition(UltPos);
	}

private:
	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	/// <summary>
	/// ?_?????????
	/// </summary>
	/// <returns>?_????????W</returns>
	const Vector3 TargetChange();
	AtkTimingState m_AtkTmingState = Num_State;
	Game* m_game;//ゲーム
	KnightPlayer* m_knightPlayer;//剣士プレイヤーvoid Rotation();
	void SearchEnemy();
	void Rotation();
	Vector3					m_forward;
	bool					m_isSearchEnemy = false;
	bool                    m_SearchPlayer_OR_AI = false;
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;					//コライダー。
	RigidBody				m_rigidBody;						//剛体。	
	CharUltFlag* charUltFlag = nullptr;
	Neutral_Enemy* m_targetEnemy = nullptr;					// 今追いかけているエネミー      
	Actor* m_targetActor = nullptr;
	bool UltimateSkillFlag = false;
	bool PL = true;
	float SkillSpeed = 270.0f;
	float UltimateSkillTimer = 0;
	Level3DRender m_knightAIPoslevel;      //剣士AIのポジションレベル
	Vector3                 m_patrolPos[5];
	int P = -1;
	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	int enemyAmount = 0;
	Vector3 nearPos = Vector3::Zero;

};

