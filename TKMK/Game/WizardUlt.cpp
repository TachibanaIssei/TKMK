#include "stdafx.h"
#include "WizardUlt.h"
#include "Player.h"
#include "KnightAI.h"
#include "Actor.h"
#include "Game.h"
#include "GameCamera.h"

WizardUlt::WizardUlt()
{

}

WizardUlt::~WizardUlt()
{
	DeleteGO(UltCollision);
	// 全てを終わらせる！！！！！！！
	//m_game->SetStopFlag(false);
	////m_targetActor->UltEnd();
	//m_CreatMeActor->UltEnd();
	//地上にいることを示すカウンターを減らす
	m_CreatMeActor->SubOnGroundCharCounter();
	//ターゲットの地上にいるフラグをfalseにする	
	m_targetActor->ChangeGroundChackflag(false);
	
	//自分が必殺技を打った最後の雷なら
	if (UltEndFlag == true)
	{
		gameCamera->GameCameraUltEnd();
	}
}

bool WizardUlt::Start()
{
	CreatCollision();
	//必殺技を打ったキャラの名前
	m_CreatMeActor = FindGO<Actor>(UltCollision->GetCreatorName());

	//攻撃対象のキャラの名前
	m_targetActor = FindGO<Actor>(SetTargetActorName());

	Thunder = NewGO<EffectEmitter>(0);
	Thunder->Init(enEffect_Knight_Thunder);

	Vector3 ThunderPos = m_position;
	ThunderPos.y = 0.0f;
	Thunder->SetPosition(ThunderPos);
	Thunder->SetScale(Vector3::One * 12.0f);
	Thunder->Play();

	return true;
}

void WizardUlt::Update()
{
	if (Thunder->IsPlay() == false)
	{
		//攻撃対象のキャラにダメージを与える処理
		Damege();
		//自身を消す
		DeleteGO(this);
	}

	
	//一秒ごとに雷を落とす
	/*if (m_timer > 1.0f)
	{

		FallThunder();
		
	}
	else
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
	}*/
}

void WizardUlt::Move()
{
	m_position = m_targetActor->GetPosition();
	m_position.y += 100.0f;

	UltCollision->SetPosition(m_position);
}

//雷を落とす
void WizardUlt::FallThunder()
{
	//for (auto actor : m_game->GetActors())
	//{
	//	//生成するキャラと必殺技を打ったキャラの名前が同じなら処理を飛ばす
	//	if (m_CreatMeActor->GetName() == actor->GetName())
	//	{
	//		continue;
	//	}

	//	//地上にいるAIにだけ雷を落とす(エフェクト生成)
	//	if (actor->IsGroundIn() == true)
	//	{
	//		Thunder = NewGO<EffectEmitter>(0);
	//		Thunder->Init(enEffect_Knight_Thunder);

	//		Vector3 ThunderPos = actor->GetPosition();
	//		ThunderPos.y = 0.0f;
	//		Thunder->SetPosition(ThunderPos);
	//		Thunder->SetScale(Vector3::One * 12.0f);
	//		Thunder->Play();
	//	}

	//	//タイマーリセット
	//	m_timer = 0.0f;
	//}
}

void WizardUlt::Damege()
{
	int Randam = rand() % 100;
	int Damege = 0;
	//5分の1の確率で即死
	if (Randam > 0)
	{
		Damege = 1000;
		//攻撃対象のキャラにダメージを与える
		m_targetActor->Dameged(Damege, m_CreatMeActor);
		int downlevel = 1;
		//必殺技を打ったキャラのレベルを下げる
		
	}

	
}
