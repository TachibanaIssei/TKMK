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

bool Player::Start()
{

	game = FindGO<Game>("game");
	//gameUI = FindGO<GameUI>("m_gameUI");

	//�I�����ꂽ�L�����𐶐�����
	switch (m_selectCharctar)
	{
	case enKnight:
		//���m�v���C���[�𐶐�
		knightPlayer = NewGO<KnightPlayer>(0, "m_knightplayer");
		knightPlayer->SetSGame(game);
		//knightPlayer->SetGameUI(gameUI);
		break;

	case enWizard:
		//���@�g���v���C���[�̐���
		wizardPlayer = NewGO<WizardPlayer>(0, "wizardPlayer");
		wizardPlayer->SetSGame(game);
		break;

	default:
		break;
	}


	
	//int a = 0;
	return true;
}


