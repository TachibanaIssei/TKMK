#include "stdafx.h"
#include "WizardBase.h"
#include "Status.h"
#include "GameUI.h"

WizardBase::WizardBase()
{
	//�X�e�[�^�X��ǂݍ���
	m_Status.Init("Wizard");
	Lv = 1;                    //���x��
	AtkSpeed = 10;              //�U�����x

	Cooltime = 15;            //�X�L���̃N�[���^�C��
	SkillTimer = Cooltime;

	AvoidanceCoolTime = 2;     ///����̃N�[���^�C��
	AvoidanceTimer = AvoidanceCoolTime;

	Point = 0;                 //�G��|���Ď�ɓ��ꂽ�|�C���g
	GetExp = 0;                //�����̓G��|�����Ƃ��̌o���l
	ExpTable = 5;              //�o���l�e�[�u��
}

WizardBase::~WizardBase()
{

}

/// <summary>
/// ���@�g���̃��f����A�j���[�V�����A�L�����R���̏�����
/// </summary>
void WizardBase::SetModel()
{
	//�v���C���[
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Wizard/Wizard_Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Wizard/Wizard_Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	/*m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);*/

	//���@�g���̃��f����ǂݍ���
	m_modelRender.Init("Assets/modelData/character/Wizard/Wizard.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	//m_modelRender.SetScale(Vector3(0.1f, 0.1f, 0.1f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	m_charCon.Init(
		15.0f,
		35.0f,
		m_position
	);
}

void WizardBase::ExpProcess(int Exp)
{

}

/// <summary>
/// ��]����
/// </summary>
void WizardBase::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rot.SetRotationYFromDirectionXZ(m_moveSpeed);
		//�G�`������ɉ�]��������B
		m_modelRender.SetRotation(m_rot);
	}
}

void WizardBase::Dameged(int damege)
{

}

void WizardBase::Death()
{

}

void WizardBase::UltimateSkill()
{

}

/// <summary>
/// ���X�|�[��������W�̃Z�b�g
/// </summary>
void WizardBase::SetRespawn()
{
	//���X�|�[��������W2�Ԃ̎擾
	GetRespawnPos();
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���@�g��
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
}

/// <summary>
/// �A�j���[�V�����Đ��̏���
/// </summary>
void WizardBase::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);

	switch (m_wizardState)
	{
	case enWizardState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.4f);
		break;
	case enWizardState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
	case enWizardState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	case enWizardState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Atk, 0.3f);
		break;
	case enWizardState_Skill:
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.3f);
		break;
	case enWizardState_UltimateSkill:
		//���������K�v�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I
		//m_modelRender.SetAnimationSpeed(1.2f);
		m_modelRender.PlayAnimation(enAnimationClip_UltimateSkill, 0.1);
		break;
	case enWizardState_Avoidance:
		m_modelRender.SetAnimationSpeed(1.5f);
		m_modelRender.PlayAnimation(enAnimationClip_Avoidance, 0.1f);
		break;
	case enWizardState_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.4f);
		break;
	case enWizardState_Death:
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.4f);
	default:
		break;
	}
}

/// <summary>
/// �A�j���[�V�����̃X�e�[�g�̏���
/// </summary>
void WizardBase::ManageState()
{
	switch (m_wizardState)
	{
	case enWizardState_Idle:
		OnProcessIdleStateTransition();
		break;
	case enWizardState_Walk:
		OnProcessIdleStateTransition();
		break;
	case enWizardState_Run:
		OnProcessRunStateTransition();
		break;
	case enWizardState_Attack:
		OnProcessAttackStateTransition();
		break;
	case enWizardState_Skill:
		OnProcessSkillAtkStateTransition();
		break;
	case enWizardState_UltimateSkill:
		OnProcessUltimateSkillAtkStateTransition();
		break;
	case enWizardState_Avoidance:
		OnProcessAvoidanceStateTransition();
		break;
	case enWizardState_Damege:
		OnProcessDamegeStateTransition();
		break;
	case enWizardState_Death:
		OnProcessDeathStateTransition();
		break;

	}
}

/// <summary>
/// �����A�j���[�V�������Đ�����Ă���Ȃ�B
/// </summary>
void WizardBase::OnProcessCommonStateTransition()
{
	//�X�e�B�b�N�̓��͗ʂ���������
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//if (Lv < 2) {
			m_wizardState = enWizardState_Walk;
		//}
		//else
			//����X�e�[�g
			//m_wizardState = enWizardState_Run;

		return;
	}
	else
	{
		//�Ȃ�������ҋ@�X�e�[�g
		m_wizardState = enWizardState_Idle;
		return;
	}
}

void WizardBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

void WizardBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

void WizardBase::OnProcessAttackStateTransition()
{

}

void WizardBase::OnProcessSkillAtkStateTransition()
{

}

void WizardBase::OnProcessUltimateSkillAtkStateTransition()
{

}

void WizardBase::OnProcessAvoidanceStateTransition()
{

}

void WizardBase::OnProcessDamegeStateTransition()
{

}

void WizardBase::OnProcessDeathStateTransition()
{

}


