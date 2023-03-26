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
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Wizard/Wizard_Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enWizardState_Attack].Load("Assets/animData/Wizard/Wizard_Attack.tka");
	m_animationClips[enWizardState_Attack].SetLoopFlag(false);
	m_animationClips[enWizardState_Avoidance].Load("Assets/animData/Wizard/Wizard_Avoidance.tka");
	m_animationClips[enWizardState_Avoidance].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Wizard/Wizard_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Wizard/Wizard_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Skill].Load("Assets/animData/Wizard/Wizard_Warp.tka");
	m_animationClips[enAnimationClip_Skill].SetLoopFlag(false);


	//���@�g���̃��f����ǂݍ���
	m_modelRender.Init("Assets/modelData/character/Wizard/Wizard.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(Vector3(0.7f, 0.7f, 0.7f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	m_charCon.Init(
		10.0f,
		39.0f,
		m_position
	);
}

/// <summary>
/// �����̓G��|�����Ƃ��̌o���l�̏���
/// </summary>
/// <param name="GetExp">�����̓G�̌o���l</param>
void WizardBase::ExpProcess(int Exp)
{
	//�������x����10(Max)�Ȃ�
	if (Lv == 10)return;
	//���g�̌o���l�ɓG��|�����Ƃ��Ɏ�ɓ����o���l�𑫂�
	GetExp += Exp;
	//��ɓ��ꂽ�o���l���o���l�e�[�u���̂ق����傫��������
	if (GetExp < ExpTable) return;      //�����o��
	else {
		//�o���l�e�[�u������ɓ��ꂽ�o���l�̂ق����傫��������
		//���x���A�b�v
		LevelUp(LvUpStatus, m_Status, Lv);
		//���x���ɍ��킹�ă��x���̉摜��ύX����
		gameUI->LevelFontChange(Lv);
		switch (Lv)
		{
		case 2:
			ExpTable = 10;
			break;
		case 3:
			ExpTable = 20;
			break;
		case 4:
			ExpTable = 30;
			break;
		case 5:
			ExpTable = 40;
			break;
		case 6:
			ExpTable = 50;
			break;
		case 7:
			ExpTable = 60;
			break;
		case 8:
			ExpTable = 70;
			break;
		case 9:
			ExpTable = 80;
			break;
		default:
			break;
		}
	}
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

/// <summary>
/// �_���[�W���󂯂��Ƃ��̏���
/// </summary>
/// <param name="damege">�G�̍U����</param>
void WizardBase::Dameged(int damege)
{
	m_Status.Hp -= damege;
	//���g��HP��0�ȉ��Ȃ�
	if (m_Status.Hp <= 0) {
		//�|���ꂽ�Ƃ��̏����ɑJ��
		//���S�X�e�[�g
		m_wizardState = enWizardState_Death;
		m_Status.Hp = 0;
		//Death();
		//SetRespawn();


	}
	else {
		//�_���[�W�X�e�[�g
		m_wizardState = enWizardState_Damege;
		//���G���ԃt���O
		//invincibleFlag = true;
	}
}

/// <summary>
/// 
/// </summary>
void WizardBase::Death()
{
	////���S�X�e�[�g
	//m_playerState = enKnightState_Death;
	//���x�����P������
	levelDown(LvUpStatus, m_Status, Lv, 1);
	//HP���ő�ɂ���
	m_Status.Hp = m_Status.MaxHp;
	//�o���l�����Z�b�g
	ExpReset(Lv, GetExp);
	//����̃��x���̌o���l�e�[�u���ɂ���
	ExpTableChamge(Lv, ExpTable);


	//���x���ɍ��킹�ă��x���̉摜��ύX����
	gameUI->LevelFontChange(Lv);
}

/// <summary>
/// �X�L���̏���
/// </summary>
/// <param name="position"></param>
/// <param name="rotation"></param>
/// <param name="charCon"></param>
void WizardBase::Skill(Vector3& position,Quaternion& rotation, CharacterController& charCon)
{
	m_moveSpeed = Vector3::AxisZ;
	//��]��
	rotation.Apply(m_moveSpeed);
	position += m_moveSpeed * 500.0f;
	m_moveSpeed *= 1000.0f;
	rotation.AddRotationDegY(360.0f);


	Vector3 WarpPos;
	//���[�v�������W���I�u�W�F�N�g�Ƃ��Ԃ��Ă���Ȃ�
	m_WarpCollisionSolver.Execute
	(
		WarpPos,
		position,
		oldPosition
	);

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	charCon.SetPosition(position);
}

/// <summary>
/// �K�E�Z�̏���
/// </summary>
void WizardBase::UltimateSkill()
{

}

/// <summary>
/// �A�j���[�V�����Đ����ɒ����ړ�����������̌���
/// </summary>
/// <param name="moveSpeed">�X�e�B�b�N�̈ړ��ʂƏ�Z���������X�s�[�h�̒l</param>
/// <param name="stickL">�X�e�B�b�N�̈ړ��̓��͗�</param>
void WizardBase::AnimationMove(float moveSpeed, Vector3 stickL)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//�ړ��̓��͗ʂ��Ȃ��Ȃ�
	if (stickL.x == 0.0f && stickL.y == 0.0f) {
		//�O�Ɉړ�
		stickL.x = 0.0f;
		stickL.y = 1.0f;
	}

	m_Skill_Forward = Vector3::Zero;
	m_Skill_Right = Vector3::Zero;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	m_Skill_Forward = g_camera3D->GetForward();
	m_Skill_Right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	m_Skill_Forward.y = 0.0f;
	m_Skill_Right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�status�̃X�s�[�h����Z�B
	m_Skill_Right *= stickL.x * moveSpeed;
	m_Skill_Forward *= stickL.y * moveSpeed;
}

//�����ړ�������
void WizardBase::MoveStraight(Vector3& right, Vector3& forward)
{
	//�ړ�����
	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	//Vector3 m_SkillSpeed; 
	m_moveSpeed = right + forward;
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
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
		m_modelRender.SetAnimationSpeed(0.9f);
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	case enWizardState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Atk, 0.2f);
		break;
	case enWizardState_Skill:
		m_modelRender.SetAnimationSpeed(1.2f);
		m_modelRender.PlayAnimation(enAnimationClip_Skill, 0.6f);
		break;
	case enWizardState_UltimateSkill:
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
		m_modelRender.SetAnimationSpeed(1.2f);
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
		if (Lv < 4) {
			m_wizardState = enWizardState_Walk;
		}
		else
			//����X�e�[�g
			m_wizardState = enWizardState_Run;

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
	//�A�^�b�N�̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		//�U�����n�߂����̔����false�ɂ���
		//AtkState = false;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessSkillAtkStateTransition()
{
	//�X�L���̃A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
		//�X�L���G���h�t���O��true�ɂ���
		SkillEndFlag = true;
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessUltimateSkillAtkStateTransition()
{

}

void WizardBase::OnProcessAvoidanceStateTransition()
{
	//����̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AvoidanceFlag = false;
		AvoidanceEndFlag = true;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
		//�ҋ@�X�e�[�g
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessDamegeStateTransition()
{
	//�_���[�W���󂯂��Ƃ��̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		m_wizardState = enWizardState_Idle;
		//���G���ԃX�e�[�g
		//invincibleFlag = false;
		OnProcessCommonStateTransition();
	}
}

void WizardBase::OnProcessDeathStateTransition()
{
	//�_���[�W���󂯂��Ƃ��̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���X�|�[��������W�Ɏ��g�̍��W���Z�b�g
		SetRespawn();
		Death();
		//�ҋ@�X�e�[�g
		m_wizardState = enWizardState_Idle;
		OnProcessCommonStateTransition();
	}
}


