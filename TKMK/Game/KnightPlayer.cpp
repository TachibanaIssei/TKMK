#include "stdafx.h"
#include "KnightPlayer.h"

KnightPlayer::KnightPlayer()
{
	//恐らくキャラコンのエラー
	SetModel();
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

	//Bボタンが押されたら。
	/*if (g_pad[0]->IsTrigger(enButtonB))
	{
		if(ComboState==0)
		FirstAtkState = true;
	}*/
	
	//攻撃処理
	Attack();

	//回転処理
	Rotation();

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

	//
	//OnProcessCommonStateTransition();
	////待機のステートの遷移処理
	//OnProcessIdleStateTransition();
	////歩きのステートの遷移処理
	//OnProcessRunStateTransition();
	////一段目のアタックのステートの遷移処理
	//OnProcessFirstAtkStateTransition();
	////二段目のアタックのステートの遷移処理
	//OnProcessSecondAtkStateTransition();
	////ダメージを受けたときのステートの遷移処理
	//OnProcessDamegeStateTransition();
	////HPが0になったときのステートの遷移処理
	//OnProcessDeathStateTransition();


	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}



void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
