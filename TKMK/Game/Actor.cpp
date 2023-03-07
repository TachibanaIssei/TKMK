#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

//���X�|�[��������W��ݒ肷��
void Actor::GetRespawnPos()
{
	m_respawnLevel.Init("Assets/level3D/respawnLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Knight") == true) {
			//����̍��W
			if (objData.number == 0) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
			//�E��̍��W
			if (objData.number == 1) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
			//�E���̍��W
			if (objData.number == 2) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
			//�����̍��W
			if (objData.number == 3) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
		}
		return true;
		});
}

/// <summary>
/// ���x���A�b�v�̏���
/// </summary>
/// <param name="lus">���x���A�b�v���ɑ�������X�e�[�^�X</param>
/// <param name="nowStatus">���݂̃X�e�[�^�X</param>
/// <param name="Level">���݂̃��x��</param>
void Actor::LevelUp(LvUpStatus& lus,Status& nowStatus,int& Level)
{
	nowStatus.MaxHp+= lus.LvHp;
	nowStatus.Hp += lus.LvHp;
	nowStatus.Atk += lus.LvAtk;
	nowStatus.Speed += lus.LvSpeed;
	Level++;
}

/// <summary>
/// ���x���_�E���̏���
/// </summary>
/// /// <param name="lus">���x���A�b�v���ɑ�������X�e�[�^�X</param>
/// <param name="nowStatus">���݂̃X�e�[�^�X</param>
/// <param name="Level">���݂̃��x��</param>
/// <param name="downLevel">�����郌�x���̐�</param>
void Actor::levelDown(LvUpStatus& lus, Status& nowStatus, int& Level, int downLevel)
{
	Level-= downLevel;
	//�������x����0�Ȃ�1�ɂ���
	if (Level == 0) {
		Level = 1; 
		return;
	}

	nowStatus.MaxHp-= downLevel*lus.LvHp;
	nowStatus.Hp -= downLevel*lus.LvHp;
	nowStatus.Atk -= downLevel*lus.LvAtk;
	nowStatus.Speed -= downLevel*lus.LvSpeed;
}

/// <summary>
/// ���X�|�[�������Ƃ��̃��x���ɂ���Čo���l��ύX����
/// </summary>
/// <param name="Lv">���݂̃��x��</param>
/// <param name="getExp">�o���l</param>
void Actor::ExpReset(int& Lv, int& getExp)
{
	//�o���l�����Z�b�g
	switch (Lv)
	{
	case 1:
		getExp = 0;
		break;
	case 2:
		getExp = 5;
		break;
	case 3:
		getExp = 10;
		break;
	case 4:
		getExp = 20;
		break;
	default:
		break;
	}
}

/// <summary>
/// �o���l�e�[�u����ύX����
/// </summary>
/// <param name="Lv">���݂̃��x��</param>
/// <param name="expTable">�o���l�e�[�u��</param>
void Actor::ExpTableChamge(int& Lv, int& expTable)
{
	switch (Lv)
	{
	case 1:
		expTable = 5;
		break;
	case 2:
		expTable = 10;
		break;
	case 3:
		expTable = 20;
		break;
	case 4:
		expTable = 30;
		break;

	default:
		break;
	}
}

/// <summary>
/// �X�L�����g�p������̃N�[���^�C���̏���
/// </summary>
/// <param name="SkillCooltimer">�X�L���̃N�[���^�C��</param>
/// <param name="skillstate">�X�L�����g�p�������̔���</param>
void Actor::COOlTIME(float SkillCooltimer, bool& skillstate)
{
	//�X�L�����g�p���ꂽ��
	if (skillstate==true)
	{
		//timer���X�L���̃N�[���^�C�����傫���Ȃ�����B
		if (timer >= SkillCooltimer)
		{
			//�X�L���g�p�\
			skillstate = false;
			timer = 0;
		}
		else timer += g_gameTime->GetFrameDeltaTime();
	}
	
}




