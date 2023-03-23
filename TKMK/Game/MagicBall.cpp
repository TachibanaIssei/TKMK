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
	m_moveSpeed *= 200.0f;
	m_rotation.AddRotationDegY(360.0f);

	//�R���W�����I�u�W�F�N�g���쐬����B
	BallCollision = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	BallCollision->CreateSphere(m_position, Quaternion::Identity, 20.0f/*50.0f * m_scale.z*/);

	//�p�҂��v���C���[��������B
	if (m_enMagician == enMagician_Player)
	{
		//���O��player_fireball�ɂ���B
		BallCollision->SetName("player_fireball");
	}
	//�p�҂����@�g����������B
	else if (m_enMagician == enMagician_Wizard)
	{
		//���O��enemy_fireball�ɂ���B
		BallCollision->SetName("enemy_fireball");
	}

	//���菊�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	BallCollision->SetIsEnableAutoDelete(false);

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

