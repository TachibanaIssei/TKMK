#include "stdafx.h"
#include "KnightUlt.h"

namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 120.0f, 15.0f);
	const float SphereSize = 20.0f;
}

KnightUlt::KnightUlt()
{
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);
}

KnightUlt::~KnightUlt()
{
	DeleteGO(UltCollision);
	DeleteGO(UltDeleteJudgeCollision);
}

bool KnightUlt::Start()
{
	//�ŏ��̍��W
	m_firstposition = m_position;
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	//����������W(���m�̏����O)
	m_position += m_moveSpeed * 20.0f;
	
	m_rotation.AddRotationDegY(360.0f);

	//��������������W���ǂɓ������Ă��邩�ǂ̌������Ȃ琶�����Ȃ�
	if (MakeCheck() == true)
	{
		DeleteGO(this);
	}

	//�ړ����x�����߂�
	m_moveSpeed *= 180.0f;


	SetCollision(CollsionSize);

	model.InitBackGround("Assets/modelData/character/Knight/Knight_Ult.tkm");
	model.SetPosition(m_position);
	model.SetRotation(m_rotation);
	model.Update();

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

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			return 0.0f;
		}

		//�ǂƂԂ�������B
		//�t���O��true�ɁB
		isHit = true;
		return 0.0f;
	}
};

//�������邩�`�F�b�N����(�ǂ��������琶�����Ȃ�)
bool KnightUlt::MakeCheck()
{
	m_Checkposition = m_position;
	m_Checkposition += m_moveSpeed * 30.0f;

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓L�����̍��W�B
	start.setOrigin(btVector3(m_firstposition.x, m_firstposition.y + 70.0f, m_firstposition.z));
	//�I�_�͕K�E�Z�̍��W�B
	end.setOrigin(btVector3(m_Checkposition.x, m_Checkposition.y + 70.0f, m_Checkposition.z));

	//�ǂ̔����Ԃ�
	SweepResultWall callback_Wall;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback_Wall);
	//�ǂƏՓ˂����I
	if (callback_Wall.isHit == true)
	{
		return true;
	}

	return false;
}

void KnightUlt::Render(RenderContext& rc)
{
	model.Draw(rc);
}