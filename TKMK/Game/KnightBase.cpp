#include "stdafx.h"
#include "KnightBase.h"
#include "Actor.h"


KnightBase::KnightBase()
{
	Lv=1;                    //���x��
	AtkSpeed=20;              //�U�����x
	Cooltime=5;            //�X�L���̃N�[���^�C��
	Point=0;                 //�G��|���Ď�ɓ��ꂽ�|�C���g
	GetExp=0;                //�����̓G��|�����Ƃ��̌o���l
	ExpTable=10;              //�o���l�e�[�u��

}

KnightBase::~KnightBase()
{

}

bool KnightBase::Start()
{
	//�v���C���[
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_FirstAtk].Load("Assets/animData/Knight/Knight_FirstAtk.tka");
	m_animationClips[enAnimationClip_FirstAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_SecondAtk].Load("Assets/animData/Knight/Knight_SecondAtk.tka");
	m_animationClips[enAnimationClip_SecondAtk].SetLoopFlag(false);
	/*m_animationClips[enAnimationClip_lastAtk].Load("Assets/animData/UnitychanJump.tka");
	m_animationClips[enAnimationClip_lastAtk].SetLoopFlag(false);*/

	//���m���f����ǂݍ���
	m_modelRender.Init("Assets/modelData/character/Knight/Knight.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_position = { 0.0f,0.0f,0.0f };
	m_modelRender.SetPosition(m_position);
	//m_modelRender.SetScale(Vector3(0.1f, 0.1f, 0.1f));

	m_rot.SetRotationY(0.0f);
	m_modelRender.SetRotation(m_rot);

	/*m_modelRender.AddAnimationEvent([&](const wchar_t* clipName,
		const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});*/

	m_charCon.Init(
		15.0f,
		35.0f,
		m_position
	);

	return true;
}

void KnightBase::Update()
{
	/*if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
	}*/
	//�d��
	//m_moveSpeed.y -= 980.0f * 1.0f / 60.0f;

	//if (g_pad[0]->IsTrigger(enButtonA))
	//{
	//	m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);

	//	//�W�����v������
	//	m_moveSpeed.y += 500.0f;

	//}
	//if (g_pad[0]->IsTrigger(enButtonB))
	//{
	//	m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
	//}
	//if (g_pad[0]->IsTrigger(enButtonX))
	//{
	//	m_modelRender.PlayAnimation(enAnimationClip_Jump, 0.1f);
	//}

	

	


	/*if (ComboState == 1 && g_pad[0]->IsTrigger(enButtonB))
	{
		m_animState = enKnightState_SecondAtk;
	}*/

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Death();
	}

	//
	Move();
	Attack();
	//��]����
	Rotation();
	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//���f���𓮂���
	m_modelRender.SetPosition(m_position);
	//���f���̃A�v�f
	m_modelRender.Update();
}
void KnightBase::Move()
{
	if (IsEnableMove() == false)
	{
		return;
	}

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�status�̃X�s�[�h����Z�B
	right *= stickL.x * status.Speed;
	forward *= stickL.y * status.Speed;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right + forward;
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//�L�����R���𓮂���
	//m_position = m_charCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charCon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
	}
}
void KnightBase::Rotation()
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

void KnightBase::Attack()
{
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������Đ�����Ă��Ȃ��Ȃ�B
	if (m_animState != enKnightState_FirstAtk)
	{
		//�����o���B
		return;
	}

	ComboTimer += g_gameTime->GetFrameDeltaTime();

	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������Đ�����Ă���1�b�`3�b�̊ԂȂ�
	if (ComboTimer >= 4.0f && ComboTimer <= 6.0f)
	{
		if (ComboState == 1 && g_pad[0]->IsTrigger(enButtonB))
		{
			m_animState = enKnightState_SecondAtk;
			ComboState = 0;
			ComboTimer = 0;
		}
	}

}

void KnightBase::Dameged(int damege)
{

}

void KnightBase::Skill()
{

}

void KnightBase::UltimateSkill()
{

}

void KnightBase::Death()
{
	m_modelRender.SetPosition(m_respawnPos[0]);
}

void KnightBase::PlayAnimation()
{
	switch (m_animState)
	{
	case enKnightState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case enKnightState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	case enKnightState_FirstAtk:
		m_modelRender.PlayAnimation(enAnimationClip_FirstAtk, 0.1f);
		break;
	case enKnightState_SecondAtk:
		m_modelRender.PlayAnimation(enAnimationClip_SecondAtk);
		break;
	default:
		break;
	}
}

void KnightBase::ManageState()
{
	switch (m_animState)
	{
	case enKnightState_Idle:
		OnProcessIdleStateTransition();
		break;
	case enKnightState_Run:
		OnProcessRunStateTransition();
		break;
	case enKnightState_FirstAtk:
		OnProcessFirstAtkStateTransition();
		break;
	case enKnightState_SecondAtk:
		OnProcessSecondAtkStateTransition();
		break;

	}
}
void KnightBase::OnProcessCommonStateTransition()
{
	
	
	//A�{�^�������ꂽ�烌�x���オ��
	/*if (g_pad[0]->IsTrigger(enButtonA))
	{
		LevelUp(LvUpStatus, status);
	}*/

	//B�{�^�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_animState = enKnightState_FirstAtk;
		//
		ComboState = 1;
		return;
	}
	//�X�e�B�b�N�̓��͗ʂ���������
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//����X�e�[�g
		m_animState = enKnightState_Run;
		return;
	}
	else
	{

		//�Ȃ�������ҋ@�X�e�[�g
		m_animState = enKnightState_Idle;
		return;
	}
	
}

void KnightBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

void KnightBase::OnProcessFirstAtkStateTransition()
{
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

void KnightBase::OnProcessSecondAtkStateTransition()
{
	//2�i�ڂ̃A�^�b�N�̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}



void KnightBase::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"Jump") == 0)
	{
		m_moveSpeed.y += 500.0f;
	}
}

void KnightBase::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
