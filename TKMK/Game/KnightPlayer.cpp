#include "stdafx.h"
#include "KnightPlayer.h"


KnightPlayer::KnightPlayer()
{
	SetModel();
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	m_respawnPos[respawnNumber].y += m_position_YUp;
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

	
}

KnightPlayer::~KnightPlayer()
{

}


void KnightPlayer::Update()
{
	//�O�t���[���̍��W���擾
	OldPosition = m_position;

	//�ړ�����
	Move(m_position, m_charCon, status);
	
	//�U������
	Attack();

	//�X�L���X�e�[�g��ture�̊�
	if (SkillState == true) {
		//�ړ��������s��(�X�L���g�p���͒����ړ��̂�)�B
		Skill(m_Skill_Right, m_Skill_Forward);
	}

	//��]����
	Rotation();

	//�N�[���^�C���̏���
	COOlTIME(Cooltime, SkillEndFlag);

	
	//���x���A�b�v����
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if(Lv!=5)
		ExpProcess(exp);
	}

	//�_���[�W���󂯂�
	/*if (g_pad[0]->IsTrigger(enButtonX))
	{
		Dameged(dddd);
	}*/

	//�����蔻��
	Collition();

	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();

	//���m��Y���W�����Ȃ̂�Y���W���グ��
	m_position.y = m_position_YUp;

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void KnightPlayer::Attack()
{
	//�A�łōU���ł��Ȃ��Ȃ�

	//��i�ڂ̃A�^�b�N�����Ă��Ȃ��Ȃ�
	//if (AtkState == false)
	//{
	//	//B�{�^�������ꂽ��U������
	//	if (g_pad[0]->IsTrigger(enButtonA))
	//	{
	//		m_animState = enKnightState_ChainAtk;
	//		
	//		//FirstAtkFlag = true;
	//		//�R���{��1���₷
	//		//ComboState++;
	//		AtkState = true;
	//	}
	//}
	
	if (m_AtkTmingState == FirstAtk_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_AtkTmingState = SecondAtk_State;
		}
	}

	if (m_AtkTmingState == SecondAtkStart_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_AtkTmingState = LastAtk_State;
		}
	}


	//�X�L���𔭓����鏈��
	//B�{�^���������ꂽ��
	if (SkillEndFlag==false && SkillState == false && g_pad[0]->IsTrigger(enButtonB))
	{
		status.Speed += 120.0f;
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;

		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();

		m_Skill_Forward = Vector3::Zero;
		m_Skill_Right = Vector3::Zero;

		//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
		m_Skill_Forward = g_camera3D->GetForward();
		m_Skill_Right = g_camera3D->GetRight();
		//y�����ɂ͈ړ������Ȃ��B
		m_Skill_Forward.y = 0.0f;
		m_Skill_Right.y = 0.0f;

		//���X�e�B�b�N�̓��͗ʂ�status�̃X�s�[�h����Z�B
		m_Skill_Right *= stickL.x * status.Speed;
		m_Skill_Forward *= stickL.y * status.Speed;


		SkillState = true;
		//�X�L���X�e�[�g��ture�̊�
		//Skill(m_Skill_Right, m_Skill_Forward);
		//AtkCollistionFlag = true;
	}

	//�K�E�Z�𔭓����鏈��
	//X�{�^���������ꂽ��
	if (Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		//�A�j���[�V�����Đ��A���x�����R������
		UltimateSkill();
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
	if(AtkCollistionFlag ==true)AtkCollisiton();
}

/// <summary>
/// �A�j���[�V�����C�x���g�̍Đ�
/// </summary>
/// <param name="clipName"></param>
/// <param name="eventName"></param>
void KnightPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		m_AtkTmingState =FirstAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
	}
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
	}
	//�O�i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
	}
	//�X�L���̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		//m_AtkTmingState = LastAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
	}
	//////////////////////////////////////////////////////////////////////////
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"FirstAttack_End") == 0)
	{
		
		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
	}
	///��i�ڂ̃A�^�b�N�̃A�j���[�V�������I�������
	if (wcscmp(eventName, L"FirstToIdle") == 0)
	{
		//�{�^����������Ă��Ȃ�������
		if (m_AtkTmingState != SecondAtk_State)
		{
			AtkState = false;
			m_animState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}

	//��i�ڂ̃A�^�b�N�̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"SecondAttack_End") == 0)
	{
		
		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
		//�{�^����������Ă��Ȃ�������
		if (m_AtkTmingState != LastAtk_State)
		{
			AtkState = false;
			m_animState = enKnightState_Idle;
			m_AtkTmingState = Num_State;
		}
	}
	//�O�i�ڂ̃A�^�b�N�̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"LastAttack_End") == 0)
	{
		m_AtkTmingState = Num_State;
		AtkState = false;
		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
	}
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false) {
		m_animState = enKnightState_Idle;
		AtkState = false;
	}

	//�O�i�ڂ̃A�^�b�N�̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"SkillAttack_End") == 0)
	{
		m_AtkTmingState = Num_State;
		AtkState = false;
		//�X�L���̈ړ����������Ȃ��悤�ɂ���
		SkillState = false;
		status.Speed -= 120.0f;
		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
	}
}

void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
