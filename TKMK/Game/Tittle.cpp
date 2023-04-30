#include "stdafx.h"
#include "Tittle.h"
#include "CharacterSelect.h"
#include "Game.h"

Tittle::Tittle()
{

}

Tittle::~Tittle()
{

}

bool Tittle::Start()
{
	//�w�i�̏�����
	m_spriteRender.Init("Assets/sprite/Title/first_title/titleBack.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();
	
	//title�̃��S
	m_titleLogo.Init("Assets/sprite/Title/first_title/gameTitleLogo.DDS", 400.0f,200.0f);
	m_titleLogo.SetPosition(m_firstPosition);
	m_titleLogo.SetScale(m_titleLogoScale);
	m_titleLogo.Update();

	//�Ή�
	m_fire.Init("Assets/sprite/Title/first_title/fireFlower.DDS", 1500.0f, 400.0f);
	m_fire.SetPosition(m_firstPosition);
	m_fire.SetScale(g_vec3One);
	m_fire.Update();

	//Select�̏�����
	//"START"��I��
	m_start.Init("Assets/sprite/Title/ModeScene/START_white.DDS", 300.0f, 90.0f);
	m_start.SetPosition(m_firstPosition);
	m_start.SetScale(g_vec3One);
	m_start.Update();
	//"START"�I��
	m_startST.Init("Assets/sprite/Title/ModeScene/START_color.DDS", 300.0f, 90.0f);
	m_startST.SetPosition(m_firstPosition);
	m_startST.SetScale(g_vec3One);
	m_startST.Update();
	//"HOWTOPLAY"��I��
	m_operation.Init("Assets/sprite/Title/ModeScene/HOWTOPLAY_white.DDS", 500.0f, 90.0f);
	m_operation.SetPosition(m_firstPosition);
	m_operation.SetScale(g_vec3One);
	m_operation.Update();
	//"HOWTOPLAY"�I��
	m_operationST.Init("Assets/sprite/Title/ModeScene/HOWTOPLAY_color.DDS", 500.0f, 90.0f);
	m_operationST.SetPosition(m_firstPosition);
	m_operationST.SetMulColor(m_colorST);
	m_operationST.SetScale(g_vec3One);
	m_operationST.Update();
	//"OPTION"��I��
	m_option.Init("Assets/sprite/Title/ModeScene/OPTION_white.DDS", 370.0f, 90.0f);
	m_option.SetPosition(m_firstPosition);
	m_option.SetScale(g_vec3One);
	m_option.Update();
	//"OPTION"�I��
	m_optionST.Init("Assets/sprite/Title/ModeScene/OPTION_color.DDS", 370.0f, 90.0f);
	m_optionST.SetPosition(m_firstPosition);
	m_optionST.SetMulColor(m_colorST);
	m_optionST.SetScale(g_vec3One);
	m_optionST.Update();

	//��������摜
	m_operationPic.Init("Assets/sprite/Controller.DDS", 1920.0f, 1080.0f);
	m_operationPic.SetPosition(m_firstPosition);
	m_operationPic.SetScale(g_vec3One);
	m_operationPic.Update();

	//�L�����N�^�[�����摜
	//�Z���N�g�摜
	m_Opchoice.Init("Assets/sprite/CharaExplanation/CharaOpSelect.DDS", 1000.0f, 100.0f);
	m_Opchoice.SetPosition(m_firstPosition);
	m_Opchoice.SetScale(g_vec3One);
	m_Opchoice.Update();
	//���m
	m_KnightOp.Init("Assets/sprite/CharaExplanation/KnightOP.DDS", 1920.0f, 1080.0f);
	m_KnightOp.SetPosition(m_firstPosition);
	m_KnightOp.SetScale(g_vec3One);
	m_KnightOp.Update();
	//���@�g��
	m_WizardOp.Init("Assets/sprite/CharaExplanation/wizardOP.DDS", 1920.0f, 1080.0f);
	m_WizardOp.SetPosition(m_firstPosition);
	m_WizardOp.SetScale(g_vec3One);
	m_WizardOp.Update();

	//Select�̃C���X�g
	//"START"�̎��̃C���X�g
	m_iluststart.Init("Assets/sprite/Title/ModeScene/llust_Start.DDS", 500.0f, 600.0f);
	m_iluststart.SetPosition(m_RightfirstPosition);
	m_iluststart.SetScale(g_vec3One);
	m_iluststart.Update();
	//"START"�̎��̐�����
	m_iluststartOp.Init("Assets/sprite/Title/ModeScene/Start_Setumei.DDS", 850.0f, 100.0f);
	m_iluststartOp.SetPosition(m_RightfirstPosition);
	m_iluststartOp.SetScale(g_vec3One);
	m_iluststartOp.Update();
	//"HOWTOPLAY"�̎��̃C���X�g
	m_ilusthowtoplay.Init("Assets/sprite/Title/ModeScene/llust_HowToPlay.DDS", 500.0f, 500.0f);
	m_ilusthowtoplay.SetPosition(m_RightfirstPosition);
	m_ilusthowtoplay.SetScale(g_vec3One);
	m_ilusthowtoplay.Update();
	//"HOWTOPLAY"�̎��̐�����
	m_ilusthowtoplayOp.Init("Assets/sprite/Title/ModeScene/HowToPlay_Setumei.DDS", 700.0f, 100.0f);
	m_ilusthowtoplayOp.SetPosition(m_RightfirstPosition);
	m_ilusthowtoplayOp.SetScale(g_vec3One);
	m_ilusthowtoplayOp.Update();
	//"OPTION"�̎��̃C���X�g
	m_ilustoption.Init("Assets/sprite/Title/ModeScene/llust_Option.DDS", 500.0f, 500.0f);
	m_ilustoption.SetPosition(m_RightfirstPosition);
	m_ilustoption.SetScale(g_vec3One);
	m_ilustoption.Update();
	//"OPTION"�̎��̐�����
	m_ilustoptionOp.Init("Assets/sprite/Title/ModeScene/Option_Setumei.DDS", 550.0f, 100.0f);
	m_ilustoptionOp.SetPosition(m_RightfirstPosition);
	m_ilustoptionOp.SetScale(g_vec3One);
	m_ilustoptionOp.Update();

	//BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/gameBGM/TitleBGM1.wav");
	//�I����
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/sentaku/sentaku4.wav");

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(1);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.5f);

	return true;
}

void Tittle::Update()
{
	Scene();

	m_timer++;

	m_spriteRender.Update();
	m_titleLogo.Update();
	m_fire.Update();
	m_start.Update();
	m_operation.Update();
	m_option.Update();
	m_operationPic.Update();
	m_iluststart.Update();
	m_iluststartOp.Update();
	m_ilusthowtoplay.Update();
	m_ilusthowtoplayOp.Update();
	m_ilustoption.Update();
	m_ilustoptionOp.Update();
}

//�V�[���Z���N�g
void Tittle::Scene()
{
	//�����ŏ���PressA�̉�ʂ�������
	if (m_titleScene == enTitleScene_PressAScene)
	{
		//�^�C�g����ʂ̎��̏ꏊ�Ɉړ�����
		m_titleLogo.SetPosition(m_titleLogoPosition);
		m_titleLogo.SetScale(m_titleLogoScale);
		m_fire.SetPosition(m_titleLogoPosition);
		//��ʊO�̉��Ɉړ�����
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		m_option.SetPosition(m_firstPosition);
	}
	//����Select��ʂ�������
	if (m_titleScene == enTitleScene_Change)
	{
		if (LogoComplement < 1.0f)
		{
			if (m_timer % 2 == 0)
			{
				//���`�⊮
				m_operationPosition.Lerp(LogoComplement, m_firstPosition, m_Central);
				m_startPosition.Lerp(LogoComplement, m_firstPosition, m_Top);
				m_optionPosition.Lerp(LogoComplement, m_firstPosition, m_Under);
				m_LogoPosition.Lerp(LogoComplement, m_titleLogoPosition, m_selectLogoPosition);
				m_LogoScale.Lerp(LogoComplement, m_titleLogoScale, m_selectLogoScale);
				m_fireScale.Lerp(LogoComplement, m_titlefireScale, m_selectfireScale);
				m_ilust.Lerp(LogoComplement, m_RightfirstPosition, m_selectilust);
				m_ilustOp.Lerp(LogoComplement, m_RightfirstPosition, m_selectilustOp);

				//���`�⊮�������̂�SetPosition�ɓ����
				m_fire.SetPosition(m_LogoPosition);
				m_fire.SetScale(m_fireScale);
				m_operation.SetPosition(m_operationPosition);
				m_start.SetPosition(m_startPosition);
				m_option.SetPosition(m_optionPosition);
				m_titleLogo.SetPosition(m_LogoPosition);
				m_titleLogo.SetScale(m_LogoScale);
				m_iluststart.SetPosition(m_ilust);
				m_iluststartOp.SetPosition(m_ilustOp);

				//�⊮��
				LogoComplement += 0.03f;

			}
		}
	}
	if (m_titleScene == enTitleScene_Select)
	{
		//�������������ʂ�L�����N�^�[������ʂ��������A�܂��Z���N�g��ʂ�������\������
		if (m_operationLook == enOperationLook_UnSeem || m_characterOpLook == enCharacterOpLook_UnSeem || LogoComplement < 1.0f)
		{
			m_titleLogo.SetPosition(m_selectLogoPosition);
			m_start.SetPosition(m_Top);
			m_operation.SetPosition(m_Central);
			m_option.SetPosition(m_Under);
			m_titleLogo.SetScale(m_selectLogoScale);
		}
		Select();
		Ilust();
		Operation();
		CharacterOp();
		//�Q�[����ʂւ̑J��
		if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Start) {
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
			//game��ʂ֑J��
			CharacterSelect* characterSelect = NewGO<CharacterSelect>(0, "game");
			DeleteGO(this);
			DeleteGO(m_bgm);
		}
	}
	//�ŏ��̉�ʂ�A�{�^���������ꂽ��
	if (m_titleScene == enTitleScene_PressAScene && g_pad[0]->IsTrigger(enButtonA))
	{
		//���`�ϊ��Ɉڂ�
		titleScene = 1;
	}
	if (m_titleScene == enTitleScene_Change && LogoComplement > 1.0f)
	{
		//�Z���N�g��ʂɈڂ�
		titleScene = 2;
	}
	
	switch (titleScene)
	{
	case 0:
		m_titleScene = enTitleScene_PressAScene;
		break;
	case 1:
		m_titleScene = enTitleScene_Change;
		break;
	case 2:
		m_titleScene = enTitleScene_Select;
		break;
	}
}

//�Z���N�g�J�[�\��
void Tittle::Select()
{
	if (m_operationLook == enOperationLook_UnSeem && m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			selectPosition++;
			if (selectPosition > 2)
				selectPosition = 0;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			selectPosition--;
			if (selectPosition < 0)
				selectPosition = 2;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
	}

	switch (selectPosition)
	{
	case 0:
		//"�͂��߂�"
		m_tSelectPosition = enSelectPosition_Start;
		//�F���t����"Start"�������Ă���
		m_startST.SetPosition(m_Top);
		//����ȊO�͉�ʊO�ɂ���
		m_operationST.SetPosition(m_firstPosition);
		m_optionST.SetPosition(m_firstPosition);
		break;
	case 1:
		//"�������"
		m_tSelectPosition = enSelectPosition_Option;
		//�F���t����"Option"�������Ă���
		m_optionST.SetPosition(m_Under);
		//����ȊO�͉�ʊO�ɂ���
		m_startST.SetPosition(m_firstPosition);
		m_operationST.SetPosition(m_firstPosition);
		break;
	case 2:
		//"�L�����N�^�[����"
		m_tSelectPosition = enSelectPosition_Operation;
		//�F���t����"HOWTOPLAY"�������Ă���
		m_operationST.SetPosition(m_Central);
		//����ȊO�͉�ʊO�ɂ���
		m_startST.SetPosition(m_firstPosition);
		m_optionST.SetPosition(m_firstPosition);
		break;
	}	

	m_start.Update();
	m_startST.Update();
	m_operation.Update();
	m_operationST.Update();
	m_option.Update();
	m_optionST.Update();
}
//�������
void Tittle::Operation()
{
	//������ʂ���^�C�g����ʂւ̑J��
	if (g_pad[0]->IsTrigger(enButtonB) && m_operationLook == enOperationLook_Seem) {
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//������ʂ��\��
		m_operationLook = enOperationLook_UnSeem;
	}

	//������ʂւ̑J��
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Operation && m_operationLook == enOperationLook_UnSeem) {
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//������ʂ�\��
		m_operationLook = enOperationLook_Seem;
	}
	//�����ʂ�\��
	if (m_operationLook == enOperationLook_Seem)
	{
		m_operationPic.SetPosition(m_opPosition);
	}
	//�����ʂ��\��
	if (m_operationLook == enOperationLook_UnSeem)
	{
		m_operationPic.SetPosition(m_firstPosition);
	}
	
}

//�L�����N�^�[����
void Tittle::CharacterOp()
{
	//�L�����N�^�[������ʂ���^�C�g���ւ̑J��
	if (g_pad[0]->IsTrigger(enButtonB) && m_characterOpLook == enCharacterOpLook_Seem)
	{
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//�L�����N�^�[������ʂ��\��
		m_characterOpLook = enCharacterOpLook_UnSeem;
	}

	//�L�����N�^�[������ʂւ̑J��
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Option && m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//�L�����N�^�[������ʂ�\��
		m_characterOpLook = enCharacterOpLook_Seem;
	}

	//�\�����̏���
	if (m_characterOpLook == enCharacterOpLook_Seem)
	{
		m_fire.SetScale(0.6f, 1.0f, 0.0f);
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		m_option.SetPosition(m_firstPosition);
		//�\���L�[������������
		if (g_pad[0]->IsTrigger(enButtonLeft) && characterOpPosition != 0)
		{
			characterOpPosition--;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		//�\���L�[�E����������
		if (g_pad[0]->IsTrigger(enButtonRight) && characterOpPosition < 3)
		{
			characterOpPosition++;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}

		switch (characterOpPosition)
		{
		case 0:
			m_characterOpPosition = enCharacterOpPosition_Knight;
			m_fire.SetPosition(m_KnightCursor + m_charaLeftCursor);
			m_WizardOp.SetPosition(m_firstPosition);
			m_KnightOp.SetPosition(m_opPosition);
			break;
		case 1:
			m_characterOpPosition = enCharacterOpPosition_Wizard;
			m_fire.SetPosition(m_WizardCursor + m_charaLeftCursor);
			m_KnightOp.SetPosition(m_firstPosition);
			m_WizardOp.SetPosition(m_opPosition);
			break;
		case 2:
			m_characterOpPosition = enCharacterOpPosition_Zombie;
			m_fire.SetPosition(m_ZombieCursor + m_charaLeftCursor);
			break;
		case 3:
			m_characterOpPosition = enCharacterOpPosition_Mitei;
			m_fire.SetPosition(m_MiteiCursor + m_charaLeftCursor);
			break;
		}
		m_Opchoice.SetPosition(0.0f, 450.0f, 0.0f);
		m_start.Update();
		m_operation.Update();
		m_option.Update();
	}
	//�L����������ʂ��\���ɂ���Ƃ��̏���
	if (m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		m_KnightOp.SetPosition(m_firstPosition);
		m_WizardOp.SetPosition(m_firstPosition);
		m_Opchoice.SetPosition(m_firstPosition);
	}
	m_KnightOp.Update();
	m_WizardOp.Update();
	m_Opchoice.Update();
	m_fire.Update();
}

//�I����ʂ̃C���X�g
void Tittle::Ilust()
{
	switch (m_tSelectPosition)
	{
	case enSelectPosition_Start:
		//�w��̈ʒu�Ɉړ�������
		m_iluststart.SetPosition(m_selectilust);
		m_iluststartOp.SetPosition(m_selectilustOp);
		//����ȊO�͉�ʊO�Ɉړ�
		m_ilusthowtoplay.SetPosition(m_RightfirstPosition);
		m_ilusthowtoplayOp.SetPosition(m_RightfirstPosition);
		m_ilustoption.SetPosition(m_RightfirstPosition);
		m_ilustoptionOp.SetPosition(m_RightfirstPosition);
		break;
	case enSelectPosition_Operation:
		//�w��̈ʒu�Ɉړ�������
		m_ilusthowtoplay.SetPosition(m_selectilust);
		m_ilusthowtoplayOp.SetPosition(m_selectilustOp);
		//����ȊO�͉�ʊO�Ɉړ�
		m_iluststart.SetPosition(m_RightfirstPosition);
		m_iluststartOp.SetPosition(m_RightfirstPosition);
		m_ilustoption.SetPosition(m_RightfirstPosition);
		m_ilustoptionOp.SetPosition(m_RightfirstPosition);
		break;
	case enSelectPosition_Option:
		//�w��̈ʒu�Ɉړ�������
		m_ilustoption.SetPosition(m_selectilust);
		m_ilustoptionOp.SetPosition(m_selectilustOp);
		//����ȊO�͉�ʊO�Ɉړ�
		m_iluststart.SetPosition(m_RightfirstPosition);
		m_iluststartOp.SetPosition(m_RightfirstPosition);
		m_ilusthowtoplay.SetPosition(m_RightfirstPosition);
		m_ilusthowtoplayOp.SetPosition(m_RightfirstPosition);
		break;
		break;
	}
}

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_iluststart.Draw(rc);
	m_iluststartOp.Draw(rc);
	m_ilusthowtoplay.Draw(rc);
	m_ilusthowtoplayOp.Draw(rc);
	m_ilustoption.Draw(rc);
	m_ilustoptionOp.Draw(rc);
	m_fire.Draw(rc);
	m_titleLogo.Draw(rc);
	m_start.Draw(rc);
	m_startST.Draw(rc);
	m_operation.Draw(rc);
	m_operationST.Draw(rc);
	m_option.Draw(rc);
	m_optionST.Draw(rc);
	m_operationPic.Draw(rc);
	m_KnightOp.Draw(rc);
	m_WizardOp.Draw(rc);
	m_Opchoice.Draw(rc);
}
