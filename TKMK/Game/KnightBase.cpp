#include "stdafx.h"
#include "KnightBase.h"
#include "Status.h"

KnightBase::KnightBase()
{
	//ステータスを読み込む
	m_Status.Init("Knight");
	Lv=1;                    //レベル
	AtkSpeed=20;              //攻撃速度

	Cooltime=5;            //スキルのクールタイム
	SkillTimer = Cooltime;

	AvoidanceCoolTime = 2;     ///回避のクールタイム
	AvoidanceTimer = AvoidanceCoolTime;

	Point=0;                 //敵を倒して手に入れたポイント
	GetExp=0;                //中立の敵を倒したときの経験値
	ExpTable=5;              //経験値テーブル
	respawnNumber = 0;        //リスポーンする座標の番号

	
}

KnightBase::~KnightBase()
{

}

void KnightBase::SetModel()
{
	//プレイヤー
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_ChainAtk].Load("Assets/animData/Knight/Knight_ChainAttack.tka");
	m_animationClips[enAnimationClip_ChainAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Skill].Load("Assets/animData/Knight/Knight_Skill.tka");
	m_animationClips[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClips[enAnimationClip_UltimateSkill].Load("Assets/animData/Knight/Knight_UltimateAttack.tka");
	m_animationClips[enAnimationClip_UltimateSkill].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Knight/Knight_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Knight/Knight_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Avoidance].Load("Assets/animData/Knight/Knight_Avoidance.tka");
	m_animationClips[enAnimationClip_Avoidance].SetLoopFlag(false);

	//剣士モデルを読み込み
	m_modelRender.Init("Assets/modelData/character/Knight/Knight_02.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	//「mixamorig:RightHand」(右手)ボーンのID(番号)を取得する。
	m_swordBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");
	//攻撃時のアニメーションイベント剣士の座標のID(番号)を取得する。
	AtkEndPosId = m_modelRender.FindBoneID(L"movePos");

	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	//m_modelRender.SetScale(Vector3(0.1f, 0.1f, 0.1f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	m_charCon.Init(
		15.0f,
		35.0f,
		m_position
	);
}

/// <summary>
/// 中立の敵を倒したときの経験値の処理
/// </summary>
/// <param name="GetExp">中立の敵の経験値</param>
void KnightBase::ExpProcess(int Exp)
{
	//自身の経験値に敵を倒したときに手に入れる経験値を足す
	GetExp += Exp;
	//手に入れた経験値より経験値テーブルのほうが大きかったら
	if (GetExp < ExpTable) return;      //抜け出す
	else {
		//経験値テーブルより手に入れた経験値のほうが大きかったら
		//レベルアップ
		LevelUp(LvUpStatus,m_Status,Lv);

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

		default:
			break;
		}
	}
}

/// <summary>
/// 移動処理
/// </summary>

/// <summary>
/// 回転処理
/// </summary>
void KnightBase::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rot.SetRotationYFromDirectionXZ(m_moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rot);
	}
}

/// <summary>
/// 攻撃時の当たり判定の処理
/// </summary>
void KnightBase::AtkCollisiton()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	//collisionPosition += forward * 50.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition, //座標。
		Quaternion::Identity, //回転。
		Vector3(70.0f, 15.0f, 15.0f) //大きさ。
	);
	collisionObject->SetName("player_attack");

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//matrix.MakeRotationZ(90.0f);
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

/// <summary>
/// 必殺技発動時の当たり判定の処理
/// </summary>
/// <param name="oldpostion">前フレームの座標</param>
/// <param name="position">現在の座標</param>
void KnightBase::UltimateSkillCollistion(Vector3& oldpostion,Vector3& position)
{	
	//コリジョン生成していないなら
	if (UltCollisionSetFlag == false)
	{
		//コリジョンの座標をプレイヤーと同じに設定
		UltCollisionPos = position;
		UltCollisionPos.y += 50.0f;

		//前フレームの座標を代入
		Vector3 oldPosition = oldpostion;
		//前フレームの座標から現在のフレームに向かうベクトルを計算する
		collisionRot = position - oldPosition;

		//Y方向のベクトルを0.0fにする
		collisionRot.y = 0.0f;

		//正規化
		collisionRot.Normalize();

		Quaternion rot;
		//Y軸回りの回転クォータニオンを作成
		rot.SetRotationYFromDirectionXZ(collisionRot);
		//ベクトルにクォータニオンを適応
		rot.Apply(oldpostion);

		//コリジョンオブジェクトを作成する。
		collisionObject = NewGO<CollisionObject>(0);
		Vector3 collitionPosition = position;
		collitionPosition.y += 50.0f;
		//collisionPosition.y += 50.0f;
		//ボックス状のコリジョンを作成する。
		collisionObject->CreateBox(collitionPosition, //座標。
			Quaternion(rot), //回転。
			Vector3(300.0f, 50.0f, 15.0f) //大きさ。
		);
		collisionObject->SetIsEnableAutoDelete(false);
		collisionObject->SetName("player_UltimateSkill");

		UltCollisionSetFlag = true;
	}
	else
	{
		//移動速度設定
		UltCollisionPos += collisionRot * 4.0f;
		//座標を設定
		collisionObject->SetPosition(UltCollisionPos);
	}
}

void KnightBase::Collition()
{
	//被ダメージ、ダウン中、必殺技、通常攻撃時はダメージ判定をしない。
	if (m_animState == enKnightState_Damege || 
		m_animState == enKnightState_Death ||
		m_animState == enKnightState_UltimateSkill ||
		m_animState == enKnightState_ChainAtk ||
		m_animState == enKnightState_Skill ||
		m_animState == enKnightState_Avoidance)
	{
		return;
	}
	else
	{
		//敵の攻撃用のコリジョンを取得する名前一緒にする
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンが自身のキャラコンに当たったら
			if (collision->IsHit(m_charCon))
			{
				//エネミーの攻撃力を取ってくる

				//hpを10減らす
				Dameged(Enemy_atk);
			}
		}
	}
	
}

/// <summary>
/// ダメージを受けたときの処理
/// </summary>
/// <param name="damege">敵のダメージ</param>
void KnightBase::Dameged(int damege)
{
	m_Status.Hp -= damege;
	//自身のHPが0以下なら
	if (m_Status.Hp <= 0) {
		//倒されたときの処理に遷移
		//死亡ステート
		m_animState = enKnightState_Death;
		m_Status.Hp = 0;
		//Death();
		//SetRespawn();

	}
	else {
		//ダメージステート
		m_animState = enKnightState_Damege;
		//無敵時間フラグ
		//invincibleFlag = true;
	}
}

/// <summary>
/// スキルを使用したときの処理
/// </summary>
//void KnightBase::Skill(Vector3& right, Vector3& forward)
//{
//	//スキルステート
//	m_animState = enKnightState_Skill;
//
//	//移動処理
//	//移動速度にスティックの入力量を加算する。
//	//Vector3 m_SkillSpeed; 
//	m_moveSpeed = right + forward;
//	//キャラクターコントローラーを使って座標を移動させる。
//	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
//
//	//当たり判定作成
//	//SkillState = true;
//	
//}

/// <summary>
/// レベル4で必殺技を使用したときの処理
/// </summary>
void KnightBase::UltimateSkill()
{
	//レベルを3下げる
levelDown(LvUpStatus, m_Status, Lv, 3);
	//経験値をリセット
	ExpReset(Lv, GetExp);
	//レベルの経験値テーブルにする
	ExpTableChamge(Lv, ExpTable);

	m_animState = enKnightState_UltimateSkill;

}

/// <summary>
/// リスポーンする座標のセット
/// </summary>
void KnightBase::SetRespawn()
{
	//リスポーンする座標0番の取得
	GetRespawnPos();
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
}

/// <summary>
/// 自身が倒されたときの処理
/// </summary>
void KnightBase::Death()
{
	////死亡ステート
	//m_animState = enKnightState_Death;
	//レベルを１下げる
	levelDown(LvUpStatus, m_Status, Lv,1);
	//HPを最大にする
	m_Status.Hp = m_Status.MaxHp;
	//経験値をリセット
	ExpReset(Lv,GetExp);
	//一つ下のレベルの経験値テーブルにする
	ExpTableChamge(Lv,ExpTable);
}

/// <summary>
/// アニメーション再生時に直線移動させる方向の決定
/// </summary>
void KnightBase::AnimationMove()
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	m_Skill_Forward = Vector3::Zero;
	m_Skill_Right = Vector3::Zero;

	//カメラの前方向と右方向のベクトルを持ってくる。
	m_Skill_Forward = g_camera3D->GetForward();
	m_Skill_Right = g_camera3D->GetRight();
	//y方向には移動させない。
	m_Skill_Forward.y = 0.0f;
	m_Skill_Right.y = 0.0f;

	//左スティックの入力量とstatusのスピードを乗算。
	m_Skill_Right *= stickL.x * m_Status.Speed;
	m_Skill_Forward *= stickL.y * m_Status.Speed;
}

//直線移動させる
void KnightBase::MoveStraight(Vector3& right, Vector3& forward)
{
	//移動処理
	//移動速度にスティックの入力量を加算する。
	//Vector3 m_SkillSpeed; 
	m_moveSpeed = right + forward;
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

/// <summary>
/// アニメーション再生の処理
/// </summary>
void KnightBase::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);

	switch (m_animState)
	{
	case enKnightState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle,0.4f);
		break;
	case enKnightState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run,0.2f);
		break;
	case enKnightState_ChainAtk:
		m_modelRender.PlayAnimation(enAnimationClip_ChainAtk, 0.3f);
		break;
	case enKnightState_Skill:
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.3f);
		break;
	case enKnightState_UltimateSkill:
		m_modelRender.PlayAnimation(enAnimationClip_UltimateSkill,0.1f);
		break;
	case enKnightState_Avoidance:
		m_modelRender.PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	case enAnimationClip_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.4f);
		break;
	case enAnimationClip_Death:
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.4f);
	default:
		break;
	}
}

/// <summary>
/// アニメーションのステートの処理
/// </summary>
void KnightBase::ManageState()
{
	switch (m_animState)
	{
	case enKnightState_Idle:
		OnProcessIdleStateTransition();
		break;
	case enKnightState_Run:
		OnProcessRunStateTransition();
		break;
	case enKnightState_ChainAtk:
		OnProcessChainAtkStateTransition();
		break;
	case enKnightState_Skill:
		OnProcessSkillAtkStateTransition();
		break;
	case enKnightState_UltimateSkill:
		OnProcessUltimateSkillAtkStateTransition();
		break;
	case enKnightState_Avoidance:
		OnProcessAvoidanceStateTransition();
		break;
	case enAnimationClip_Damege:
		OnProcessDamegeStateTransition();
		break;
	case enAnimationClip_Death:
		OnProcessDeathStateTransition();
		break;

	}
}

/// <summary>
/// 歩きアニメーションが再生されているなら。
/// </summary>
void KnightBase::OnProcessCommonStateTransition()
{
	//スティックの入力量があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
			//走りステート
		m_animState = enKnightState_Run;
		return;
	}
	else
	{
		//なかったら待機ステート
		m_animState = enKnightState_Idle;
		return;
	}
}

/// <summary>
/// Idleアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

/// <summary>
/// Runアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

/// <summary>
/// FirstAtkアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessChainAtkStateTransition()
{
	//チェインアタックのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		//攻撃を始めたかの判定をfalseにする
		AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// スキルのアニメーション再生されているときの処理
/// </summary>
void KnightBase::OnProcessSkillAtkStateTransition()
{
	//スキルのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AtkState = false;
		SkillEndFlag = true;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		//待機ステート
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// 必殺技アニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessUltimateSkillAtkStateTransition()
{
	//必殺技アニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AtkState = false;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		//待機ステート
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

//回避のアニメーションが再生されているときの処理
void KnightBase::OnProcessAvoidanceStateTransition()
{
	//回避のアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AvoidanceFlag = false;
		AvoidanceEndFlag = true;
		//ボタンプッシュフラグをfalseにする
		pushFlag = false;
		//待機ステート
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Damegeアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessDamegeStateTransition()
{
	//ダメージを受けたときのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		m_animState = enKnightState_Idle;
		//無敵時間ステート
		//invincibleFlag = false;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Deathアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessDeathStateTransition()
{
	//ダメージを受けたときのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//リスポーンする座標に自身の座標をセット
		SetRespawn();
		Death();
		//待機ステート
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}