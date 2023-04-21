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
	m_spriteRender.Init("Assets/sprite/Title/title.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();
	
	//title�̃��S
	m_titleLogo.Init("Assets/sprite/Title/titleLogo.DDS", 400.0f,200.0f);
	m_titleLogo.SetPosition(m_firstPosition);
	m_titleLogo.SetScale(m_titleLogoScale);
	m_titleLogo.Update();

	//Select�̏�����
	//"�͂��߂�"
	m_start.Init("Assets/sprite/Title/Start.DDS", 200.0f, 100.0f);
	m_start.SetPosition(m_firstPosition);
	m_start.SetMulColor(m_color);
	m_start.SetScale(g_vec3One);
	m_start.Update();
	//"�������"
	m_operation.Init("Assets/sprite/Title/Operation.DDS", 300.0f, 100.0f);
	m_operation.SetPosition(m_firstPosition);
	m_operation.SetMulColor(m_color);
	m_operation.SetScale(0.7f, 1.0f, 1.0f);
	m_operation.Update();
	//"�L�����N�^�[����"
	m_charaExplanation.Init("Assets/sprite/Title/CharaExplanation.DDS", 200.0f, 100.0f);
	m_charaExplanation.SetPosition(m_firstPosition);
	m_charaExplanation.SetMulColor(m_color);
	m_charaExplanation.SetScale(1.7f, 1.7f, 1.0f);
	m_charaExplanation.Update();
	//�I������Ƃ��̃J�[�\��
	m_choice.Init("Assets/sprite/Choice.DDS", 100.0f, 100.0f);
	m_choice.SetPosition(m_firstPosition);
	m_choice.SetScale(0.6f, 1.0f, 1.0f);
	m_choice.Update();

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
	m_choice.Update();
	m_start.Update();
	m_operation.Update();
	m_charaExplanation.Update();
	m_operationPic.Update();
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
		//��ʊO�̉��Ɉړ�����
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		m_charaExplanation.SetPosition(m_firstPosition);
		m_choice.SetPosition(m_firstPosition);
	}
	//����Select��ʂ�������
	if (m_titleScene == enTitleScene_Change)
	{
		if (LogoComplement < 1.0f)
		{
			if (m_timer % 2 == 0)
			{
				//���`�⊮
				m_operationPosition.Lerp(LogoComplement, m_firstPosition, m_Top);
				m_startPosition.Lerp(LogoComplement, m_firstPosition, m_Central);
				m_charaExplanationPosition.Lerp(LogoComplement, m_firstPosition, m_Under);
				m_LogoPosition.Lerp(LogoComplement, m_titleLogoPosition, m_selectLogoPosition);
				m_LogoScale.Lerp(LogoComplement, m_titleLogoScale, m_selectLogoScale);

				//���`�⊮�������̂�SetPosition�ɓ����
				m_choice.SetPosition(m_startPosition + m_LeftCursor);
				m_operation.SetPosition(m_operationPosition);
				m_start.SetPosition(m_startPosition);
				m_charaExplanation.SetPosition(m_charaExplanationPosition);
				m_titleLogo.SetPosition(m_LogoPosition);
				m_titleLogo.SetScale(m_LogoScale);

				//�⊮��
				LogoComplement += 0.01f;

			}
		}
	}
	if (m_titleScene == enTitleScene_Select)
	{
		//�������������ʂ�L�����N�^�[������ʂ��������A�܂��Z���N�g��ʂ�������\������
		if (m_operationLook == enOperationLook_UnSeem || m_characterOpLook == enCharacterOpLook_UnSeem || LogoComplement < 1.0f)
		{
			m_titleLogo.SetPosition(m_selectLogoPosition);
			m_start.SetPosition(m_Central);
			m_operation.SetPosition(m_Top);
			m_charaExplanation.SetPosition(m_Under);
			//m_choice.SetPosition(m_Central);
			m_titleLogo.SetScale(m_selectLogoScale);
			m_choice.SetScale(0.6f, 1.0f, 1.0f);
		}
		Select();
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
			selectPosition--;
			if (selectPosition < 0)
				selectPosition = 2;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			selectPosition++;
			if (selectPosition > 2)
				selectPosition = 0;
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
		//�����Ɉړ�������
		m_choice.SetPosition(m_Central + m_LeftCursor);
		//��������
		m_operation.SetMulColor(m_color);
		m_charaExplanation.SetMulColor(m_color);
		//�ʏ�̓����x�ɖ߂�
		m_start.SetMulColor(g_vec3One);
		break;
	case 1:
		//"�������"
		m_tSelectPosition = enSelectPosition_CharaExplanation;
		//���Ɉړ�������
		m_choice.SetPosition(m_Under + m_LeftCursor);
		//��������
		m_start.SetMulColor(m_color);
		m_operation.SetMulColor(m_color);
		//�ʏ�̓����x�ɖ߂�
		m_charaExplanation.SetMulColor(g_vec3One);
		break;
	case 2:
		//"�L�����N�^�[����"
		m_tSelectPosition = enSelectPosition_Operation;
		//��Ɉړ�������
		m_choice.SetPosition(m_Top + m_LeftCursor);
		//��������
		m_start.SetMulColor(m_color);
		m_charaExplanation.SetMulColor(m_color);
		//�ʏ�̓����x�ɖ߂�
		m_operation.SetMulColor(g_vec3One);
		break;
	}	

	m_choice.Update();
	m_start.Update();
	m_operation.Update();
	m_charaExplanation.Update();
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
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_CharaExplanation && m_characterOpLook == enCharacterOpLook_UnSeem)
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
		m_choice.SetScale(0.6f, 1.0f, 0.0f);
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		m_charaExplanation.SetPosition(m_firstPosition);
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
			m_choice.SetPosition(m_KnightCursor + m_charaLeftCursor);
			m_WizardOp.SetPosition(m_firstPosition);
			m_KnightOp.SetPosition(m_opPosition);
			break;
		case 1:
			m_characterOpPosition = enCharacterOpPosition_Wizard;
			m_choice.SetPosition(m_WizardCursor + m_charaLeftCursor);
			m_KnightOp.SetPosition(m_firstPosition);
			m_WizardOp.SetPosition(m_opPosition);
			break;
		case 2:
			m_characterOpPosition = enCharacterOpPosition_Zombie;
			m_choice.SetPosition(m_ZombieCursor + m_charaLeftCursor);
			break;
		case 3:
			m_characterOpPosition = enCharacterOpPosition_Mitei;
			m_choice.SetPosition(m_MiteiCursor + m_charaLeftCursor);
			break;
		}
		m_Opchoice.SetPosition(0.0f, 450.0f, 0.0f);
		m_start.Update();
		m_operation.Update();
		m_charaExplanation.Update();
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
	m_choice.Update();
}

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_titleLogo.Draw(rc);
	m_KnightOp.Draw(rc);
	m_WizardOp.Draw(rc);
	m_choice.Draw(rc);
	m_Opchoice.Draw(rc);
	m_start.Draw(rc);
	m_operation.Draw(rc);
	m_charaExplanation.Draw(rc);
	m_operationPic.Draw(rc);
}
