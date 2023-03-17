#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"


KnightAI::KnightAI()
{
	SetModel();
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	respawnNumber = 1;        //���X�|�[��������W�̔ԍ�
	m_respawnPos[respawnNumber].y += m_position_YUp;
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);
}
KnightAI::~KnightAI()
{

}
void KnightAI::Update()
{
	Rotation();
	SearchPlayer();

	m_modelRender.Update();
}
void KnightAI::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}
void KnightAI::AvoidanceSprite()
{

}

void KnightAI::Rotation()
{
	m_rotation.AddRotationDegY(1.0f);
	m_modelRender.SetRotation(m_rotation);

	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
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
void KnightAI::SearchPlayer()
{
	m_isSearchPlayer = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 playerPosition = m_knightPlayer->GetPosition();
	Vector3 diff = playerPosition - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));
	//�v���C���[�����E���ɋ��Ȃ�������B
	if (Math::PI * 0.35f <= fabsf(angle))
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 70.0f, playerPosition.z));

	SweepResultWall callback;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�Փ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//�ǂƏՓ˂����I
	if (callback.isHit == true)
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return;
	}

	//�ǂƏՓ˂��ĂȂ��I�I
	//�v���C���[�������t���O��true�ɁB
	m_isSearchPlayer = true;
}
void KnightAI::Attack()
{

}

void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	if (m_isSearchPlayer == false)
	{
		m_fontRender.SetText(L"������Ȃ��E�E�E");
	}
	else
	{
		m_fontRender.SetText(L"�������I");
	}
	m_fontRender.Draw(rc);
}

