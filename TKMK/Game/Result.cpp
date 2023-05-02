#include "stdafx.h"
#include "Result.h"
#include "Tittle.h"
#include "Game.h"

#include "system/system.h"

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
	m_spriteRender.Init("Assets/sprite/Result/titleBack.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//"タイトルに戻る"
	m_GOtitle.Init("Assets/sprite/Result/changeTitle.DDS", 500.0f, 100.0f);
	m_GOtitle.SetPosition(FirstPos[4]);
	m_GOtitle.SetScale(g_vec3One);
	m_GOtitle.Update();

	//"ゲームを終了"
	m_gameover.Init("Assets/sprite/Result/gameover.DDS", 500.0f, 100.0f);
	m_gameover.SetPosition(FirstPos[5]);
	m_gameover.SetScale(g_vec3One);
	m_gameover.Update();

	//選択カーソル
	m_choice.Init("Assets/sprite/Choice.DDS", 100.0f, 100.0f);
	m_choice.SetPosition(FirstPos[6]);
	m_choice.SetScale(0.6f, 1.1f, 1.0f);
	m_choice.Update();

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
		Select();
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
			m_PlayerRank1.SetText(Rank1);
			m_PlayerRank1.SetPosition(FirstPos[i]);
			m_PlayerRank1.SetColor(g_vec4Black);
			m_PlayerRank1.SetScale(WordScale);
			break;
		case 2:
			swprintf(Rank2, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			m_PlayerRank2.SetText(Rank2);
			m_PlayerRank2.SetPosition(FirstPos[i]);
			m_PlayerRank2.SetColor(g_vec4Black);
			m_PlayerRank2.SetScale(WordScale);
			break;
		case 3:
			swprintf(Rank3, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			m_PlayerRank3.SetText(Rank3);
			m_PlayerRank3.SetPosition(FirstPos[i]);
			m_PlayerRank3.SetColor(g_vec4Black);
			m_PlayerRank3.SetScale(WordScale);
			break;
		case 4:
			swprintf(Rank4, L"%d,%d,%d pt", Player[i].Rank, Player[i].NameNum, Player[i].Point);
			m_PlayerRank4.SetText(Rank4);
			m_PlayerRank4.SetPosition(FirstPos[i]);
			m_PlayerRank4.SetColor(g_vec4Black);
			m_PlayerRank4.SetScale(WordScale);
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
			for (int i = 0; i < 7; i++)
			{
				MovePos[i].Lerp(Complement, FirstPos[i], RankPos[i]);
				switch (i)
				{
				case 0:
					m_PlayerRank1.SetPosition(MovePos[i]);
					break;
				case 1:
					m_PlayerRank2.SetPosition(MovePos[i]);
					break;
				case 2:
					m_PlayerRank3.SetPosition(MovePos[i]);
					break;
				case 3:
					m_PlayerRank4.SetPosition(MovePos[i]);
					break;
				case 4:
					m_GOtitle.SetPosition(MovePos[i]);
					break;
				case 5:
					m_gameover.SetPosition(MovePos[i]);
					break;
				case 6:
					m_choice.SetPosition(MovePos[i]);
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

	m_GOtitle.Update();
	m_gameover.Update();
	m_choice.Update();
}

void Result::Select()
{
	if (g_pad[0]->IsTrigger(enButtonLeft) && select != 0)
	{
		select -= 1;
	}
	if (g_pad[0]->IsTrigger(enButtonRight) && select < 1)
	{
		select += 1;
	}

	switch (select)
	{
	case 0:
		m_cursor = enCursorPos_title;
		m_choice.SetPosition(RankPos[6]);
		break;
	case 1:
		m_cursor = enCursorPos_exit;
		m_choice.SetPosition(RankPos[5]+ Leftcursor);
		break;
	}

	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_cursor == enCursorPos_title)
	{
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
	}
	//ゲームを終了
	if (g_pad[0]->IsTrigger(enButtonA) && m_cursor == enCursorPos_exit)
	{
		g_gameLoop.m_isLoop = false;
	}

	m_choice.Update();
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_PlayerRank1.Draw(rc);
	m_PlayerRank2.Draw(rc);
	m_PlayerRank3.Draw(rc);
	m_PlayerRank4.Draw(rc);
	m_choice.Draw(rc);
	m_GOtitle.Draw(rc);
	m_gameover.Draw(rc);
}