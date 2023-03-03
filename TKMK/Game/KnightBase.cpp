#include "stdafx.h"
#include "KnightBase.h"


KnightBase::KnightBase()
{
	Lv=1;                    //レベル
	AtkSpeed=20;              //攻撃速度
	Cooltime=5;            //スキルのクールタイム
	Point=0;                 //敵を倒して手に入れたポイント
	GetExp=0;                //中立の敵を倒したときの経験値
	ExpTable=10;              //経験値テーブル
	respawnNumber = 0;        //リスポーンする座標の番号
}

KnightBase::~KnightBase()
{

}

bool KnightBase::Start()
{
	//プレイヤー
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_FirstAtk].Load("Assets/animData/Knight/Knight_FirstAtk.tka");
	m_animationClips[enAnimationClip_FirstAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_SecondAtk].Load("Assets/animData/Knight/Knight_SecondAtk.tka");
	m_animationClips[enAnimationClip_SecondAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Knight/Knight_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Knight/Knight_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);

	//剣士モデルを読み込み
	m_modelRender.Init("Assets/modelData/character/Knight/Knight.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	//m_modelRender.SetScale(Vector3(0.1f, 0.1f, 0.1f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	/*m_modelRender.AddAnimationEvent([&](const wchar_t* clipName,
		const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});*/

	m_charCon.Init(
		15.0f,
		35.0f,
		m_position
	);

	return true;
}

void KnightBase::Update()
{

	
	/*if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
	}*/
	//重力
	//m_moveSpeed.y -= 980.0f * 1.0f / 60.0f;

	//if (g_pad[0]->IsTrigger(enButtonA))
	//{
	//	m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);

	//	//ジャンプさせる
	//	m_moveSpeed.y += 500.0f;

	//}
	//if (g_pad[0]->IsTrigger(enButtonB))
	//{
	//	m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
	//}
	//if (g_pad[0]->IsTrigger(enButtonX))
	//{
	//	m_modelRender.PlayAnimation(enAnimationClip_Jump, 0.1f);
	//}

	//レベルアップの処理
	////Aボタン押されたらレベル上がる
	//if (g_pad[0]->IsTrigger(enButtonA))
	//{
	//	LevelUp(LvUpStatus, status);
	//}
	//移動処理
	Move();
	//攻撃処理
	Attack();
	//回転処理
	Rotation();
	//
	//レベルアップする
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if(Lv!=5)
		ExpProcess(exp);
	}


	//ダメージを受ける
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		Dameged(dddd);
	}
	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();

	//モデルを動かす
	m_modelRender.SetPosition(m_position);
	//モデルのアプデ
	m_modelRender.Update();
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
	if (GetExp < ExpTable) { return; }      //抜け出す
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

//移動の処理
void KnightBase::Move()
{
	if (IsEnableMove() == false)
	{
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

//剣士の回転処理
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

//攻撃の処理
void KnightBase::Attack()
{
	//一段目のアタックのアニメーションが再生されていないなら。
	if (m_animState != enKnightState_FirstAtk)
	{
		//抜け出す。
		return;
	}
	else
	{
		ComboTimer += g_gameTime->GetFrameDeltaTime();

		//一段目のアタックのアニメーションが再生されてから1秒～3秒の間なら
		if (ComboTimer >= 4.0f && ComboTimer <= 6.0f)
		{
			if (ComboState == 1 && g_pad[0]->IsTrigger(enButtonB))
			{
				m_animState = enKnightState_SecondAtk;
				ComboState = 0;
				ComboTimer = 0;
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

//スキルを使用したときの処理
void KnightBase::Skill()
{

}

//必殺技を使用したときの処理
void KnightBase::UltimateSkill()
{

}

//
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

//自身が倒されたときの処理
void KnightBase::Death()
{
	//死亡ステート
	m_animState = enKnightState_Death;
	levelDown(LvUpStatus, status, Lv);
	status.Hp = status.MaxHp;
}
//アニメーション再生の処理
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
//アニメーションのステートの処理
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
//歩きアニメーションが再生されているなら。
void KnightBase::OnProcessCommonStateTransition()
{
	//Bボタン押されたら攻撃する
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_animState = enKnightState_FirstAtk;
		//コンボを1増やす
		ComboState = 1;
		return;
	}

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
//Idleアニメーションが再生されているときの処理
void KnightBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}
//Runアニメーションが再生されているときの処理
void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}
//FirstAtkアニメーションが再生されているときの処理
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
//SecondAtkアニメーションが再生されているときの処理
void KnightBase::OnProcessSecondAtkStateTransition()
{
	//2段目のアタックのアニメーション再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステート
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}
//Damegeアニメーションが再生されているときの処理
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
//Deathアニメーションが再生されているときの処理
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
//
void KnightBase::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"Jump") == 0)
	{
		m_moveSpeed.y += 500.0f;
	}
}

void KnightBase::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
