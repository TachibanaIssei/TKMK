#include "stdafx.h"
#include "KnightBase.h"


KnightBase::KnightBase()
{
	Lv=1;                    //レベル
	AtkSpeed=20;              //攻撃速度
	Cooltime=5;            //スキルのクールタイム
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
	m_animationClips[enAnimationClip_FirstAtk].Load("Assets/animData/Knight/Knight_ChainAttack.tka");
	m_animationClips[enAnimationClip_FirstAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_SecondAtk].Load("Assets/animData/Knight/Knight_SecondAtk.tka");
	m_animationClips[enAnimationClip_SecondAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Knight/Knight_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Knight/Knight_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);

	//剣士モデルを読み込み
	m_modelRender.Init("Assets/modelData/character/Knight/Knight_02.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	//「mixamorig:RightHand」(右手)ボーンのID(番号)を取得する。
	m_swordBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");
	//攻撃時のアニメーションイベント剣士の座標のID(番号)を取得する。
	AtkEndPosID = m_modelRender.FindBoneID(L"movePos");

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
		LevelUp(LvUpStatus,status,Lv);

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
void KnightBase::Move()
{
	//攻撃、ダメージ、死亡アニメーションが再生中なら
	if (IsEnableMove() == false)
	{
		//抜け出す
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
	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//キャラコンを動かす
	//m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charCon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
	}
}

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
		Vector3(110.0f, 15.0f, 15.0f) //大きさ。
	);
	collisionObject->SetName("player_attack");

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//matrix.MakeRotationZ(90.0f);
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

/// <summary>
/// ダメージを受けたときの処理
/// </summary>
/// <param name="damege">敵のダメージ</param>
void KnightBase::Dameged(int damege)
{
	status.Hp -= damege;
	//自身のHPが0以下なら
	if (status.Hp <= 0) {
		//倒されたときの処理に遷移
		Death();
	}
	else {
		m_animState = enKnightState_Damege;
	}
}

/// <summary>
/// スキルを使用したときの処理
/// </summary>
void KnightBase::Skill()
{
	//m_animState = enKnightState_Skill;
	m_animState = enKnightState_SecondAtk;
	//当たり判定作成
}

/// <summary>
/// レベル4で必殺技を使用したときの処理
/// </summary>
void KnightBase::UltimateSkill()
{
	//レベルを3下げる
	levelDown(LvUpStatus, status, Lv, 3);
	m_animState = enKnightState_SecondAtk;

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
	//死亡ステート
	m_animState = enKnightState_Death;
	//レベルを１下げる
	levelDown(LvUpStatus, status, Lv,1);
	//HPを最大にする
	status.Hp = status.MaxHp;
	//経験値をリセット
	ExpReset(Lv,GetExp);
	//一つ下のレベルの経験値テーブルにする
	ExpTableChamge(Lv,ExpTable);
}

/// <summary>
/// アニメーション再生の処理
/// </summary>
void KnightBase::PlayAnimation()
{
	switch (m_animState)
	{
	case enKnightState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case enKnightState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	case enKnightState_FirstAtk:
		m_modelRender.PlayAnimation(enAnimationClip_FirstAtk, 0.1f);
		break;
	case enKnightState_SecondAtk:
		m_modelRender.PlayAnimation(enAnimationClip_SecondAtk,0.1f);
		break;
	case enAnimationClip_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.1f);
		break;
	case enAnimationClip_Death:
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.1f);
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
	case enKnightState_FirstAtk:
		OnProcessFirstAtkStateTransition();
		break;
	case enKnightState_SecondAtk:
		OnProcessSecondAtkStateTransition();
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
void KnightBase::OnProcessFirstAtkStateTransition()
{
	//一段目のアタックのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// SecondAtkアニメーションが再生されているときの処理
/// </summary>
void KnightBase::OnProcessSecondAtkStateTransition()
{
	//2段目のアタックのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AtkState = false;
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
		//待機ステート
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

///// <summary>
///// アニメーションイベントの再生
///// </summary>
///// <param name="clipName"></param>
///// <param name="eventName"></param>
//void KnightBase::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
//{
//	if (wcscmp(eventName, L"Jump") == 0)
//	{
//		m_moveSpeed.y += 500.0f;
//	}
//}

//void KnightBase::Render(RenderContext& rc)
//{
//	m_modelRender.Draw(rc);
//}
