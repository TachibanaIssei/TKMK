#include "stdafx.h"
#include "CharacterSelect.h"
#include "Game.h"

CharacterSelect::CharacterSelect()
{

}

CharacterSelect::~CharacterSelect()
{

}

bool CharacterSelect::Start()
{
	//キャラクターセレクト画面
	m_CharacterSelectPic.Init("Assets/sprite/CharacterSelectPic.DDS", 1920.0f, 1080.0f);
	m_CharacterSelectPic.SetPosition(0.0f, 0.0f, 0.0f);
	m_CharacterSelectPic.SetScale(1.0f, 1.0f, 1.0f);
	m_CharacterSelectPic.Update();

	//剣士の画像
	m_KnightPic.Init("Assets/sprite/Knight.DDS", 300.0f, 300.0f); 
	m_KnightPic.SetPosition(-510.0f, 75.0f, 0.0f);
	m_KnightPic.SetScale(1.0f, 1.0f, 1.0f);
	m_KnightPic.Update();

	//魔法使いの画像
	m_WizardPic.Init("Assets/sprite/Wizard.DDS", 300.0f, 300.0f);
	m_WizardPic.SetPosition(-40.0f, 75.0f, 0.0f );
	m_WizardPic.SetScale(1.0f, 1.0f, 1.0f);
	m_WizardPic.Update();

	//カーソル
	m_SelectCursor.Init("Assets/sprite/SelectCurSor.DDS", 1200.0f, 675.0f);
	m_SelectCursor.SetPosition(curPosition);
	m_SelectCursor.SetScale(1.0f, 1.0f, 1.0f);
	m_SelectCursor.Update();

	return true;
}

void CharacterSelect::Update()
{
	Cursor();
	//Aボタンを押した時
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Game* game = NewGO<Game>(0, "game");
		//キャラクターセレクトが
		switch (m_characterSelect)
		{
			//剣士だったら
		case enCharacterSelect_Knight:
			game->SetCharacterSelect(enCharacterSelect_Knight);
			break;
			//魔法使いだったら
		case enCharacterSelect_Wizard:
			game->SetCharacterSelect(enCharacterSelect_Wizard);
			break;
			//未定
		case enCharacterSelect_Mitei:
			game->SetCharacterSelect(enCharacterSelect_Knight);
			break;
			//未定
		case enCharacterSelect_Mitei2:
			game->SetCharacterSelect(enCharacterSelect_Wizard);
			break;
		}
		DeleteGO(this);
	}

	m_CharacterSelectPic.Update();
	m_KnightPic.Update();
	m_WizardPic.Update();
	m_SelectCursor.Update();
}

void CharacterSelect::Cursor()
{
	//上下左右移動
	if (g_pad[0]->IsTrigger(enButtonUp) && CursorNum > 1)
	{			
		CursorNum -= 2;
	}
	if (g_pad[0]->IsTrigger(enButtonDown) && CursorNum < 2)
	{
		CursorNum += 2;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft) && CursorNum != 0 && CursorNum != 2)
	{
		CursorNum -= 1;
	}
	if (g_pad[0]->IsTrigger(enButtonRight) && CursorNum != 3 && CursorNum != 1)
	{
		CursorNum += 1;
	}

	switch (CursorNum)
	{
	case 0:
		m_characterSelect = enCharacterSelect_Knight;
		curPosition = { -510.0f,75.0f,0.0f };
		break;
	case 1:
		m_characterSelect = enCharacterSelect_Wizard;
		curPosition = { -40.0f,75.0f,0.0f };
		break;
	case 2:
		m_characterSelect = enCharacterSelect_Mitei;
		curPosition = { -510.0f,-270.0f,0.0f };
		break;
	case 3:
		m_characterSelect = enCharacterSelect_Mitei2;
		curPosition = { -40.0f,-275.0f,0.0f };
		break;
	}
	m_SelectCursor.SetPosition(curPosition);
	m_SelectCursor.Update();
}

void CharacterSelect::Render(RenderContext& rc)
{
	m_CharacterSelectPic.Draw(rc);
	m_KnightPic.Draw(rc);
	m_WizardPic.Draw(rc);
	m_SelectCursor.Draw(rc);
}

