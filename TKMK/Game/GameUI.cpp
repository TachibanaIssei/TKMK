#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Actor.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"

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

	const Vector3 LV_NUBER_POS = Vector3(770.0f, -360.0f, 0.0f);
	const Vector3 LvPos = Vector3(640.0f, -310.0f, 0.0f);       //Lv
	const Vector3 MaxLvPos = Vector3(920.0f, -400.0f, 0.0f);       // /10

	const Vector3 FLAME_POS = Vector3(920.0f,-480.0f,0.0f);    //���x����o���l�̃t���[��

	const float DownPointPosY = 100.0f;

	const Vector3 EXPERIENCE_POS = Vector3(750.0f, -500.0f, 0.0f);  //�|�C���g
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


	//Level
	m_LevelFont.SetPosition(LV_NUBER_POS);
	m_LevelFont.SetScale(2.0f);
	m_LevelFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LevelFont.SetRotation(0.0f);
	m_LevelFont.SetShadowParam(true, 2.0f, g_vec4Black);

	for (int num = 0; num < Characters; num++)
	{
		m_PointFont[num].SetPosition(PointPos);
		m_PointFont[num].SetScale(1.5f);
		m_PointFont[num].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_PointFont[num].SetRotation(0.0f);
		m_PointFont[num].SetShadowParam(true, 2.0f, g_vec4Black);

		PointPos.y -= DownPointPosY;
	}
	

	//�J�E���g�_�E��
	m_CountDownFont.SetPosition(Vector3::Zero);
	m_CountDownFont.SetScale(8.0f);
	m_CountDownFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_CountDownFont.SetRotation(0.0f);
	m_CountDownFont.SetShadowParam(true, 2.0f, g_vec4Black);

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

		//�o���l�o�[�̕\�s�{�b�g�ɂ���todo
		m_ExperienceBar_flont.Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", 600.0f, 120.0f);
		m_ExperienceBar_flont.SetPosition(Vector3::Zero);
		m_ExperienceBar_flont.SetPivot(EXPERIENCEGAUGE_PIVOT);
		m_ExperienceBar_flont.SetScale(0.5, 0.5, 1.0);

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
		m_SkillRender.Init("Assets/sprite/skill_flame.DDS", 162, 162);
		m_SkillRender.SetPosition(Skill_Pos);
		m_SkillRender.SetScale(1.1, 1.1);
		//�K�E�Z�̃A�C�R��
		m_UltRender.Init("Assets/sprite/ult_flame.DDs", 162, 162);
		m_UltRender.SetPosition(Ult_Pos);
		m_UltRender.SetScale(1.2, 1.2);

		m_Lv.Update();
		m_LvNumber.Update();
		m_MaxLv.Update();
		m_Flame.Update();
		m_ExperienceFlame.Update();
		m_ExperienceBar_flont.Update();
		m_SkillRender.Update();
		m_UltRender.Update();
	}

	//HP�֘A
	{
		//HP�̃t�H���g
		m_HpFont.SetPosition(-540.0f, -465.0f, 0.0f);
		m_HpFont.SetScale(1.0f);
		m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_HpFont.SetRotation(0.0f);
		m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//HP�Q�[�W���̉摜��ǂݍ���
		m_statusBar.Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", 600.0f, 120.0f);
		m_statusBar.SetPosition(HP_BAR_POS);
		m_statusBar.SetScale(1.0, 1.0, 1.0);

		//HP�Q�[�W�̕\�̉摜��ǂݍ���
		m_hpBar.Init("Assets/sprite/gameUI/HPBar_HP.DDS", 580.0f, 90.0f);
		//�s�{�b�g��ݒ肷��
		m_hpBar.SetPivot(HPGAUGE_PIVOT);
		m_hpBar.SetPosition(HP_BAR_FLONT_POS);

		//HP�Q�[�W�̃t���[���̉摜��ǂݍ���
		m_HPFrame.Init("Assets/sprite/gameUI/HPBar_flame.DDS", 600.0f, 120.0f);
		m_HPFrame.SetPosition(HP_BAR_POS);
		m_HPFrame.SetScale(1.0, 1.0, 1.0);

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

	m_GameUIState = m_GameState;

	return true;
}

void GameUI::Update()
{
	//game�N���X�̃|�[�Y�̃t���O�������Ă���ԏ������s��Ȃ�
	if (m_GameUIState== m_PauseState) {
		return;
	}

	if (m_game->NowGameState() == 0)
	{
		CountDown();
	}
	



	//���x���̕\��
	//int LEVEL=m_knightplayer->SetLevel();
	int LEVEL = player->CharSetLevel();
	wchar_t Lv[255];
	swprintf_s(Lv, 255, L"%d", LEVEL);
	m_LevelFont.SetText(Lv);

	CharPoint();
	
	//�������Ԃ̕\��
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d:%02d", int(m_game->GetMinutesTimer()),int(m_game->GetSecondsTimer()));
	//�\������e�L�X�g��ݒ�B
	m_time_left.SetText(wcsbuf);
	
	EXPBar();

	HPBar();
	
	//����o�[
	
}

void GameUI::CountDown()
{
	//�J�E���g�_�E��
	int COUNTDOWNTIMER = m_game->CountDownMinutes();
	wchar_t CDT[255];
	if (m_game->CountDownMinutes() <= 0)
	{
		swprintf_s(CDT, 255, L"Fight!");
	}
	else
	{
		swprintf_s(CDT, 255, L"%d", COUNTDOWNTIMER);
	}

	m_CountDownFont.SetText(CDT);
}

void GameUI::HPBar()
{
	int HP = player->CharSetHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d", HP);
	m_HpFont.SetText(hp);

	Vector3 HpScale = Vector3::One;
	//HP�o�[�̌����Ă��������B
	HpScale.x = (float)player->CharSetHp() / (float)player->CharSetMaxHp();
	m_hpBar.SetScale(HpScale);

	m_hpBar.Update();
}

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

void GameUI::EXPBar()
{
	//�o���l�̕\��
	Vector3 EXPScale = Vector3::One;

	nowEXP = player->CharSetEXP();

	nowEXPTable = player->CharSetEXPTable();

	
	//HP�o�[�̌����Ă��������B
	EXPScale.x = (float)nowEXP / (float)nowEXPTable;
	m_ExperienceBar_flont.SetScale(EXPScale);
	m_ExperienceBar_flont.Update();
}

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
		swprintf_s(P, 255, L"%dP", POINT);
		m_PointFont[num].SetText(P);

		num++;
	}

	//�N�ɉ����}�[�N���邩���߂�

}

void GameUI::Render(RenderContext& rc)
{
	//game�N���X�̃|�[�Y�̃t���O�������Ă���ԏ������s��Ȃ�
	if (m_GameUIState != m_PauseState) {
		//���x����o���l�̃t���[��
		m_Flame.Draw(rc);
		//�o���l
		m_ExperienceFlame.Draw(rc);
		m_ExperienceBar_flont.Draw(rc);

	//m_LevelNameFont.Draw(rc);
		m_HpNameFont.Draw(rc);

		m_TimeAndPointRender.Draw(rc);

		m_time_left.Draw(rc);

		/*m_AtkFont.Draw(rc);
		m_SpeedFont.Draw(rc);*/
		m_statusBar.Draw(rc);
		m_hpBar.Draw(rc);
		//m_playerFaceBack.Draw(rc);
		m_HPFrame.Draw(rc);
		m_SkillRender.Draw(rc);
		m_UltRender.Draw(rc);

		m_HpFont.Draw(rc);
		m_Lv.Draw(rc);
		m_LvNumber.Draw(rc);
		m_MaxLv.Draw(rc);
		m_LevelFont.Draw(rc);

		//�|�C���g��`��
		for (int num = 0; num < Characters; num++) {
			m_PointFont[num].Draw(rc);
		}
		

		if (m_game->NowGameState() == 0) {
			m_CountDownFont.Draw(rc);
		}
		
	}
	
}
