#include "stdafx.h"
#include "KnightAI.h"
#include "Game.h"
#include "KnightPlayer.h"
#include "Neutral_Enemy.h"


KnightAI::KnightAI()
{
	m_Status.Init("Knight");
	SetModel();
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	respawnNumber = 1;        //���X�|�[��������W�̔ԍ�
	m_respawnPos[respawnNumber].y /*+= m_position_YUp*/;
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
	m_Neutral_Enemy = FindGO<Neutral_Enemy>("Neutral_Enemy");
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);

	
	m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);

	//���m��Y���W�����Ȃ̂�Y���W���グ��
	m_position.y = m_position_YUp;

	m_modelRender.SetPosition(m_position);
}
KnightAI::~KnightAI()
{

}
void KnightAI::Update()
{
	Rotation();
	//SearchPlayer();

	

	//m_position = m_charCon.Execute(m_moveSpeed, 0.1f / 60.0f);

	////���m��Y���W�����Ȃ̂�Y���W���グ��
	//m_position.y = m_position_YUp;
	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}
void KnightAI::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}
void KnightAI::AvoidanceSprite()
{

}
void KnightAI::ChaseAI()
{
	
}
void KnightAI::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rot.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rot);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rot.Apply(m_forward);
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
void KnightAI::SearchEnemy()
{
	m_isSearchEnemy = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 AIPos = m_Neutral_Enemy->GetPosition();
	Vector3 diff = AIPos - m_position;

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
	end.setOrigin(btVector3(AIPos.x, AIPos.y + 70.0f, AIPos.z));

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
	m_isSearchEnemy = true;
}
void KnightAI::Attack()
{
	//�A�łōU���ł��Ȃ��Ȃ�

	//��i�ڂ̃A�^�b�N�����Ă��Ȃ��Ȃ�
	if (pushFlag == false && AtkState == false)
	{
		//B�{�^�������ꂽ��U������
			m_playerState = enKnightState_ChainAtk;

			//FirstAtkFlag = true;
			//�R���{��1���₷
			//ComboState++;
			pushFlag = true;
			AtkState = true;
	}
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������X�^�[�g�����Ȃ�
	if (m_AtkTmingState == FirstAtk_State)
	{
		
			//�X�e�[�g���i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = SecondAtk_State;
		
	}

	if (m_AtkTmingState == SecondAtkStart_State)
	{
	
			//�X�e�[�g���O�i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = LastAtk_State;
		
	}


	//�X�L���𔭓����鏈��
	//B�{�^���������ꂽ��
	if (pushFlag == false && SkillEndFlag == false && SkillState == false && g_pad[0]->IsTrigger(enButtonB))
	{

		//�ړ����x���グ��
		m_Status.Speed += 120.0f;

		//AnimationMove(SkillSpeed);
		pushFlag = true;
		SkillState = true;
		//AtkCollistionFlag = true;
	}

	//�K�E�Z�𔭓����鏈��
	//X�{�^���������ꂽ��
	if (pushFlag == false && Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		pushFlag = true;
		//�A�j���[�V�����Đ��A���x�����R
		UltimateSkill();



		//�A���e�B���b�gSE
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(16);
		se->Play(false);
		se->SetVolume(0.3f);

		//�K�E�Z�����t���O���Z�b�g
		UltimateSkillFlag = true;
	}

	//�K�E�Z�����t���O���Z�b�g����Ă���Ȃ�
	if (UltimateSkillFlag == true)
	{
		UltimateSkillTimer += g_gameTime->GetFrameDeltaTime();
		//�K�E�Z�^�C�}�[��3.0f�܂ł̊�
		if (UltimateSkillTimer <= 3.0f)
		{
			//�R���W�����̍쐬�A�ړ�����
			UltimateSkillCollistion(OldPosition, m_position);
		}
		else
		{
			//�U�����L���Ȏ��Ԃ����Z�b�g
			UltimateSkillTimer = 0;
			//�K�E�Z�����t���O�����Z�b�g
			UltimateSkillFlag = false;
			//�R���W�����폜
			DeleteGO(collisionObject);
			//�R���W�����쐬�t���O�����Z�b�g
			UltCollisionSetFlag = false;
		}
	}

	//�U�����X�L�����g�p���Ă���Ȃ�
	//�R���W�����쐬
	//if (AtkCollistionFlag == true) AtkCollisiton();

}

void KnightAI::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}

