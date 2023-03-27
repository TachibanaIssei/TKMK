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
	void Move();
	void Collition();
	void Attack();
	void Render(RenderContext& rc);
	void ChaseEnemy();
	void AtkCollisiton();
	void LevelMove();
	void ChasePlayer_OR_AI();
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
	/// �U���ł���Ȃ�true
	/// </summary>
	/// <returns></returns>
	const bool CanAttack() const;
	/// <summary>
	/// �����ɍU���͂���������
	/// </summary>
	/// <param name="attack">���m�̍U���͂����������ϐ�</param>
	/// <returns></returns>
	int SetKnightAIAtk(/*int& attack*/) {
		return m_Status.Atk;
	}
	/// <summary>
	/// �����̓G��|�����Ƃ��̌o���l�̏���
	/// </summary>
	/// <param name="GetExp">�����̓G�̌o���l</param>
	void ExpProcess(int Exp);
	/// <summary>
	/// ���񂷂���W��ݒ肷��
	/// </summary>
	/// <param name="pos">���񂷂���W</param>
	/// <param name="number">���W���Z�b�g����ԍ�</param>
	void SetPatrolPos(Vector3 pos, int number) {
		m_patrolPos[number] = pos;
	};

private:
	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	AtkTimingState m_AtkTmingState = Num_State;
	Game* m_game;//�Q�[��
	KnightPlayer* m_knightPlayer;//���m�v���C���[void Rotation();
	void SearchEnemy();
	void Rotation();
	Quaternion				m_rotation;
	Vector3					m_forward;
	bool					m_isSearchEnemy = false;
	bool                    m_SearchPlayer_OR_AI = false;
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;							//�R���C�_�[�B
	RigidBody				m_rigidBody;						//���́B	
	Neutral_Enemy*          m_Neutral_Enemy = nullptr;
	bool UltimateSkillFlag = false;
	bool PL = true;
	float SkillSpeed = 270.0f;
	float UltimateSkillTimer = 0;
	Level3DRender m_knightAIPoslevel;      //���mAI�̃|�W�V�������x��
	Vector3                 m_patrolPos[5];
	int P = -1;
	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	int enemyAmount = 0;
	Vector3 nearPos = Vector3::Zero;
};

