#include "stdafx.h"
#include "KnightPlayer.h"

KnightPlayer::KnightPlayer()
{
	//���炭�L�����R���̃G���[
	SetModel();
}

KnightPlayer::~KnightPlayer()
{

}

//bool KnightPlayer::Start()
//{
//
//	return true;
//}

void KnightPlayer::Update()
{
	//�ړ�����
	Move();

	//B�{�^���������ꂽ��B
	/*if (g_pad[0]->IsTrigger(enButtonB))
	{
		if(ComboState==0)
		FirstAtkState = true;
	}*/
	
	//�U������
	Attack();

	//��]����
	Rotation();

	//���x���A�b�v����
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if(Lv!=5)
		ExpProcess(exp);
	}

	//�_���[�W���󂯂�
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		Dameged(dddd);
	}

	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();

	//
	//OnProcessCommonStateTransition();
	////�ҋ@�̃X�e�[�g�̑J�ڏ���
	//OnProcessIdleStateTransition();
	////�����̃X�e�[�g�̑J�ڏ���
	//OnProcessRunStateTransition();
	////��i�ڂ̃A�^�b�N�̃X�e�[�g�̑J�ڏ���
	//OnProcessFirstAtkStateTransition();
	////��i�ڂ̃A�^�b�N�̃X�e�[�g�̑J�ڏ���
	//OnProcessSecondAtkStateTransition();
	////�_���[�W���󂯂��Ƃ��̃X�e�[�g�̑J�ڏ���
	//OnProcessDamegeStateTransition();
	////HP��0�ɂȂ����Ƃ��̃X�e�[�g�̑J�ڏ���
	//OnProcessDeathStateTransition();


	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}



void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
