#include "stdafx.h"
#include "GameUI.h"

#include "KnightPlayer.h"
namespace
{
	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//�Q�[�W�̃s�{�b�g
	const Vector2 HPGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//HP�Q�[�W�̃s�{�b�g
	const Vector3 STATUS_BAR_POS = Vector3(-400.0f, -420.0f, 0.0f);	//�X�e�[�^�X�o�[�|�W�V����
	const Vector3 TIME_AND_POINT = Vector3(-550.0,-350.0f, 0.0f);	//�v���C���[�̊�̘g�̃|�W�V����
	const Vector3 HP_BAR_POS = Vector3(-400.0f, -420.0f, 0.0f);	//HP�o�[�|�W�V����

	const float HP_BAR_WIDTH = 1400.0f;     //HP�o�[�̒���
	const float HP_BAR_HIGHT = 200.0f;      //HP�o�[�̍���
	const Vector3 HP_BAR_SIZE = Vector3(HP_BAR_WIDTH, HP_BAR_HIGHT, 0.0f);  //HP�o�[�̃T�C�Y

	const Vector3 Skill_Pos = Vector3(700.0f, -225.0f, 0.0f);   //�X�L���A�C�R���|�W�V����
	const Vector3 Ult_Pos = Vector3(700.0f, -370.0f, 0.0f);     //�K�E�Z�A�C�R���|�W�V����


}
GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_knightplayer = FindGO<KnightPlayer>("m_knightplayer");

	//Level
	m_LevelFont.SetPosition(-850.0f, -460.0f, 0.0f);
	m_LevelFont.SetScale(2.0f);
	m_LevelFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LevelFont.SetRotation(0.0f);
	m_LevelFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_LevelNameFont.SetText(L"Lv");
	m_LevelNameFont.SetPosition(-950.0f, -460.0f,0.0f);
	m_LevelNameFont.SetScale(2.0f);
	m_LevelNameFont.SetRotation(0.0f);
	m_LevelNameFont.SetShadowParam(true, 2.0f, g_vec4Black);

	//HP
	/*m_HpFont.SetPosition(-800.0f, -280.0f, 0.0f);
	m_HpFont.SetScale(2.0f);
	m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_HpFont.SetRotation(0.0f);
	m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_HpNameFont.SetText(L"HP");
	m_HpNameFont.SetPosition(-900.0f, -280.0f, 0.0f);
	m_HpNameFont.SetScale(2.0f);
	m_HpNameFont.SetRotation(0.0f);
	m_HpNameFont.SetShadowParam(true, 2.0f, g_vec4Black);*/


	//HP�Q�[�W���̉摜��ǂݍ���
	m_statusBar.Init("Assets/sprite/HP_bar_ura.DDS", 600.0f, 85.0f);
	m_statusBar.SetPosition(HP_BAR_POS);
	m_statusBar.SetScale(1.0,1.0,1.0);

	//HP�Q�[�W�̃t���[���̉摜��ǂݍ���
	m_playerFaceFrame.Init("Assets/sprite/HP_flame.DDS", 600.0f, 85.0f);
	m_playerFaceFrame.SetPosition(HP_BAR_POS);
	m_playerFaceFrame.SetScale(1.0, 1.0, 1.0);

	////�������ԂƊl���|�C���g
	m_TimeAndPointRender.Init("Assets/sprite/time_point.DDS", 600.0f, 140.0f);
	m_TimeAndPointRender.SetPosition(TIME_AND_POINT);
	m_TimeAndPointRender.SetScale(0.8,0.7,1.0);

	//�X�L���̃A�C�R��
	m_SkillRender.Init("Assets/sprite/skill_flame.DDS", 162, 162);
	m_SkillRender.SetPosition(Skill_Pos);
	m_SkillRender.SetScale(1, 1);
	//�K�E�Z�̃A�C�R��
	m_UltRender.Init("Assets/sprite/ult_flame.DDs", 162, 162);
	m_UltRender.SetPosition(Ult_Pos);
	m_UltRender.SetScale(1, 1);

	//HP�Q�[�W�\�̉摜�ǂݍ���
	m_hpBar.Init("Assets/sprite/HP_bar.DDS", 600.0f, 85.0f);
	//�s�{�b�g��ݒ肷��
	m_hpBar.SetPivot(HPGAUGE_PIVOT);
	m_hpBar.SetPosition({ -700.0f, -420.0f, 0.0f });
	m_hpBar.SetScale(0.4f, 0.4f, 1.0f);
	//�X�V����
	m_statusBar.Update();
	m_hpBar.Update();
	//m_hp = PLAYER_MAXHP;
	m_playerFaceFrame.Update();
	m_TimeAndPointRender.Update();
	m_SkillRender.Update();
	m_UltRender.Update();

	return true;
}

void GameUI::Update()
{
	int LEVEL=m_knightplayer->SetLevel();
	wchar_t Lv[255];
	swprintf_s(Lv, 255, L"%d", LEVEL);
	m_LevelFont.SetText(Lv);

	//0���łȂ��Ȃ�
	if (MinutesTimer != 0) {
		SecondsTimer -= g_gameTime->GetFrameDeltaTime();
		//0�b�ȉ��Ȃ�
		if (SecondsTimer <= 0) {
			//1�����炷
			MinutesTimer--;
			//60�b�ɖ߂�
			SecondsTimer = 60.0f;
		}
	}
	//�������Ԃ̕\��
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d:%d", int(MinutesTimer),int(SecondsTimer));
	
	//�\������e�L�X�g��ݒ�B
	m_time_left.SetText(wcsbuf);
	//�t�H���g�̐ݒ�B
	m_time_left.SetPosition(Vector3(-880.0f, -390.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_time_left.SetScale(1.5f);
	//�t�H���g�̐F��ݒ�B
	m_time_left.SetColor({ 1.0f,1.0f,1.0f,1.0f });
	m_time_left.SetShadowParam(true, 2.0f, g_vec4Black);


	HPBar();
	
	//����o�[
	
}

void GameUI::HPBar()
{
	Vector3 HpScale = Vector3::One;
	//HP�o�[�̌����Ă��������B
	HpScale.x = (float)m_knightplayer->SetHp() / (float)m_knightplayer->SetMaxHp();
	m_hpBar.SetScale(HpScale);



	//m_HPBerPos.x = -0.0f;
	////HP�o�[�摜�����񂹂ɕ\������
	//Vector3 BerSizeSubtraction = HPBerSend(HP_BAR_SIZE, HpScale);	//�摜�̌��̑傫��
	//m_HPBerPos.x -= BerSizeSubtraction.x;

	//m_hpBar.SetPosition(Vector3(m_HPBerPos.x, HP_BAR_POS.y, 0.0f));
	
	m_hpBar.Update();
}
void GameUI::Render(RenderContext& rc)
{
	m_LevelFont.Draw(rc);
	m_LevelNameFont.Draw(rc);
	m_HpNameFont.Draw(rc);

	m_TimeAndPointRender.Draw(rc);

	m_time_left.Draw(rc);

	/*m_AtkFont.Draw(rc);
	m_SpeedFont.Draw(rc);*/
	m_statusBar.Draw(rc);
	m_hpBar.Draw(rc);
	//m_playerFaceBack.Draw(rc);
	m_playerFaceFrame.Draw(rc);
	m_SkillRender.Draw(rc);
	m_UltRender.Draw(rc);
}
