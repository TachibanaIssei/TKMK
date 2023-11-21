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
}
void Actor::OnDestroy() 
{
}
void Actor::Move(Vector3& position, CharacterController& charcon,Status& status,Vector3 stickL, int number)
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
	Vector3 forward = g_camera3D[number]->GetForward();
	Vector3 right = g_camera3D[number]->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();
	right.Normalize();
	//移動の入力量とstatusのスピードを乗算。
	right *= stickL.x * status.GetSpeed();
	forward *= stickL.y * status.GetSpeed();

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
void Actor::LevelUp(int& Level)
{
	if (Level >= 10) {
	return;
	}

	int maxHp = m_status.GetMaxHp() + LvUPStatus.LvHp;
	int hp = m_status.GetHp() + LvUPStatus.LvHp;
	int attackPower = m_status.GetAttackPower() + LvUPStatus.LvAtk;
	float speed = m_status.GetSpeed() + LvUPStatus.LvSpeed;

	m_status.SetMaxHp(maxHp);
	m_status.SetHp(hp);
	m_status.SetAttackPower(attackPower);
	m_status.SetSpeed(speed);
	Level++;
}

/// <summary>
/// レベルダウンの処理
/// </summary>
/// /// <param name="lus">レベルアップ時に増加するステータス</param>
/// <param name="nowStatus">現在のステータス</param>
/// <param name="Level">現在のレベル</param>
/// <param name="downLevel">下げるレベルの数</param>
void Actor::levelDown(int& Level, int downLevel)
{
	//もしレベルが0なら1にする
	if (Level == 1) {
		Level = 1;
		return;
	}

	for (int count = 0; count < downLevel; count++) {

		if (Level == 1) {
			return;
		}

		int maxHp = m_status.GetMaxHp() - LvUPStatus.LvHp;
		m_status.SetMaxHp(maxHp);
		//もしHPがMaxHpを上回るなら
		if (m_status.GetHp() > m_status.GetMaxHp())
		{
			//HPとMaxHpを同じにする
			m_status.SetHp(m_status.GetMaxHp());
		}
		int attackPower = m_status.GetAttackPower() - LvUPStatus.LvAtk;
		int speed = m_status.GetSpeed() - LvUPStatus.LvSpeed;
		m_status.SetAttackPower(attackPower);
		m_status.SetSpeed(speed);
		Level -= 1;
	}


	//Level-= downLevel;
	////もしレベルが0なら1にする
	//if (Level == 0) {
	//	Level = 1; 
	//	return;
	//}
	//

	//m_status.m_maxHp-= downLevel* LvUPStatus.LvHp;
	////もしHPがMaxHpを上回るなら
	//if (m_status.m_hp > m_status.m_maxHp)
	//{
	//	//HPとMaxHpを同じにする
	//	m_status.m_hp = m_status.m_maxHp;
	//}

	//m_status.m_attackPower -= downLevel* LvUPStatus.LvAtk;
	//m_status.m_speed -= downLevel* LvUPStatus.LvSpeed;
}
/// <summary>
/// 中立の敵を倒したときの経験値の処理
/// </summary>
/// <param name="m_getExp">中立の敵の経験値</param>
void Actor::ExpProcess(int Exp)
{
	//ウサギを倒したときの処理がおかしくなる
	//もしレベルが10(Max)なら
	if (m_lv == 10)return;
	//自身の経験値に敵を倒したときに手に入れる経験値を足す
	m_getExp += Exp;
	//経験値を保存する
	m_saveEXP += Exp;
	while (true)
	{
		//もしレベルが10(Max)なら
		if (m_lv == 10)return;

		//手に入れた経験値より経験値テーブルのほうが大きかったら
		if (m_getExp < m_expTable) {
			break;      //抜け出す
		}
		//経験値テーブルより手に入れた経験値のほうが大きかったら
		else {

			while (true)
			{
				//もしレベルが10(Max)なら
				if (m_lv == 10)return;

				//手に入れた経験値より経験値テーブルのほうが大きかったら
				if (m_getExp >= m_expTable) {
					//レベルアップ
					LevelUp(m_lv);
				}

				//今の経験値テーブルを代入
				m_oldExpTable = m_expTable;
				//経験値テーブルを更新する
				//経験値を0にリセットする
				//m_getExp = 0;

				m_getExp -= m_expTable;

				//テーブル変更
				if (m_lv >= 3) {
				m_expTable = 10;
					
				}
				else
				{
					m_expTable = 5;
					//m_getExp -= m_expTable;
				}

				

				//経験値が0になったら抜け出す
				if (m_getExp == 0) {
					m_getExp = 0;
					return;
				}
				else if (m_getExp < m_expTable)
				{
					//m_getExp *= -1;
					return;
				}


				//

			}
			
		}

	}
}

//レベルを下げる
void Actor::LevelDownProcess(int downlevel)
{
	//レベルを１下げる
	levelDown(m_lv, downlevel);
	//経験値をリセット
	ExpReset(m_lv, m_getExp);
	//一つ下のレベルの経験値テーブルにする
	ExpTableChamge(m_lv, m_expTable);
}

/// <summary>
/// リスポーンしたときのレベルによって経験値を変更する
/// </summary>
/// <param name="m_lv">現在のレベル</param>
/// <param name="getExp">経験値</param>
void Actor::ExpReset(int& m_lv, int& getExp)
{
	getExp = 0;
}

/// <summary>
/// 経験値テーブルを変更する
/// </summary>
/// <param name="m_lv">現在のレベル</param>
/// <param name="expTable">経験値テーブル</param>
void Actor::ExpTableChamge(int& m_lv, int& expTable)
{
	if (m_lv >= 3) {
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
void Actor::CoolTime(float m_skillCoolTime, bool& skillEndFlag,float& timer)
{

	//スキルのアニメーション再生が終わったら
	if (skillEndFlag ==true)
	{
		if (timer <= 0)
		{
			//スキル使用可能
			skillEndFlag = false;
			timer = m_skillCoolTime;
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
	Vector3 diff = player->GetCharcterPosition() - m_position;

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
				if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_DuoPlay)
				{
					if (m_enPlayerNumber == enPlayerNumber_1P)
					{
						fade->StartFadeOut(1.0f,Fade::enFadeSpriteType_Left);
					}
					else if (m_enPlayerNumber == enPlayerNumber_2P)
					{
						fade->StartFadeOut(1.0f, Fade::enFadeSpriteType_Right);
					}
				}
				else if (g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_TrioPlay ||
					g_renderingEngine->GetGameMode() == RenderingEngine::enGameMode_QuartetPlay)
				{
					if (m_enPlayerNumber == enPlayerNumber_1P)
					{
						fade->StartFadeOut(1.0f, Fade::enFadeSpriteType_LeftUp);
					}
					else if (m_enPlayerNumber == enPlayerNumber_2P)
					{
						fade->StartFadeOut(1.0f, Fade::enFadeSpriteType_RightUp);
					}
					else if (m_enPlayerNumber == enPlayerNumber_3P)
					{
						fade->StartFadeOut(1.0f, Fade::enFadeSpriteType_LeftDown);
					}
					else if (m_enPlayerNumber == enPlayerNumber_4P)
					{
						fade->StartFadeOut(1.0f, Fade::enFadeSpriteType_RightDown);
					}
				}
				else {
					//フェードアウト
					//画面を明るくする
					fade->StartFadeOut(1.0f);
				}
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
