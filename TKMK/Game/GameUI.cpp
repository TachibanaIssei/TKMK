#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Actor.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Fade.h"

namespace
{
	const int Characters = 4;

	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//�Q�[�W�̃s�{�b�g
	const Vector2 HPGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//HP�Q�[�W�̃s�{�b�g
	const Vector2 EXPERIENCEGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//�o���l�Q�[�W�̃s�{�b�g

	const Vector3 STATUS_BAR_POS = Vector3(-450.0f, -500.0f, 0.0f);	//�X�e�[�^�X�o�[�|�W�V����
	const Vector3 TIME_POS = Vector3(0.0,470.0f, 0.0f);	//�������Ԃ̍��W
	const Vector3 TIME_FONT_POS = Vector3(-80.0, 500.0f, 0.0f);	//�������Ԃ̍��W

	const Vector3 HP_BAR_POS = Vector3(-670.0f, -480.0f, 0.0f);	//HP�o�[�|�W�V����
	const Vector3 HP_BAR_FLONT_POS = Vector3(-960.0f, -480.0f, 0.0f);	//HP�o�[�̕\�̃|�W�V����

	const float HP_BAR_WIDTH = 1400.0f;     //HP�o�[�̒���
	const float HP_BAR_HIGHT = 200.0f;      //HP�o�[�̍���
	const Vector3 HP_BAR_SIZE = Vector3(HP_BAR_WIDTH, HP_BAR_HIGHT, 0.0f);  //HP�o�[�̃T�C�Y

	const Vector3 Skill_Pos = Vector3(520.0f, -280.0f, 0.0f);   //�X�L���A�C�R���|�W�V����
	const Vector3 Ult_Pos = Vector3(470.0f, -445.0f, 0.0f);     //�K�E�Z�A�C�R���|�W�V����

	const Vector3 LV_NUBER_POS = Vector3(780.0f, -360.0f, 0.0f);
	const Vector3 LvPos = Vector3(640.0f, -310.0f, 0.0f);       //Lv
	const Vector3 MaxLvPos = Vector3(920.0f, -400.0f, 0.0f);       // /10

	const Vector3 FLAME_POS = Vector3(920.0f,-480.0f,0.0f);    //���x����o���l�̃t���[��

	const float DownPointPosY = 100.0f;

	const Vector3 EXPERIENCE_POS = Vector3(750.0f, -500.0f, 0.0f);  //�o���l�e�[�u��

	const float EXPBAR_WIDTH = 300.0f;
	const float EXPBAR_HEIGHT = 70.0f;
	const Vector3 EXPERIENCE_BAR_POS = Vector3(750.0f, -500.0f, 0.0f);	//�o���l�o�[�̍��W
	const Vector3 EXPBAR_SIZE = Vector3(EXPBAR_WIDTH, EXPBAR_HEIGHT, 0.0f);	//�o���l�o�[�̃T�C�Y

	const Vector3 UPTOLEVEL_POS = Vector3(820.0f, -480.0f, 0.0f);		//���x���A�b�v�܂łɕK�v�Ȍo���l�̗�

	const Vector3 RESPWANCOUNT_POS = Vector3(0.0f, 0.0f, 0.0f);		//���X�|�[��������̃J�E���g

	const Vector3 SmallScale = Vector3(0.1f, 0.1f, 0.0f);

	const Vector3 FightSmallScale = Vector3(0.5f, 0.5f, 0.0f);

	const Vector3 RespawnInPos = Vector3(0.0f, 300.0f, 0.0f);		//Respawn in�̍��W

	const Vector3 RespawnCountPos = Vector3(0.0f, -200.0f, 0.0f);		//�̍��W

	const Vector3 ADDPOINTPOS = Vector3(20.0f, 10.0f, 0.0f);
}
GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	player = FindGO<Player>("player");
	fade = FindGO<Fade>("fade");
	//�L�����̃A�C�R��
	//�u���[
	m_CharIcon[0].Init("Assets/sprite/gameUI/Knight_Blue.DDS", 80.0f, 80.0f);
	//���b�h
	m_CharIcon[1].Init("Assets/sprite/gameUI/Knight_Red.DDS", 80.0f, 80.0f);
	//�O���[��
	m_CharIcon[2].Init("Assets/sprite/gameUI/Knight_Green.DDS", 80.0f, 80.0f);
	//�C�G���[
	m_CharIcon[3].Init("Assets/sprite/gameUI/Knight_Yellow.DDS", 80.0f, 80.0f);

	//�|�C���g�֘A
	{
		//�L�����̃|�C���g��\��
		m_Actors = m_game->GetActors();
		int num = 0;
		for (auto actor: m_Actors)
		{
			//�|�C���g��\��
			m_PointFont[num].SetPosition(PointPos[num]);
			m_PointFont[num].SetScale(1.3f);
			m_PointFont[num].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_PointFont[num].SetRotation(0.0f);
			m_PointFont[num].SetShadowParam(true, 2.0f, g_vec4Black);

			//�v���C���[�����m�Ȃ�
			if (actor->IsMatchName(knightname))
			{
				//�A�C�R�������m�ɂ���(�u���[)
				
				m_CharIcon[num].SetPosition(CharIconPos[num]);
				//�t���[�����v���C���[�p�ɂ���
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame_player.DDS", 300.0f, 100.0f);
			}
			//�v���C���[�����@�g���Ȃ�
			else if (actor->IsMatchName(wizardname))
			{
				//�A�C�R���𖂖@�g���ɂ���
				
				//�t���[�����v���C���[�p�ɂ���
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame_player.DDS", 300.0f, 100.0f);
			}
			//����ȊO(AI)�Ȃ�
			else
			{
				m_CharIcon[num].SetPosition(CharIconPos[num]);
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}
			
			m_PointFlame[num].SetPosition(PointFlamePos[num]);
			m_PointFlame[num].SetScale(1.0f, 1.0f, 1.0f);
			m_PointFlame[num].Update();
			//
			m_CharIcon[num].Update();

			num++;
		}

	}
	
	//���X�|�[���֘A
	{
		//Respawn in�̉摜
		m_RespawnIn.Init("Assets/sprite/gameUI/RespawnIn.DDS", 900.0f, 200.0f);
		m_RespawnIn.SetPosition(RespawnInPos);
		m_RespawnIn.SetScale(Vector3::One);

		//���X�|�[���̔w�i�̉摜
		m_Respawn_Back.Init("Assets/sprite/gameUI/Respawn_back.DDS", 1920, 1080.0f);
		m_Respawn_Back.SetPosition(Vector3::Zero);
		m_Respawn_Back.SetScale(Vector3::One);

		//���X�|�[���̃J�E���g�_�E���̉摜
		m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
		m_RespawnCountNumber.SetPosition(RespawnCountPos);
		m_RespawnCountNumber.SetScale(Vector3::One);

		m_RespawnIn.Update();
		m_Respawn_Back.Update();
		m_RespawnCountNumber.Update();
	}

	//�X�^�[�g�܂ł̃J�E���g�_�E��
	m_CountNumper.Init("Assets/sprite/gameUI/count3.DDS", 1920.0f, 1080.0f);
	m_CountNumper.SetPosition(Vector3::Zero);
	m_CountNumper.SetScale(m_gameCountScale);
	m_CountNumper.Update();

	//�E���̃t���[��
	{
		//���x����o���l�̃t���[��
		m_Flame.Init("Assets/sprite/gameUI/LevelBar.DDS", 1200.0f, 500.0f);
		m_Flame.SetPosition(FLAME_POS);
		m_Flame.SetScale(1.0, 1.0, 1.0);
		
		//�o���l�̃t���[��
		m_ExperienceFlame.Init("Assets/sprite/gameUI/ExperienceBar.DDS", 600.0f, 120.0f);
		m_ExperienceFlame.SetPosition(EXPERIENCE_POS);
		m_ExperienceFlame.SetScale(0.5, 0.5, 1.0);
		
		//�o���l�o�[�̕\�s�{�b�g�ɂ���
		m_EXPBerPos = EXPERIENCE_BAR_POS;
		m_ExperienceBar_flont.Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_WIDTH, EXPBAR_HEIGHT);
		m_ExperienceBar_flont.SetPosition(m_EXPBerPos);
		//m_ExperienceBar_flont.SetPivot(EXPERIENCEGAUGE_PIVOT);
		m_ExperienceBar_flont.SetScale(0.5, 50.0, 1.0);

		//�o���l�o�[�̗�
		m_ExperienceBar_back.Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", 600.0f, 120.0f);
		m_ExperienceBar_back.SetPosition(EXPERIENCE_POS);
		m_ExperienceBar_back.SetScale(0.5, 0.5, 1.0);

		//���x���A�b�v�܂łɕK�v�Ȍo���l�̗�
		m_ExpFont.SetPosition(UPTOLEVEL_POS);
		m_ExpFont.SetScale(1.0f);
		m_ExpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_ExpFont.SetRotation(0.0f);
		m_ExpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//Lv�̉摜��ǂݍ���
		m_Lv.Init("Assets/sprite/gameUI/Lv.DDS", 196.0f, 150.0f);
		m_Lv.SetPosition(LvPos);
		m_Lv.SetScale(0.4, 0.4, 1.0);

		//Lv1�̉摜��ǂݍ���
		m_LvNumber.Init("Assets/sprite/gameUI/Lv1.DDS", 150.0f, 150.0f);
		m_LvNumber.SetPosition(LV_NUBER_POS);
		m_LvNumber.SetScale(1.4, 1.4, 1.0);

		// /10�̉摜��ǂݍ���
		m_MaxLv.Init("Assets/sprite/gameUI/maxLv.DDS", 196.0f, 150.0f);
		m_MaxLv.SetPosition(MaxLvPos);
		m_MaxLv.SetScale(0.5, 0.5, 1.0);

		//�X�L���̃A�C�R��
		m_SkillRender.Init("Assets/sprite/gameUI/skil_Icon.DDS", 162, 162);
		m_SkillRender.SetPosition(Skill_Pos);
		m_SkillRender.SetScale(1.1, 1.1);
		//�K�E�Z�̃A�C�R��
		m_UltRender.Init("Assets/sprite/gameUI/Ult_Icon.DDs", 162, 162);
		m_UltRender.SetPosition(Ult_Pos);
		m_UltRender.SetScale(1.2, 1.2);

		m_Lv.Update();
		m_LvNumber.Update();
		m_MaxLv.Update();
		m_Flame.Update();
		m_ExperienceFlame.Update();
		m_ExperienceBar_flont.Update();
		m_ExperienceBar_back.Update();
		m_SkillRender.Update();
		m_UltRender.Update();
	}

	//HP�֘A
	{
		//HP�̃t�H���g
		m_HpFont.SetPosition(-650.0f, -465.0f, 0.0f);
		m_HpFont.SetScale(1.0f);
		m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_HpFont.SetRotation(0.0f);
		m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//HP�Q�[�W���̉摜��ǂݍ���
		m_statusBar.Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", 600.0f, 120.0f);
		m_statusBar.SetPosition(HP_BAR_POS);
		m_statusBar.SetScale(1.0, 0.7, 1.0);

		//HP�Q�[�W�̕\�̉摜��ǂݍ���
		m_hpBar.Init("Assets/sprite/gameUI/HPBar_HP.DDS", 580.0f, 80.0f);
		//�s�{�b�g��ݒ肷��
		m_hpBar.SetPivot(HPGAUGE_PIVOT);
		m_hpBar.SetPosition(HP_BAR_FLONT_POS);

		//HP�Q�[�W�̃t���[���̉摜��ǂݍ���
		m_HPFrame.Init("Assets/sprite/gameUI/HPBar_flame.DDS", 600.0f, 120.0f);
		m_HPFrame.SetPosition(HP_BAR_POS);
		m_HPFrame.SetScale(1.0, 0.7, 1.0);

		//�X�V����
		m_statusBar.Update();
		m_HPFrame.Update();
		m_hpBar.Update();
	}

	//�������ԂƊl���|�C���g
	{
		m_TimeAndPointRender.Init("Assets/sprite/gameUI/timer.DDS", 1100.0f, 400.0f);
		m_TimeAndPointRender.SetPosition(TIME_POS);
		m_TimeAndPointRender.SetScale(0.35, 0.3, 0.3);

		//�t�H���g�̐ݒ�B
		m_time_left.SetPosition(TIME_FONT_POS);
		//�t�H���g�̑傫����ݒ�B
		m_time_left.SetScale(1.6f);
		//�t�H���g�̐F��ݒ�B
		m_time_left.SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_time_left.SetShadowParam(true, 2.0f, g_vec4Black);

		m_TimeAndPointRender.Update();
	}

	m_GameUIState = m_GameStartState;

	return true;
}

void GameUI::Update()
{
	//game�N���X�̃|�[�Y�̃t���O�������Ă���ԏ������s��Ȃ�
	if (m_GameUIState== m_PauseState) {
		return;
	}

	//�Q�[���̃X�e�[�g��gameStart�Ȃ�
	if (m_game->NowGameState() == 0)
	{
		CountDown();
	}
	
	//�v���C���[�����X�|�[���ҋ@���Ȃ�
	if (player->CharGetRespawnTime() > 0)
	{
		RespawnCountDown();
	}
	
	//game�N���X�̃X�^�[�g�̃t���O�������Ă���ԏ������s��Ȃ�
	if (m_GameUIState == m_GameStartState) {
		return;
	}

	CharPoint();
	
	//�������Ԃ̕\��
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d:%02d", int(m_game->GetMinutesTimer()),int(m_game->GetSecondsTimer()));
	//�\������e�L�X�g��ݒ�B
	m_time_left.SetText(wcsbuf);
	
	EXPBar();

	HPBar();
}

//�Q�[���X�^�[�g�̃J�E���g�_�E��
void GameUI::CountDown()
{
	//�J�E���g�_�E��
	//int COUNTDOWNTIMER = m_game->CountDownMinutes();
	int StartCountDown= m_game->CountDownMinutes();

	if(m_game->CountDownMinutes() <= 0)
	{
		m_CountNumper.Init("Assets/sprite/gameUI/fight!.DDS", 1920.0f, 1080.0f);
		//m_gameCountScale = Vector3(5.0f, 5.0f, 0.0f);
		m_Color = 1.0f;
		m_fightFlag = true;
	}

	if (oldtStartCount != StartCountDown)
	{
		switch (StartCountDown)
		{
		case 1:
			m_CountNumper.Init("Assets/sprite/gameUI/count1.DDS", 1920.0f, 1080.0f);
			m_gameCountScale = Vector3(0.2f, 0.2f, 0.0f);
			m_Color = 1.0f;
			break;
		case 2:
			m_CountNumper.Init("Assets/sprite/gameUI/count2.DDS", 1920.0f, 1080.0f);
			m_gameCountScale = Vector3(0.2f, 0.2f, 0.0f);
			m_Color = 1.0f;
			break;
		default:
			break;
		}
	}
	//�摜��Fight!�łȂ����X�P�[����100�ȉ��Ȃ�
	else if(m_fightFlag == false&&m_gameCountScale.x<100.0f)
	{
		//���X�ɕ�����傫������
		m_gameCountScale += SmallScale;
		//�����������ɂ���
		m_Color -= 0.02f;
		
		m_CountNumper.SetScale(m_gameCountScale);
		//
		m_CountNumper.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_Color));
	}

	//�摜��Fight!�Ȃ�
	if(m_fightFlag==true)
	{
		if (m_FightScale.x < m_gameCountScale.x)
		{
			//���X�ɕ���������������
			m_gameCountScale -= FightSmallScale;
		}
		
		m_CountNumper.SetScale(m_gameCountScale);
		//
		m_CountNumper.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_Color));
	}

	
	m_CountNumper.Update();

	oldtStartCount = StartCountDown;
}

//���X�|�[������܂ł̃J�E���g�_�E��
void GameUI::RespawnCountDown()
{
	//�J�E���g�_�E��
	int RESPAWNCOUNTDOWN = player->CharGetRespawnTime();

	if (oldRespawnCount != RESPAWNCOUNTDOWN)
	{
		switch (RESPAWNCOUNTDOWN)
		{
		case 0:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut0.DDS", 300, 500.0f);
			//��ʂ��Â����Ă䂭
			fade->StartFadeIn(2.0f);
			break;
		case 1:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut1.DDS", 300, 500.0f);
			break;
		case 2:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
			break;
		default:
			break;
		}
	}

	oldRespawnCount = RESPAWNCOUNTDOWN;

	m_RespawnCountNumber.Update();
}

//�v���C���[��HP�̕\���̏���
void GameUI::HPBar()
{
	int HP = player->CharSetHp();
	int MaxHP = player->CharSetMaxHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d/%d", HP, MaxHP);
	m_HpFont.SetText(hp);

	Vector3 HpScale = Vector3::One;
	//HP�o�[�̌����Ă��������B
	HpScale.x = (float)player->CharSetHp() / (float)player->CharSetMaxHp();
	m_hpBar.SetScale(HpScale);

	m_hpBar.Update();
}

//�������Ԃ̕\���̏���
void GameUI::Timer()
{
		//0�b�ȉ��Ȃ�
		if (SecondsTimer <= 0) {
			//1�����炷
			MinutesTimer--;
			//����0���Ȃ�A�b��0�ɂ���
			if (MinutesTimer < 0) {
				SecondsTimer = 0.0f;
				MinutesTimer = 0.0f;
				GameEndFlag = true;
			}
			//60�b�ɖ߂�
			else SecondsTimer = 60.0f;
		}
		else SecondsTimer -= g_gameTime->GetFrameDeltaTime();
	
}

//�v���C���[�̌o���l�̕\���̏���todo
void GameUI::EXPBar()
{
	//�o���l�̕\��
	Vector3 EXPScale = Vector3::One;
	//�v���C���[�̌o���l���擾
	float nowEXP = player->CharSetEXP();
	//���̌o���l�e�[�u�����擾
	float nowEXPTable = player->CharSetEXPTable();
	//�O�̃��x���̌o���l�e�[�u�����擾
	float oldEXPTable = player->CharSetOldEXPTable();

	//�ŏI�I�Ȍo���l�e�[�u��
	float finalEXPTable = nowEXPTable - oldEXPTable;
	//�ŏI�I�Ȍo���l
	float finalEXP = nowEXP - oldEXPTable;

	//HP�o�[�̑����Ă��������B
	EXPScale.x = (float)finalEXP / (float)finalEXPTable;
	m_ExperienceBar_flont.SetScale(EXPScale);

	//EXP�o�[�摜�����񂹂ɕ\������
	Vector3 BerSizeSubtraction = HPBerSend(EXPBAR_SIZE, EXPScale);	//�摜�̌��̑傫��

	if (finalEXPTable != oldEXPTable)
	{
		m_EXPBerPos.x = EXPERIENCE_BAR_POS.x;
	}

	//�o���l�̗ʂ��ς�����Ƃ�����
	if (finalEXP != oldEXP)
	{
		m_EXPBerPos.x -= BerSizeSubtraction.x;
	}
	
	
	m_ExperienceBar_flont.SetPosition(Vector3(m_EXPBerPos.x, m_EXPBerPos.y, 0.0f));
	//m_ExperienceBar_flont.SetScale(EXPScale);
	m_ExperienceBar_flont.Update();


	//���x���A�b�v�܂łɕK�v�Ȍo���l�̗�
	int UpToLevel = nowEXPTable - nowEXP;
	wchar_t UTL[255];
	swprintf_s(UTL, 255, L"%d", UpToLevel);
	m_ExpFont.SetText(UTL);

	oldEXP = finalEXP;
	oldEXPTable = finalEXPTable;
}

//
Vector3& GameUI::HPBerSend(Vector3 size, Vector3 scale)
{
	Vector3 expBerSize = size;								//�摜�̌��̑傫��
	Vector3 changeBerSize = Vector3::Zero;					//�摜���X�P�[���ϊ��������Ƃ̑傫��
	Vector3 BerSizeSubtraction = Vector3::Zero;				//�摜�̌��ƕϊ���̍�

	changeBerSize.x = expBerSize.x * scale.x;
	BerSizeSubtraction.x = expBerSize.x - changeBerSize.x;
	BerSizeSubtraction.x /= 2.0f;

	return BerSizeSubtraction;
}

//�L�����̃|�C���g�ƁB�|�C���g����ԑ����L�����ɉ����}�[�N������\���̏���
void GameUI::CharPoint()
{
	//�L�����̃|�C���g��\��
	m_Actors = m_game->GetActors();

	int num = 0;
	for (auto actor:m_Actors)
	{
		charPoint[num] = actor->GetPoint();

		//�|�C���g�̕\��
		int POINT = charPoint[num];
		wchar_t P[255];
		swprintf_s(P, 255, L"%dp", POINT);
		m_PointFont[num].SetText(P);


		if (MaxPoint <= charPoint[num])
		{
			m_PointFont[num].SetScale(1.8f);
			Vector3 FontPos;
			FontPos= ADDPOINTPOS + PointPos[num];
			m_PointFont[num].SetPosition(FontPos);

			m_PointFlame[num].SetScale(1.7f,1.3f,0.0f);
			m_PointFlame[num].Update();
			MaxPoint = charPoint[num];
		}
		else
		{
			m_PointFont[num].SetScale(1.3f);
			m_PointFont[num].SetPosition(PointPos[num]);
			m_PointFlame[num].SetScale(1.0f, 1.0f, 0.0f);
			m_PointFlame[num].Update();
		}


		num++;
	}

	
	//�N�ɉ����}�[�N���邩���߂�
}

void GameUI::Render(RenderContext& rc)
{
	//game�N���X�̃|�[�Y�̃t���O�������Ă���ԏ������s��Ȃ�
	if (m_GameUIState != m_PauseState && m_GameUIState != m_GameStartState) {
		//���x����o���l�̃t���[��
		m_Flame.Draw(rc);
		//�o���l�̗�
		m_ExperienceBar_back.Draw(rc);
		//�o���l�̕\ �ϓ�����
		m_ExperienceBar_flont.Draw(rc);
		//�o���l�t���[��
		m_ExperienceFlame.Draw(rc);
		//
		m_ExpFont.Draw(rc);
		
		m_HpNameFont.Draw(rc);

		m_TimeAndPointRender.Draw(rc);

		m_time_left.Draw(rc);

		m_statusBar.Draw(rc);
		m_hpBar.Draw(rc);
		
		m_HPFrame.Draw(rc);
		m_SkillRender.Draw(rc);
		m_UltRender.Draw(rc);

		m_HpFont.Draw(rc);
		m_Lv.Draw(rc);
		m_LvNumber.Draw(rc);
		m_MaxLv.Draw(rc);
		

		//�|�C���g��`��
		int num = 0;
		for (auto actor:m_Actors) {
			m_PointFlame[num].Draw(rc);
			m_PointFont[num].Draw(rc);
			m_CharIcon[num].Draw(rc);
			num++;
		}
		
		//���X�|�[������܂ł̎���
		if (player->CharGetRespawnTime() > 0)
		{
			m_Respawn_Back.Draw(rc);
			m_RespawnIn.Draw(rc);
			m_RespawnCountNumber.Draw(rc);
		}
	}
	else
	{
		if (m_game->NowGameState() == 0) {
			m_CountNumper.Draw(rc);
		}
	}
	
}
