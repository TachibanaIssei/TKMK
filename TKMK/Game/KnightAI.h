#pragma once
#include "KnightBase.h"
#include "Status.h"
class Game;
class KnightPlayer;
class Neutral_Enemy;

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
	/// �G�l�~�[�ւ̍U���ł���Ȃ�true
	/// </summary>
	/// <returns></returns>
	const bool CanAttackenemy();
	/// <summary>
	/// �L�����N�^�[�ւ̍U���ł���Ȃ�true
	/// </summary>
	/// <returns></returns>
	const bool CanAttackActor();
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
	/// �_����������߂�
	/// </summary>
	/// <returns>�_������̍��W</returns>
	const Vector3 TargetChange();

	AtkTimingState m_AtkTmingState = Num_State;
	Game* m_game;//�Q�[��
	KnightPlayer* m_knightPlayer;//���m�v���C���[void Rotation();
	void SearchEnemy();
	void Rotation();
	Vector3					m_forward;
	bool					m_isSearchEnemy = false;
	bool                    m_SearchPlayer_OR_AI = false;
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;					//�R���C�_�[�B
	RigidBody				m_rigidBody;						//���́B	
	Neutral_Enemy*          m_Neutral_Enemy = nullptr;			// ���ǂ������Ă���G�l�~�[      
	Neutral_Enemy*          m_targetEnemy = nullptr;          //�^�[�Q�b�g�Ȃ��Ă�G
	Actor*					m_targetActor = nullptr;
	bool UltimateSkillFlag = false;
	bool PL = true;
	float SkillSpeed = 270.0f;
	float UltimateSkillTimer = 0;
	Level3DRender m_knightAIPoslevel;      //���mAI�̃|�W�V�������x��
	int enemyAmount = 0;
	Vector3 nearPos = Vector3::Zero;

};

