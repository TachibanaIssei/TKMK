#include "stdafx.h"
#include "KnightBase.h"


KnightBase::KnightBase()
{
	Lv=1;                    //���x��
	AtkSpeed=20;              //�U�����x
	Cooltime=5;            //�X�L���̃N�[���^�C��
	Point=0;                 //�G��|���Ď�ɓ��ꂽ�|�C���g
	GetExp=0;                //�����̓G��|�����Ƃ��̌o���l
	ExpTable=5;              //�o���l�e�[�u��
	respawnNumber = 0;        //���X�|�[��������W�̔ԍ�

	
}

KnightBase::~KnightBase()
{

}

void KnightBase::SetModel()
{
	//�v���C���[
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Knight/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_FirstAtk].Load("Assets/animData/Knight/Knight_ChainAttack.tka");
	m_animationClips[enAnimationClip_FirstAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_SecondAtk].Load("Assets/animData/Knight/Knight_SecondAtk.tka");
	m_animationClips[enAnimationClip_SecondAtk].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damege].Load("Assets/animData/Knight/Knight_Damege.tka");
	m_animationClips[enAnimationClip_Damege].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/Knight/Knight_Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);

	//���m���f����ǂݍ���
	m_modelRender.Init("Assets/modelData/character/Knight/Knight_02.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	//�umixamorig:RightHand�v(�E��)�{�[����ID(�ԍ�)���擾����B
	m_swordBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");
	//�U�����̃A�j���[�V�����C�x���g���m�̍��W��ID(�ԍ�)���擾����B
	AtkEndPosID = m_modelRender.FindBoneID(L"movePos");

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

/// <summary>
/// �����̓G��|�����Ƃ��̌o���l�̏���
/// </summary>
/// <param name="GetExp">�����̓G�̌o���l</param>
void KnightBase::ExpProcess(int Exp)
{
	//���g�̌o���l�ɓG��|�����Ƃ��Ɏ�ɓ����o���l�𑫂�
	GetExp += Exp;
	//��ɓ��ꂽ�o���l���o���l�e�[�u���̂ق����傫��������
	if (GetExp < ExpTable) return;      //�����o��
	else {
		//�o���l�e�[�u������ɓ��ꂽ�o���l�̂ق����傫��������
		//���x���A�b�v
		LevelUp(LvUpStatus,status,Lv);

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

		default:
			break;
		}
	}
}

/// <summary>
/// �ړ�����
/// </summary>
void KnightBase::Move()
{
	//�U���A�_���[�W�A���S�A�j���[�V�������Đ����Ȃ�
	if (IsEnableMove() == false)
	{
		//�����o��
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

/// <summary>
/// ��]����
/// </summary>
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

/// <summary>
/// �U�����̓����蔻��̏���
/// </summary>
void KnightBase::AtkCollisiton()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	//collisionPosition += forward * 50.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition, //���W�B
		Quaternion::Identity, //��]�B
		Vector3(110.0f, 15.0f, 15.0f) //�傫���B
	);
	collisionObject->SetName("player_attack");

	//�uSword�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//matrix.MakeRotationZ(90.0f);
	//�uSword�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

/// <summary>
/// �_���[�W���󂯂��Ƃ��̏���
/// </summary>
/// <param name="damege">�G�̃_���[�W</param>
void KnightBase::Dameged(int damege)
{
	status.Hp -= damege;
	//���g��HP��0�ȉ��Ȃ�
	if (status.Hp <= 0) {
		//�|���ꂽ�Ƃ��̏����ɑJ��
		Death();
	}
	else {
		m_animState = enKnightState_Damege;
	}
}

/// <summary>
/// �X�L�����g�p�����Ƃ��̏���
/// </summary>
void KnightBase::Skill()
{
	//m_animState = enKnightState_Skill;
	m_animState = enKnightState_SecondAtk;
	//�����蔻��쐬
}

/// <summary>
/// ���x��4�ŕK�E�Z���g�p�����Ƃ��̏���
/// </summary>
void KnightBase::UltimateSkill()
{
	//���x����3������
	levelDown(LvUpStatus, status, Lv, 3);
	m_animState = enKnightState_SecondAtk;

}

/// <summary>
/// ���X�|�[��������W�̃Z�b�g
/// </summary>
void KnightBase::SetRespawn()
{
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);
}

/// <summary>
/// ���g���|���ꂽ�Ƃ��̏���
/// </summary>
void KnightBase::Death()
{
	//���S�X�e�[�g
	m_animState = enKnightState_Death;
	//���x�����P������
	levelDown(LvUpStatus, status, Lv,1);
	//HP���ő�ɂ���
	status.Hp = status.MaxHp;
	//�o���l�����Z�b�g
	ExpReset(Lv,GetExp);
	//����̃��x���̌o���l�e�[�u���ɂ���
	ExpTableChamge(Lv,ExpTable);
}

/// <summary>
/// �A�j���[�V�����Đ��̏���
/// </summary>
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
		m_modelRender.PlayAnimation(enAnimationClip_SecondAtk,0.1f);
		break;
	case enAnimationClip_Damege:
		m_modelRender.PlayAnimation(enAnimationClip_Damege, 0.1f);
		break;
	case enAnimationClip_Death:
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.1f);
	default:
		break;
	}
}

/// <summary>
/// �A�j���[�V�����̃X�e�[�g�̏���
/// </summary>
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
	case enAnimationClip_Damege:
		OnProcessDamegeStateTransition();
		break;
	case enAnimationClip_Death:
		OnProcessDeathStateTransition();
		break;

	}
}

/// <summary>
/// �����A�j���[�V�������Đ�����Ă���Ȃ�B
/// </summary>
void KnightBase::OnProcessCommonStateTransition()
{
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

/// <summary>
/// Idle�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessIdleStateTransition()
{
	OnProcessCommonStateTransition();
}

/// <summary>
/// Run�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessRunStateTransition()
{
	OnProcessCommonStateTransition();
}

/// <summary>
/// FirstAtk�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
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

/// <summary>
/// SecondAtk�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessSecondAtkStateTransition()
{
	//2�i�ڂ̃A�^�b�N�̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		AtkState = false;
		//�ҋ@�X�e�[�g
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Damege�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessDamegeStateTransition()
{
	//�_���[�W���󂯂��Ƃ��̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

/// <summary>
/// Death�A�j���[�V�������Đ�����Ă���Ƃ��̏���
/// </summary>
void KnightBase::OnProcessDeathStateTransition()
{
	//�_���[�W���󂯂��Ƃ��̃A�j���[�V�����Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���X�|�[��������W�Ɏ��g�̍��W���Z�b�g
		SetRespawn();
		//�ҋ@�X�e�[�g
		m_animState = enKnightState_Idle;
		OnProcessCommonStateTransition();
	}
}

///// <summary>
///// �A�j���[�V�����C�x���g�̍Đ�
///// </summary>
///// <param name="clipName"></param>
///// <param name="eventName"></param>
//void KnightBase::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
//{
//	if (wcscmp(eventName, L"Jump") == 0)
//	{
//		m_moveSpeed.y += 500.0f;
//	}
//}

//void KnightBase::Render(RenderContext& rc)
//{
//	m_modelRender.Draw(rc);
//}
