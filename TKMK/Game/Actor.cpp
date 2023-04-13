#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

void Actor::Move(Vector3& position, CharacterController& charcon,Status& status,Vector3 stickL)
{
	//特定のアニメーションが再生中なら
	if (IsEnableMove() == false)
	{
		//抜け出す　移動処理を行わない
		return;
	}

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();
	//左スティックの入力量とstatusのスピードを乗算。
	right *= stickL.x * status.Speed;
	forward *= stickL.y * status.Speed;

	//プレイヤーの前方向の情報を更新
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(forward.x) >= 0.001f || fabsf(forward.z) >= 0.001f)
	{
		m_Forward = forward+ right;
	}

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right + forward;
	//重力を付与する
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//キャラクターコントローラーを使って座標を移動させる。
	position = charcon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//地面についた。
	if (charcon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}
	//キャラクターコントローラーを使って座標を移動させる。早くなってしまう
	//position = charcon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//m_modelRender.SetPosition(position);

}

//リスポーンする座標を設定する
void Actor::GetRespawnPos()
{
	m_respawnLevel.Init("Assets/level3D/CharRespawn2Level.tkl", [&](LevelObjectData& objData) {

		if (objData.ForwardMatchName(L"CharPos") == true) {
			//左上の座標
			if (objData.number == 0) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
				return true;
			}
			//右上の座標
			if (objData.number == 1) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
				return true;
			}
			//右下の座標
			if (objData.number == 2) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
				return true;
			}
			//左下の座標
			if (objData.number == 3) {
				SetRespawnPos(objData.position, objData.rotation, objData.number);
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
/// 中立の敵を倒したときの経験値の処理
/// </summary>
/// <param name="GetExp">中立の敵の経験値</param>
void Actor::ExpProcess(int Exp)
{
	//もしレベルが10(Max)なら
	if (Lv == 10)return;
	//自身の経験値に敵を倒したときに手に入れる経験値を足す
	GetExp += Exp;
	//手に入れた経験値より経験値テーブルのほうが大きかったら
	if (GetExp < ExpTable) return;      //抜け出す
	else {
		//経験値テーブルより手に入れた経験値のほうが大きかったら
		//レベルアップ
		LevelUp(LvUPStatus, m_Status, Lv);
		//レベルに合わせてレベルの画像を変更する
		switch (Lv)
		{
		case 2:
			ExpTable = 10;
			break;
		case 3:
			ExpTable = 20;
			break;
		case 4:
			ExpTable = 30;
			break;
		case 5:
			ExpTable = 40;
			break;
		case 6:
			ExpTable = 50;
			break;
		case 7:
			ExpTable = 60;
			break;
		case 8:
			ExpTable = 70;
			break;
		case 9:
			ExpTable = 80;
			break;
		default:
			break;
		}
	}
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

/// <summary>
/// リスポーンしたときに塔から飛び降りる処理
/// </summary>
void Actor::RespawnMove(Vector3& position, Quaternion& rotation, CharacterController& charCon)
{
	float jump = 10.0f;
	//飛び降りる
	//ジャンプする
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_RespawnJumpFlag = true;
	}
	
	if (m_RespawnJumpFlag == true)
	{
		if (Count < 20)
		{
			m_moveSpeed = Vector3::AxisZ;
			rotation.Apply(m_moveSpeed);
			//飛ぶ方向を決める
			m_moveSpeed *= 400.0f;

			position.y += jump;
			position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

			Count++;
		}
	}
	
	charCon.SetPosition(position);
	

	//m_TowerToGroundFlag = true;
}


