#include "stdafx.h"
#include "Actor.h"
#include "Player.h"
#include "Fade.h"

Actor::Actor()
{

}

Actor::~Actor()
{
	// エフェクト削除
	if (GetPower != nullptr) {
		GetPower->DeleteEffect();
		DeleteGO(GetPower);
	}

	if (PowerUpEfk != nullptr) {
		PowerUpEfk->DeleteEffect();
		DeleteGO(PowerUpEfk);
	}
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
	//移動の入力量とstatusのスピードを乗算。
	right *= stickL.x * status.Speed;
	forward *= stickL.y * status.Speed;

	//プレイヤーの前方向の情報を更新
	//xかzの移動速度があったら
	if (fabsf(forward.x) >= 0.001f || fabsf(forward.z) >= 0.001f)
	{
		//前方向を計算する
		m_Forward = forward + right;
	}

	//移動速度に前方向と右方向の入力量を加算する。
	m_moveSpeed += right + forward;
	//重力を付与する
	m_moveSpeed.y -= 600.0f * g_gameTime->GetFrameDeltaTime();

	//地面についた。
	if (charcon.IsOnGround()) {

		m_moveSpeed.y = 0.0f;

		//ジャンプフラグがtrueだったら
		if (m_RespawnJumpFlag == true)
		{
			//座標を上げる
			RespawnMove();
			//フラグをfalseにする
			m_RespawnJumpFlag = false;
		}
	}

	//↓生成するクラスでキャラコンを更新する
	//キャラクターコントローラーを使って座標を移動させる。
	//position = charcon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

//リスポーンする座標を設定する
void Actor::GetRespawnPos()
{
	m_respawnLevel.Init("Assets/level3D/AIPOS2.tkl", [&](LevelObjectData& objData) {

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
/// 攻撃UP
/// </summary>
void Actor::AttackUP()
{
	//攻撃UP関連
	if (GetPower != nullptr)
	{
		if (GetPower->GetEffect()->IsPlay() == false)
		{
			PowerUpEfk = NewGO<ChaseEFK>(3);
			PowerUpEfk->SetEffect(EnEFK::enEffect_Knight_PowerUP, this, Vector3::One * 10.0f);

			GetPower->DeleteEffect();
			DeleteGO(GetPower);

			GetPower = nullptr;
		}
	}

	if (PowerUpTimer > 0.0f)
	{
		PowerUpTimer -= g_gameTime->GetFrameDeltaTime();
		if (PowerUpTimer <= 0.0f)
		{
			//攻撃UP終了
			AttackUPEnd();
		}
	}
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
	{
		//HPとMaxHpを同じにする
		nowStatus.Hp = nowStatus.MaxHp;
	}

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

	while (true)
	{
		//もしレベルが10(Max)なら
		if (Lv == 10)return;

		//手に入れた経験値より経験値テーブルのほうが大きかったら
		if (GetExp < ExpTable) {
			break;      //抜け出す
		}
		//経験値テーブルより手に入れた経験値のほうが大きかったら
		else {
			//レベルアップ
			LevelUp(LvUPStatus, m_Status, Lv);
			//今の経験値テーブルを代入
			m_oldExpTable = ExpTable;
			//経験値テーブルを更新する
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
	case 5:
		getExp = 30;
		break;
	case 6:
		getExp = 40;
		break;
	case 7:
		getExp = 50;
		break;
	case 8:
		getExp = 60;
		break;
	case 9:
		getExp = 70;
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
/// クールタイムの処理
/// </summary>
/// <param name="SkillCooltimer">クールタイム</param>
/// <param name="skillstate">スキルや回避が終わったかの判定</param>
/// <param name="timer">クールタイムを計算する変数</param>
void Actor::COOlTIME(float Cooltime, bool& skillEndFlag,float& timer)
{

	//スキルのアニメーション再生が終わったら
	if (skillEndFlag ==true)
	{
		if (timer <= 0)
		{
			//スキル使用可能
			skillEndFlag = false;
			timer = Cooltime;
		}
		else timer -= g_gameTime->GetFrameDeltaTime();   //timerを進める

	}
	
}

/// <summary>
/// リスポーンしたときに塔から飛び降りる処理
/// </summary>
void Actor::RespawnMove()
{
	//飛び降りる
	//ジャンプする
		m_moveSpeed.y = 350.0f;
		//position.y += jump;
		//m_RespawnJumpFlag = true;
}

/// <summary>
/// プレイヤーとの距離によって音量調整する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="Max">音量の最大値</param>
/// <param name="Min">音量の最低値</param>
/// <returns>音量</returns>
float Actor::SoundSet(Player* player, float Max, float Min)
{
	Vector3 diff = player->GetCharPosition() - m_position;

	float Len = diff.Length();

	const float min = 0.1f;
	const float max = 1500.0f;

	float nomalizeValue = (abs(Len) - min) / (max - min);


	float Vol = Math::Lerp(nomalizeValue, Max, Min);

	if (Vol < 0)
	{
		Vol *= -1.0f;
	}

	return Vol;
}

/// <summary>
/// キャラがやられてからリスポーンするまでの時間を計る
/// </summary>
/// /// <param name="DeathToRespwanFlag"></param>
bool Actor::DeathToRespawnTimer(bool DeathToRespwanFlag,Fade* fade)
{
	//キャラがやられたら
	if (m_DeathToRespwanFlag == true)
	{
		//タイマー減少
		m_respwanTimer -= g_gameTime->GetFrameDeltaTime();
		//2秒以上経ったら
		if (m_respwanTimer <= 0.0f)
		{
			//画面を明るくする
			fade->StartFadeOut(1.0f);

			m_DeathToRespwanFlag = false;
		}
		//やられている
		return true;
	}
	//やられていない
	return false;
}
