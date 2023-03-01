#include "stdafx.h"
#include "KnightBase.h"


KnightBase::KnightBase()
{

}

KnightBase::~KnightBase()
{

}

bool KnightBase::Start()
{
	//プレイヤー
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/Knight_run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	/*m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/UnitychanJump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);*/

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

	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 120.0f;
	forward *= stickL.y * 120.0f;

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


	//モデルを動かす
	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}

void KnightBase::Attack()
{

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
