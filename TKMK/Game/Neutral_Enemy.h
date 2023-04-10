#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include "Status.h"
#include "Level3DRender.h"
//�N���X�錾
class KnightPlayer;
class Game;
class GameCamera;
class Neutral_Enemy;
class Patrolnumb;
class KnightAI;
class Player;
class MagicBall;
class Actor;

/// <summary>
/// �����̓G
/// </summary>
class Neutral_Enemy : public IGameObject
{

public:
	Neutral_Enemy();
	~Neutral_Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void HPBar();

	//�����̓G�̃X�e�[�g
	enum EnNEState {
		enNeutral_Enemy_Idle,					//�ҋ@�B
		enNeutral_Enemy_Chase,					//�ǐՁB
		enNeutral_Enemy_Attack,			        //�U��
		enNeutral_Enemy_ReceiveDamage,			//��_���[�W�B
		enNeutral_Enemy_Death,					//�_�E���B
		enNeutral_Enemy_Pause,                  //�|�[�Y���
		enNEutral_Enemy_Patrol,                 //����
	};

		


	void SetNeutral_EnemyGame(Game* NEgame)
	{
		m_game = NEgame;
	}
	Game* GetNeutral_EnemyGame()
	{
		return m_game;
	}



	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
	void SetRotation(Quaternion rotation)
	{
		m_rot = rotation;
	}

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	


	void Move();
	/// <summary>
	/// �ǐ�
	/// </summary>
	void Chase();
	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �����蔻��
	/// </summary>
	void Collision();
	/// <summary>
	/// �ǂ�������Ώۂ�T��
	/// ����������true��Ԃ�
	/// </summary>
	bool Search();

	void SetKnightPlayer(Actor* knightPlayer)
	{
		m_targetActor = knightPlayer;
	}
	Actor* GetKnightPlayer()
	{
		return m_targetActor;
	}

	/// <summary>
	/// �U���p�̓����蔻��
	/// </summary>
	void MakeAttackCollision();

	/// <summary>
	/// ���X�|�[��
	/// </summary>
	//void Respawn();

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	void ManageState();

	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// �A�j���[�V�����C�x���g�p�̊֐��B
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O�B</param>
	/// <param name="eventName">�A�j���[�V�����C�x���g�̃L�[�̖��O�B</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// 
	/// </summary>
	void ProcessCommonStateTransition();

	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessIdleStateTransition();

	/// <summary>
	/// ����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessRunStateTransition();

	/// <summary>
	/// �ǐՃX�e�[�g�̔w�J�ڏ����B
	/// </summary>
	void ProcessChaseStateTransition();

	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessAttackStateTransition();

	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessReceiveDamageStateTransition();

	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessDeathStateTransition();

	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessPatrolStateTransition();
	/// <summary>
	/// �U���ł���Ȃ�true
	/// </summary>
	/// <returns></returns>
	const bool CanAttack() const;

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

	/// <summary>
	/// �v���C���[�̃X�e�[�g��ύX
	/// </summary>
	/// <param name="gamescene">�ύX�������X�e�[�g�̖��O</param>
	void SetNeutral_EnemyState(EnNEState gamescene) {
		m_Neutral_EnemyState = gamescene;
	}
	/// <summary>
	/// ���񂷂���W��ݒ肷��
	/// </summary>
	/// <param name="pos">���񂷂���W</param>
	/// <param name="number">���W���Z�b�g����ԍ�</param>
	void SetPatrolPos(Vector3 pos, int number) {
		m_patrolPos[number] = pos;
	};

	/// <summary>
	/// �v���C���[�̃A�N�^�[��ݒ肷��
	/// </summary>
	void SetPlayerActor(Actor* actor) {
		m_player = actor;
	}

	enum EnAnimationClip {                      //�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Run,					//����A�j���[�V�����B
		enAnimationClip_Attack,					//�U���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Death,					//�_�E���A�j���[�V�����B
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};

private:
	AnimationClip m_animationClips[enAnimationClip_Num];       //�A�j���[�V�����N���b�v
	ModelRender   m_modelRender;                               //���f�������_�[
	Vector2		  m_HPBerPos = Vector2::Zero;				   //HP�o�[�̃|�W�V����
	Vector2	   	  m_HPWindowPos = Vector2::Zero;			   //HP�g�̃|�W�V����
	Vector2		  m_HPBackPos = Vector2::Zero;			       //HP�w�i�̃|�W�V����
	Vector3       m_position;                                  //���W
	Vector3       m_moveSpeed;                                 //�ړ����x
	Vector3       m_forward = Vector3::AxisZ;                  //���ʂ̃x�N�g��
	Quaternion    m_rot;                                       //�N�H�[�^�j�I��
	Vector3       m_scale = Vector3{0.2f,0.2f,0.2f};           //�傫��
	CharacterController m_charaCon;                            //�L�����R��
	EnNEState m_Neutral_EnemyState = enNeutral_Enemy_Idle;     //�����̓G�̃X�e�[�g�B
	Vector3   RadiusPos;
	SoundSource* m_se = nullptr;

	Actor* m_targetActor = nullptr;
	Actor* m_player = nullptr;
	Actor* m_lastAttackActor = nullptr;		// �Ō�Ɏ������U���������

	Game* m_game = nullptr;                               
	Neutral_Enemy* m_Neutral_Enemy=nullptr; 


	GameCamera* m_gameCamera = nullptr;
	Player* player = nullptr;
	MagicBall* magicBall = nullptr;
	KnightAI* m_knightAI = nullptr;

	Level3DRender m_EnemyPoslevel;      //�G�l�~�[�̃|�W�V�������x��
	Status m_Status;                    //�X�e�[�^�X
	SpriteRender		m_HPBar;		//HP�o�[�摜
	SpriteRender		m_HPFrame;		//HP�g�摜
	SpriteRender		m_HPBack;		//HP�w�i�摜	
	
	FontRender				m_fontRender;
	SphereCollider			m_sphereCollider;
	RigidBody				m_rigidBody;
	Vector3                 m_inRespawnPosition[12];
	Vector3                 m_patrolPos[9];
	Vector3 nowPos = Vector3::Zero;
	Vector3 m_hagikiPower;
	bool m_UnderAttack = false;              //�U������
	int m_AttackBoneId = 1;                  //���̃{�[����ID
	//�����̓G
	float	m_chaseTimer = 0.0f;			//�ǐՃ^�C�}�[�B
	float	m_idleTimer = 0.0f;		        //�ҋ@�^�C�}�[�B
	float   m_stopTimer = 1.0f;             //�~�܂��Ă��܂����^�C�}�[

	int enemyColorRam = 0;					//�G�l�~�[�̃J���[�����߂�
	int AtkPass = 10;						//�n���U���̗͂�
	int HpPass = 30;						//�񕜗�

	//�U�����󂯂��Ƃ��ɑ���̍U���͂��i�[����ϐ�
	int GetAtk=0;
	//���ꂽ���̃t���O
	bool Deathflag = false;
	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	int Exp = 5;

	//std::vector<Neutral_Enemy*>::iterator m_number;
	int P = -1;
	
};

