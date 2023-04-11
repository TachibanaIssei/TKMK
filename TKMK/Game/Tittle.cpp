#include "stdafx.h"
#include "Tittle.h"

#include "Game.h"

Tittle::Tittle()
{

}

Tittle::~Tittle()
{

}

bool Tittle::Start()
{
	//tittle‚Ì‰Šú‰»
	m_spriteRender.Init("Assets/sprite/tittle.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//Select‚Ì‰Šú‰»
	m_start.Init("Assets/sprite/Start.DDS", 200.0f, 100.0f);
	m_start.SetPosition(0.0f, -150.0f, 0.0f);
	m_start.SetScale(1.0f, 1.0f, 1.0f);
	m_start.Update();

	m_operation.Init("Assets/sprite/Operation.DDS", 300.0f, 100.0f);
	m_operation.SetPosition(0.0f, -250.0f, 0.0f);
	m_operation.SetScale(0.7f, 1.0f, 1.0f);
	m_operation.Update();

	m_charaExplanation.Init("Assets/sprite/CharaExplanation.DDS", 200.0f, 100.0f);
	m_charaExplanation.SetPosition(0.0f, -350.0f, 0.0f);
	m_charaExplanation.SetScale(1.7f, 1.7f, 1.0f);
	m_charaExplanation.Update();

	m_choice.Init("Assets/sprite/Choice.DDS", 200.0f, 100.0f);
	m_choice.SetPosition(m_sPosition);
	m_choice.SetScale(1.7f, 1.0f, 1.0f);
	m_choice.Update();

	//‘€ìà–¾‰æ‘œ
	m_operationPic.Init("Assets/sprite/Controller.DDS", 1920.0f, 1080.0f);
	m_operationPic.SetPosition(m_opPosition);
	m_operationPic.SetScale(1.0f, 1.0f, 1.0f);
	m_operationPic.Update();

	//BGM‚ÌÝ’è
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/gameBGM/TitleBGM1.wav");
	//‘I‘ð‰¹
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
	//ƒQ[ƒ€‰æ–Ê‚Ö‚Ì‘JˆÚ
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Start) {
		//‘I‘ð‰¹
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//game‰æ–Ê‚Ö‘JˆÚ
		Game* game = NewGO<Game>(0, "game");
		DeleteGO(this);
		DeleteGO(m_bgm);
	}

	//à–¾‰æ–Ê‚©‚çƒ^ƒCƒgƒ‹‰æ–Ê‚Ö‚Ì‘JˆÚ
	if (g_pad[0]->IsTrigger(enButtonB) && m_operationPosition == enOperationPosition_Seem) {
		//‘I‘ð‰¹
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//à–¾‰æ–Ê‚ðo‚·
		m_operationPosition = enOperationPosition_UnSeem;
		Operation();
	}

	//à–¾‰æ–Ê‚Ö‚Ì‘JˆÚ
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Operation && m_operationPosition == enOperationPosition_UnSeem){
		//‘I‘ð‰¹
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//à–¾‰æ–Ê‚ðo‚·
		m_operationPosition = enOperationPosition_Seem;
		Operation();
	}


	m_spriteRender.Update();
	m_choice.Update();
	m_start.Update();
	m_operation.Update();
	m_charaExplanation.Update();
	m_operationPic.Update();
	
}
void Tittle::Select()
{
	if (m_operationPosition == enOperationPosition_UnSeem)
	{
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			if (selectPosition != 0)
				selectPosition--;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			if (selectPosition < 2)
				selectPosition++;
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
		m_sPosition = { 0.0f, -150.0f, 0.0f };
		break;
	case 1:
		m_tSelectPosition = enSelectPosition_Operation;
		m_sPosition = { 0.0f, -250.0f, 0.0f };
		break;
	case 2:
		m_tSelectPosition = enSelectPosition_CharaExplanation;
		m_sPosition = { 0.0f, -350.0f, 0.0f };
		break;
	}
	m_choice.SetPosition(m_sPosition);
	m_choice.Update();
}

void Tittle::Operation()
{
	if (m_operationPosition == enOperationPosition_Seem)
	{
		m_opPosition = { 0.0f,0.0f,0.0f };
	}
	if (m_operationPosition == enOperationPosition_UnSeem)
	{
		m_opPosition = { 0.0f,2000.0f,0.0f };
	}

	m_operationPic.SetPosition(m_opPosition);
	m_operationPic.Update();
}


void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_choice.Draw(rc);
	m_start.Draw(rc);
	m_operation.Draw(rc);
	m_charaExplanation.Draw(rc);
	m_operationPic.Draw(rc);
}
