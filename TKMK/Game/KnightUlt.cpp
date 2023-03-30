#include "stdafx.h"
#include "KnightUlt.h"

namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 120.0f, 15.0f);
}

KnightUlt::KnightUlt()
{

}

KnightUlt::~KnightUlt()
{
	DeleteGO(UltCollision);
}

bool KnightUlt::Start()
{
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);

	m_position += m_moveSpeed * 20.0f;
	m_moveSpeed *= 200.0f;
	m_rotation.AddRotationDegY(360.0f);

	SetCollision(CollsionSize);

	

	return true;
}

void KnightUlt::Update()
{
	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��B
	UltCollision->SetPosition(m_position);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 4.0f) {
		//���g���폜����B
		DeleteGO(this);
	}

	UltCollision->Update();
}