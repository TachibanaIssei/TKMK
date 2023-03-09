#include "stdafx.h"
#include "Neutral_Enemy.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>
#include"KnightPlayer.h"

Neutral_Enemy::Neutral_Enemy()
{

}

Neutral_Enemy::~Neutral_Enemy()
{

}

bool Neutral_Enemy::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Neutral_Enemy/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Neutral_Enemy/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Neutral_Enemy/Attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Neutral_Enemy/Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Neutral_Enemy/Damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/character/Neutral_Enemy/Neutral_Enemy.tkm", m_animationClips, enAnimationClip_Num);

	//���W��ݒ�
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rot);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	//�傫������
	
	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		10.0f,			//���a�B
		10.2f,			//�����B
		m_position		//���W�B
	);
	//���̃{�[����ID���擾����
	m_AttackBoneId = m_modelRender.FindBoneID(L"HeadTipJoint");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");

	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisY;
	m_rot.Apply(m_forward);
	//�X�e�[�^�X��ǂݍ���
	m_Status.Init("Enemy");
	return true;
}

void Neutral_Enemy::Update()
{

	//�ǐՏ����B
	Chase();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�U�������B
	Attack();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();

	//���f���̍X�V�B
	m_modelRender.Update();
}

void Neutral_Enemy::Rotation()
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

void Neutral_Enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_Neutral_EnemyState != enNeutral_Enemy_Chase)
	{
		return;
	}
	//m_targetPointPosition = m_knightPlayer->GetPosition();
	//bool isEnd;
	////if(){
	//	// �p�X����
	//m_pathFiding.Execute(
	//	m_path,							// �\�z���ꂽ�p�X�̊i�[��
	//	m_nvmMesh,						// �i�r���b�V��
	//	m_position,						// �J�n���W
	//	m_targetPointPosition,			// �ړ��ڕW���W
	//	PhysicsWorld::GetInstance(),	// �����G���W��	
	//	20.0f,							// AI�G�[�W�F���g�̔��a
	//	50.0f							// AI�G�[�W�F���g�̍����B
	//);
	////}
	//// �p�X����ړ�����B
	//m_position = m_path.Move(
	//	m_position,
	//	3.0f,
	//	isEnd
	//);
	Vector3 pos = m_position;
	m_charaCon.SetPosition(pos);
	m_modelRender.SetPosition(pos);
}

void Neutral_Enemy::Collision()
{
	if (m_Neutral_EnemyState == enNeutral_Enemy_ReceiveDamage || m_Neutral_EnemyState == enNeutral_Enemy_Death)
	{
		return;
	}

	//�G�̍U���p�̃R���W�������擾����
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�q���W�����̔z���for���ŉ�
	for (auto collision : collisions)
	{
		if (collision->IsHit(m_charaCon))
		{
			//hp�����炷

			//if (m_hp == 0)
			//{
			//	//���S�X�e�[�g�ɑJ�ڂ���B
			//	m_Neutral_EnemyState = enNeutral_Enemy_Death;
			//}
			//else {
			//	//��_���[�W�X�e�[�g�ɑJ�ڂ���B
			//	m_Neutral_EnemyState = enNeutral_Enemy_ReceiveDamage;
			//	//���ʉ��Đ�
			//}
		}
	}

}

void Neutral_Enemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ�������
	if (m_Neutral_EnemyState != enNeutral_Enemy_Attack)
	{
		return;
	}
	//�U�����蒆�ł����
	if (m_UnderAttack = true)
	{
		//�U���p�̃R���W�������쐬
		MakeAttackCollision();
	}

}

const bool Neutral_Enemy::SearchEnemy()const
{
	//���m����G�l�~�[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
	//�{�X�ƃv���C���[�̋�����������x�߂�������B
	if (diff.LengthSq() <= 100.0 * 100.0f)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K������B
		diff.Normalize();
		//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff);
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��180����菬������΁B
		if (angle <= (Math::PI / 180.0f) * 180.0f)
		{
			//�v���C���[���������I
			return true;
		}
	}

	return false;
}

void Neutral_Enemy::MakeAttackCollision()
{
	//�U������p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_AttackBoneId)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateSphere(m_position, Quaternion::Identity,20.0f);
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}
void Neutral_Enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//�G������������U��
	//�v���C���[����������B
	if (SearchEnemy() == true)
	{
		Vector3 diff = m_knightPlayer->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 320.0f;
		//�U���ł��鋗���Ȃ�B
		if (CanAttack() == true)
		{
			//�����ɂ���āA�U�����邩�ҋ@�����邩�����肷��B	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//�U���X�e�[�g�Ɉڍs����B
				m_Neutral_EnemyState = enNeutral_Enemy_Attack;
				m_UnderAttack = false;
				return;
			}
			else
			{
				//�ҋ@�X�e�[�g�Ɉڍs����B
				m_Neutral_EnemyState = enNeutral_Enemy_Idle;
				return;
			}

		}
		//�U���ł��Ȃ������Ȃ�B
		else
		{
			//�����ɂ���āA�ǐՂ�����
			int ram = rand() % 100;
			if (ram > 40)
			{
				//�ǐՃX�e�[�g�Ɉڍs����B
				m_Neutral_EnemyState = enNeutral_Enemy_Chase;
				return;
			}
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����B
		m_Neutral_EnemyState = enNeutral_Enemy_Idle;
		return;

	}
}

void Neutral_Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.5f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}
void Neutral_Enemy::ProcessRunStateTransition()
{
	//���̃X�e�[�g�֑J�ڂ���B
	ProcessCommonStateTransition();
}

void Neutral_Enemy::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�B
	if (CanAttack())
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.5f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Neutral_Enemy::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Neutral_Enemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_Neutral_EnemyState = enNeutral_Enemy_Chase;
		Vector3 diff = m_knightPlayer->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 100.0f;
	}
}

void Neutral_Enemy::ProcessDeathStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		
		//���g���폜����B
		DeleteGO(this);
	}
}

void Neutral_Enemy::ManageState()
{
	switch (m_Neutral_EnemyState)
	{
		//�ҋ@�X�e�[�g
	case enNeutral_Enemy_Idle:
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g
	case enNeutral_Enemy_Chase:
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g
	case enNeutral_Enemy_Attack:
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g
	case enNeutral_Enemy_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//���S�X�e�[�g
	case enNeutral_Enemy_Death:
		ProcessDeathStateTransition();
		break;
	}
}

void Neutral_Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.5f);
	switch(m_Neutral_EnemyState)
	{
		//�ҋ@�X�e�[�g
	case enNeutral_Enemy_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g
	case enNeutral_Enemy_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
		//�U���X�e�[�g
	case enNeutral_Enemy_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		break;
		//��_���[�W�X�e�[�g
	case enNeutral_Enemy_ReceiveDamage:
		m_modelRender.PlayAnimation(enNeutral_Enemy_ReceiveDamage, 0.5f);
		break;
		//���S�X�e�[�g
	case enNeutral_Enemy_Death:
		m_modelRender.PlayAnimation(enNeutral_Enemy_Death, 0.5f);
		break;
	}
}

void Neutral_Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O��Attack_start�̎�
	if (wcscmp(eventName, L"Attack_start") == 0) {
		//�U�����̔����true�ɂ���
		m_UnderAttack = true;
		//�U���G�t�F�N�g�𔭐�������


		//�傫����ݒ肷��B

		//���W�𒲐�

		//�G�t�F�N�g�Đ�

		//���ʉ����Đ�����

	}
	//�L�[�̖��O��attack_end�̎�
	else if (wcscmp(eventName,L"Attack_end")==0){
		m_UnderAttack = false;
	
	}
}

const bool Neutral_Enemy::CanAttack()const
{
	//�����̓G����v���C���[�Ɍ������x�N�g�����v�Z����
	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
	//�������߂�������
	if (diff.LengthSq() <= 75.0f * 75.0f)
	{
		//�U���ł���
		return true;
	}
	//�U���ł��Ȃ�
	return false;
}

void Neutral_Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}