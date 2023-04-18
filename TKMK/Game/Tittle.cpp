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
	//tittle�̏�����
	m_spriteRender.Init("Assets/sprite/tittle.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//Select�̏�����
	m_start.Init("Assets/sprite/Start.DDS", 200.0f, 100.0f);
	m_start.SetPosition(m_CentralPosition);
	m_start.SetScale(g_vec3One);
	m_start.Update();

	m_operation.Init("Assets/sprite/Operation.DDS", 300.0f, 100.0f);
	m_operation.SetPosition(m_TopPosition);
	m_operation.SetScale(0.7f, 1.0f, 1.0f);
	m_operation.Update();

	m_charaExplanation.Init("Assets/sprite/CharaExplanation.DDS", 200.0f, 100.0f);
	m_charaExplanation.SetPosition(m_UnderPosition);
	m_charaExplanation.SetScale(1.7f, 1.7f, 1.0f);
	m_charaExplanation.Update();

	m_choice.Init("Assets/sprite/Choice.DDS", 200.0f, 100.0f);
	m_choice.SetPosition(m_sPosition);
	m_choice.SetScale(1.7f, 1.0f, 1.0f);
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
	Select();
	Operation();
	CharacterOp();
	//�Q�[����ʂւ̑J��
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Start) {
		//�I����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//game��ʂ֑J��
		CharacterSelect* characterSelect = NewGO<CharacterSelect>(0, "game");
		DeleteGO(this);
		DeleteGO(m_bgm);
	}

	if (m_operationLook != enOperationLook_UnSeem || m_characterOpLook != enCharacterOpLook_UnSeem)
	{
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		m_charaExplanation.SetPosition(m_firstPosition);
	}
	else
	{
		m_choice.SetScale(1.7f, 1.0f, 1.0f);
		/*m_start.SetPosition(0.0f, -150.0f, 0.0f);
		m_operation.SetPosition(0.0f, -250.0f, 0.0f);
		m_charaExplanation.SetPosition(0.0f, -350.0f, 0.0f);
		m_choice.SetPosition(m_CentralPosition);
		m_choice.SetScale(1.7f, 1.0f, 1.0f);*/
	}

	m_spriteRender.Update();
	m_choice.Update();
	m_start.Update();
	m_operation.Update();
	m_charaExplanation.Update();
	m_operationPic.Update();
	
}
//�Z���N�g�J�[�\��
void Tittle::Select()
{
	if (m_operationLook == enOperationLook_UnSeem && m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		if (g_pad[0]->IsTrigger(enButtonUp) )
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
		m_tSelectPosition = enSelectPosition_Start;
		m_start.SetPosition(m_CentralPosition);
		m_operation.SetPosition(m_TopPosition);
		m_charaExplanation.SetPosition(m_UnderPosition);
		break;
	case 1:
		m_tSelectPosition = enSelectPosition_CharaExplanation;
		m_start.SetPosition(m_TopPosition);
		m_operation.SetPosition(m_UnderPosition);
		m_charaExplanation.SetPosition(m_CentralPosition);
		break;
	case 2:
		m_tSelectPosition = enSelectPosition_Operation;
		m_start.SetPosition(m_UnderPosition);
		m_operation.SetPosition(m_CentralPosition);
		m_charaExplanation.SetPosition(m_TopPosition);
		break;
	}
	//�Z���N�g�J�[�\���𒆉��Ɏ����Ă���
	m_sPosition = m_CentralPosition;

	m_choice.SetPosition(m_CentralPosition);
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

	if (m_operationLook == enOperationLook_Seem)
	{
		m_operationPic.SetPosition(m_opPosition);
	}
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
		m_choice.SetScale(1.2f, 1.6f, 0.0f);
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
			m_choice.SetPosition(-400.0f,450.0f,0.0f);
			m_WizardOp.SetPosition(m_firstPosition);
			m_KnightOp.SetPosition(m_opPosition);
			break;
		case 1:
			m_characterOpPosition = enCharacterOpPosition_Wizard;
			m_choice.SetPosition(-130.0f, 450.0f, 0.0f);
			m_KnightOp.SetPosition(m_firstPosition);
			m_WizardOp.SetPosition(m_opPosition);
			break;
		case 2:
			m_characterOpPosition = enCharacterOpPosition_Zombie;
			m_choice.SetPosition(145.0f, 450.0f, 0.0f);
			break;
		case 3:
			m_characterOpPosition = enCharacterOpPosition_Mitei;
			m_choice.SetPosition(425.0f, 450.0f, 0.0f);
			break;
		}
		m_Opchoice.SetPosition(0.0f, 450.0f, 0.0f);
	}

	//��\���ɂ���Ƃ��̏���
	if (m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		m_KnightOp.SetPosition(m_firstPosition);
		m_WizardOp.SetPosition(m_firstPosition);
		m_Opchoice.SetPosition(m_firstPosition);
		m_choice.SetPosition(m_CentralPosition);
	}
	m_KnightOp.Update();
	m_WizardOp.Update();
	m_Opchoice.Update();
	m_choice.Update();
}

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_KnightOp.Draw(rc);
	m_WizardOp.Draw(rc);
	m_choice.Draw(rc);
	m_Opchoice.Draw(rc);
	m_start.Draw(rc);
	m_operation.Draw(rc);
	m_charaExplanation.Draw(rc);
	m_operationPic.Draw(rc);
}
