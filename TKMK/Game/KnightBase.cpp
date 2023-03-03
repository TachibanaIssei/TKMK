#include "stdafx.h"
#include "KnightBase.h"
#include "Actor.h"


KnightBase::KnightBase()
{
	Lv=1;                    //レベル
	AtkSpeed=20;              //攻撃速度
	Cooltime=5;            //スキルのクールタイム
	Point=0;                 //敵を倒して手に入れたポイント
	GetExp=0;                //中立の敵を倒したときの経験値
	ExpTable=10;              //経験値テーブル

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
	/*m_animationClips[enAnimationClip_lastAtk].Load("Assets/animData/UnitychanJump.tka");
	m_animationClips[enAnimationClip_lastAtk].SetLoopFlag(false);*/

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

	

	


	/*if (ComboState == 1 && g_pad[0]->IsTrigger(enButtonB))
	{
		m_animState = enKnightState_SecondAtk;
	}*/

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Death();
	}

	//
	Move();
	Attack();
	//回転処理
	Rotation();
	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();
	//モデルを動かす
	m_modelRender.SetPosition(m_position);
	//モデルのアプデ
	m_modelRender.Update();
}
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

void KnightBase::Attack()
{
	//一段目のアタックのアニメーションが再生されていないなら。
	if (m_animState != enKnightState_FirstAtk)
	{
		//抜け出す。
		return;
	}

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

void KnightBase::Dameged(int damege)
{

}

void KnightBase::Skill()
{

}

void KnightBase::UltimateSkill()
{

}

void KnightBase::Death()
{
	m_modelRender.SetPosition(m_respawnPos[0]);
}

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
		m_modelRender.PlayAnimation(enAnimationClip_SecondAtk);
		break;
	default:
		break;
	}
}

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

	}
}
void KnightBase::OnProcessCommonStateTransition()
{
	
	
	//Aボタン押されたらレベル上がる
	/*if (g_pad[0]->IsTrigger(enButtonA))
	{
		LevelUp(LvUpStatus, status);
	}*/

	//Bボタン押されたら
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_animState = enKnightState_FirstAtk;
		//
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

void KnightBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

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
