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
	//tittleの初期化
	m_spriteRender.Init("Assets/sprite/tittle.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//Selectの初期化
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

	//操作説明画像
	m_operationPic.Init("Assets/sprite/Controller.DDS", 1920.0f, 1080.0f);
	m_operationPic.SetPosition(m_opPosition);
	m_operationPic.SetScale(1.0f, 1.0f, 1.0f);
	m_operationPic.Update();

	//BGMの設定
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/gameBGM/TitleBGM1.wav");
	//選択音
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
	//ゲーム画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Start) {
		//選択音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//game画面へ遷移
		CharacterSelect* characterSelect = NewGO<CharacterSelect>(0, "game");
		DeleteGO(this);
		DeleteGO(m_bgm);
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
		if (g_pad[0]->IsTrigger(enButtonUp) && selectPosition != 0)
		{
			selectPosition--;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonDown) && selectPosition < 2)
		{
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
	//説明画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonB) && m_operationPosition == enOperationPosition_Seem) {
		//選択音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//説明画面を消す
		m_operationPosition = enOperationPosition_UnSeem;
	}

	//説明画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Operation && m_operationPosition == enOperationPosition_UnSeem) {
		//選択音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//説明画面を出す
		m_operationPosition = enOperationPosition_Seem;
	}

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
