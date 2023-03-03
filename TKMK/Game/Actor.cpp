#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}
//レベルアップ時に増加するステータス
//現在のステータス
void Actor::LevelUp(LvUpStatus& lus,Status& nowStatus)
{

	nowStatus.Hp += lus.LvHp;
	nowStatus.Atk += lus.LvAtk;
	nowStatus.Speed += lus.LvSpeed;
}

void Actor::COOlTIME(float SkillCooltimer, bool skillstate)
{
	//スキルが使用されたら
	if (skillstate)
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
		//timerがスキルのクールタイムより大きくなったら。
		if (m_timer >= SkillCooltimer)
		{
			//スキル使用可能
			skillstate = false;
			m_timer = SkillCooltimer;
		}
	}
	
}



