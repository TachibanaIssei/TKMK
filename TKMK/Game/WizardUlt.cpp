#include "stdafx.h"
#include "WizardUlt.h"
#include "Player.h"
#include "KnightAI.h"
#include "Actor.h"

WizardUlt::WizardUlt()
{

}

WizardUlt::~WizardUlt()
{
	DeleteGO(UltCollision);
}

bool WizardUlt::Start()
{
	CreatCollision();
	//作成したアクターの名前のインスタンスを探す
	m_CreatMeActor = FindGO<Actor>(UltCollision->GetCreatorName());

	//ターゲットのアクターのインスタンスを探す
	m_targetActor = FindGO<Actor>(SetTargetActorName());

	return true;
}

void WizardUlt::Update()
{
	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();
	//タイマーが10秒を超えると
	if (m_timer > 3)
	{
		//ダメージを受けるか受けないか決める
		Damege();
		//ポイントを増やす

		DeleteGO(this);
	}

	Move();
}

void WizardUlt::Move()
{
	m_position = m_targetActor->GetPosition();
	m_position.y += 100.0f;

	UltCollision->SetPosition(m_position);
}

void WizardUlt::Damege()
{
	int Randam = rand() % 100;
	int Damege = 0;
	//もし２０以下なら即死
	if (Randam < 20)
	{
		Damege = 1000;
		//ターゲットのキャラにダメージを与える(即死)
		m_targetActor->Dameged(Damege, m_CreatMeActor);
		int downlevel = 1;
		//この技を打ったキャラのレベルを下げる
		m_CreatMeActor->levelDown(
			m_CreatMeActor->GetLvUPStatus(), 
			m_CreatMeActor->GetStatus(), 
			m_CreatMeActor->GetLevel(), 
			downlevel);
	}

	
}
