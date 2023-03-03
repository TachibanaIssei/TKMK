#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

//リスポーンする座標を設定する
void Actor::GetRespawnPos()
{
	m_respawnLevel.Init("Assets/level3D/respawnLevel.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"Knight") == true) {
			//左上の座標
			if (objData.number == 0) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
			//右上の座標
			if (objData.number == 1) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
			//右下の座標
			if (objData.number == 2) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
			//左下の座標
			if (objData.number == 3) {
				SetRespawnPos(objData.position, objData.number);
				return true;
			}
		}
		return true;
		});
}

/// <summary>
/// レベルアップの処理
/// </summary>
/// <param name="lus">レベルアップ時に増加するステータス</param>
/// <param name="nowStatus">現在のステータス</param>
/// <param name="Level">現在のレベル</param>
void Actor::LevelUp(LvUpStatus& lus,Status& nowStatus,int& Level)
{
	nowStatus.MaxHp+= lus.LvHp;
	nowStatus.Hp += lus.LvHp;
	nowStatus.Atk += lus.LvAtk;
	nowStatus.Speed += lus.LvSpeed;
	Level++;
}

/// <summary>
/// レベルダウンの処理
/// </summary>
/// /// <param name="lus">レベルアップ時に増加するステータス</param>
/// <param name="nowStatus">現在のステータス</param>
/// <param name="Level">現在のレベル</param>
void Actor::levelDown(LvUpStatus& lus, Status& nowStatus, int& Level)
{
	//もしレベルが0なら1にする
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




