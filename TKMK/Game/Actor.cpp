#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

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




