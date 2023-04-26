#pragma once

#include "Actor.h"


//class Status;
class Game;
class GameUI;

class KnightBase:public Actor
{
public:
	KnightBase();
	virtual ~KnightBase();

	//enum PlayerState {
	//	enKnightState_Idle,
	//	enKnightState_Walk,
	//	enKnightState_Run,
	//	enKnightState_ChainAtk,
	//	enKnightState_Damege,
	//	enKnightState_Death,
	//	enKnightState_Skill,
	//	enKnightState_UltimateSkill,
	//	enKnightState_Avoidance,
	//	enKnightState_Jump,
	//	enKnightState_Fall,
	//	enKnightState_Num,
	//	enKnightState_Pause,        //�Q�[���̏�Ԃ��󂯎��
	//};

	/// <summary>
	/// ���f����Init�A�L�����R���̏�����
	/// </summary>
	void SetModel();

	/// <summary>
	/// �����̓G��|�����Ƃ��̌o���l�̏���
	/// </summary>
	/// <param name="GetExp">�����̓G�̌o���l</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	virtual void Attack()=0;

	/// <summary>
	/// ���g�̓����蔻��
	/// </summary>
	void Collition();

	/// <summary>
	/// �_���[�W���󂯂��Ƃ��̏���
	/// </summary>
	/// <param name="damege">�G�̍U����</param>
	/// <param name="playerGivePoints">�|�C���g��^����L����</param>
	void Dameged(int damege,Actor* CharGivePoints);

	/// <summary>
	/// ���g���|���ꂽ�Ƃ��̏���
	/// </summary>
	void Death();

	/// <summary>
	/// �X�L�����g�p�����Ƃ��̏���
	/// </summary>
	//void Skill(Vector3& right,Vector3& forward);

	/////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// �K�E�Z�𔭓������Ƃ��̏���
	/// </summary>
	void UltimateSkill();
	//////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// ���X�|�[��������W�̃Z�b�g
	/// </summary>
	void SetRespawn();

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �ʏ�U���̓����蔻��̏���
	/// </summary>
	void AtkCollisiton();

	/// <summary>
	/// �K�E�Z�̓����蔻��̏���
	/// </summary>
	void UltimateSkillCollistion(Vector3& oldpostion, Vector3& position);

	/// <summary>
	/// �A�j���[�V�����̃X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// �A�j���[�V�����Đ����̈ړ������A�ړ����x�����߂�
	/// </summary>
	/// <param name="moveSpeed">�X�e�B�b�N�̈ړ��ʂƏ�Z���������X�s�[�h�̒l</param>
	/// <param name="stickL">�X�e�B�b�N�̈ړ��̓��͗�</param>
	void AnimationMove(float moveSpeed,Vector3 stickL);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="right"></param>
	/// <param name="forward"></param>
	void MoveStraight(Vector3& right, Vector3& forward);

	/// <summary>
	/// �A�j���[�V�����C�x���g
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	virtual void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)=0;

	/// <summary>
	/// ���W�̃Z�b�g
	/// </summary>
	/// <param name="PS"></param>
	inline void SetPosition(Vector3 PS) { m_position = PS; }
	inline void SetCharaconPosition(Vector3 PS) { m_charCon.SetPosition(PS); }

	/// <summary>
	/// ���m�̍��W��Ԃ�l�Ƃ��ĕԂ�
	/// </summary>
	virtual Vector3 GetPosition() = 0;

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}
	Game* GetSGame()
	{
		return m_game;
	}
	//
	void SetGameUI(GameUI* gameUI)
	{
		m_gameUI = gameUI;
	}
	GameUI* GetSGameUI()
	{
		return m_gameUI;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ����Ȃ�false��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_charState != enCharState_Attack &&
			m_charState != enCharState_UltimateSkill &&
			m_charState != enCharState_Skill &&
			m_charState != enCharState_Avoidance &&
			m_charState != enCharState_Damege &&
			m_charState != enCharState_Death;

		/*return m_charState != enKnightState_ChainAtk &&
			m_charState != enKnightState_UltimateSkill &&
			m_charState != enKnightState_Skill &&
			m_charState != enKnightState_Avoidance &&
			m_charState != enKnightState_Damege&&
			m_charState != enKnightState_Death;*/
	}



	/// <summary>
	/// ���݂̃��x����Ԃ�
	/// </summary>
	/// <returns>���݂̃��x��</returns>
	int& SetLevel(){
		return Lv;
	}

	/// <summary>
	/// ���݂̃q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns>���݂�HP</returns>
	int& SetHp() {
		return m_Status.Hp;
	}

	

	Quaternion& GetRot()
	{
		return m_rot;
  }


	/// <summary>
	/// ���݂̃}�b�N�X�q�b�g�|�C���g��Ԃ�
	/// </summary>
	/// <returns></returns>
	int& SetMaxHp() {
		return m_Status.MaxHp;
	}
	//////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// �v���C���[�̃X�e�[�g��ύX
	/// </summary>
	/// <param name="gamescene">�ύX�������X�e�[�g�̖��O</param>
	/*void SetPlayerState(PlayerState gamescene) {
		m_charState = gamescene;
		m_charState = gamescene;
	}*/

	/// <summary>
	/// ���X�|�[������ԍ������߂�
	/// </summary>
	void SetRespawnNumber(int number)
	{
		respawnNumber = number;
	}
	
protected:
	/// <summary>
	///���G���ԗp
	/// </summary>
	void Invincible();
	void PlayAnimation();
	//���ʂ̃X�e�[�g�̑J�ڏ���
	void OnProcessCommonStateTransition();
	//�ҋ@�̃X�e�[�g�̑J�ڏ���
	void OnProcessIdleStateTransition();
	//�����̃X�e�[�g�̑J�ڏ���
	void OnProcessRunStateTransition();
	//�W�����v�̃X�e�[�g�̑J�ڏ���
	void OnProcessJumpStateTransition();
	//�������̃X�e�[�g�̑J�ڏ���
	void OnProcessFallStateTransition();
	//�`�F�C���A�^�b�N�̃X�e�[�g�̑J�ڏ���
	void OnProcessChainAtkStateTransition();
	//�X�L���̃X�e�[�g�̑J�ڏ���
	void OnProcessSkillAtkStateTransition();
	//����̃X�e�[�g�̑J�ڏ���
	void OnProcessAvoidanceStateTransition();
	//�K�E�Z�̃X�e�[�g�̑J�ڏ���
	void OnProcessUltimateSkillAtkStateTransition();
	//�_���[�W���󂯂��Ƃ��̃X�e�[�g�̑J�ڏ���
	void OnProcessDamegeStateTransition();
	//HP��0�ɂȂ����Ƃ��̃X�e�[�g�̑J�ڏ���
	void OnProcessDeathStateTransition();
	//�K�E�Z�̏I��
	void UltEnd();

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_ChainAtk,
		enAnimationClip_Damege,
		enAnimationClip_Death,
		enAnimationClip_Skill,
		enAnimationClip_UltimateSkill,
		enAnimationClip_Avoidance,
		enAnimationClip_Jump,
		enAnimationClip_Fall,
		enAnimationClip_Num,
	};

	Game* m_game=nullptr;
	GameUI* m_gameUI = nullptr;

	//�����X�e�[�^�X �ő�HP�AHP�A�U���́A�X�s�[�h
	

	Vector3 firstposition;                                //�ŏ��̍��W
	Vector3 OldPosition = Vector3::Zero;                  //�O�̃t���[���̍��W
	float m_position_YUp = 33.0f;                         //���f���̎������ɂ���̂�Y���W��50.0f�グ��
	Vector3 m_forward = Vector3::AxisZ;                   //���ʃx�N�g��
	Vector3 collisionRot= Vector3::Zero;                  //�K�E�Z
	CollisionObject* collisionObject;                     //�R���W����
	Vector3 UltCollisionPos= Vector3::Zero;               //�K�E�Z�̓����蔻��̍��W
	Vector3 m_Skill_Right = Vector3::Zero;                 //�J�����̉E����
	Vector3 m_Skill_Forward = Vector3::Zero;               //�J�����̑O����
	//Vector3 m_SwordPos = Vector3::Zero;						//���̍��W
	CharacterController m_charCon;                        //�L�����N�^�[�R���g���[��
	
	

	AnimationClip m_animationClips[enAnimationClip_Num]; //�A�j���[�V�����N���b�v
	//PlayerState m_charState = enKnightState_Idle/* = enKnightState_Num*/;
	
	Actor* m_lastAttackActor = nullptr;		// �Ō�Ɏ������U���������
	Actor* m_Neutral_enemy = nullptr;       //�����̓G�p�̃_���[�W���󂯂��Ƃ��Ɏg���C���X�^���X�Bnullptr�̂܂܂ɂ���

	enum AtkTimingState
	{
		FirstAtk_State,
		SecondAtk_State,
		SecondAtkStart_State,
		LastAtk_State,
		Num_State,

	};
	AtkTimingState m_AtkTmingState = Num_State;

	//���݂̃R���{
	int ComboState = 0;
	//�R���{���p�����鎞�Ԃ��L�^����
	float ComboTimer = 0;
	//�{�^���������ꂽ���̔���
	bool pushFlag = false;
	//��i�ڂ̃A�^�b�N���������̔���
	bool AtkState = false;
	//�X�L���̃A�j���[�V�����Đ����I��������̔���
	bool SkillEndFlag = false;
	//����A�j���[�V�������Đ��������̔���
	bool AvoidanceFlag = false;
	//
	bool AvoidanceEndFlag = false;
	//�u�v�{�[����ID
	int m_swordBoneId = -1;
	//�U���A�j���[�V�����C�x���g�Đ����̌��m�̍��W���擾����
	int AtkEndPosId= -1;
	//�X�L���̃N�[���^�C�����v�Z����^�C�}�[
	float SkillTimer = 0;
	//����̃N�[���^�C�����v�Z����^�C�}�[
	float AvoidanceTimer = 0;
	//���G���Ԃ��v�Z����^�C�}�[
	float invincibleTimer = 0;
	//�l�������o���l��
	int exp=1;
	//Newtral_Enemy�̍U����
	int Enemy_atk = 10;
	//�K�E�Z�g�p�̃t���O
	bool UltCollisionSetFlag = false;
	//�U�����̌��̃R���W������\�����邩�̃t���O
	bool AtkCollistionFlag = false;

	bool jampAccumulateflag = false;

};

