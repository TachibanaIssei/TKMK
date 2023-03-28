#include "stdafx.h"
#include "KnightUlt.h"

namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 50.0f, 15.0f);
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

	//�R���W�����I�u�W�F�N�g���쐬����B
	UltCollision = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	UltCollision->CreateBox(m_position, m_rotation, CollsionSize);

	//�p�҂��v���C���[��������B
	if (m_enUltSkill == enUltSkill_Player)
	{
		//���O��player_fireball�ɂ���B
		UltCollision->SetName("player_Ult");
	}
	//�p�҂����@�g����������B
	else if (m_enUltSkill == enUltSkill_Knight)
	{
		//���O��enemy_fireball�ɂ���B
		UltCollision->SetName("KnightAI_Ult");
	}

	//���菊�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	UltCollision->SetIsEnableAutoDelete(false);

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