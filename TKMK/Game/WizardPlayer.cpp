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

	//�ړ�����
	Move(m_position, m_charCon, m_Status);

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

void WizardPlayer::Attack()
{

}

void WizardPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

void WizardPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

