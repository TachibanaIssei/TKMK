#include "stdafx.h"
#include "KnightPlayer.h"

KnightPlayer::KnightPlayer()
{
	SetModel();
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標0番の取得
	GetRespawnPos();
	m_respawnPos[respawnNumber].y += m_position_YUp;
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

	
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

	////スキルを発動する処理
	////Bボタンが押されたら
	//if (SkillState == false && g_pad[0]->IsTrigger(enButtonB))
	//{
	//	status.Speed += 120.0f;
	//	Skill();
	//	AtkCollistionFlag = true;
	//}

	////必殺技を発動する処理
	////Xボタンが押されたら
	//if (Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	//{
	//	//アニメーション再生、レベルを３下げる
	//	UltimateSkill();
	//	//必殺技発動フラグをセット
	//	UltimateSkillFlag = true;
	//}

	////必殺技発動フラグがセットされているなら
	//if (UltimateSkillFlag == true)
	//{
	//	UltimateSkillTimer += g_gameTime->GetFrameDeltaTime();
	//	//必殺技タイマーが3.0fまでの間
	//	if (UltimateSkillTimer <= 3.0f)
	//	{
	//		//コリジョンの作成、移動処理
	//		UltimateSkillCollistion(OldPosition, m_position);
	//	}
	//	else
	//	{
	//		//攻撃が有効な時間をリセット
	//		UltimateSkillTimer = 0;
	//		//必殺技発動フラグをリセット
	//		UltimateSkillFlag = false;
	//		//コリジョン削除
	//		DeleteGO(collisionObject);
	//		//コリジョン作成フラグをリセット
	//		UltCollisionSetFlag = false;
	//	}
	//}
	
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

	//剣士のY座標が腰なのでY座標を上げる
	m_position.y = m_position_YUp;

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
	
	if (m_AtkTmingState == FirstAtk_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_AtkTmingState = SecondAtk_State;
		}
	}

	if (m_AtkTmingState == SecondAtkStart_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_AtkTmingState = LastAtk_State;
		}
	}


	//スキルを発動する処理
	//Bボタンが押されたら
	if (SkillState == false && g_pad[0]->IsTrigger(enButtonB))
	{
		status.Speed += 120.0f;
		Skill();
		AtkCollistionFlag = true;
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

	//攻撃かスキルを使用しているなら
	//コリジョン作成
	if(AtkCollistionFlag ==true)AtkCollisiton();
}

/// <summary>
/// アニメーションイベントの再生
/// </summary>
/// <param name="clipName"></param>
/// <param name="eventName"></param>
void KnightPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//一段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		m_AtkTmingState =FirstAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
	}
	//二段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
	}
	//三段目のアタックのアニメーションが始まったら
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//剣のコリジョンを生成
		AtkCollistionFlag = true;
	}
	//////////////////////////////////////////////////////////////////////////
	//一段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"FirstAttack_End") == 0)
	{
		
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
	}
	///一段目のアタックのアニメーションが終わったら
	if (wcscmp(eventName, L"FirstToIdle") == 0)
	{
		//ボタンが押されていなかったら
		if (m_AtkTmingState != SecondAtk_State)
		{
			AtkState = false;
			m_animState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}

	//二段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"SecondAttack_End") == 0)
	{
		
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
		//ボタンが押されていなかったら
		if (m_AtkTmingState != LastAtk_State)
		{
			AtkState = false;
			m_animState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}
	//三段目のアタックのアニメーションで剣を振り終わったら
	if (wcscmp(eventName, L"LastAttack_End") == 0)
	{
		m_AtkTmingState = Num_State;
		AtkState = false;
		//剣のコリジョンを生成しない
		AtkCollistionFlag = false;
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
