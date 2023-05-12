#include "stdafx.h"
#include "ExpforKnight.h"
#include "Game.h"
namespace {
	const Vector2 EXPERIENCE_BAR_POS = Vector2(900.0f, -620.0f);	//�o���l�o�[�̍��W
}
ExpforKnight::ExpforKnight()
{

}

ExpforKnight::~ExpforKnight()
{

}

bool ExpforKnight::Start()
{
	//�v���C���[���o���l���l������摜
	kyokusen.Init("Assets/sprite/1.DDS", 96.0f, 96.0f, AlphaBlendMode_Add);

		//���[���h���W���X�N�������W�ɕϊ�
		Vector3 EFKWorldPOS = m_EnemyPos;

		g_camera3D->CalcScreenPositionFromWorldPosition(m_EFKSCPOS, EFKWorldPOS);
		center.x = (m_EFKSCPOS.x - EXPERIENCE_BAR_POS.x) / 2.0f;
		center.y = (m_EFKSCPOS.y - EXPERIENCE_BAR_POS.y) / 2.0f;
		center.y += 400.0f;


	return true;
}

void ExpforKnight::Update()
{
	Bezier();

	if (m_Timer >= 1)
	{
		DeleteGO(this);
	}

}

void ExpforKnight::Bezier()
{
	MovePos.Lerp(m_Timer, m_EFKSCPOS, center);
	MovePos2.Lerp(m_Timer, center, EXPERIENCE_BAR_POS);
	MovePos3.Lerp(m_Timer, MovePos, MovePos2);

	kyokusen.SetPosition(MovePos3.x,MovePos3.y,0.0f);
	kyokusen.Update();

	m_Timer += 0.04f;

}
void ExpforKnight::Render(RenderContext& rc)
{

	kyokusen.Draw(rc);
}