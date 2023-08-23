#include "stdafx.h"
#include "ExpforKnight.h"
#include "Game.h"
#include "GameUI.h"
namespace {
	const Vector2 EXPERIENCE_BAR_POS = Vector2(900.0f, -620.0f);	//�o���l�o�[�̍��W
}
ExpforKnight::ExpforKnight()
{

}

ExpforKnight::~ExpforKnight()
{
	gameUI = FindGO<GameUI>("m_gameUI");
	gameUI->ChangeEXPUpFlag(true);
}

bool ExpforKnight::Start()
{
	//�v���C���[���o���l���l������摜

	m_NormalExp.Init("Assets/sprite/Exp.DDS",128.0f, 128.0f, AlphaBlendMode_Add);

	//���[���h���W���X�N�������W�ɕϊ�
	Vector3 EFKWorldPOS = m_EnemyPos;

	g_camera3D->CalcScreenPositionFromWorldPosition(m_EFKSCPOS, EFKWorldPOS);
	center.x = (m_EFKSCPOS.x - EXPERIENCE_BAR_POS.x) / 2.0f;
	center.y = (m_EFKSCPOS.y - EXPERIENCE_BAR_POS.y) / 2.0f;
	center.x -= 200.0f;
	center.y += 800.0f;
	if (isRabbit)
	{
		Randamu = rand() % 601 - 300;
		center.x += Randamu;
		Randamu = rand() % 601 - 300;
		center.y += Randamu;
	}

	else
	{
		Randamu = rand() % 401 - 200;
		center.x += Randamu;
		Randamu = rand() % 401 - 200;
		center.y += Randamu;
	}
	
	return true;
}

void ExpforKnight::Update()
{
	Bezier();

	if (m_Timer >= 0.98)
	{
		DeleteGO(this);
	}
	if (LeftTimer >= 1)
	{
		DeleteGO(this);
	}
}

void ExpforKnight::Bezier()
{
	
	MovePos.Lerp(LeftTimer, m_EFKSCPOS, center);
	MovePos2.Lerp(m_Timer, center, EXPERIENCE_BAR_POS);
	MovePos3.Lerp(m_Timer, MovePos, MovePos2);

	m_NormalExp.SetPosition(MovePos3.x,MovePos3.y,0.0f);
	m_NormalExp.Update();

	if (LeftTimer > 0.2f && LeftTimer < 0.24f)
	{
		LeftTimer += 0.02f * 0.2f;
		m_Timer += 0.03f * 0.2f;
	}
	else
	{
		LeftTimer += 0.02f;
		m_Timer += 0.03f;
	}
	

}
void ExpforKnight::Render(RenderContext& rc)
{

	m_NormalExp.Draw(rc);

}