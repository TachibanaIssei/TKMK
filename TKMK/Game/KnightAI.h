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
	// 評価値用の構造体
	struct EvalData
	{
		int eval;
		bool chaseOrEscape;
	};


	//逃げてる時にターゲットを変える
	void EscapeChange(EvalData& evaldata, Vector3 targetPos);
	//追跡と逃げる
	void ChaseAndEscape();
	void Rotation();
	void LotNextAction();

	//アクターの評価値の計算
	void CalculatAIAttackEvaluationValue();
	//中立の敵の評価値の計算
	void CalculatEnemyAttackEvaluationValue();

	EvalData CalculateTargetEnemy(Neutral_Enemy* enemy);
	EvalData CalculateTargetAI(Actor* actor);
	
	enum Action {
		AttackAI,
		AttackEnemy,

	};

	Game*                   m_game;//ゲーム
	Actor*                  Lvactor = nullptr; 

	KnightPlayer* m_knightPlayer;		//剣士プレイヤーvoid Rotation();

	Vector3                 TargePos = Vector3::Zero;
	Vector3                 m_aiForward = Vector3::Zero;
	Vector3                 m_patrolPos[5];
	Vector3					m_forward;
	bool					m_isSearchEnemy = false;
	bool                    m_SearchPlayer_OR_AI = false;
	FontRender              m_Name;
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;					//コライダー。
	RigidBody				m_rigidBody;						//剛体。	
	
	//falseだったら追いかける、trueだったら逃げる
	bool                    m_ChaseOrEscape = false;

	//どうしようもない配置の時に逃げるタイマー
	float					m_EscapeTimer = 0.0f;

	bool                    UltimateSkillFlag = false;
	bool                    UltFlug = false;
	float                   UltimateSkillTimer = 0.0f;

	Level3DRender           m_knightAIPoslevel;      //剣士AIのポジションレベル

	std::vector<Neutral_Enemy*> neutral_enemys;
	//エネミーの評価値
	std::vector<EvalData> Evaluation_valueEnemy;
	//アクターの評価値
	std::vector<EvalData> Evaluation_valueActor;
	//今自分の近くにいるエネミーのリスト
	std::vector<Neutral_Enemy*> be_Enemy;
	
	//スキル発射時の移動量
	Vector3 m_skillMove = Vector3::Zero;

	///////////////
	bool SkillFlag = false;
};

