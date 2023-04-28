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
		Player[i] = { charPoints[i],i+1};
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
	/*m_spriteRender.Init("Assets/sprite/result.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(1.0f, 1.0f, 1.0f);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();*/

	return true;
}

void Result::Update()
{
	//最初の処理
	if (m_change == enChange_first)
	{
		Rank();
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_change = enChange_move;
		}
	}
	//線形補間中の処理
	if (m_change == enChange_move)
	{
		Move();
	}
	//線形補間が終わった後の処理
	if (m_change == enChange_stop)
	{
		//リザルト画面からタイトル画面への遷移
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			Tittle* tittle = NewGO<Tittle>(0, "tittle");
			tittle->SetTitleScene(titleScene);
			DeleteGO(this);
		}
	}
}

void Result::Rank()
{
	wchar_t Rank1[256];
	wchar_t Rank2[256];
	wchar_t Rank3[256];
	wchar_t Rank4[256];

	for (int i = 0; i < PLAYER; i++)
	{
		switch (Player[i].Rank)
		{
		case 1:
			swprintf(Rank1, L"%d,%d,%d pt", Player[i].Rank,Player[i].NameNum, Player[i].Point);
			PlayerRank1.SetText(Rank1);
			PlayerRank1.SetPosition(FirstPos[i]);
			PlayerRank1.SetColor(g_vec4Black);
			PlayerRank1.SetScale(scale);
			break;
		case 2:
			swprintf(Rank2, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			PlayerRank2.SetText(Rank2);
			PlayerRank2.SetPosition(FirstPos[i]);
			PlayerRank2.SetColor(g_vec4Black);
			PlayerRank2.SetScale(scale);
			break;
		case 3:
			swprintf(Rank3, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			PlayerRank3.SetText(Rank3);
			PlayerRank3.SetPosition(FirstPos[i]);
			PlayerRank3.SetColor(g_vec4Black);
			PlayerRank3.SetScale(scale);
			break;
		case 4:
			swprintf(Rank4, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			PlayerRank4.SetText(Rank4);
			PlayerRank4.SetPosition(FirstPos[i]);
			PlayerRank4.SetColor(g_vec4Black);
			PlayerRank4.SetScale(scale);
			break;
		}
	}
	
}

void Result::Move()
{
	if (Complement < 1.0f)
	{
		if (m_timer % 2 == 0)
		{
			for (int i = 0; i < PLAYER; i++)
			{
				MovePos[i].Lerp(Complement, FirstPos[i], RankPos[i]);
				switch (i)
				{
				case 0:
					PlayerRank1.SetPosition(MovePos[i]);
					break;
				case 1:
					PlayerRank2.SetPosition(MovePos[i]);
					break;
				case 2:
					PlayerRank3.SetPosition(MovePos[i]);
					break;
				case 3:
					PlayerRank4.SetPosition(MovePos[i]);
					break;
				}
			}
			Complement += 0.04f;
		}
	}
	else
	{
		m_change = enChange_stop;
	}
	m_timer++;
}

void Result::Render(RenderContext& rc)
{
	//m_spriteRender.Draw(rc);
	PlayerRank1.Draw(rc);
	PlayerRank2.Draw(rc);
	PlayerRank3.Draw(rc);
	PlayerRank4.Draw(rc);
}