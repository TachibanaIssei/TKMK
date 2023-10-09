#include "stdafx.h"
#include "Tittle.h"
#include "CharacterSelect.h"
#include "Game.h"
#include "Fade.h"

namespace TitleConst
{
	const Vector3 START_IMAGE_SCALE = Vector3(0.4f, 0.4f, 1.0f);

	const Vector3 START_CURSOR_POS = { -730.0f,-130.0f,0.0f };			//�X�^�[�g�I�����̌��̃J�[�\���̈ʒu
	const Vector3 HOWTOPLAY_CURSOR_POS = { -830.0f,-245.0f,0.0f };		//�V�ѕ��I�����̌��̃J�[�\���̈ʒu
}

Tittle::Tittle()
{

}

Tittle::~Tittle()
{

}

bool Tittle::Start()
{
	InitSprite();
	ResistSound();

	m_bgm = NewGO<SoundSource>(0);

	return true;
}

void Tittle::Update()
{
	Scene();
	FadeSprite();

	m_timer++;

	SpriteUpdate();
}

void Tittle::InitSprite()
{
	//�I���̃J�[�\��
	m_choiceCursor.Init("Assets/sprite/Select/pointer_black.DDS", 220.0f, 220.0f);
	m_choiceCursor.SetPosition(TitleConst::START_CURSOR_POS);
	m_choiceCursor.SetScale(0.6f, 0.6f, 0.6f);
	Quaternion rot;
	rot.SetRotationDegZ(225.0f);
	m_choiceCursor.SetRotation(rot);
	m_choiceCursor.Update();

	//�w�i�̏�����
	m_spriteRender.Init("Assets/sprite/Title/first_title/titleBack.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(g_vec3Zero);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//PressAButton
	m_pressAButton.Init("Assets/sprite/Title/first_title/PressAButton.DDS", 700.0f, 150.0f);
	m_pressAButton.SetPosition(0.0f, -300.0f, 0.0f);
	m_pressAButton.Update();

	//title�̃��S
	m_titleLogo.Init("Assets/sprite/Title/first_title/gameTitleLogo_SWORDARENA.DDS", 1200.0f, 200.0f);
	m_titleLogo.SetPosition(m_titleLogoPosition);
	m_titleLogo.SetMulColor(m_color);
	m_titleLogo.SetScale(m_titleLogoScale);
	m_titleLogo.Update();

	//title�̌�(�E)
	m_titleswordwhite.Init("Assets/sprite/Title/first_Title/sword_right.DDS", 300.0f, 400.0f);
	m_titleswordwhite.SetPosition(m_Toprightfirstposition);
	m_titleswordwhite.SetScale(g_vec3One);
	m_titleswordwhite.Update();

	//title�̌�(��)
	m_titleswordbrack.Init("Assets/sprite/Title/first_Title/sword_left.DDS", 300.0f, 400.0f);
	m_titleswordbrack.SetPosition(m_Topleftfirstposition);
	m_titleswordbrack.SetScale(g_vec3One);
	m_titleswordbrack.Update();

	//�Ή�
	m_fire.Init("Assets/sprite/Title/first_title/fireFlower_SWORDARENA.DDS", 1500.0f, 400.0f);
	m_fire.SetPosition(m_titleLogoPosition);
	m_fire.SetMulColor(m_color);
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
	/*m_option.Init("Assets/sprite/Title/ModeScene/OPTION_white.DDS", 370.0f, 90.0f);
	m_option.SetPosition(m_firstPosition);
	m_option.SetScale(g_vec3One);
	m_option.Update();*/
	//"OPTION"�I��
	/*m_optionST.Init("Assets/sprite/Title/ModeScene/OPTION_color.DDS", 370.0f, 90.0f);
	m_optionST.SetPosition(m_firstPosition);
	m_optionST.SetMulColor(m_colorST);
	m_optionST.SetScale(g_vec3One);
	m_optionST.Update();*/

	//��������摜
	m_operationPic.Init("Assets/sprite/Title/nonGame_howTo.DDS", 1920.0f, 1080.0f);
	m_operationPic.SetPosition(g_vec3Zero);
	m_operationPic.SetScale(g_vec3One);
	m_operationPic.Update();

	m_operationInGame.Init("Assets/sprite/Title/Game_howTo.DDS", 1920.0f, 1080.0f);
	m_operationInGame.SetPosition(g_vec3Zero);
	m_operationInGame.Update();

	m_ghostExplanation.Init("Assets/sprite/Title/ghost_explanation.DDS", 1920.0f, 1080.0f);
	m_ghostExplanation.SetPosition(g_vec3Zero);
	m_ghostExplanation.Update();

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
	m_iluststart.Init("Assets/sprite/Title/ModeScene/StageImage.DDS", 1901.0f, 664.0f);
	m_iluststart.SetPosition(m_RightfirstPosition);
	m_iluststart.SetScale(TitleConst::START_IMAGE_SCALE);
	m_iluststart.Update();
	//"START"�̎��̐�����
	m_iluststartOp.Init("Assets/sprite/Title/ModeScene/Start_Setumei.DDS", 850.0f, 110.0f);
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
}

void Tittle::ResistSound()
{
	//BGM�̐ݒ�
	g_soundEngine->ResistWaveFileBank(enSound_Title_Fanfare, "Assets/sound/titleBGM/fanfare1.wav");
	g_soundEngine->ResistWaveFileBank(enSound_Title_BGM, "Assets/sound/titleBGM/titleBGM2.wav");
	//�I����
	g_soundEngine->ResistWaveFileBank(enSound_Title_Choise, "Assets/sound/sentaku/sentaku4.wav");
	//�a����
	g_soundEngine->ResistWaveFileBank(enSound_Title_Slash2, "Assets/sound/titleBGM/titleSE/zangeki2.wav");
	g_soundEngine->ResistWaveFileBank(enSound_Title_Slash1, "Assets/sound/titleBGM/titleSE/zangeki1.wav");
	//�L�������艹
	g_soundEngine->ResistWaveFileBank(enSound_OK, "Assets/sound/sentaku/kettei3.wav");
	//�L�����Z����
	g_soundEngine->ResistWaveFileBank(enSound_Cancel, "Assets/sound/sentaku/cancel.wav");
	//�^�C�g����ʌ��艹
	g_soundEngine->ResistWaveFileBank(enSound_TitleOK, "Assets/sound/titleBGM/titleketei.wav");
}

void Tittle::SpriteUpdate()
{
	m_choiceCursor.Update();
	m_spriteRender.Update();
	m_titleLogo.Update();
	m_pressAButton.Update();
	m_titleLogo.Update();
	m_fire.Update();
	m_start.Update();
	m_operation.Update();
	//m_option.Update();
	m_operationPic.Update();
	m_iluststart.Update();
	m_iluststartOp.Update();
	m_ilusthowtoplay.Update();
	m_ilusthowtoplayOp.Update();
	m_ilustoption.Update();
	m_ilustoptionOp.Update();
	m_titleswordwhite.Update();
	m_titleswordbrack.Update();
}

//�V�[���Z���N�g
void Tittle::Scene()
{
	//�����ŏ���PressA�̉�ʂ�������
	if (m_titleScene == enTitleScene_PressAScene)
	{
		m_fadeSeem = false;
		if (m_linear == enLinear_sword1)
		{
			if (swordright >= 1.1f)
			{
				swordright -= 0.11;
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(enSound_Title_Slash1);
				se->SetVolume(SEVolume);
				se->Play(false);
				m_linear = enLinear_sword2;
			}

			//���`���
			m_swordright.Lerp(swordright, m_Toprightfirstposition, m_swordPosition);
			//���`�⊮�������̂�SetPosition�ɓ����
			m_titleswordwhite.SetPosition(m_swordright);

			m_RightPos = m_swordright;

			//�⊮��
			swordright += 0.07f;
		}

		else if (m_linear == enLinear_sword2)
		{
			if (swordleft >= 1.1f)
			{
				swordleft -= 0.11;
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(enSound_Title_Slash1);
				se->SetVolume(SEVolume);
				se->Play(false);
				m_linear = enLinear_delay;
			}
			//���`���
			m_swordleft.Lerp(swordleft, m_Topleftfirstposition, m_swordPosition);
			//���`�⊮�������̂�SetPosition�ɓ����
			m_titleswordbrack.SetPosition(m_swordleft);

			m_LeftPos = m_swordleft;

			//�⊮��
			swordleft += 0.07f;
		}
		else if (m_linear == enLinear_delay)
		{
			if (delaytime > 20)
			{
				m_linear = enLinear_Logo;
			}
			//�x�点�鎞��
			delaytime++;
		}
		else if (m_linear == enLinear_Logo)
		{
			if (firstLogo > 0.8f)
			{
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(enSound_Title_Slash2);
				se->SetVolume(SEVolume);
				se->Play(false);
				m_linear = enLinear_End;
			}
			m_titlefadeSeem = true;
		
			//���`���
			m_firsttitleScale.Lerp(firstLogo, m_firsttitleScale, m_titleLogoScale);
			m_fireScale.Lerp(firstLogo, m_firsttitleScale, m_titleLogoScale);

			//���`��Ԃ������̂�SetPosition�ɓ����
			m_titleLogo.SetScale(m_firsttitleScale);
			m_fire.SetScale(m_fireScale);
			//�⊮��
			firstLogo += 0.03f;
		}
		else if(m_linear == enLinear_End)
		{
			m_fadeSeem = true;
			m_titleanim = true;
		}
		//�^�C�g����ʂ̎��̏ꏊ�Ɉړ�����
		/*m_titleLogo.SetPosition(m_titleLogoPosition);
		m_titleLogo.SetScale(m_titleLogoScale);*/
		m_fire.SetPosition(m_titleLogoPosition);
		//��ʊO�̉��Ɉړ�����
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		//m_option.SetPosition(m_firstPosition);
	}
	//����Select��ʂ�������
	if (m_titleScene == enTitleScene_Change)
	{
		if (LogoComplement <= 1.0f)
		{
			if (fadetime > 30)
			{
				m_fadeSeem = false;
			}
			if (m_timer % 2 == 0 && m_fadeSeem == false)
			{
				//���`�⊮
				m_operationPosition.Lerp(LogoComplement, m_firstPosition, m_Central);
				m_startPosition.Lerp(LogoComplement, m_firstPosition, m_Top);
				m_optionPosition.Lerp(LogoComplement, m_firstPosition, m_Under);
				m_LogoPosition.Lerp(LogoComplement, m_titleLogoPosition, m_selectLogoPosition + m_LeftLogo);
				m_LogoScale.Lerp(LogoComplement, m_titleLogoScale, m_selectLogoScale);
				m_fireScale.Lerp(LogoComplement, m_titlefireScale, m_selectfireScale);
				m_ilust.Lerp(LogoComplement, m_RightfirstPosition, m_selectilust);
				m_ilustOp.Lerp(LogoComplement, m_RightfirstPosition, m_selectilustOp);
				m_swordright.Lerp(LogoComplement, m_RightPos, m_selectLogoPosition);
				m_swordleft.Lerp(LogoComplement, m_LeftPos, m_selectLogoPosition);
				m_swordscale.Lerp(LogoComplement, m_titleswordscale, m_selectswordscale);
				//���`�⊮�������̂�SetPosition�ɓ����
				m_fire.SetPosition(m_LogoPosition);
				m_fire.SetScale(m_fireScale);
				m_operation.SetPosition(m_operationPosition);
				m_start.SetPosition(m_startPosition);
				//m_option.SetPosition(m_optionPosition);
				m_titleLogo.SetPosition(m_LogoPosition);
				m_titleLogo.SetScale(m_LogoScale);
				m_iluststart.SetPosition(m_ilust);
				m_iluststartOp.SetPosition(m_ilustOp);
				m_titleswordwhite.SetPosition(m_swordright);
				m_titleswordbrack.SetPosition(m_swordleft);
				m_titleswordwhite.SetScale(m_swordscale);
				m_titleswordbrack.SetScale(m_swordscale);

				//�⊮��
				LogoComplement += 0.05f;
			}
			fadetime += 1;
		}
	}
	if (m_titleScene == enTitleScene_Select)
	{
		//�������������ʂ�L�����N�^�[������ʂ��������A�܂��Z���N�g��ʂ�������\������
		if (m_operationLook == enOperationLook_UnSeem || m_characterOpLook == enCharacterOpLook_UnSeem || LogoComplement < 1.0f)
		{
			m_titleLogo.SetPosition(m_selectLogoPosition + m_LeftLogo);
			m_start.SetPosition(m_Top);
			m_operation.SetPosition(m_Central);
			//m_option.SetPosition(m_Under);
			m_titleLogo.SetScale(m_selectLogoScale);
		}
		Select();
		Ilust();
		Operation();
		CharacterOp();
		//�Q�[����ʂւ̑J��
		if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Start) {
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_TitleOK);
			se->Play(false);
			se->SetVolume(1.0f);

			m_fade = FindGO<Fade>("fade");
			m_fade->StartFadeIn(1.0f);

			//game��ʂ֑J��
			CharacterSelect* characterSelect = NewGO<CharacterSelect>(0, "game");
			DeleteGO(this);
			DeleteGO(m_bgm);
		}
	}
	//�ŏ��̉�ʂ�A�{�^���������ꂽ��
	if (m_titleScene == enTitleScene_PressAScene && m_titleanim == true && g_pad[0]->IsTrigger(enButtonA))
	{
		//���`�ϊ��Ɉڂ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_TitleOK);
		se->Play(false);
		se->SetVolume(1.0f);
		titleScene = 1;
		m_isWaitFadeout = true;
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
		m_isComplement = true;
		break;
	case 1:
		m_titleScene = enTitleScene_Change;
		if (m_bgm->IsPlaying() == false)
		{
			m_bgm->Init(enSound_Title_BGM);
			m_bgm->Play(true);
			m_bgm->SetVolume(0.5f);
		}
		m_isComplement = true;
		break;
	case 2:
		m_titleScene = enTitleScene_Select;
		m_isComplement = false;
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
			if (selectPosition > 1)
				selectPosition = 0;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			selectPosition--;
			if (selectPosition < 0)
				selectPosition = 1;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
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
		//m_optionST.SetPosition(m_firstPosition);

		//���̃J�[�\���̈ʒu��ݒ�
		m_choiceCursor.SetPosition(TitleConst::START_CURSOR_POS);
		break;
	case 1:
		//"�L�����N�^�[����"
		m_tSelectPosition = enSelectPosition_Operation;
		//�F���t����"HOWTOPLAY"�������Ă���
		m_operationST.SetPosition(m_Central);
		//����ȊO�͉�ʊO�ɂ���
		m_startST.SetPosition(m_firstPosition);
		//m_optionST.SetPosition(m_firstPosition);

		//���̃J�[�\���̈ʒu��ݒ�
		m_choiceCursor.SetPosition(TitleConst::HOWTOPLAY_CURSOR_POS);
		break;
	case 2:
		//"�������"
		m_tSelectPosition = enSelectPosition_Option;
		//�F���t����"Option"�������Ă���
		//m_optionST.SetPosition(m_Under);
		//����ȊO�͉�ʊO�ɂ���
		m_startST.SetPosition(m_firstPosition);
		m_operationST.SetPosition(m_firstPosition);
		break;
	}	

	m_start.Update();
	m_startST.Update();
	m_operation.Update();
	m_operationST.Update();
	//m_option.Update();
	//m_optionST.Update();
}
//�������
void Tittle::Operation()
{
	//������ʂ���^�C�g����ʂւ̑J��
	if (g_pad[0]->IsTrigger(enButtonB) && m_operationLook == enOperationLook_Seem) {
		//�L�����Z����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Cancel);
		se->Play(false);
		se->SetVolume(1.0f);
		//������ʂ��\��
		m_operationLook = enOperationLook_UnSeem;
		m_operationPageNumber = enOperationpage_Num;
	}

	//������ʂւ̑J��
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Operation && m_operationLook == enOperationLook_UnSeem) {
		//���艹
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_TitleOK);
		se->Play(false);
		se->SetVolume(1.0f);
		//������ʂ�\��
		m_operationLook = enOperationLook_Seem;
		m_operationPageNumber = enOperationPage_nonGame;
	}
	
	//�y�[�W�̐؂�ւ�����
	if (m_operationLook == enOperationLook_Seem)
	{
		if (g_pad[0]->IsTrigger(enButtonLeft))
		{
			m_operationPageNumber--;
			if (m_operationPageNumber < 0)
			{
				m_operationPageNumber = 0;
				return;
			}

			//�I����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonRight))
		{
			m_operationPageNumber++;
			if (m_operationPageNumber > 2)
			{
				m_operationPageNumber = 2;
				return;
			}

			//�I����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
	}
	else
	{
		m_operationPageNumber = 3;
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
		se->Init(enSound_TitleOK);
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
		se->Init(enSound_TitleOK);
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
		//m_option.SetPosition(m_firstPosition);
		//�\���L�[������������
		if (g_pad[0]->IsTrigger(enButtonLeft) && characterOpPosition != 0)
		{
			characterOpPosition--;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		//�\���L�[�E����������
		if (g_pad[0]->IsTrigger(enButtonRight) && characterOpPosition < 3)
		{
			characterOpPosition++;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}

		switch (characterOpPosition)
		{
		case 0:
			m_characterOpPosition = enCharacterOpPosition_Knight;
			m_WizardOp.SetPosition(m_firstPosition);
			m_KnightOp.SetPosition(m_opPosition);
			break;
		case 1:
			m_characterOpPosition = enCharacterOpPosition_Wizard;
			m_KnightOp.SetPosition(m_firstPosition);
			m_WizardOp.SetPosition(m_opPosition);
			break;
		case 2:
			m_characterOpPosition = enCharacterOpPosition_Zombie;
			break;
		case 3:
			m_characterOpPosition = enCharacterOpPosition_Mitei;
			m_fire.SetPosition(m_MiteiCursor);
			break;
		}
		m_Opchoice.SetPosition(0.0f, 450.0f, 0.0f);
		m_start.Update();
		m_operation.Update();
		//m_option.Update();
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
void Tittle::FadeSprite()
{
	if (m_fadeSeem == true)
	{
		//PressA�̃t�F�[�h
		if (m_isWaitFadeout)
		{
			//�t�F�[�h�̊Ԋu�i�Z���j
			m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
		}
		else
		{
			//�t�F�[�h�̊Ԋu�i�����j
			m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
		}
		m_pressAButton.SetMulColor({ 1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha)) });
	}
	else
	{
		//�����ɂ��Č����Ȃ�����
		m_pressAButton.SetMulColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	}
	if (m_titlefadeSeem == true)
	{
		m_titleLogo.SetMulColor(m_colorST);
		m_fire.SetMulColor(m_colorST);
	}
	else
	{
		m_titleLogo.SetMulColor(m_color);
		m_fire.SetMulColor(m_color);
	}
}

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_pressAButton.Draw(rc);
	m_iluststart.Draw(rc);
	m_iluststartOp.Draw(rc);
	m_ilusthowtoplay.Draw(rc);
	m_ilusthowtoplayOp.Draw(rc);
	m_ilustoption.Draw(rc);
	m_ilustoptionOp.Draw(rc);
	m_titleswordwhite.Draw(rc);
	m_titleswordbrack.Draw(rc);
	m_fire.Draw(rc);
	m_titleLogo.Draw(rc);
	m_start.Draw(rc);
	m_startST.Draw(rc);
	m_operation.Draw(rc);
	m_operationST.Draw(rc);
	//m_option.Draw(rc);
	//m_optionST.Draw(rc);
	//���`��Ԓ��͕\�����Ȃ�
	if(!m_isComplement) m_choiceCursor.Draw(rc);

	switch (m_operationPageNumber)
	{
	case(enOperationPage_nonGame):
		m_operationPic.Draw(rc);
		break;
	case(enOperationPage_InGame):
		m_operationInGame.Draw(rc);
		break;
	case(enOperationPage_Ghost):
		m_ghostExplanation.Draw(rc);
		break;
	default:
		break;
	}

	m_KnightOp.Draw(rc);
	m_WizardOp.Draw(rc);
	m_Opchoice.Draw(rc);
}
