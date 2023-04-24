#pragma once
#include "KnightBase.h"
#include "Status.h"
#include "WizardUlt.h"    //�֐���.cpp�Ɉڂ��������todo
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
	/// �U���ł���Ȃ�true
	/// </summary>
	/// <returns></returns>
	const bool CanAttack();

	const bool CanSkill();
	
private:
	// �]���l�p�̍\����
	struct EvalData
	{
		int eval;
		bool chaseOrEscape;
	};


	//�����Ă鎞�Ƀ^�[�Q�b�g��ς���
	void EscapeChange(EvalData& evaldata, Vector3 targetPos);
	//�ǐՂƓ�����
	void ChaseAndEscape();
	void Rotation();
	void LotNextAction();

	//�A�N�^�[�̕]���l�̌v�Z
	void CalculatAIAttackEvaluationValue();
	//�����̓G�̕]���l�̌v�Z
	void CalculatEnemyAttackEvaluationValue();

	EvalData CalculateTargetEnemy(Neutral_Enemy* enemy);
	EvalData CalculateTargetAI(Actor* actor);
	
	enum Action {
		AttackAI,
		AttackEnemy,

	};

	Game*                   m_game;//�Q�[��
	Actor*                  Lvactor = nullptr; 

	KnightPlayer* m_knightPlayer;		//���m�v���C���[void Rotation();

	Vector3                 TargePos = Vector3::Zero;
	Vector3                 m_aiForward = Vector3::Zero;
	Vector3                 m_patrolPos[5];
	Vector3					m_forward;
	bool					m_isSearchEnemy = false;
	bool                    m_SearchPlayer_OR_AI = false;
	FontRender              m_Name;
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;					//�R���C�_�[�B
	RigidBody				m_rigidBody;						//���́B	
	
	//false��������ǂ�������Atrue�������瓦����
	bool                    m_ChaseOrEscape = false;

	//�ǂ����悤���Ȃ��z�u�̎��ɓ�����^�C�}�[
	float					m_EscapeTimer = 0.0f;

	bool                    UltimateSkillFlag = false;
	bool                    UltFlug = false;
	float                   UltimateSkillTimer = 0.0f;

	Level3DRender           m_knightAIPoslevel;      //���mAI�̃|�W�V�������x��

	std::vector<Neutral_Enemy*> neutral_enemys;
	//�G�l�~�[�̕]���l
	std::vector<EvalData> Evaluation_valueEnemy;
	//�A�N�^�[�̕]���l
	std::vector<EvalData> Evaluation_valueActor;
	//�������̋߂��ɂ���G�l�~�[�̃��X�g
	std::vector<Neutral_Enemy*> be_Enemy;
	
	//�X�L�����ˎ��̈ړ���
	Vector3 m_skillMove = Vector3::Zero;

	///////////////
	bool SkillFlag = false;
};

