#include "stdafx.h"
#include "WizardUlt.h"
#include "Player.h"
#include "KnightAI.h"

WizardUlt::WizardUlt()
{

}

WizardUlt::~WizardUlt()
{

}

bool WizardUlt::Start()
{
	switch (m_DamegedChar)
	{
	case enDamegedChar_Plater:
		player = FindGO<Player>("player");


		break;

	case enDamegedChar_KnightAI:
		knightAI = FindGO<KnightAI>("knightAI");


		break;

	case enDamegedChar_WizardAI:

		break;

	case enDamegedChar_ZombieAI:

		break;

	case enDamegedChar_MonsterAI:

		break;

	default:
		break;
	}


	return true;
}

void WizardUlt::Update()
{
	switch (m_DamegedChar)
	{
	case enDamegedChar_Plater:


		break;

	case enDamegedChar_KnightAI:

		//int probability = rand() % 100;
		//int damege=0;
		////ämó¶Ç™50Åìà»â∫Ç»ÇÁ
		//if (probability < 50)
		//{
		//	damege = 1000;
		//}

		//knightAI->Dameged(damege);

		//DeleteGO(this);
		break;

	case enDamegedChar_WizardAI:

		break;

	case enDamegedChar_ZombieAI:

		break;

	case enDamegedChar_MonsterAI:

		break;

	default:
		break;
	}
}
