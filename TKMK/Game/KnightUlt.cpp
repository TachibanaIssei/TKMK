#include "stdafx.h"
#include "KnightUlt.h"

namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 120.0f, 15.0f);
	const float SphereSize = 20.0f;
}

KnightUlt::KnightUlt()
{

}

KnightUlt::~KnightUlt()
{
	DeleteGO(UltCollision);
	DeleteGO(UltDeleteJudgeCollision);
}

bool KnightUlt::Start()
{
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 20.0f;
	m_moveSpeed *= 180.0f;
	m_rotation.AddRotationDegY(360.0f);

	SetCollision(CollsionSize);

	model.Init("Assets/modelData/character/Knight/Knight_Ult.tkm");
	model.SetPosition(m_position);
	model.SetRotation(m_rotation);
	model.Update();


	//m_position_judge = m_moveSpeed * 20.0f;

	//�R���W�����I�u�W�F�N�g���쐬����B
	UltDeleteJudgeCollision = NewGO<CollisionObject>(0);
	UltDeleteJudgeCollision->CreateSphere(m_position, m_rotation, SphereSize);
	//�R���W�����̖��O
	UltDeleteJudgeCollision->SetName("judgeCollision");
	//���菊�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	UltDeleteJudgeCollision->SetIsEnableAutoDelete(false);

	return true;
}

void KnightUlt::Update()
{
	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	m_position_judge = m_position;
	//�O����
	m_forward = m_moveSpeed;

	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��
	UltCollision->SetPosition(m_position);
	//�ǔ���p�̓����蔻��̍��W��ݒ肷��
	UltDeleteJudgeCollision->SetPosition(m_position);
	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 4.0f) {
		//���g���폜����B
		DeleteGO(this);
	}


	model.SetPosition(m_position);
	model.Update();


	UltCollision->Update();
	UltDeleteJudgeCollision->Update();
}

void KnightUlt::Render(RenderContext& rc)
{
	model.Draw(rc);
}