#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;
class KnightPlayer;
class Neutral_Enemy;
class CharUltFlag;

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
	//void ChaseEnemy();
	void AtkCollisiton();
	//void LevelMove();
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
	void Patrol();
	/// <summary>

	/// 攻撃できるならtrue
	/// </summary>
	/// <returns></returns>
	const bool CanAttackenemy();
	/// <summary>

	/// </summary>
	/// <returns></returns>
	const bool CanAttackActor();
	/// 巡回する座標を設定する
	/// </summary>
	/// <param name="pos">巡回する座標</param>
	/// <param name="number">座標をセットする番号</param>
	void SetPatrolPos(Vector3 pos, int number) {
		m_patrolPos[number] = pos;
	};
	void SetNeutral_Enemy(Neutral_Enemy* ptr)
	{
		m_Neutral_Enemy = ptr;
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
	/// �_�������߂�
	/// </summary>
	/// <returns>�_������̍�W</returns>
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
	Neutral_Enemy*          m_Neutral_Enemy = nullptr;			// 今追いかけているエネミー      
	Actor*					m_targetActor = nullptr;
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

