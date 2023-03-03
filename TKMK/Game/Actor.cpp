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
void Actor::levelDown(LvUpStatus& lus, Status& nowStatus, int& Level)
{
	//�������x����0�Ȃ�1�ɂ���
	if (Level == 0) {
		Level = 1; 
		return;
	}

	nowStatus.MaxHp-= lus.LvHp;
	nowStatus.Hp -= lus.LvHp;
	nowStatus.Atk -= lus.LvAtk;
	nowStatus.Speed -= lus.LvSpeed;
	Level--;
}

void Actor::COOlTIME(float SkillCooltimer, bool skillstate)
{
	//�X�L�����g�p���ꂽ��
	if (skillstate)
	{
		timer += g_gameTime->GetFrameDeltaTime();
		//timer���X�L���̃N�[���^�C�����傫���Ȃ�����B
		if (timer >= SkillCooltimer)
		{
			//�X�L���g�p�\
			skillstate = false;
			timer = SkillCooltimer;
		}
	}
	
}




