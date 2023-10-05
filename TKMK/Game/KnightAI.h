#pragma once
#include "KnightBase.h"
#include "Status.h"

class Game;
class KnightPlayer;
class Neutral_Enemy;
class WizardUlt;
class EnemyHpBar;

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
	bool Start();
	void Update();
	void Attack();
	bool UltimaitSkillTime();
	void Render(RenderContext& rc);
	void AtkCollisiton();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	inline Vector3 GetPosition() { return m_position; }
	void Move();
	/// <summary>
	/// 必殺技の当たり判定生成する
	void MakeUltSkill();
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

	const bool CanUlt();

	/// <summary>
	/// ラストアタックの移動計算
	/// </summary>
	void LastAttackMove() {
		m_LastAttackMove = TargePos - m_position;
		m_LastAttackMove.Normalize();
	}

	void SetAndPlaySoundSource(EnSound soundNumber);

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
	/*void Rotation();*/
	void LotNextAction();

	//アクターの評価値の計算
	void CalculatAIAttackEvaluationValue();
	//中立の敵の評価値の計算
	void CalculatEnemyAttackEvaluationValue();

	EvalData CalculateTargetEnemy(Neutral_Enemy* enemy);
	EvalData CalculateTargetAI(Actor* actor);

	void IsLevelEffect(int oldlevel, int nowlevel);

	enum Action {
		AttackAI,
		AttackEnemy,

	};

private:
	KnightPlayer* m_knightPlayer;		//剣士プレイヤー
	
	Vector3                 TargePos = Vector3::Zero;
	Vector3                 m_aiForward = Vector3::Zero;
	Vector3                 m_patrolPos[5];
	Vector3					m_forward=Vector3::Zero;
	bool					m_isSearchEnemy = false;
	bool                    m_SearchPlayer_OR_AI = false;
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
	
	Vector3 m_LastAttackMove = Vector3::Zero;
	///////////////
	bool SkillFlag = false;
	bool m_swordEffectFlag = false;
	int ATKtiming = 0;

	std::array<EnemyHpBar*, enPlayerNumber_Num>	m_enemyHpBar = { nullptr,nullptr,nullptr,nullptr };		//敵の上部に描画されるHPバー
};

