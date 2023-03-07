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


void KnightPlayer::Update()
{
	//前フレームの座標を取得
	OldPosition = m_position;

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
		//アニメーション再生、レベルを３下げる
		UltimateSkill();
		//必殺技発動フラグをセット
		UltimateSkillFlag = true;
	}

	//必殺技発動フラグがセットされているなら
	if (UltimateSkillFlag == true)
	{
		UltimateSkillTimer += g_gameTime->GetFrameDeltaTime();
		//必殺技タイマーが3.0fまでの間
		if (UltimateSkillTimer <= 3.0f)
		{
			//コリジョンの作成、移動処理
			UltimateSkillCollistion(OldPosition, m_position);
		}
		else
		{
			//攻撃が有効な時間をリセット
			UltimateSkillTimer = 0;
			//必殺技発動フラグをリセット
			UltimateSkillFlag = false;
			//コリジョン削除
			DeleteGO(collisionObject);
			//コリジョン作成フラグをリセット
			UltCollisionSetFlag = false;
		}
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
	//連打で攻撃できなくなる

	//一段目のアタックをしていないなら
	if (AtkState == false)
	{
		//Bボタン押されたら攻撃する
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_animState = enKnightState_ChainAtk;
			
			//FirstAtkFlag = true;
			//コンボを1増やす
			//ComboState++;
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


	if(AtkCollistionFlag ==true)AtkCollisiton();
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
		AtkCollistionFlag = true;
	}

	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		SecondAtkStartFlag = true;
		AtkCollistionFlag = true;
	}

	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		AtkCollistionFlag = true;
	}

		if (wcscmp(eventName, L"FirstAttack_End") == 0)
		{
			FirstAtkFlag = false;
			AtkState = false;
			AtkCollistionFlag = false;
			////ボタンが押されていなかったら
			//	if (SecondAtkFlag == false)
			//	{
			//		m_animState = enKnightState_Idle;
			//		//座標
			//	}
		}

		if (wcscmp(eventName, L"FirstToIdle") == 0)
		{
			//ボタンが押されていなかったら
			if (SecondAtkFlag == false)
			{
				AtkState = false;
				m_animState = enKnightState_Idle;
				//座標
			}
		}

		if (wcscmp(eventName, L"SecondAttack_End") == 0)
		{
			SecondAtkFlag = false;
			SecondAtkStartFlag = false;
			AtkState = false;
			AtkCollistionFlag = false;
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
			AtkCollistionFlag = false;
			//座標

		}
		//アニメーションの再生が終わったら
		if (m_modelRender.IsPlayingAnimation() == false) {
			m_animState = enKnightState_Idle;
			AtkState = false;
		}

}

void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
