#include "stdafx.h"
#include "Actor.h"
#include "Player.h"
#include "Fade.h"
//todoレベルが上がった時経験値0スタートにする
Actor::Actor()
{

}

Actor::~Actor()
{
	K2_LOG("Actor::~Actor\n");
	/*if (PowerUpEfk != nullptr) {
		PowerUpEfk->DeleteEffect();
		DeleteGO(PowerUpEfk);
	}*/
}
void Actor::OnDestroy() 
{
	K2_LOG("Actor::OnDestroy\n");
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
		m_forwardNow = forward + right;
	}

	//移動速度に前方向と右方向の入力量を加算する。
	m_moveSpeed += right + forward;
	//重力を付与する
	m_moveSpeed.y -= 900.0f * g_gameTime->GetFrameDeltaTime();

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
}

//リスポーンする座標を設定する
void Actor::GetRespawnPos()
{
	m_respawnLevel.Init("Assets/level3D/AIPOS3.tkl", [&](LevelObjectData& objData) {

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

///// <summary>
///// 攻撃UP
///// </summary>
//void Actor::AttackUP()
//{
//	if (PowerUpTimer > 0.0f)
//	{
//		m_atkUpSpriteFlag = true;
//		PowerUpTimer -= g_gameTime->GetFrameDeltaTime();
//
//		if (PowerUpTimer <= 0.0f)
//		{
//			//攻撃UP終了
//			AttackUPEnd();
//			//攻撃力アップアイコンを消す
//			m_atkUpSpriteFlag = false;
//
//		}
//	}
//}

/// <summary>
/// レベルアップの処理
/// </summary>
/// <param name="lus">レベルアップ時に増加するステータス</param>
/// <param name="nowStatus">現在のステータス</param>
/// <param name="Level">現在のレベル</param>
void Actor::LevelUp(LvUpStatus& lus, Status& nowStatus, int& Level)
{
	if (Level >= 10) {
	return;
	}

	nowStatus.MaxHp += lus.LvHp;
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
	//ウサギを倒したときの処理がおかしくなる
	//もしレベルが10(Max)なら
	if (Lv == 10)return;
	//自身の経験値に敵を倒したときに手に入れる経験値を足す
	GetExp += Exp;
	//経験値を保存する
	m_SaveEXP += Exp;
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

			while (true)
			{
				//もしレベルが10(Max)なら
				if (Lv == 10)return;

				//手に入れた経験値より経験値テーブルのほうが大きかったら
				if (GetExp >= ExpTable) {
					//レベルアップ
					LevelUp(LvUPStatus, m_Status, Lv);
				}

				//今の経験値テーブルを代入
				m_oldExpTable = ExpTable;
				//経験値テーブルを更新する
				//経験値を0にリセットする
				//GetExp = 0;

				GetExp -= ExpTable;

				//テーブル変更
				if (Lv >= 3) {
				ExpTable = 10;
					
				}
				else
				{
					ExpTable = 5;
					//GetExp -= ExpTable;
				}

				

				//経験値が0になったら抜け出す
				if (GetExp == 0) {
					GetExp = 0;
					return;
				}
				else if (GetExp < ExpTable)
				{
					//GetExp *= -1;
					return;
				}


				//

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
	getExp = 0;
}

/// <summary>
/// 経験値テーブルを変更する
/// </summary>
/// <param name="Lv">現在のレベル</param>
/// <param name="expTable">経験値テーブル</param>
void Actor::ExpTableChamge(int& Lv, int& expTable)
{
	if (Lv >= 3) {
		expTable = 10;
	}
	else
	{
		expTable = 5;
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
	//ジャンプする
	m_moveSpeed.y = 280.0f;
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
	const float max = 1000.0f;

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
bool Actor::DeathToRespawnTimer(bool& DeathToRespwanFlag,Fade* fade,bool fadeFlag)
{
	//キャラがやられたら
	if (DeathToRespwanFlag == true)
	{
		//タイマー減少
		m_respwanTimer -= g_gameTime->GetFrameDeltaTime();
		//2秒以上経ったら
		if (m_respwanTimer <= 0.0f)
		{
			if (fadeFlag == true) {
				//フェードアウト
			//画面を明るくする
				fade->StartFadeOut(1.0f);
			}
			//
			DeathToRespwanFlag = false;
			//やられていない
			return false;
		}
		//やられている
		return true;
	}
	//やられていない
	return false;
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct IsGroundResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Ground) {
			//衝突したのは壁ではない。
			return 0.0f;
		}

		//壁とぶつかったら。
		//フラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};


//アクターが地面に接地しているか確かめる
bool Actor::IsActorGroundChack()
{
	m_boxCollider.Create(Vector3(1.0f, 1.0f, 1.0f));

	Vector3 actorpos = m_position;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(actorpos.x, actorpos.y + 10.0f, actorpos.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(actorpos.x, actorpos.y - 2.0f, actorpos.z));

		//壁の判定を返す
		IsGroundResult callback_Ground;
		//コライダーを始点から終点まで動かして。
		//壁と衝突するかどうかを調べる。
		PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_boxCollider.GetBody(), start, end, callback_Ground);
		//壁と衝突した！
		if (callback_Ground.isHit == true)
		{
			//地面にいても死んでいたら
			if (NowCharState() == enCharState_Death)
			{
				return false;
			}
			else
			{
				return true;
			}

		}
		else
		{
			return false;
		}


	
}
