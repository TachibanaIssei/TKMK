#include "stdafx.h"
#include "WizardPlayer.h"
#include "Game.h"
#include "Neutral_Enemy.h"

WizardPlayer::WizardPlayer()
{
	//���@�g���̃��f����ǂݍ���
	SetModel();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//���X�|�[��������W2�Ԃ̎擾
	GetRespawnPos();
	respawnNumber = 2;        //���X�|�[��������W�̔ԍ�

	m_position.y = m_position_YUp;
	
	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);

	m_respawnPos[respawnNumber].y = m_position_YUp;
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

	//m_position=m_respawnPos[respawnNumber];
	

	//m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);

	//���m��Y���W�����Ȃ̂�Y���W���グ��
	//m_position.y = m_position_YUp;

	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

WizardPlayer::~WizardPlayer()
{

}

void WizardPlayer::Update()
{
	//game�N���X�̃|�[�Y�̃t���O�������Ă���ԏ������s��Ȃ�
	if (m_wizardState == enWizardState_Pause) {
		return;
	}

	//��𒆂Ȃ�
	if (AvoidanceFlag == true) {
		m_wizardState = enWizardState_Avoidance;
		//�ړ��������s��(�����ړ��̂�)�B
		MoveStraight(m_Skill_Right, m_Skill_Forward);
	}



	Attack();
	//�������
	Avoidance();
	//����N�[���^�C���̏���
	COOlTIME(AvoidanceCoolTime, AvoidanceEndFlag, AvoidanceTimer);

	//���x���A�b�v����
	//if (g_pad[0]->IsTrigger(/*enButtonLB1*/enButtonA))
	//{
	//	if (Lv != 10)
	//		ExpProcess(exp);
	//	//m_Status.GetExp += 5;
	//	//m_gameUI->LevelFontChange(Lv);
	//}


	//�ړ�����
	//�ړ�����
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	Move(m_position, m_charCon, m_Status, stickL);

	//��]����
	Rotation();

	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();

	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

/// <summary>
/// �U������
/// </summary>
void WizardPlayer::Attack()
{
	if (pushFlag==false&&g_pad[0]->IsTrigger(enButtonA))
	{
		m_wizardState = enWizardState_Attack;

		//FirstAtkFlag = true;
		//�R���{��1���₷
		//ComboState++;
		pushFlag = true;
	}
}

/// <summary>
/// 
/// </summary>
void WizardPlayer::Avoidance()
{
	//RB�{�^���������ꂽ��B
	//���
	if (pushFlag == false && AvoidanceEndFlag == false && g_pad[0]->IsTrigger(enButtonRB1)) {
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();
		AnimationMove(AvoidanceSpeed, stickL);
		pushFlag = true;
		AvoidanceFlag = true;
	}
}

void WizardPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

void WizardPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

