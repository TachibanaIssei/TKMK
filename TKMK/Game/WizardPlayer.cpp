#include "stdafx.h"
#include "WizardPlayer.h"
#include "Game.h"
#include "Neutral_Enemy.h"

WizardPlayer::WizardPlayer()
{
	//魔法使いのモデルを読み込む
	SetModel();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//リスポーンする座標2番の取得
	GetRespawnPos();
	respawnNumber = 2;        //リスポーンする座標の番号

	m_position.y = m_position_YUp;
	
	//リスポーンする座標のセット
	//キャラコン
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);

	m_respawnPos[respawnNumber].y = m_position_YUp;
	//剣士
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

	//m_position=m_respawnPos[respawnNumber];
	

	//m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);

	//剣士のY座標が腰なのでY座標を上げる
	//m_position.y = m_position_YUp;

	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

WizardPlayer::~WizardPlayer()
{

}

void WizardPlayer::Update()
{
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_wizardState == enWizardState_Pause) {
		return;
	}

	//回避中なら
	if (AvoidanceFlag == true) {
		m_wizardState = enWizardState_Avoidance;
		//移動処理を行う(直線移動のみ)。
		MoveStraight(m_Skill_Right, m_Skill_Forward);
	}



	Attack();
	//回避処理
	Avoidance();
	//回避クールタイムの処理
	COOlTIME(AvoidanceCoolTime, AvoidanceEndFlag, AvoidanceTimer);

	//レベルアップする
	//if (g_pad[0]->IsTrigger(/*enButtonLB1*/enButtonA))
	//{
	//	if (Lv != 10)
	//		ExpProcess(exp);
	//	//m_Status.GetExp += 5;
	//	//m_gameUI->LevelFontChange(Lv);
	//}


	//移動処理
	//移動処理
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	Move(m_position, m_charCon, m_Status, stickL);

	//回転処理
	Rotation();

	//ステート
	ManageState();
	//アニメーションの再生
	PlayAnimation();

	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

/// <summary>
/// 攻撃処理
/// </summary>
void WizardPlayer::Attack()
{
	if (pushFlag==false&&g_pad[0]->IsTrigger(enButtonA))
	{
		m_wizardState = enWizardState_Attack;

		//FirstAtkFlag = true;
		//コンボを1増やす
		//ComboState++;
		pushFlag = true;
	}
}

/// <summary>
/// 
/// </summary>
void WizardPlayer::Avoidance()
{
	//RBボタンが押されたら。
	//回避
	if (pushFlag == false && AvoidanceEndFlag == false && g_pad[0]->IsTrigger(enButtonRB1)) {
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();
		AnimationMove(AvoidanceSpeed, stickL);
		pushFlag = true;
		AvoidanceFlag = true;
	}
}

void WizardPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

void WizardPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

