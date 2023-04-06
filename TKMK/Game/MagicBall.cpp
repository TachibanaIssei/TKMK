#include "stdafx.h"
#include "MagicBall.h"

MagicBall::MagicBall()
{

}

MagicBall:: ~MagicBall()
{
	DeleteGO(BallCollision);
}

bool MagicBall::Start()
{
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);

	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 250.0f;
	m_rotation.AddRotationDegY(360.0f);

	SetCollision();

	return true;
}

void MagicBall::Update()
{
	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	
	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��B
	BallCollision->SetPosition(m_position);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 2.0f) {
		//���g���폜����B
		DeleteGO(this);
	}

	BallCollision->Update();
}

