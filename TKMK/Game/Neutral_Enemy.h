#pragma once
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include "Status.h"
//�N���X�錾
class KnightPlayer;
class Game;
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
	//�����̓G�̃X�e�[�g
	enum EnNEState {
		enNeutral_Enemy_Idle,					//�ҋ@�B
		enNeutral_Enemy_Chase,					//�ǐՁB
		enNeutral_Enemy_Attack,			        //�U��
		enNeutral_Enemy_ReceiveDamage,			//��_���[�W�B
		enNeutral_Enemy_Death,					//�_�E���B
	};

	void SetNeutral_EnemyGame(Game* NEgame)
	{
		m_game = NEgame;
	}
	Game* GetNeutral_EnemyGame()
	{
		return m_game;
	}
private:
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
	/// �v���C���[������������
	/// </summary>
	const bool SearchEnemy()const;
	/// <summary>
	/// �U���p�̓����蔻��
	/// </summary>
	void MakeAttackCollision();
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
	/// �U���ł���Ȃ�true
	/// </summary>
	/// <returns></returns>
	const bool CanAttack() const;

	

	enum EnAnimationClip {                      //�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Run,					//����A�j���[�V�����B
		enAnimationClip_Attack,					//�U���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Death,					//�_�E���A�j���[�V�����B
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};

	AnimationClip m_animationClips[enAnimationClip_Num];//�A�j���[�V�����N���b�v
	ModelRender   m_modelRender;              //���f�������_�[
	Vector3 m_position;                       //���W
	Vector3 m_moveSpeed;                      //�ړ����x
	Vector3 m_forward = Vector3::AxisZ;      //���ʂ̃x�N�g��
	Quaternion m_rot;                        //�N�H�[�^�j�I��
	Vector3 m_scale = Vector3{0.2f,0.2f,0.2f};          //�傫��
	CharacterController m_charaCon;          //�L�����R��
	EnNEState m_Neutral_EnemyState = enNeutral_Enemy_Idle;    //�����̓G�̃X�e�[�g�B
	bool m_UnderAttack = false;              //�U������
	int m_AttackBoneId = 1;                  //���̃{�[����ID
	Game* m_game = nullptr;                  //�Q�[��
	KnightPlayer* m_knightPlayer;                        //���m
	float					m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float					m_idleTimer = 0.0f;		                    //�ҋ@�^�C�}�[�B
	Status m_Status;                           //�X�e�[�^�X
	//�i�r�Q�[�V�������b�V��
	TknFile m_tknFile;
	PhysicsStaticObject m_bgObject;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	Vector3 m_targetPointPosition;
};

