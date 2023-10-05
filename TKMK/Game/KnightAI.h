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
	/// �K�E�Z�̓����蔻�萶������
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
	/// �U���ł���Ȃ�true
	/// </summary>
	/// <returns></returns>
	const bool CanAttack();
	
	const bool CanSkill();

	const bool CanUlt();

	/// <summary>
	/// ���X�g�A�^�b�N�̈ړ��v�Z
	/// </summary>
	void LastAttackMove() {
		m_LastAttackMove = TargePos - m_position;
		m_LastAttackMove.Normalize();
	}

	void SetAndPlaySoundSource(EnSound soundNumber);

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
	/*void Rotation();*/
	void LotNextAction();

	//�A�N�^�[�̕]���l�̌v�Z
	void CalculatAIAttackEvaluationValue();
	//�����̓G�̕]���l�̌v�Z
	void CalculatEnemyAttackEvaluationValue();

	EvalData CalculateTargetEnemy(Neutral_Enemy* enemy);
	EvalData CalculateTargetAI(Actor* actor);

	void IsLevelEffect(int oldlevel, int nowlevel);

	enum Action {
		AttackAI,
		AttackEnemy,

	};

private:
	KnightPlayer* m_knightPlayer;		//���m�v���C���[
	
	Vector3                 TargePos = Vector3::Zero;
	Vector3                 m_aiForward = Vector3::Zero;
	Vector3                 m_patrolPos[5];
	Vector3					m_forward=Vector3::Zero;
	bool					m_isSearchEnemy = false;
	bool                    m_SearchPlayer_OR_AI = false;
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
	
	Vector3 m_LastAttackMove = Vector3::Zero;
	///////////////
	bool SkillFlag = false;
	bool m_swordEffectFlag = false;
	int ATKtiming = 0;

	std::array<EnemyHpBar*, enPlayerNumber_Num>	m_enemyHpBar = { nullptr,nullptr,nullptr,nullptr };		//�G�̏㕔�ɕ`�悳���HP�o�[
};

