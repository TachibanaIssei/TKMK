#include "stdafx.h"
#include "KnightPlayer.h"
#include "Game.h"
#include "Neutral_Enemy.h"
#include "KnightUlt.h"
#include "GameUI.h"
//�X�L���g�����Ƃ��ɔ͈͓��ɓG�������炻�̕����Ɍ������Ă���
//for���AfindGO�g��
//HP0�ɂȂ��Ă����ȂȂ���莀�ʂƂ��ɂق��̃X�e�[�g�Ɉڂ�Ȃ��悤�ɂ���

namespace {
	const Vector2 AVOIDANCE_BAR_POVOT = Vector2(1.0f,1.0f);
	const Vector3 AVOIDANCE_BAR_POS = Vector3(98.0f, -397.0f, 0.0f);

	const Vector3 AVOIDANCE_FLAME_POS = Vector3(0.0f, -410.0f, 0.0f);
}

KnightPlayer::KnightPlayer()
{
	m_gameUI = FindGO<GameUI>("m_gameUI");

	SetModel();
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//���X�|�[��������W0�Ԃ̎擾
	GetRespawnPos();
	respawnNumber = 0;        //���X�|�[��������W�̔ԍ�
	//m_respawnPos[respawnNumber].y += m_position_YUp;

	//m_position=

	//���X�|�[��������W�̃Z�b�g
	//�L�����R��
	m_charCon.SetPosition(m_respawnPos[respawnNumber]);
	//���m
	m_modelRender.SetPosition(m_respawnPos[respawnNumber]);

	//m_position=m_respawnPos[respawnNumber];

	m_position = m_charCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	
	//���m��Y���W�����Ȃ̂�Y���W���グ��
	//m_position.y = m_position_YUp;

	m_modelRender.SetPosition(m_position);
	//m_modelRender.Update();

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

KnightPlayer::~KnightPlayer()
{

}

void KnightPlayer::Update()
{
	//todo
	//game�N���X�̃|�[�Y�̃t���O�������Ă���ԏ������s��Ȃ�
	if (m_knightState == enKnightState_Pause) {
		return;
	}
	//���̃t���[���ƑO�̃t���[���̃��x��������Ă�����
	if (oldLv != Lv) {
		//���x���ɍ��킹��GameUI�̃��x���̉摜��ύX����
		m_gameUI->LevelFontChange(Lv);
	}

	oldLv = Lv;

	int SkillCoolTime = SkillTimer;
	wchar_t Skill[255];
	swprintf_s(Skill, 255, L"%d", SkillCoolTime);
	Skillfont.SetText(Skill);


	//�O�t���[���̍��W���擾
	OldPosition = m_position;

	//�ړ�����
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	Move(m_position, m_charCon, m_Status, stickL);
	
	////RB�{�^���������ꂽ��B
	////���
	//if (AvoidanceEndFlag==false && AvoidanceFlag == false && g_pad[0]->IsTrigger(enButtonRB1)) {
	//	//����X�e�[�g
	//	//m_knightState = enKnightState_Avoidance;
	//	AnimationMove();
	//	AvoidanceFlag = true;
	//}
	
	//��𒆂Ȃ�
	if (AvoidanceFlag == true) {
		m_knightState = enKnightState_Avoidance;
		//�ړ��������s��(�����ړ��̂�)�B
		MoveStraight(m_Skill_Right, m_Skill_Forward);
	}

	//�X�e�[�g���f�X�̂Ƃ��{�^���������Ȃ��悤�ɂ���
	if (m_knightState != enKnightState_Death) {
		//�U������
		Attack();
		//�������
		Avoidance();
	}
	

	//�X�L���g�p���Ȃ�
	if (SkillState == true) {
		//�X�L���X�e�[�g
		m_knightState = enKnightState_Skill;
		//�ړ��������s��(�����ړ��̂�)�B
		MoveStraight(m_Skill_Right, m_Skill_Forward);
	}

	//��]����
	Rotation();

	//�X�L���N�[���^�C���̏���
	COOlTIME(Cooltime, SkillEndFlag,SkillTimer);

	//����N�[���^�C���̏���
	COOlTIME(AvoidanceCoolTime, AvoidanceEndFlag, AvoidanceTimer);

	//���x���A�b�v����
	//if (g_pad[0]->IsTrigger(/*enButtonLB1*/enButtonA))
	//{
	//	if(Lv!=10)
	//	ExpProcess(exp);
	//	//m_Status.GetExp += 5;
	//	//m_gameUI->LevelFontChange(Lv);
	//}

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

	if (AvoidanceTimer != AvoidanceCoolTime)
	{
		//����̃X�v���C�g�̕\���̏���
		AvoidanceSprite();
	}
	

	//���m��Y���W�����Ȃ̂�Y���W���グ��
	m_position.y = m_position_YUp;

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	
}

//�U������
void KnightPlayer::Attack()
{
	
	//�A�łōU���ł��Ȃ��Ȃ�

	//��i�ڂ̃A�^�b�N�����Ă��Ȃ��Ȃ�
	if (pushFlag==false&&AtkState == false)
	{
		//B�{�^�������ꂽ��U������
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_knightState = enKnightState_ChainAtk;
			
			//FirstAtkFlag = true;
			//�R���{��1���₷
			//ComboState++;
			pushFlag = true;
			AtkState = true;
		}
	}
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������X�^�[�g�����Ȃ�
	if (m_AtkTmingState == FirstAtk_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�X�e�[�g���i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = SecondAtk_State;
		}
	}

	if (m_AtkTmingState == SecondAtkStart_State)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�X�e�[�g���O�i�ڂ̃A�^�b�N�̃A�j���[�V�����X�^�[�g�X�e�[�g�ɂ���
			m_AtkTmingState = LastAtk_State;
		}
	}


	//�X�L���𔭓����鏈��
	//B�{�^���������ꂽ��
	if (pushFlag == false && SkillEndFlag==false && SkillState == false && g_pad[0]->IsTrigger(enButtonB))
	{

		//�ړ����x���グ��
		m_Status.Speed += 120.0f;
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();
		//�X�L�����g���Ƃ��̃X�s�[�h���g��
		AnimationMove(SkillSpeed, stickL);
		pushFlag = true;
		SkillState = true;
		//AtkCollistionFlag = true;
	}

	//�K�E�Z�𔭓����鏈��
	//X�{�^���������ꂽ��
	if (pushFlag == false && Lv >= 4 && g_pad[0]->IsTrigger(enButtonX))
	{
		pushFlag = true;
		//�A�j���[�V�����Đ��A���x�����R������
		UltimateSkill();

		//�A���e�B���b�gSE
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(16);
		se->Play(false);
		se->SetVolume(0.3f);

		//�K�E�Z�����t���O���Z�b�g
		UltimateSkillFlag = true;
	}

	
	//�U�����X�L�����g�p���Ă���Ȃ�
	//�R���W�����쐬
	if (AtkCollistionFlag == true) AtkCollisiton();
	
}

/// <summary>
/// �������
/// </summary>
void KnightPlayer::Avoidance()
{
	//RB�{�^���������ꂽ��B
	//���
	if (pushFlag == false && AvoidanceEndFlag == false && AvoidanceFlag == false && g_pad[0]->IsTrigger(enButtonRB1)) {
		//����X�e�[�g
		//m_knightState = enKnightState_Avoidance;
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();
		AnimationMove(AvoidanceSpeed, stickL);
		pushFlag = true;
		AvoidanceFlag = true;
	}
}

/// <summary>
/// �K�E�Z�̓����蔻�萶������
/// </summary>
void KnightPlayer::MakeUltSkill()
{
	KnightUlt* knightUlt = NewGO<KnightUlt>(0,"knightUlt");
	//����҂̖��O������
	knightUlt->SetCreatorName(GetName());

	Vector3 UltPos = m_position;
	UltPos.y += 60.0f;
	knightUlt->SetPosition(UltPos);
	knightUlt->SetRotation(m_rot);
	knightUlt->SetEnUlt(KnightUlt::enUltSkill_Player);
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
		//���P�i�ډ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(13);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"SecondAttack_Start") == 0)
	{
		m_AtkTmingState = SecondAtkStart_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
		//���Q�i�ډ�
		SoundSource* se = NewGO<SoundSource>(0); 
		se->Init(14);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//�O�i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"LastAttack_Start") == 0)
	{
		m_AtkTmingState = LastAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;
		//���R�i�ډ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(15);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//�X�L���̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"SkillAttack_Start") == 0)
	{
		m_Status.Atk += 20;
		//m_AtkTmingState = LastAtk_State;
		//���̃R���W�����𐶐�
		AtkCollistionFlag = true;

		//�X�L�����𔭐�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(11);
		se->Play(false);
		se->SetVolume(0.3f);
	}
	//�K�E�Z�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"UltimateAttack_Start") == 0)
	{
		
		//�K�E�Z�̓����蔻��̃N���X���쐬
		MakeUltSkill();
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
			//�{�^���v�b�V���t���O��false�ɂ���
			pushFlag = false;
			AtkState = false;
			m_knightState = enKnightState_Idle;
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
			//�{�^���v�b�V���t���O��false�ɂ���
			pushFlag = false;
			AtkState = false;
			m_knightState = enKnightState_Idle;
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
		m_knightState = enKnightState_Idle;
		AtkState = false;
		//�{�^���v�b�V���t���O��false�ɂ���
		pushFlag = false;
	}

	//�X�L���̃A�j���[�V�����Ō���U��I�������
	if (wcscmp(eventName, L"SkillAttack_End") == 0)
	{
		m_Status.Atk -= 20;
		m_AtkTmingState = Num_State;
		AtkState = false;
		//�X�L���̈ړ����������Ȃ��悤�ɂ���
		SkillState = false;
		m_Status.Speed -= 120.0f;
		//���̃R���W�����𐶐����Ȃ�
		AtkCollistionFlag = false;
	}
	//����A�j���[�V�������I�������
	if (wcscmp(eventName, L"Avoidance_End") == 0)
	{
		//�ړ����������Ȃ��悤�ɂ���

		AvoidanceFlag = false;
		//m_AtkTmingState = Num_State;
	
	}
}

void KnightPlayer::AvoidanceSprite()
{
	Vector3 AvoidanceScale = Vector3::One;
	//HP�o�[�̌����Ă��������B
	AvoidanceScale.x = (float)AvoidanceTimer / (float)AvoidanceCoolTime;
	m_Avoidance_barRender.SetScale(AvoidanceScale);

	m_Avoidance_flameRender.Update();
	m_Avoidance_barRender.Update();
}

void KnightPlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//�X�L���̃N�[���^�C���ƃ^�C�}�[���Ⴄ�������\��
	if(SkillTimer!=Cooltime)
	Skillfont.Draw(rc);
	//����̃N�[���^�C���ƃ^�C�}�[���Ⴄ�������\��
	if (AvoidanceTimer != AvoidanceCoolTime)
	{
		m_Avoidance_flameRender.Draw(rc);
		m_Avoidance_barRender.Draw(rc);
	}
	
}
