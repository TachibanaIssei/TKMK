#pragma once
#include "Status.h"
#include "Level3DRender.h"

class Actor:public IGameObject
{
public:
	Actor();
	virtual ~Actor();
	
protected:

	/// <summary>
	/// ���x���A�b�v���ɑ�������X�e�[�^�X
	/// </summary>
	struct LvUpStatus {
		int LvHp;                  //���x���A�b�v���̃q�b�g�|�C���g
		int LvAtk;                 //���x���A�b�v���̍U����
		float LvSpeed;             //���x���A�b�v���̈ړ����x
	};

public:

	//�Q�[���N���X�̌��݂̏�Ԃ������X�e�[�g
	enum EnGameState
	{
		enStart,
		enGame,
		enPause,
		enEnd
	};
	EnGameState m_GameState= enGame;

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="position">�L�����N�^�[�̌��݂̍��W</param>
	/// <param name="charcon">�L�����R��</param>
	/// <param name="status">�L�����̃X�e�[�^�X</param>
	/// <param name="stickL">�ړ��̓��͗�</param>
	void Move(Vector3& position,CharacterController& charcon, Status& status,Vector3 stickL);

	/// <summary>
	/// �ړ������������Ȃ��A�j���[�V�����̃X�e�[�g������
	/// </summary>
	/// <returns>�ړ��������Ȃ��A�j���[�V�����̃X�e�[�g</returns>
	virtual bool IsEnableMove()const = 0;

	/// <summary>
	/// ���X�|�[��������W���擾����
	/// </summary>
	void GetRespawnPos();

	virtual void SetRespawn() = 0;

	/// <summary>
	/// �U���̏���,�������z�֐��Ȃ̂Ŕh���N���X�Ŏ���
	/// </summary>
	virtual void Attack() = 0;

	/// <summary>
	/// �U�����󂯂��Ƃ��̏���
	/// </summary>
	/// <param name="damege">�G�̍U����</param>
	virtual void Dameged(int damege) = 0;

	/// <summary>
	/// �K�E�Z��ł���
	/// </summary>
	virtual void UltimateSkill() = 0;

	/// <summary>
	/// �X�L�����g�p���ꂽ��̃N�[���^�C���̏���
	/// </summary>
	/// <param name="SkillCooltimer">�X�L���̃N�[���^�C��</param>
	/// <param name="skillstate">�X�L�����g�p�������̔���</param>
	/// <param name="timer">�N�[���^�C�����v�Z����ϐ�</param>
	void COOlTIME(float SkillCooltimer,bool& skillstate,float& timer);
	
	/// <summary>
	/// ���݂̃��x����Ԃ��@KnightBase�̂������Ă���to do
	/// </summary>
	/// <returns></returns>
	inline int& GetLevel() { return Lv; }

	/// <summary>
	/// ���x���A�b�v�̏���
	/// </summary>
	/// <param name="lus">���x���A�b�v���ɑ�������X�e�[�^�X</param>
	/// <param name="nowStatus">���݂̃X�e�[�^�X</param>
	/// <param name="Level">���݂̃��x��</param>
	void LevelUp(LvUpStatus& lus, Status& nowStatus,int& Level);

	/// <summary>
	/// ���x���_�E���̏���
	/// </summary>
	/// <param name="lus">���x���A�b�v���ɑ�������X�e�[�^�X</param>
	/// <param name="nowStatus">���݂̃X�e�[�^�X</param>
	/// <param name="Level">���݂̃��x��</param>
	void levelDown(LvUpStatus& lus, Status& nowStatus, int& Level,int downLevel);

	/// <summary>
    /// ���X�|�[�������Ƃ��̃��x���ɂ���Čo���l��ύX����
    /// </summary>
    /// <param name="Lv">���݂̃��x��</param>
	/// <param name="getExp">�o���l</param>
	void ExpReset(int& Lv, int& getExp);

	/// <summary>
    /// �o���l�e�[�u����ύX����
    /// </summary>
    /// <param name="Lv">���݂̃��x��</param>
    /// <param name="expTable">�o���l�e�[�u��</param>
	void ExpTableChamge(int& Lv,int& expTable);

	/// <summary>
	/// �|�C���g�𑝂₷����
	/// </summary>
	/// <param name="point">���݂̃|�C���g</param>
	void PointProcess(int& point)
	{
		//�|�C���g��1���₷
		point++;
	}

	/// <summary>
	/// ���񂾂Ƃ��̏���
	/// </summary>
	virtual void Death() = 0;

	/// <summary>
	/// ���X�|�[��������W��ݒ肷��
	/// </summary>
	/// <param name="pos">���X�|�[��������W</param>
	/// <param name="number">���W���Z�b�g����ԍ�</param>
	void SetRespawnPos(Vector3 pos,Quaternion rot, int number) {
		m_respawnPos[number] = pos;
		m_respawnRotation[number] = rot;
	};

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
/// knightPlayer
/// </summary>
/// <returns>false???\???????</returns>
	const bool GetSpriteFlag() const
	{
		return m_spriteFlag;
	}

	/// <summary>
	/// �����ɍU���͂���������
	/// </summary>
	int GetAtk() {
		return m_Status.Atk;
	}

	/// <summary>
	/// �����̓G��|�����Ƃ��̌o���l�̏���
	/// </summary>
	/// <param name="GetExp">�����̓G�̌o���l</param>
	void ExpProcess(int Exp);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gameState"></param>
	void ChangeGameState(EnGameState gameState)
	{
		m_GameState = gameState;
	}

	/// <summary>
	/// ���݂̃|�C���g�Ԃ�
	/// </summary>
	int GetPoint() {
		return Point;
	}

private:
    Level3DRender m_respawnLevel;

	

protected:
	int Lv;                    //���x��
	int AtkSpeed;              //�U�����x
	float Cooltime;            //�X�L���̃N�[���^�C��
	float AvoidanceCoolTime;    //����̃N�[���^�C��
	bool SkillState = false;     //�X�L�����g�p�\���ǂ����̔���
	int Point;                 //�G��|���Ď�ɓ��ꂽ�|�C���g
	int GetExp;                //�����̓G��|�����Ƃ��̌o���l
	int ExpTable;              //�o���l�e�[�u��
	int respawnNumber;         //���X�|�[��������W�̔ԍ�
	bool isDeath = false;      //���񂾂��ǂ����̔���
	bool invincibleFlag = false;     //���G���ԃt���O
	int Ponit = 0;                  //�l�������|�C���g
	//Status m_Status;           //�X�e�[�^�X
	Vector3 m_respawnPos[4];    //���X�|�[��������W�̔z��
	Quaternion m_respawnRotation[4];
	Vector3 m_moveSpeed = Vector3::Zero;      //�ړ���

	Vector3 m_Forward;          //�v���C���[�̑O����

	Vector3 m_position = Vector3::Zero;                   //���W
	ModelRender m_modelRender;                            //���f�������_�[
	Quaternion m_rot = Quaternion::Identity;              //��]
	bool m_spriteFlag = true;
	Status m_Status;                                      //�X�e�[�^�X
	//���x���A�b�v���ɑ�������X�e�[�^�X
	LvUpStatus LvUPStatus = { 30,5,10.0f };
};

