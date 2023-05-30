#include "stdafx.h"
#include "Pause.h"
#include "Game.h"

namespace {
	const Vector3 Menu_BackPos = Vector3(0.0f, 210.0f, 0.0f);
	const Vector3 Menu_HowToPlayPos = Vector3(0.0f, 60.0f, 0.0f);
	const Vector3 Menu_BGMPos = Vector3(-245.0f, -70.0f, 0.0f);
	const Vector3 Menu_SEPos = Vector3(-245.0f, -185.0f, 0.0f);
	const Vector3 Menu_QuitGamePos = Vector3(0.0f, -320.0f, 0.0f);

	const Vector3 Menu_Flame_BGMPos = Vector3(90.0f, -68.0f, 0.0f);
	const Vector3 Menu_Flame_SEPos = Vector3(90.0f, -183.0f, 0.0f);
	const Vector3 Menu_SelectBar_BGMPos = Vector3(-136.0f, -68.0f, 0.0f);
	const Vector3 Menu_SelectBar_SEPos = Vector3(-136.0f, -183.0f, 0.0f);

	const Vector2 Menu_Bar_Pibot = Vector2(0.0f, 0.5f);
}

Pause::Pause()
{
}

Pause::~Pause()
{
}

bool Pause::Start()
{
	game = FindGO<Game>("game");

	//背景
	m_Pause_Back.Init("Assets/sprite/PauseMenu/pause_back.DDS", 1920.0f, 1080.0f);
	m_Pause_Back.SetPosition(g_vec3Zero);
	m_Pause_Back.SetScale(1.0f, 1.0f, 1.0f);
	m_Pause_Back.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.4f));
	m_Pause_Back.Update();
	//ポーズメイン
	m_Pause_Front.Init("Assets/sprite/PauseMenu/Pause2_flame.DDS", 1920.0f, 1080.0f);
	m_Pause_Front.SetPosition(g_vec3Zero);
	m_Pause_Front.SetScale(1.0f, 1.0f, 1.0f);
	m_Pause_Front.Update();

	//Back
	m_Menu_Back.Init("Assets/sprite/PauseMenu/Pause2_Back_decision.DDS", 300.0f, 100.0f);		m_Menu_Back.SetPosition(Menu_BackPos);
	m_Menu_Back.SetScale(1.0f, 1.0f, 1.0f);
	m_Menu_Back.Update();
	//HowToPlay
	m_Menu_HowToPlay.Init("Assets/sprite/PauseMenu/Pause2_HowToPlay.DDS", 620.0f, 120.0f);
	m_Menu_HowToPlay.SetPosition(Menu_HowToPlayPos);
	m_Menu_HowToPlay.SetScale(1.0f, 1.0f, 1.0f);
	m_Menu_HowToPlay.Update();
	//BGM
	m_Menu_BGM.Init("Assets/sprite/PauseMenu/Pause2_BGM.DDS", 300.0f, 100.0f);
	m_Menu_BGM.SetPosition(Menu_BGMPos);
	m_Menu_BGM.SetScale(1.0f, 1.0f, 1.0f);
	m_Menu_BGM.Update();
	//SE
	m_Menu_SE.Init("Assets/sprite/PauseMenu/Pause2_SE.DDS", 300.0f, 100.0f);
	m_Menu_SE.SetPosition(Menu_SEPos);
	m_Menu_SE.SetScale(1.0f, 1.0f, 1.0f);
	m_Menu_SE.Update();
	//QuitGame  
	m_Menu_QuitGame.Init("Assets/sprite/PauseMenu/Pause2_QuitGame.DDS", 620.0f, 120.0f);
	m_Menu_QuitGame.SetPosition(Menu_QuitGamePos);
	m_Menu_QuitGame.SetScale(1.0f, 1.0f, 1.0f);
	m_Menu_QuitGame.Update();
	
	m_Menu_BGM_Bar.Init("Assets/sprite/PauseMenu/musicBar.DDS", 452.0f, 40.0f);
	m_Menu_BGM_Bar.SetPosition(Menu_SelectBar_BGMPos);
	m_Menu_BGM_Bar.SetPivot(Menu_Bar_Pibot);
	SetBGMBarScale();
	m_Menu_BGM_Bar.Update();

	m_Menu_Flame_BGM.Init("Assets/sprite/PauseMenu/musicFlame.DDS", 480.0f, 60.0f);
	m_Menu_Flame_BGM.SetPosition(Menu_Flame_BGMPos);
	m_Menu_Flame_BGM.SetScale(1.0f, 1.0f, 1.0f);
	m_Menu_Flame_BGM.Update();

	m_Menu_SE_Bar.Init("Assets/sprite/PauseMenu/musicBar.DDS", 452.0f, 40.0f);
	m_Menu_SE_Bar.SetPosition(Menu_SelectBar_SEPos);
	m_Menu_SE_Bar.SetPivot(Menu_Bar_Pibot);
	SetSEBarScale();
	m_Menu_SE_Bar.Update();

	m_Menu_Flame_SE.Init("Assets/sprite/PauseMenu/musicFlame.DDS", 480.0f, 60.0f);
	m_Menu_Flame_SE.SetPosition(Menu_Flame_SEPos);
	m_Menu_Flame_SE.SetScale(1.0f, 1.0f, 1.0f);
	m_Menu_Flame_SE.Update();

	//操作説明
	m_operationPic.Init("Assets/sprite/PauseMenu/game_explanation.DDS", 1920.0f, 1080.0f);
	m_operationPic.SetPosition(Vector3::Zero);
	m_operationPic.SetScale(g_vec3One);
	m_operationPic.Update();

	return true;
}

void Pause::Update()
{
	if (HowToPlaySpriteFlag == false)
	{
		//ポーズ時の移動処理
		PauseMove();

		//エンターキーでポーズ画面を閉じる
		if(g_pad[0]->IsTrigger(enButtonStart))
		{
			game->ChangeGameState(Game::enGameState_BetweenGameAndPause);
			Push_OK();
			DeleteGO(this);
		}
	}

	//前のフレームのメニュー番号と今のフレームのメニュー番号が違うなら
	if (MenuNumber_old != MenuNumber) {
		//メニューのステートを選ぶ
		SelectMenu();
	}
	//ステートの管理
	MenuState();

	MenuNumber_old = MenuNumber;
}

void Pause::PauseMove()
{
	if (MenuNumber < 4) {
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->SetVolume(game->GetSoundEffectVolume());
			se->Play(false);
			MenuNumber++;
		}
	}
	
	if (MenuNumber > 0) {
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->SetVolume(game->GetSoundEffectVolume());
			se->Play(false);
			MenuNumber--;
		}
	}
	
}

void Pause::SelectMenu()
{
	if (MenuNumber == 0)
	{
		m_EnPauseMenu = enPauseMenuState_Back;
		m_Menu_Back.Init("Assets/sprite/PauseMenu/Pause2_Back_decision.DDS", 300.0f, 100.0f);
	}
	else
	{
		m_Menu_Back.Init("Assets/sprite/PauseMenu/Pause2_Back.DDS", 300.0f, 100.0f);
	}

	if (MenuNumber == 1)
	{
		m_EnPauseMenu = enPauseMenuState_HowToPlay;
		m_Menu_HowToPlay.Init("Assets/sprite/PauseMenu/Pause2_HowToPlay_decision.DDS", 620.0f, 120.0f);
	}
	else
	{
		m_Menu_HowToPlay.Init("Assets/sprite/PauseMenu/Pause2_HowToPlay.DDS", 620.0f, 120.0f);
	}

	if (MenuNumber == 2)
	{
		m_EnPauseMenu = enPauseMenuState_BGM;
		m_Menu_BGM.Init("Assets/sprite/PauseMenu/Pause2_BGM_decision.DDS", 300.0f, 100.0f);
	}
	else
	{
		m_Menu_BGM.Init("Assets/sprite/PauseMenu/Pause2_BGM.DDS", 300.0f, 100.0f);
	}

	if (MenuNumber == 3)
	{
		m_EnPauseMenu = enPauseMenuState_SE;
		m_Menu_SE.Init("Assets/sprite/PauseMenu/Pause2_SE_decision.DDS", 300.0f, 100.0f);
	}
	else
	{
		m_Menu_SE.Init("Assets/sprite/PauseMenu/Pause2_SE.DDS", 300.0f, 100.0f);
	}

	if (MenuNumber == 4)
	{
		m_EnPauseMenu = enPauseMenuState_QuitGame;
		m_Menu_QuitGame.Init("Assets/sprite/PauseMenu/Pause2_QuitGame_decision.DDS", 620.0f, 120.0f);
	}
	else
	{
		m_Menu_QuitGame.Init("Assets/sprite/PauseMenu/Pause2_QuitGame.DDS", 620.0f, 120.0f);
	}
}

void Pause::MenuState()
{
	switch (m_EnPauseMenu)
	{
	case enPauseMenuState_Back:
		Menu_Back();
		break;
	case enPauseMenuState_HowToPlay:
		Menu_HowToPlay();
		break;
	case enPauseMenuState_BGM:
		Menu_BGM();
		break;
	case enPauseMenuState_SE:
		Menu_SE();
		break;
	case enPauseMenuState_QuitGame:
		Menu_QuitGame();
		break;
	}
}

void Pause::Menu_Back()
{
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA)) {

		game->ChangeGameState(Game::enGameState_BetweenGameAndPause);
		Push_OK();
		DeleteGO(this);
	}
}

void Pause::Menu_HowToPlay()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Push_OK();
		//フラグ反転
		HowToPlaySpriteFlag = !HowToPlaySpriteFlag;
	}
}

void Pause::Menu_BGM()
{
	//音量を上げる
	if (g_pad[0]->IsPress(enButtonRight)) {
		if (game->GetBGMVolume() < game->GetMaxBGMVolume())
		{
			game->SetBGMVolume(m_addVolume);
			game->ChangeBGMVolume();
		}
	}
	//音量を下げる
	if (g_pad[0]->IsPress(enButtonLeft)) {
		if (game->GetBGMVolume() > 0)
		{
			game->SetBGMVolume(m_subVolume);
			game->ChangeBGMVolume();

			if (game->GetBGMVolume() <= 0) {
				game->SetBGMVolume(0.0f);
			}
		}
	}

	SetBGMBarScale();

}

void Pause::Menu_SE()
{
	//音量を上げる
	if (g_pad[0]->IsPress(enButtonRight)) {
		if (game->GetSoundEffectVolume()<game->GetMaxSoundEffectVolume())
		{
			game->SetSoundEffectVolume(m_addVolume);
			
			if (game->GetSoundEffectVolume() >= game->GetMaxSoundEffectVolume()) {
				game->SetSoundEffectVolume(game->GetMaxSoundEffectVolume());
			}
		}

	}
	//音量を下げる
	if (g_pad[0]->IsPress(enButtonLeft)) {
		if (game->GetSoundEffectVolume() > 0)
		{
			game->SetSoundEffectVolume(m_subVolume);

			if (game->GetSoundEffectVolume() <= 0) {
				game->SetSoundEffectVolume(0.0f);
			}
		}

	}

	//バーの処理
	SetSEBarScale();
	

}

void Pause::Menu_QuitGame()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		SoundSource* se = NewGO<SoundSource>(0);
		Push_OK();
		QuitGameFlag = true;
		DeleteGO(this);
	}
}

void Pause::Push_OK()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(enSound_Pause_Screen);
	se->SetVolume(game->GetSoundEffectVolume());
	se->Play(false);
}

void Pause::SetBGMBarScale()
{
	Vector3 BGMScale = Vector3::One;
	//増えていく割合。
	BGMScale.x = (float)game->GetBGMVolume() / (float)game->GetMaxBGMVolume();

	m_Menu_BGM_Bar.SetScale(BGMScale);
	m_Menu_BGM_Bar.Update();
}

void Pause::SetSEBarScale()
{
	Vector3 SEScale = Vector3::One;
	//増えていく割合。
	SEScale.x = (float)game->GetSoundEffectVolume() / (float)game->GetMaxSoundEffectVolume();

	m_Menu_SE_Bar.SetScale(SEScale);
	m_Menu_SE_Bar.Update();
}

void Pause::Render(RenderContext& rc)
{
	m_Pause_Back.Draw(rc);
	m_Pause_Front.Draw(rc);
	m_Menu_Back.Draw(rc);        //Back
	m_Menu_HowToPlay.Draw(rc);   //HowToPlay
	m_Menu_BGM.Draw(rc);         //BGM
	m_Menu_SE.Draw(rc);          //SE
	m_Menu_QuitGame.Draw(rc);    //QuitGame


	m_Menu_BGM_Bar.Draw(rc);
	m_Menu_Flame_BGM.Draw(rc);

	m_Menu_SE_Bar.Draw(rc);
	m_Menu_Flame_SE.Draw(rc);

	if (HowToPlaySpriteFlag == true)
	{
		m_operationPic.Draw(rc);
	}
}
