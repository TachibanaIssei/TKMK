#include "stdafx.h"
#include "KnightPlayer.h"

KnightPlayer::KnightPlayer()
{
	SetModel();
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
}

KnightPlayer::~KnightPlayer()
{

}

//bool KnightPlayer::Start()
//{
//
//	return true;
//}

void KnightPlayer::Update()
{
	//移動処理
	Move();
	
	//攻撃処理
	Attack();

	//回転処理
	Rotation();

	//クールタイムの処理
	COOlTIME(Cooltime, SkillState);

	//スキルを発動する処理
	//Bボタンが押されたら
	if (SkillState==false&&g_pad[0]->IsTrigger(enButtonB))
	{
		Skill();
		SkillState = true;
	}

	//必殺技を発動する処理
	//Xボタンが押されたら
	if (Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		UltimateSkill();
	}

	//レベルアップする
	/*if (g_pad[0]->IsTrigger(enButtonA))
	{
		if(Lv!=5)
		ExpProcess(exp);
	}*/

	//ダメージを受ける
	/*if (g_pad[0]->IsTrigger(enButtonX))
	{
		Dameged(dddd);
	}*/

	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void KnightPlayer::Attack()
{
	//一段目のアタックをしていないなら
	if (AtkState == false)
	{
		//Bボタン押されたら攻撃する
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_animState = enKnightState_FirstAtk;

			//FirstAtkFlag = true;
			//コンボを1増やす
			ComboState++;
			AtkState = true;
		}
	}
	
	if (FirstAtkFlag == true)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			SecondAtkFlag = true;
		}
	}

	if (SecondAtkStartFlag == true)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			LastAtkFlag = true;
		}
	}


	////一段目のアタックのアニメーションが再生されていないなら。
	//if (ComboState<1/*m_animState != enKnightState_FirstAtk*/)
	//{
	//	//抜け出す。
	//	return;
	//}
	//
	//{
	//	ComboTimer += g_gameTime->GetFrameDeltaTime();

	//	//一段目のアタックのアニメーションが再生されてから1秒〜3秒の間なら
	//	if (ComboTimer >= 1.5f && ComboTimer <= 3.0f)
	//	{

	//		if (g_pad[0]->IsTrigger(enButtonA))
	//		{
	//			SecondAtkState = true;
	//			/*m_animState = enKnightState_SecondAtk;
	//			ComboState = 0;
	//			ComboTimer = 0;*/
	//		}
	//	}
	//	else if(ComboTimer > 3.0f) {
	//	/*	ComboState = 0;
	//		ComboTimer = 0;
	//		AtkState = false;*/
	//		/*if (m_modelRender.IsPlayingAnimation())
	//		{
	//			m_modelRender.PlayAnimation();
	//		}*/
	//	}

	//}
}

/// <summary>
/// アニメーションイベントの再生
/// </summary>
/// <param name="clipName"></param>
/// <param name="eventName"></param>
void KnightPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		FirstAtkFlag = true;
	}

	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		SecondAtkStartFlag = true;
	}

		if (wcscmp(eventName, L"FirstAttack_End") == 0)
		{
			FirstAtkFlag = false;
			AtkState = false;
			//ボタンが押されていなかったら
				if (SecondAtkFlag == false)
				{
					m_animState = enKnightState_Idle;
				}
		}

		if (wcscmp(eventName, L"SecondAttack_End") == 0)
		{
			SecondAtkFlag = false;
			SecondAtkStartFlag = false;
			AtkState = false;
			//ボタンが押されていなかったら
			if (LastAtkFlag == false)
			{
				m_animState = enKnightState_Idle;
			}
		}

		if (wcscmp(eventName, L"LastAttack_End") == 0)
		{
			LastAtkFlag = false;
			AtkState = false;
		}

}

void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
