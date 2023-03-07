#pragma once
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
		enNEState_Idle,					//�ҋ@�B
		enNEState_Chase,					//�ǐՁB
		enNEState_Attack,			    //�U��
		enNEState_ReceiveDamage,			//��_���[�W�B
		enNEState_Death,					//�_�E���B
	};
	void SetNEGame(Game* NEgame)
	{
		m_game = NEgame;
	}
	Game* GetNEGame()
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
	Vector3 m_scale = Vector3::One;          //�傫��
	//CharacterController m_charaCon;          //�L�����R��
	EnNEState m_NEState = enNEState_Idle;    //�����̓G�̃X�e�[�g�B
	bool m_UnderAttack = false;              //�U������
	int m_hp = 0;                            //HP
	Game* m_game = nullptr;                  //�Q�[��
	KnightPlayer* m_knightPlayer;                        //���m
	float					m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float					m_idleTimer = 0.0f;		                    //�ҋ@�^�C�}�[�B

};

