#include "stdafx.h"
#include "WizardPlayer.h"
#include "Game.h"
#include "Neutral_Enemy.h"
#include "MagicBall.h"

namespace {
	const Vector2 AVOIDANCE_BAR_POVOT = Vector2(1.0f, 1.0f);
	const Vector3 AVOIDANCE_BAR_POS = Vector3(98.0f, -397.0f, 0.0f);

	const Vector3 AVOIDANCE_FLAME_POS = Vector3(0.0f, -410.0f, 0.0f);
}

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

	//�X�L���̃N�[���^�C����\������t�H���g�̐ݒ�
	Skillfont.SetPosition(805.0f, -400.0f, 0.0f);
	Skillfont.SetScale(2.0f);
	Skillfont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	Skillfont.SetRotation(0.0f);
	Skillfont.SetShadowParam(true, 2.0f, g_vec4Black);

	//����̃t���[���̐ݒ�
	m_Avoidance_flameRender.Init("Assets/sprite/avoidance_flame.DDS", 300, 50);
	m_Avoidance_flameRender.SetPosition(AVOIDANCE_FLAME_POS);
	//����̃o�[�̐ݒ�
	m_Avoidance_barRender.Init("Assets/sprite/avoidance_bar.DDS", 194, 26);
	m_Avoidance_barRender.SetPivot(AVOIDANCE_BAR_POVOT);
	m_Avoidance_barRender.SetPosition(AVOIDANCE_BAR_POS);
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

	//�֐��ɂ���
	int SkillCoolTime = SkillTimer;
	wchar_t Skill[255];
	swprintf_s(Skill, 255, L"%d", SkillCoolTime);
	Skillfont.SetText(Skill);

	//�O�t���[���̍��W
	oldPosition = m_position;

	//��𒆂Ȃ�
	if (AvoidanceFlag == true) {
		m_wizardState = enWizardState_Avoidance;
		//�ړ��������s��(�����ړ��̂�)�B
		MoveStraight(m_Skill_Right, m_Skill_Forward);
	}


	//�X�L���N�[���^�C���̏���
	COOlTIME(Cooltime, SkillEndFlag, SkillTimer);
	
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

	//�_���[�W���󂯂�
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		Dameged(dddd);
	}

	//�ړ�����
	//�ړ�����
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	Move(m_position, m_charCon, m_Status, stickL);

	Attack();
	//�������
	Avoidance();
	//��]����
	Rotation();

	//�X�e�[�g
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�����蔻��
	Collision();

	//�N�[���^�C�����I����Ă��Ȃ��Ȃ�
	if (AvoidanceTimer != AvoidanceCoolTime)
	{
		//����̃X�v���C�g�̕\���̏���
		AvoidanceSprite();
	}

	m_position.y = m_position_YUp;
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

/// <summary>
/// �U������
/// </summary>
void WizardPlayer::Attack()
{
	//�A�^�b�N
	//A�{�^������������
	if (pushFlag==false&&g_pad[0]->IsTrigger(enButtonA))
	{
		m_wizardState = enWizardState_Attack;
		//AttackFlag = true;
		//FirstAtkFlag = true;
		pushFlag = true;
	}

	//�X�L���𔭓�
	//B�{�^������������
	if (pushFlag == false && SkillEndFlag == false && g_pad[0]->IsTrigger(enButtonB))
	{
		//HP�����炵��HP��0�ɂȂ�Ȃ烏�[�v�����Ȃ�
		if (m_Status.Hp <= 10)
		{
			return;
		}
		else
		{
			//HP��10���炷
			m_Status.Hp -= 10;
		}
		m_wizardState = enWizardState_Skill;
		SkillState = true;
		//pushFlag = true;
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
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		//�}�W�b�N�{�[���𐶐�����
		MakeMagicBall();
	}

	//���[�v���n�܂�����
	if (wcscmp(eventName, L"Warp_Start") == 0)
	{
		Skill(m_position, m_rot, m_charCon);
		m_modelRender.SetPosition(m_position);
	}

}

/// <summary>
/// �}�W�b�N�{�[���𐶐�����
/// </summary>
void WizardPlayer::MakeMagicBall()
{
	MagicBall* magicBall = NewGO<MagicBall>(0, "magicBall");

	Vector3 MagicBallPos = m_position;
	MagicBallPos.y += 15.0f;

	magicBall->SetPosition(MagicBallPos);
	magicBall->SetRotation(m_rot);
	magicBall->SetEnMagician(MagicBall::enMagician_Player);
	magicBall->GetWizardAttack(m_Status);
}

/// <summary>
/// �K�E�Z�̗��̐���
/// </summary>
void WizardPlayer::UltimateSkill()
{


}

void WizardPlayer::AvoidanceSprite()
{
	Vector3 AvoidanceScale = Vector3::One;
	//HP�o�[�̌����Ă��������B
	AvoidanceScale.x = (float)AvoidanceTimer / (float)AvoidanceCoolTime;
	m_Avoidance_barRender.SetScale(AvoidanceScale);

	m_Avoidance_flameRender.Update();
	m_Avoidance_barRender.Update();
}

void WizardPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//�X�L���̃N�[���^�C���ƃ^�C�}�[���Ⴄ�������\��
	if (SkillTimer != Cooltime)
		Skillfont.Draw(rc);
	//����̃N�[���^�C���ƃ^�C�}�[���Ⴄ�������\��
	if (AvoidanceTimer != AvoidanceCoolTime)
	{
		m_Avoidance_flameRender.Draw(rc);
		m_Avoidance_barRender.Draw(rc);
	}
}

