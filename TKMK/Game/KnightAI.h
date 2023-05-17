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
	bool Start();
	void Update();
	void Attack();
	bool UltimaitSkillTime();
	void Render(RenderContext& rc);
	void AtkCollisiton();
	void HPBar();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	inline Vector3 GetPosition() { return m_position; }
	void Move();
	/// <summary>
	/// �K�E�Z�̓����蔻�萶������
	void MakeUltSkill();
	void AvoidanceSprite();
	/// <summary>
	/// �Q�[�W�����񂹂��鏈��
	/// </summary>
	/// <param name="size">�摜�̌��̑傫��</param>
	/// <param name="scale">���݂̃X�P�[���{��</param>
	/// <returns>�ϊ��O�ƕϊ���̍�</returns>
	Vector3 HPBerSend(Vector3 size, Vector3 scale);

	/// <summary>
	/// HP�Q�[�W�̕`��t���O
	/// </summary>
	/// <returns>�`��ł���͈͂ɂ���Ƃ�true</returns>
	bool DrawHP();
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
	/// �v���C���[�̃A�N�^�[��ݒ肷��
	/// </summary>
	void SetPlayerActor(Actor* actor) {
		m_player = actor;
	}
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
	bool m_swordEffectFlag = false;



	SpriteRender		m_HP_Bar;		//HP�o�[�摜
	SpriteRender		m_HP_Frame;		//HP�g�摜
	SpriteRender		m_HP_Back;		//HP�w�i�摜
	Vector2		  m_HPBer_Pos = Vector2::Zero;				   //HP�o�[�̃|�W�V����
	Vector2	   	  m_HPWindow_Pos = Vector2::Zero;			   //HP�g�̃|�W�V����
	Vector2		  m_HPBack_Pos = Vector2::Zero;			       //HP�w�i�̃|�W�V����
	Actor* m_player = nullptr;
	

	int ATKtiming = 0;
};

