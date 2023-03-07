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

	m_scale = Vector3(0.3f,0.3f, 0.3f);
	//���W��ݒ�
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rot);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisY;
	m_rot.Apply(m_forward);
	m_knightPlayer = FindGO<KnightPlayer>("m_knightplayer");
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

}

void Neutral_Enemy::Collision()
{
	if (m_NEState == enNEState_ReceiveDamage || m_NEState == enNEState_Death)
	{
		return;
	}
	//�G�̍U���p�̃R���W�������擾����
	//const auto& collisions = g_collisionObjectManager->FindCollisionObject();

}

void Neutral_Enemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ�������
	if (m_NEState != enNEState_Attack)
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
	return true;
}

void Neutral_Enemy::MakeAttackCollision()
{

}
void Neutral_Enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	//�G������������U��
	if (SearchEnemy()==true)
	{

		Vector3 diff = m_knightPlayer->GetPosition() - m_position;
		diff.Normalize();
		m_moveSpeed = diff;
		m_NEState = enNEState_Chase;
		//�U���ł��鋗���Ȃ�
		if (CanAttack() == true)
		{
			int ram = rand() % 100;
			if (ram > 70)
			{
				//�U���X�e�[�g�Ɉڍs����
				m_NEState = enNEState_Attack;
				m_UnderAttack = false;
				return;
			}
			else
			{
				m_NEState = enNEState_Idle;
				return;
			}
		}
		else
		{
			//�����ɂ���āA�ǐՂ�����
			int ram = rand() % 100;
			if (ram > 30)
			{
				m_NEState = enNEState_Chase;
				return;
			}
		}

	}
	//�G���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����B
		m_NEState = enNEState_Idle;
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
		m_NEState = enNEState_Chase;
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
	switch (m_NEState)
	{
		//�ҋ@�X�e�[�g
	case enNEState_Idle:
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g
	case enNEState_Chase:
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g
	case enNEState_Attack:
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g
	case enNEState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//���S�X�e�[�g
	case enNEState_Death:
		ProcessDeathStateTransition();
		break;
	}
}

void Neutral_Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.5f);
	switch(m_NEState)
	{
		//�ҋ@�X�e�[�g
	case enNEState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g
	case enNEState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
		//�U���X�e�[�g
	case enNEState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		break;
		//��_���[�W�X�e�[�g
	case enNEState_ReceiveDamage:
		m_modelRender.PlayAnimation(enNEState_ReceiveDamage, 0.5f);
		break;
		//���S�X�e�[�g
	case enNEState_Death:
		m_modelRender.PlayAnimation(enNEState_Death, 0.5f);
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
	Vector3 diff = m_knightPlayer->GetPosition() - m_position;
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//�U���ł���
		return true;
	}
	//�U���ł��Ȃ�
	return false;
}

void Neutral_Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}