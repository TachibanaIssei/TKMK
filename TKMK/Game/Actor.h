#pragma once

#include "Level3DRender.h"

class Actor:public IGameObject
{
public:
	Actor();
	virtual ~Actor();

protected:

	/// <summary>
	/// �����X�e�[�^�X
	/// </summary>
	struct Status {
		int MaxHp;                 //�ő�q�b�g�|�C���g
		int Hp;                    //�q�b�g�|�C���g(�̗�)
		int Atk;                   //�U����
		float Speed;               //�ړ����x
	};

	/// <summary>
	/// ���x���A�b�v���ɑ�������X�e�[�^�X
	/// </summary>
	struct LvUpStatus {
		int LvHp;                  //���x���A�b�v���̃q�b�g�|�C���g
		int LvAtk;                 //���x���A�b�v���̍U����
		float LvSpeed;             //���x���A�b�v���̈ړ����x
	};

public:
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
	/// �X�L����ł���
	/// </summary>
	virtual void Skill() = 0;

	/// <summary>
	/// �K�E�Z��ł���
	/// </summary>
	virtual void UltimateSkill() = 0;

	/// <summary>
	/// �X�L�����g�p���ꂽ��̃N�[���^�C���̏���
	/// </summary>
	/// <param name="SkillCooltimer">�X�L���̃N�[���^�C��</param>
	/// <param name="skillstate">�X�L�����g�p�������̔���</param>
	void COOlTIME(float SkillCooltimer,bool skillstate);
	
	/// <summary>
	/// ���݂̃��x�����擾����֐�
	/// </summary>
	/// <returns></returns>
	inline int GetLevel() { return Lv; }

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
	/// /// <param name="lus">���x���A�b�v���ɑ�������X�e�[�^�X</param>
	/// <param name="nowStatus">���݂̃X�e�[�^�X</param>
	/// <param name="Level">���݂̃��x��</param>
	void levelDown(LvUpStatus& lus, Status& nowStatus, int& Level);

	/// <summary>
	/// ���񂾂Ƃ��̏���
	/// </summary>
	virtual void Death() = 0;

	/// <summary>
		/// ���X�|�[��������W��ݒ肷��
		/// </summary>
		/// <param name="pos">���X�|�[��������W</param>
		/// <param name="number">���W���Z�b�g����ԍ�</param>
	void SetRespawnPos(Vector3 pos, int number) {
		m_respawnPos[number] = pos;
	};

private:
    Level3DRender m_respawnLevel;

	float timer = 0.0f;

protected:
	int Lv;                    //���x��
	int AtkSpeed;              //�U�����x
	float Cooltime;            //�X�L���̃N�[���^�C��
	bool SkillState = false;     //�X�L�����g�p�\���ǂ����̔���
	int Point;                 //�G��|���Ď�ɓ��ꂽ�|�C���g
	int GetExp;                //�����̓G��|�����Ƃ��̌o���l
	int ExpTable;              //�o���l�e�[�u��
	int respawnNumber;         //���X�|�[��������W�̔ԍ�
	bool isDeath = false;      //���񂾂��ǂ����̔���
	Vector3 m_respawnPos[4];    //���X�|�[��������W�̔z��


};

