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
	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();
	//�^�C�}�[��10�b�𒴂����
	if (m_timer > 10)
	{
		//�_���[�W���󂯂邩�󂯂Ȃ������߂�
		Damege();
		//�|�C���g�𑝂₷

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
	//�����Q�O�ȉ��Ȃ瑦��
	if (Randam < 20)
	{
		Damege = 1000;
		m_CreatMeActor->Dameged(Damege);
	}

	
}
