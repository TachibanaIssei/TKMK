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
	//スキルが使用されたら
	if (skillstate)
	{
		timer += g_gameTime->GetFrameDeltaTime();
		//timerがスキルのクールタイムより大きくなったら。
		if (timer >= SkillCooltimer)
		{
			//スキル使用可能
			skillstate = false;
			timer = SkillCooltimer;
		}
	}
	
}




