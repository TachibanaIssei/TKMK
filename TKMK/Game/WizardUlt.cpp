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

}

bool WizardUlt::Start()
{
	UltCollision->CreateSphere(m_position, Quaternion::Identity, 100.0f);
	UltCollision->SetName("Wizard_UltSkill");
	//UltCollision->SetCreatorName();

	return true;
}

void WizardUlt::Update()
{
	//switch (m_DamegedChar)
	//{
	//case enDamegedChar_Plater:


	//	break;

	//case enDamegedChar_KnightAI:

	//	//int probability = rand() % 100;
	//	//int damege=0;
	//	////�m����50���ȉ��Ȃ�
	//	//if (probability < 50)
	//	//{
	//	//	damege = 1000;
	//	//}

	//	//knightAI->Dameged(damege);

	//	//DeleteGO(this);
	//	break;

	//case enDamegedChar_WizardAI:

	//	break;

	//case enDamegedChar_ZombieAI:

	//	break;

	//case enDamegedChar_MonsterAI:

	//	break;

	//default:
	//	break;
	//}
}
