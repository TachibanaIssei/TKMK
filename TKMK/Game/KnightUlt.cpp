#include "stdafx.h"
#include "KnightUlt.h"
#include "graphics/effect/EffectEmitter.h"
#include "Game.h"
#include "Actor.h"
#include "GameCamera.h"
#include "KnightAI.h"

//�ǂɓ�������������悤�ɂ���todo
namespace {
	const Vector3 CollsionSize = Vector3(300.0f, 120.0f, 15.0f);//�����Ȃ�
	const Vector3 CollsionSize2 = Vector3(360.0f, 120.0f, 15.0f);//����1
	const Vector3 CollsionSize3 = Vector3(420.0f, 120.0f, 15.0f);//����2

	const float SphereSize = 20.0f;
}

KnightUlt::KnightUlt()
{
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);
	// �Q�[���J������T��
	m_gameCamera = FindGO<GameCamera>("gamecamera");
}

KnightUlt::~KnightUlt()
{
	Ulteffect->Stop();
	//DeleteGO(explosion);
	DeleteGO(Ulteffect);
	DeleteGO(UltCollision);
	DeleteGO(UltDeleteJudgeCollision);
	// �S�Ă��I��点��I�I�I�I�I�I�I
	m_game->SetStopFlag(false);
	m_actor->UltEnd();
	m_gameCamera->GameCameraUltEnd();
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
	Quaternion effectRot = m_rotation;
	m_rotation.AddRotationDegY(360.0f);

	//��������������W���ǂɓ������Ă��邩�ǂ̌������Ȃ琶�����Ȃ�
	if (MakeCheck() == true)
	{
		DeleteGO(this);
	}

	//�ړ����x�����߂�
	m_moveSpeed *= 940.0f;

	//�����蔻��̐���
	SetCollision();
	//�������鎞�Ԃ̐ݒ�
	SetDeletetime();
	//�G�t�F�N�g�̐���
	MakeUlt();

	//�ǂ̓����蔻��R���W�����I�u�W�F�N�g���쐬����B
	UltDeleteJudgeCollision = NewGO<CollisionObject>(0);
	UltDeleteJudgeCollision->CreateSphere(m_position, m_rotation, SphereSize);
	//�R���W�����̖��O
	UltDeleteJudgeCollision->SetName("judgeCollision");
	//���菊�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	UltDeleteJudgeCollision->SetIsEnableAutoDelete(false);

	
	Ulteffect = NewGO<EffectEmitter>(0);
	//�a���G�t�F�N�g�̍Đ��B
	if (ColorNumb == 2)
	{	
		Ulteffect->Init(enEffect_Knight_Ult_Delete_Blue);
	}
	else if (ColorNumb == 0)
	{
		Ulteffect->Init(enEffect_Knight_Ult_Red);
	}
	else if (ColorNumb == 1)
	{
		Ulteffect->Init(enEffect_Knight_Ult_Green);
	}
	else
	{
		Ulteffect->Init(enEffect_Knight_Ult_Yellow);
	}
	
	Ulteffect->SetScale({ 4.0f,4.0f,4.0f });
	Ulteffect->SetPosition(m_position);
	Ulteffect->SetRotation(effectRot);
	Ulteffect->Play();
	//Ulteffect->Update();
	//�O����
	m_forward = m_moveSpeed;
	return true;
}

void KnightUlt::Update()
{
	//���Ԃ��؂ꂽ��
	if (m_timer > DeleteTime) {
		//���g���폜����B
		DeleteGO(this);
	}

	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

		//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��
		UltCollision->SetPosition(m_position);
		UltCollision->Update();
		//�ǔ���p�̓����蔻��̍��W��ݒ肷��
		UltDeleteJudgeCollision->SetPosition(m_position);
		UltDeleteJudgeCollision->Update();
		//�G�t�F�N�g�̍��W��ݒ�
		Ulteffect->SetPosition(m_position);
		Ulteffect->Update();

	

	////�^�C�}�[���������Ԃ�4/5�ɒB������
	//if (m_timer > DeleteTime * (3.0f / 5.0f)&& explosionflag==false) {
	//	//�����G�t�F�N�g����
	//	explosion = NewGO<EffectEmitter>(0);
	//	explosion->Init(enEffect_Knight_Ult_Delete_Blue);
	//	explosion->SetPosition(m_position);
	//	//�ړ����x���v�Z�B
	//	m_moveSpeed = Vector3::AxisZ;
	//	m_rotation.Apply(m_moveSpeed);
	//	Quaternion effectRot = m_rotation;
	//	explosion->SetRotation(effectRot);
	//	m_rotation.AddRotationDegY(360.0f);
	//	explosion->SetScale(Vector3::One * 20.0f);
	//	explosion->Play();
	//	explosionflag = true;
	//}

	//if (explosionflag == true)
	//{
	//	explosion->SetPosition(m_position);
	//	explosion->Update();
	//}
	//���f���̍��W��ݒ�
	/*model.SetPosition(m_position);
	model.Update();*/


	
	
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
	m_Checkposition += m_moveSpeed * 60.0f;

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

/// <summary>
	/// �����蔻��̐ݒ�
	/// </summary>
void KnightUlt::SetCollision()
{
	Vector3 UltCollisionSize;
	if (CharLevel < 6)
	{
		UltCollisionSize = CollsionSize;
	}
	//���x����7�ȉ��Ȃ�
	//�K�E�Z��i�K����
	else if (CharLevel < 8)
	{
		UltCollisionSize = CollsionSize2;
	}
	//���x����10�ȉ��Ȃ�
	//�K�E�Z��i�K����
	else if (CharLevel <= 10)
	{
		UltCollisionSize = CollsionSize3;
	}
	//����̃R���W�������쐬����B
	UltCollision->CreateBox(m_position, m_rotation, UltCollisionSize);
	//�R���W�����̖��O
	UltCollision->SetName("player_UltimateSkill");

	//���菊�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	UltCollision->SetIsEnableAutoDelete(false);
}

void KnightUlt::MakeUlt()
{
	Vector3 UltScale = Vector3::Zero;
	if (CharLevel < 6)
	{
		UltScale = Vector3::One;
	}
	//���x����7�ȉ��Ȃ�
	//�K�E�Z��i�K����
	else if (CharLevel < 8)
	{
		UltScale = Vector3(1.2f, 1.0f, 1.0f);
	}
	//���x����10�ȉ��Ȃ�
	//�K�E�Z��i�K����
	else if (CharLevel <= 10)
	{
		UltScale = Vector3(1.4f, 1.0f, 1.0f);
	}

	/*model.InitBackGround("Assets/modelData/character/Knight/Knight_Ult.tkm");
	model.SetPosition(m_position);
	model.SetRotation(m_rotation);
	model.SetScale(UltScale);
	model.Update();*/
}

void KnightUlt::Render(RenderContext& rc)
{
	//model.Draw(rc);
}