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

//���x���A�b�v���ɑ�������X�e�[�^�X
//���݂̃X�e�[�^�X
void Actor::LevelUp(LvUpStatus& lus,Status& nowStatus)
{

	nowStatus.Hp += lus.LvHp;
	nowStatus.Atk += lus.LvAtk;
	nowStatus.Speed += lus.LvSpeed;
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




