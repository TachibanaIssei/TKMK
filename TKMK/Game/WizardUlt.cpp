#include "stdafx.h"
#include "WizardUlt.h"
#include "Player.h"
#include "KnightAI.h"
#include "Actor.h"
#include "Game.h"

WizardUlt::WizardUlt()
{

}

WizardUlt::~WizardUlt()
{
	DeleteGO(UltCollision);
	// 全てを終わらせる！！！！！！！
	m_game->SetStopFlag(false);
	//m_targetActor->UltEnd();
	m_CreatMeActor->UltEnd();
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
	Thunder->SetScale(Vector3::One * 5.0f);
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

	m_timer += g_gameTime->GetFrameDeltaTime();
	//タイマーが３より大きくなったら
	//if (m_timer > 3)
	//{
	//	//攻撃対象のキャラにダメージを与える処理
	//	Damege();
	//	//自身を消す
	//	DeleteGO(this);
	//}

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
	//5分の1の確率で即死
	if (Randam > 0)
	{
		Damege = 1000;
		//攻撃対象のキャラにダメージを与える
		m_targetActor->Dameged(Damege, m_CreatMeActor);
		int downlevel = 1;
		//必殺技を打ったキャラのレベルを下げる
		/*m_CreatMeActor->levelDown(
			m_CreatMeActor->GetLvUPStatus(), 
			m_CreatMeActor->GetStatus(), 
			m_CreatMeActor->GetLevel(), 
			downlevel);*/
	}

	
}
