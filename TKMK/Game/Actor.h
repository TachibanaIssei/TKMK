#pragma once
class Actor
{
public:
	Actor();

	virtual ~Actor();

protected:

	//���x���A�b�v�����Ƃ��ɏオ��X�e�[�^�X�̒l
	struct LvUpStatus {
		int LvHp;                  //���x���A�b�v���̃q�b�g�|�C���g
		int LvAtk;                 //���x���A�b�v���̍U����
		int LvSpeed;               //���x���A�b�v���̈ړ����x
	};

	int Lv;                    //���x��
	int Hp;                    //�q�b�g�|�C���g(�̗�)
	int Atk;                   //�U����
	int Speed;                 //�ړ����x
	int AtkSpeed;              //�U�����x
	int Cooltime;              //�Z��ł�����̃N�[���^�C��
	int Point;                 //�G��|���Ď�ɓ��ꂽ�|�C���g
	int ShieldPoint;           //�V�[���h
	int Exp;                   //�o���l
	int ExpTable;              //�o���l�e�[�u��
	bool isDeath = false;      //���񂾂��ǂ����̔���

public:
	//�U���̏���
	//�������z�֐��Ȃ̂Ŕh���N���X�Ŏ���
	virtual void Attack() = 0;
	//�U�����󂯂��Ƃ��̏���
	virtual void Dameged(int damege) = 0;
	//�X�L����ł���
	virtual void Skill() = 0;
	//�N�[���^�C�������炷����
	virtual void COOlTIME() = 0;
	//���݂̃��x�����擾����֐�
	inline int GetLevel() { return Lv; }
	//���x���A�b�v�̏���
	void LevelUp(LvUpStatus lus);
	//���񂾂Ƃ��̏���
	virtual void Death() = 0;
};

