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
	//�쐬�����A�N�^�[�̖��O�̃C���X�^���X��T��
	m_CreatMeActor = FindGO<Actor>(UltCollision->GetCreatorName());

	//�^�[�Q�b�g�̃A�N�^�[�̃C���X�^���X��T��
	m_targetActor = FindGO<Actor>(SetTargetActorName());

	return true;
}

void WizardUlt::Update()
{
	//�^�C�}�[���Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();
	//�^�C�}�[��10�b�𒴂����
	if (m_timer > 3)
	{
		//�_���[�W��󂯂邩�󂯂Ȃ������߂�
		Damege();
		//�|�C���g�𑝂₷

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
	//���Q�O�ȉ��Ȃ瑦��
	if (Randam < 20)
	{
		Damege = 1000;
		//�^�[�Q�b�g�̃L�����Ƀ_���[�W��^����(����)
		m_targetActor->Dameged(Damege, m_CreatMeActor);
		int downlevel = 1;
		//���̋Z��ł���L�����̃��x�������
		m_CreatMeActor->levelDown(
			m_CreatMeActor->GetLvUPStatus(), 
			m_CreatMeActor->GetStatus(), 
			m_CreatMeActor->GetLevel(), 
			downlevel);
	}

	
}
