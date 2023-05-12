#include "stdafx.h"
#include "Result.h"
#include "Tittle.h"
#include "Game.h"
#include "Fade.h"
#include "system/system.h"

namespace ResultSpriteConst
{
	const Vector3 GOTITLE_ADD_CURSOR_POS = Vector3(-330.0f, 0.0f, 0.0f);
	const Vector3 GAME_FINISH_ADD_CURSOR_POS = Vector3(-280.0f, 0.0f, 0.0f);

	const Vector3 RESULT_LOGO_POS = Vector3(0.0f, 400.0f, 0.0f);		//リザルトのロゴ
}

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	fade = FindGO<Fade>("fade");
	fade->StartFadeOut(1.0f);

	Game* game = FindGO<Game>("game");
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

	InitSprite();

	g_soundEngine->ResistWaveFileBank(40, "Assets/sound/resultBGM/Result1.wav");
	g_soundEngine->ResistWaveFileBank(41, "Assets/sound/resultBGM/Result2.wav");
	g_soundEngine->ResistWaveFileBank(42, "Assets/sound/resultBGM/Result3.wav");

	m_bgm = NewGO<SoundSource>(0);
	switch (Player[0].Rank)
	{
	case 1:
		m_bgm->Init(40);
		break;
	case 4:
		m_bgm->Init(42);
		break;
	default:
		m_bgm->Init(41);
		break;
	}
	m_bgm->Play(true);
	m_bgm->SetVolume(BGMVolume);

	return true;
}

void Result::Update()
{
	//最初の処理
	if (m_change == enChange_first)
	{
		Rank();
		if (fade->GetCurrentAlpha()<=0.0f)
		{
			m_change = enChange_move;
		}
	}
	//線形補間中の処理
	if (m_change == enChange_move)
	{
		Move();
		NameMove();
	}
	//線形補間が終わった後の処理
	if (m_change == enChange_stop)
	{
		Select();
	}
}

void Result::InitSprite()
{
	//Resultの初期化
	m_spriteRender.Init("Assets/sprite/Result/titleBack.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(g_vec3Zero);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//リザルトのロゴ
	m_ResultLogo.Init("Assets/sprite/Result/RESULT.DDS", 700.0f, 150.0f);
	m_ResultLogo.SetPosition(ResultSpriteConst::RESULT_LOGO_POS);
	m_ResultLogo.Update();

	//"タイトルに戻る"非選択
	m_GOtitle.Init("Assets/sprite/Result/GOtoTitle_white.DDS", 500.0f, 100.0f);
	m_GOtitle.SetPosition(FirstPos[4]);
	m_GOtitle.SetScale(g_vec3One);
	m_GOtitle.Update();
	//"タイトルに戻る"選択
	m_GOtitleST.Init("Assets/sprite/Result/GOtoTitle_color.DDS", 500.0f, 100.0f);
	m_GOtitleST.SetPosition(RankPos[4]);
	m_GOtitleST.SetScale(g_vec3One);
	m_GOtitleST.SetMulColor(m_color);
	m_GOtitleST.Update();

	//"ゲームを終了"非選択
	m_gameover.Init("Assets/sprite/Result/EndGame_white.DDS", 400.0f, 100.0f);
	m_gameover.SetPosition(FirstPos[5]);
	m_gameover.SetScale(g_vec3One);
	m_gameover.Update();
	//"ゲームを終了"選択
	m_gameoverST.Init("Assets/sprite/Result/EndGame_color.DDS", 400.0f, 100.0f);
	m_gameoverST.SetPosition(RankPos[5]);
	m_gameoverST.SetScale(g_vec3One);
	m_gameoverST.SetMulColor(m_color);
	m_gameoverST.Update();
	
	//Playerの名前
	//"Player"
	m_PlayerNameP.Init("Assets/sprite/Result/Player.DDS", 250.0f, 100.0f);
	m_PlayerNameP.SetPosition(FirstPos[0]);
	m_PlayerNameP.Update();
	//"CPU1"
	m_CPUName1.Init("Assets/sprite/Result/CPU1.DDS", 200.0f, 75.0f);
	m_CPUName1.SetPosition(FirstPos[0]);
	m_CPUName1.Update();
	//"CPU2"
	m_CPUName2.Init("Assets/sprite/Result/CPU2.DDS", 200.0f, 75.0f);
	m_CPUName2.SetPosition(FirstPos[0]);
	m_CPUName2.Update();
	//"CPU3"
	m_CPUName3.Init("Assets/sprite/Result/CPU3.DDS", 200.0f, 75.0f);
	m_CPUName3.SetPosition(FirstPos[0]);
	m_CPUName3.Update();

	//選択のカーソル
	m_choiceCursor.Init("Assets/sprite/Select/pointer_black.DDS", 220.0f, 220.0f);
	m_choiceCursor.SetPosition(FirstPos[4]);
	m_choiceCursor.SetScale(0.6f, 0.6f, 0.6f);
	Quaternion rot;
	rot.SetRotationDegZ(225.0f);
	m_choiceCursor.SetRotation(rot);
	m_choiceCursor.Update();
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
			swprintf(Rank1, L"%d", Player[i].Point);
			m_PlayerRank1.SetText(Rank1);
			m_PlayerRank1.SetPosition(FirstPos[i]);
			m_PlayerRank1.SetColor(g_vec4Black);
			m_PlayerRank1.SetScale(WordScale);
			break;
		case 2:
			swprintf(Rank2, L"%d", Player[i].Point);
			m_PlayerRank2.SetText(Rank2);
			m_PlayerRank2.SetPosition(FirstPos[i]);
			m_PlayerRank2.SetColor(g_vec4Black);
			m_PlayerRank2.SetScale(WordScale);
			break;
		case 3:
			swprintf(Rank3, L"%d", Player[i].Point);
			m_PlayerRank3.SetText(Rank3);
			m_PlayerRank3.SetPosition(FirstPos[i]);
			m_PlayerRank3.SetColor(g_vec4Black);
			m_PlayerRank3.SetScale(WordScale);
			break;
		case 4:
			swprintf(Rank4, L"%d", Player[i].Point);
			m_PlayerRank4.SetText(Rank4);
			m_PlayerRank4.SetPosition(FirstPos[i]);
			m_PlayerRank4.SetColor(g_vec4Black);
			m_PlayerRank4.SetScale(WordScale);
			break;
		default:
			break;
		}
	}
	m_PlayerNameP.Update();
	m_CPUName1.Update();
	m_CPUName2.Update();
	m_CPUName3.Update();
}

void Result::Move()
{
	if (Complement <= 1.0f)
	{
			for (int i = 0; i < MOVE; i++)
			{
				MovePos[i].Lerp(Complement, FirstPos[i], RankPos[i]);
				switch (i)
				{
				case 0:
					m_PlayerRank1.SetPosition(MovePos[i] + PointRight);
					break;
				case 1:
					m_PlayerRank2.SetPosition(MovePos[i] + PointRight);
					break;
				case 2:
					m_PlayerRank3.SetPosition(MovePos[i] + PointRight);
					break;
				case 3:
					m_PlayerRank4.SetPosition(MovePos[i] + PointRight);
					break;
				case 4:
					m_GOtitle.SetPosition(MovePos[i]);
					break;
				case 5:
					m_gameover.SetPosition(MovePos[i]);
					break;
				default:
					break;
				}
			}
			Complement += 0.02f;
	}
	else
	{
		m_change = enChange_stop;
	}

	m_timer++;

	m_GOtitle.Update();
	m_gameover.Update();
}

void Result::NameMove()
{
	for (int i = 0; i < 4; i++)
	{
		MovePos[i].Lerp(Complement, FirstPos[i], RankPos[i]);
		switch (Player[i].NameNum)
		{
		case 1:
			m_PlayerNameP.SetPosition(MovePos[Player[i].Rank - 1]);
			break;
		case 2:
			m_CPUName1.SetPosition(MovePos[Player[i].Rank - 1]);
			break;
		case 3:
			m_CPUName2.SetPosition(MovePos[Player[i].Rank - 1]);
			break;
		case 4:
			m_CPUName3.SetPosition(MovePos[Player[i].Rank - 1]);
			break;
		default:
			break;
		}
	}
	m_PlayerNameP.Update();
	m_CPUName1.Update();
	m_CPUName2.Update();
	m_CPUName3.Update();
}

void Result::Select()
{
	if (g_pad[0]->IsTrigger(enButtonLeft) && select != 0)
	{
		select -= 1;
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->SetVolume(SEVolume);
		se->Play(false);
	}
	if (g_pad[0]->IsTrigger(enButtonRight) && select < 1)
	{
		select += 1;
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->SetVolume(SEVolume);
		se->Play(false);
	}

	switch (select)
	{
	case 0:
		m_cursor = enCursorPos_title;
		m_GOtitleST.SetMulColor(m_colorST);
		m_gameoverST.SetMulColor(m_color);

		//選択カーソルを"タイトルへ戻る"に合わせる
		m_choiceCursor.SetPosition(RankPos[4] + ResultSpriteConst::GOTITLE_ADD_CURSOR_POS);
		break;
	case 1:
		m_cursor = enCursorPos_exit;
		m_GOtitleST.SetMulColor(m_color);
		m_gameoverST.SetMulColor(m_colorST);

		//選択カーソルを"ゲーム終了"に合わせる
		m_choiceCursor.SetPosition(RankPos[5] + ResultSpriteConst::GAME_FINISH_ADD_CURSOR_POS);
		break;
	default:
		break;
	}

	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_cursor == enCursorPos_title)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->SetVolume(SEVolume);
		se->Play(false);
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
		DeleteGO(m_bgm);
	}
	//ゲームを終了
	if (g_pad[0]->IsTrigger(enButtonA) && m_cursor == enCursorPos_exit)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->SetVolume(SEVolume);
		se->Play(false);
		g_gameLoop.m_isLoop = false;
	}
	m_GOtitleST.Update();
	m_gameoverST.Update();
	m_choiceCursor.Update();
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_PlayerNameP.Draw(rc);
	m_CPUName1.Draw(rc);
	m_CPUName2.Draw(rc);
	m_CPUName3.Draw(rc);
	m_PlayerRank1.Draw(rc);
	m_PlayerRank2.Draw(rc);
	m_PlayerRank3.Draw(rc);
	m_PlayerRank4.Draw(rc);
	m_GOtitle.Draw(rc);
	m_gameover.Draw(rc);
	m_GOtitleST.Draw(rc);
	m_gameoverST.Draw(rc);
	m_ResultLogo.Draw(rc);
	m_choiceCursor.Draw(rc);
}