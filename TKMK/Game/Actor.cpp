#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

void Actor::LevelUp(LvUpStatus lus)
{
	Hp += lus.LvHp;
	Atk += lus.LvAtk;
	Speed += lus.LvSpeed;
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




