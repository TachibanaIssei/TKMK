#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

void Actor::Move(Vector3& position, CharacterController& charcon,Status& status)
{
	//特定のアニメーションが再生中なら
	if (IsEnableMove() == false)
	{
		//抜け出す　移動処理を行わない
		return;
	}

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量とstatusのスピードを乗算。
	right *= stickL.x * status.Speed;
	forward *= stickL.y * status.Speed;

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right + forward;
	//キャラクターコントローラーを使って座標を移動させる。
	position = charcon.Execute(m_moveSpeed, 1.0f / 60.0f);
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
/// <param name="downLevel">下げるレベルの数</param>
void Actor::levelDown(LvUpStatus& lus, Status& nowStatus, int& Level, int downLevel)
{
	Level-= downLevel;
	//もしレベルが0なら1にする
	if (Level == 0) {
		Level = 1; 
		return;
	}

	nowStatus.MaxHp-= downLevel*lus.LvHp;
	//もしHPがMaxHpを上回るなら
	if (nowStatus.Hp > nowStatus.MaxHp)
		//HPとMaxHpを同じにする
		nowStatus.Hp = nowStatus.MaxHp;

	nowStatus.Atk -= downLevel*lus.LvAtk;
	nowStatus.Speed -= downLevel*lus.LvSpeed;
}

/// <summary>
/// リスポーンしたときのレベルによって経験値を変更する
/// </summary>
/// <param name="Lv">現在のレベル</param>
/// <param name="getExp">経験値</param>
void Actor::ExpReset(int& Lv, int& getExp)
{
	//経験値をリセット
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
/// 経験値テーブルを変更する
/// </summary>
/// <param name="Lv">現在のレベル</param>
/// <param name="expTable">経験値テーブル</param>
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
/// スキルを使用した後のクールタイムの処理
/// </summary>
/// <param name="SkillCooltimer">スキルのクールタイム</param>
/// <param name="skillstate">スキルを使用したかの判定</param>
/// <param name="timer">クールタイムを計算する変数</param>
void Actor::COOlTIME(float SkillCooltimer, bool& skillstate,float& timer)
{

	//スキルのアニメーション再生が終わったら
	if (skillstate==true)
	{
		if (timer <= 0)
		{
			//スキル使用可能
			skillstate = false;
			timer = SkillCooltimer;

		}
		else timer -= g_gameTime->GetFrameDeltaTime();   //timerを進める

		//timerがスキルのクールタイムより大きくなったら。
		//if (timer >= SkillCooltimer)
		//{
		//	//スキル使用可能
		//	skillstate = false;
		//	timer = 0;
		//}
		//else timer += g_gameTime->GetFrameDeltaTime();   //timerを進める
	}
	
}




