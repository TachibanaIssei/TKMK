#pragma once
#include "KnightBase.h"
#include "Status.h"
#include "WizardUlt.h"    //関数を.cppに移したら消すtodo
class Game;
class KnightPlayer;
class Neutral_Enemy;
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
	const bool CanAttack();

	const bool CanSkill();


private:
	void LotNextAction();
	void CalculatAIAttackEvaluationValue();
	void CalculatEnemyAttackEvaluationValue();
	int CalculateTargetEnemy(Neutral_Enemy* enemy);
	int CalculateTargetAI(Actor* actor);

	enum Action {
		AttackAI,
		AttackEnemy,

	};
	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
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
	Actor* Lvactor = nullptr;
	bool UltimateSkillFlag = false;
	bool AIget = false;
	bool Enemyget = false;
	bool UltFlug = false;
	float UltimateSkillTimer = 0;
	Level3DRender m_knightAIPoslevel;      //剣士AIのポジションレベル
	Vector3                 m_patrolPos[5];
	int P = -1;
	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	int enemyAmount = 0;

	Vector3 TargePos = Vector3::Zero;
	Vector3 m_aiForward  = Vector3::Zero;
	Neutral_Enemy* m_targetEnemy = nullptr;					// 今追いかけているエネミー      
	Actor* m_targetActor = nullptr;

	std::vector<Neutral_Enemy*> neutral_enemys;

	//エネミーの評価値
	std::vector<int> Evaluation_valueEnemy;
	//アクターの評価値
	std::vector<int> Evaluation_valueActor;


	///////////////
	bool SkillFlag = false;
};

