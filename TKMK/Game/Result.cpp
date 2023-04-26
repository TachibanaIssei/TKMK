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

	for (int i = 0; i < 4; i++)
	{
		playerpoint[i] = charPoints[i];
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
	/*wchar_t moji2[256];
	swprintf(moji2, L"%d pt", playerpoint[0]);
	moji.SetText(moji2);
	moji.SetPosition(g_vec3Zero);
	moji.SetColor(g_vec4Black);*/

	wchar_t Player1[256];
	wchar_t Player2[256];
	wchar_t Player3[256];
	wchar_t Player4[256];

	int i, j, k;

	int rank[PLAYER];

	for (j = 0; j < PLAYER; j++)
	{
		rank[j] = 1;
		for (k = 0; k < PLAYER; k++)
		{
			if (playerpoint[j] < playerpoint[k])rank[j]++;
		}
	}
	for (i = 0; i < PLAYER; i++)
	{
		switch (playerpoint[i])
		{
		case 0:
			swprintf(Player1, L"%d,%d pt", rank[i], playerpoint[i]);
			PlayerRank1.SetText(Player1);
			PlayerRank1.SetColor(g_vec4Black);
			switch (rank[i])
			{
			case 0:
				PlayerRank1.SetPosition(RankPos[i]);
				break;
			case 1:
				PlayerRank1.SetPosition(RankPos[i]);
				break;
			case 2:
				PlayerRank1.SetPosition(RankPos[i]);
				break;
			case 3:
				PlayerRank1.SetPosition(RankPos[i]);
				break;
			}
			break;
		case 1:
			swprintf(Player1, L"%d,%3d pt", rank[i], playerpoint[i]);
			PlayerRank2.SetText(Player1);
			PlayerRank2.SetColor(g_vec4Black);
			switch (rank[i])
			{
			case 0:
				PlayerRank2.SetPosition(RankPos[i]);
				break;
			case 1:
				PlayerRank2.SetPosition(RankPos[i]);
				break;
			case 2:
				PlayerRank2.SetPosition(RankPos[i]);
				break;
			case 3:
				PlayerRank2.SetPosition(RankPos[i]);
				break;
			}
			break;
		case 2:
			swprintf(Player1, L"%d,%3d pt", rank[i], playerpoint[i]);
			PlayerRank3.SetText(Player1);
			PlayerRank3.SetColor(g_vec4Black);
			switch (rank[i])
			{
			case 0:
				PlayerRank3.SetPosition(RankPos[i]);
				break;
			case 1:
				PlayerRank3.SetPosition(RankPos[i]);
				break;
			case 2:
				PlayerRank3.SetPosition(RankPos[i]);
				break;
			case 3:
				PlayerRank3.SetPosition(RankPos[i]);
				break;
			}
			break;
		case 3:
			swprintf(Player1, L"%d,%3d pt", rank[i], playerpoint[i]);
			PlayerRank4.SetText(Player1);
			PlayerRank4.SetColor(g_vec4Black);
			switch (rank[i])
			{
			case 0:
				PlayerRank4.SetPosition(RankPos[i]);
				break;
			case 1:
				PlayerRank4.SetPosition(RankPos[i]);
				break;
			case 2:
				PlayerRank4.SetPosition(RankPos[i]);
				break;
			case 3:
				PlayerRank4.SetPosition(RankPos[i]);
				break;
			}
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