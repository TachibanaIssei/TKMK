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
	m_CreatMeActor = FindGO<Actor>(UltCollision->GetCreatorName());
	return true;
}

void WizardUlt::Update()
{
	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();
	//タイマーが10秒を超えると
	if (m_timer > 10)
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
	//m_CreatMeActor = FindGO<Actor>(UltCollision->GetCreatorName());

	m_position = m_CreatMeActor->GetPosition();
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
		m_CreatMeActor->Dameged(Damege);
	}

	
}
