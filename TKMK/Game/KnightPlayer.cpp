#include "stdafx.h"
#include "KnightPlayer.h"

KnightPlayer::KnightPlayer()
{
	SetModel();
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
}

KnightPlayer::~KnightPlayer()
{

}


void KnightPlayer::Update()
{
	//�O�t���[���̍��W���擾
	OldPosition = m_position;

	//�ړ�����
	Move();
	
	//�U������
	Attack();

	//��]����
	Rotation();

	//�N�[���^�C���̏���
	COOlTIME(Cooltime, SkillState);

	//�X�L���𔭓����鏈��
	//B�{�^���������ꂽ��
	if (SkillState==false&&g_pad[0]->IsTrigger(enButtonB))
	{
		Skill();
		SkillState = true;
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
	
	//���x���A�b�v����
	/*if (g_pad[0]->IsTrigger(enButtonA))
	{
		if(Lv!=5)
		ExpProcess(exp);
	}*/

	//�_���[�W���󂯂�
	/*if (g_pad[0]->IsTrigger(enButtonX))
	{
		Dameged(dddd);
	}*/

	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void KnightPlayer::Attack()
{
	//��i�ڂ̃A�^�b�N�����Ă��Ȃ��Ȃ�
	if (AtkState == false)
	{
		//B�{�^�������ꂽ��U������
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_animState = enKnightState_FirstAtk;
			
			//FirstAtkFlag = true;
			//�R���{��1���₷
			ComboState++;
			AtkState = true;
		}
	}
	
	if (FirstAtkFlag == true)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			SecondAtkFlag = true;
		}
	}

	if (SecondAtkStartFlag == true)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			LastAtkFlag = true;
		}
	}


	if(AtkCollistionFlag ==true)AtkCollisiton();
}

/// <summary>
/// �A�j���[�V�����C�x���g�̍Đ�
/// </summary>
/// <param name="clipName"></param>
/// <param name="eventName"></param>
void KnightPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"FirstAttack_Start") == 0)
	{
		FirstAtkFlag = true;
		AtkCollistionFlag = true;
	}

	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		SecondAtkStartFlag = true;
		AtkCollistionFlag = true;
	}

	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		AtkCollistionFlag = true;
	}

		if (wcscmp(eventName, L"FirstAttack_End") == 0)
		{
			FirstAtkFlag = false;
			AtkState = false;
			AtkCollistionFlag = false;
			//�{�^����������Ă��Ȃ�������
				if (SecondAtkFlag == false)
				{
					m_animState = enKnightState_Idle;
					//���W
				}
		}

		if (wcscmp(eventName, L"SecondAttack_End") == 0)
		{
			SecondAtkFlag = false;
			SecondAtkStartFlag = false;
			AtkState = false;
			AtkCollistionFlag = false;
			//�{�^����������Ă��Ȃ�������
			if (LastAtkFlag == false)
			{
				m_animState = enKnightState_Idle;
				//���W
				//AnimEndPos= m_modelRender.GetBone(AtkEndPosId)->GetPosition();
				//int a = 0;
			}
		}

		if (wcscmp(eventName, L"LastAttack_End") == 0)
		{
			LastAtkFlag = false;
			AtkState = false;
			AtkCollistionFlag = false;
			//���W

		}
		//�A�j���[�V�����̍Đ����I�������
		if(m_modelRender.IsPlayingAnimation()==false)m_animState = enKnightState_Idle;

}

void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}