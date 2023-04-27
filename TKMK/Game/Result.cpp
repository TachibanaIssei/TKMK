#include "stdafx.h"
#include "Result.h"

#include "Tittle.h"
#include "Game.h"

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	Game* game = FindGO<Game>("game");
	//K2EngineLow* g_k2EngineLow = nullptr;
	//g_k2EngineLow->~K2EngineLow();
	game->GetActorPoints(charPoints);

	int i, j, k, l, m;
	//ポイントを代入
	for (i = 0; i < 4; i++)
	{
		Player[i] = { charPoints[i],i};
	}
	//順位付け
	for (j = 0; j < PLAYER; j++)
	{
		for (k = 0; k < PLAYER; k++)
		{
			if (Player[j].Point < Player[k].Point)
			{
				Player[j].Rank += 1;
			}
		}
	}

	for (l = 0; l < PLAYER; l++)
	{
		for (m = 0; m < PLAYER; m++)
		{
			if (Player[l].Point == Player[m].Point)
			{
				if (Player[l].NameNum < Player[m].NameNum)
				{
					Player[m].Rank += 1;
				}
				
			}
		}
	}

	DeleteGO(game);

	//Resultの初期化
	m_spriteRender.Init("Assets/sprite/result.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	return true;
}

void Result::Update()
{
	Rank();

	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		tittle->SetTitleScene(titleScene);
		DeleteGO(this);
	}
}

void Result::Rank()
{
	wchar_t Player1[256];
	wchar_t Player2[256];
	wchar_t Player3[256];
	wchar_t Player4[256];

	for (int i = 1; i <= PLAYER; i++)
	{
		switch (Player[i].Rank)
		{
		case 1:
			swprintf(Player1, L"%d,%d,%d pt", Player[i].Rank,Player[i].NameNum, Player[i].Point);
			PlayerRank1.SetText(Player1);
			PlayerRank1.SetPosition(RankPos[i]);
			PlayerRank1.SetColor(g_vec4Black);
			break;
		case 2:
			swprintf(Player2, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			PlayerRank2.SetText(Player2);
			PlayerRank2.SetPosition(RankPos[i]);
			PlayerRank2.SetColor(g_vec4Black);
			break;
		case 3:
			swprintf(Player3, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			PlayerRank3.SetText(Player3);
			PlayerRank3.SetPosition(RankPos[i]);
			PlayerRank3.SetColor(g_vec4Black);
			break;
		case 4:
			swprintf(Player4, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			PlayerRank4.SetText(Player4);
			PlayerRank4.SetPosition(RankPos[i]);
			PlayerRank4.SetColor(g_vec4Black);
			break;
		}
	}
	
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	PlayerRank1.Draw(rc);
	PlayerRank2.Draw(rc);
	PlayerRank3.Draw(rc);
	PlayerRank4.Draw(rc);
}