#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "GameUI.h"
//#include "KnightPlayer.h"
//#include "WizardPlayer.h"

Player::Player()
{

}

Player::~Player()
{
	DeleteGO(knightPlayer);
	DeleteGO(wizardPlayer);
}

void Player::CreaetPlayer()
{
	game = FindGO<Game>("game");
	gameUI = FindGO<GameUI>("m_gameUI");

	//選択されたキャラを生成する
	switch (m_selectCharctar)
	{
	case enKnight:
		//剣士プレイヤーを生成
		knightPlayer = NewGO<KnightPlayer>(0, "m_knightplayer");
		knightPlayer->SetSGame(game);
		knightPlayer->SetGameUI(gameUI);

		playerActor = knightPlayer;
		break;

	case enWizard:
		//魔法使いプレイヤーの生成
		wizardPlayer = NewGO<WizardPlayer>(0, "wizardPlayer");
		wizardPlayer->SetSGame(game);
		wizardPlayer->SetGameUI(gameUI);

		playerActor = wizardPlayer;
		break;

	default:
		break;
	}
}



