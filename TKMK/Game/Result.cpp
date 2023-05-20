#include "stdafx.h"
#include "Result.h"
#include "Tittle.h"
#include "Game.h"
#include "Fade.h"
#include "system/system.h"

namespace ResultSpriteConst
{
	const Vector3 GOTITLE_ADD_CURSOR_POS = Vector3(-330.0f, 0.0f, 0.0f);		//"タイトルに戻る"の画像の横に出す剣の画像に加算する値
	const Vector3 GAME_FINISH_ADD_CURSOR_POS = Vector3(-280.0f, 0.0f, 0.0f);	//"ゲーム終了"の画像の横に出す剣の画像に加算する値

	const Vector3 GO_TITLE_POS = Vector3(-500.0f, -450.0f, 0.0f);				//"タイトルに戻る"の位置
	const Vector3 GAME_FINISH_POS = Vector3(525.0f, -450.0f, 0.0f);				//"ゲームを終了"の位置

	const Vector3 RESULT_LOGO_POS = Vector3(0.0f, 400.0f, 0.0f);				//リザルトのロゴの座標
	
	const Vector3 POINTS_ALIGN = { 280.0f,-135.0f,0.0f };						//ポイントの"p"を揃える
	const Vector3 No1POINTS_ALIGN = { 280.0f,-148.0f,0.0f };					//一番上のポイントの"p"を揃える
	const Vector3 POINTS_SCALE = { 1.0f,1.0f,1.0f };							//"p"の画像の拡大率
	const Vector3 NO1_POINTS_SCALE = { 1.2f,1.2f,1.0f };						//一位の"p"の画像の拡大率

	const Vector3 FONT_POINT_ADD_POS = { 110.0f,-52.0f,0.0f };					//ポイント数のフォントの位置
	const Vector3 NO1_FONT_POINT_ADD_POS = { 100.0f,-43.0f,0.0f };				//一番上のポイント数のフォントの位置

	const Vector3 PLAYER_NAME_ALIGN = { 20.0f,0.0f,0.0f };						//プレイヤー名をCPUに揃えるために加算する値
	const Vector3 LOSER_PLAYER_NAME_ALIGN = { 0.0f,-3.0f,0.0f };				//プレイヤー名をCPUに揃えるために加算する値
	
	const Vector3 NO1_PLAYER_NAME_SCALE = { 1.2f,1.2f,1.0f };					//一位の名前の拡大率
	const Vector3 LOSER_PLAYER_NAME_SCALE = { 0.85f,0.85f,1.0f };				//それ以外の名前の拡大率

	const float NAME_WIDTH = 278.0f;											//プレイヤーとCPUの名前の幅
	const float NAME_HEIGHT = 118.0f;											//プレイヤーとCPUの名前の高さ

	const Vector3 KNIGHT_FACE_ADD_POS = { -210.0f,0.0f,0.0f };					//剣士の顔の位置を調整するために加算する座標
	const Vector3 NO1_KNIGHT_FACE_ADD_POS = { -210.0f,3.0f,0.0f };				//剣士の顔の位置を調整するために加算する座標
	const Vector3 LOSER_KNIGHT_FACE_SCALE = { 0.8f,0.8f,1.0f };					//剣士の顔の拡大率
	const float KNIGHT_FACE_WIDTH = 88.0f;										//剣士の顔の幅
	const float KNIGHT_FACE_HEIGHT = 122.0f;									//剣士の顔の高さ

	const float POINTS_UNIT_WIDTH  = 54.0f;										//ポイントの単位の幅
	const float POINTS_UNIT_HEIGHT = 69.0f;										//ポイントの単位の高さ

	const Vector3 NO1_NAME_PLATE_ADD_POS = { 57.0f,-120.0f,0.0f };				//一位の名前の背景の画像に足す座標
	const Vector3 LOSER_NAME_PLATE_ADD_POS = { 50.0f,-120.0f,0.0f };			//一位以外の名前の背景の画像に足す座標
	const float NO1_NAME_PLATE_WIDTH = 818.0f;									//一位の名前の背景の画像の幅
	const float NO1_NAME_PLATE_HEIGHT = 158.0f;									//一位の名前の背景の画像の高さ
	const float LOSER_NAME_PLATE_WIDTH = 800.0f;								//一位以外の名前の背景の画像の幅
	const float LOSER_NAME_PLATE_HEIGHT = 120.0f;								//一位以外の名前の背景の画像の高さ

	const Vector3 GAMERANK_ADD_POS = { -500.0f,-120.0f,0.0f };					//順位画像の位置
	const Vector3 GAMERANK_1ST_SCALE = { 1.2f,1.2f,1.0f };						//１位の順位画像の位置
	const float GAMERANK_WIDTH = 256.0f;										//順位画像の幅
	const float GAMERANK_HEIGHT = 114.0f;										//順位画像の高さ

	const float POINT_FONT_SHADOW_OFFSET = 2.0f;								//フォントの輪郭の太さ

	const float LOSER_WORD_SCALE = 1.8f;										//フォントの大きさ
	const float NO1_WORD_SCALE = 2.0f;											//一番上のフォントの大きさ
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
	game->GetActorPoints(charPoints.data());

	int i, j, k, l, m;
	//ポイントを代入
	for (i = 0; i < 4; i++)
	{
		m_playerScore[i] = { charPoints[i],i+1};
	}
	//順位付け
	for (j = 0; j < PLAYER; j++)
	{
		for (k = 0; k < PLAYER; k++)
		{
			if (m_playerScore[j].Point < m_playerScore[k].Point)
			{
				m_playerScore[j].Rank += 1;
			}
		}
	}

	for (l = 0; l < PLAYER; l++)
	{
		for (m = 0; m < PLAYER; m++)
		{
			if (m_playerScore[l].Point == m_playerScore[m].Point)
			{
				if (m_playerScore[l].NameNum < m_playerScore[m].NameNum)
				{
					m_playerScore[m].Rank += 1;
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
	switch (m_playerScore[0].Rank)
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
	m_bgm->SetVolume(m_bgmVolume);

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
			m_change = enChange_4th;
		}
	}

	//線形補間が終わった後の処理
	if (m_change == enChange_stop)
	{
		Select();
		return;
	}

	//線形補間中の処理
	MoveLerp();
}

void Result::InitSprite()
{
	//Resultの初期化
	m_spriteRender.Init("Assets/sprite/Result/titleBack.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(g_vec3Zero);
	m_spriteRender.SetScale(g_vec3One);
	m_spriteRender.Update();

	//リザルトのロゴ
	m_resultLogo.Init("Assets/sprite/Result/RESULT.DDS", 700.0f, 150.0f);
	m_resultLogo.SetPosition(ResultSpriteConst::RESULT_LOGO_POS);
	m_resultLogo.Update();

	//"タイトルに戻る"非選択
	m_goTitle.Init("Assets/sprite/Result/GOtoTitle_white.DDS", 500.0f, 100.0f);
	m_goTitle.SetPosition(ResultSpriteConst::GO_TITLE_POS);
	m_goTitle.SetScale(g_vec3One);
	m_goTitle.Update();
	//"タイトルに戻る"選択
	m_goTitleSelect.Init("Assets/sprite/Result/GOtoTitle_color.DDS", 500.0f, 100.0f);
	m_goTitleSelect.SetPosition(ResultSpriteConst::GO_TITLE_POS);
	m_goTitleSelect.SetScale(g_vec3One);
	m_goTitleSelect.SetMulColor(m_alphaColorUnSelect);
	m_goTitleSelect.Update();

	//"ゲームを終了"非選択
	m_gameover.Init("Assets/sprite/Result/EndGame_white.DDS", 400.0f, 100.0f);
	m_gameover.SetPosition(ResultSpriteConst::GAME_FINISH_POS);
	m_gameover.SetScale(g_vec3One);
	m_gameover.Update();
	//"ゲームを終了"選択
	m_gameoverST.Init("Assets/sprite/Result/EndGame_color.DDS", 400.0f, 100.0f);
	m_gameoverST.SetPosition(ResultSpriteConst::GAME_FINISH_POS);
	m_gameoverST.SetScale(g_vec3One);
	m_gameoverST.SetMulColor(m_alphaColorUnSelect);
	m_gameoverST.Update();
	
	for (int i = 0; i < PLAYER; i++)
	{
		//剣士の顔の初期化
		switch (i)
		{
		case(0):
			m_knightFace[i].Init("Assets/sprite/Result/PlayerFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
			m_knightFace[i].SetPosition(m_spriteLerpStartPos[i]);

			m_gameRank[i].Init("Assets/sprite/Result/Number_1st.DDS", ResultSpriteConst::GAMERANK_WIDTH, ResultSpriteConst::GAMERANK_HEIGHT);
			m_gameRank[i].SetPosition(m_spriteLerpStartPos[i]);
			m_gameRank[i].SetScale(ResultSpriteConst::GAMERANK_1ST_SCALE);
			break;
		case(1):
			m_knightFace[i].Init("Assets/sprite/Result/KnightYellowFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
			m_knightFace[i].SetPosition(m_spriteLerpStartPos[i]);

			m_gameRank[i].Init("Assets/sprite/Result/Number_2nd.DDS", ResultSpriteConst::GAMERANK_WIDTH, ResultSpriteConst::GAMERANK_HEIGHT);
			m_gameRank[i].SetPosition(m_spriteLerpStartPos[i]);
			break;
		case(2):
			m_knightFace[i].Init("Assets/sprite/Result/KnightGreenFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
			m_knightFace[i].SetPosition(m_spriteLerpStartPos[i]);

			m_gameRank[i].Init("Assets/sprite/Result/Number_3rd.DDS", ResultSpriteConst::GAMERANK_WIDTH, ResultSpriteConst::GAMERANK_HEIGHT);
			m_gameRank[i].SetPosition(m_spriteLerpStartPos[i]);
			break;
		case(3):
			m_knightFace[i].Init("Assets/sprite/Result/KnightRedFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
			m_knightFace[i].SetPosition(m_spriteLerpStartPos[i]);

			m_gameRank[i].Init("Assets/sprite/Result/Number_4th.DDS", ResultSpriteConst::GAMERANK_WIDTH, ResultSpriteConst::GAMERANK_HEIGHT);
			m_gameRank[i].SetPosition(m_spriteLerpStartPos[i]);
			break;
		}

		//ポイントの単位の"p"の画像
		m_pointsUnit[i].Init("Assets/sprite/Result/P.DDS", ResultSpriteConst::POINTS_UNIT_WIDTH, ResultSpriteConst::POINTS_UNIT_HEIGHT);
		m_pointsUnit[i].SetPosition(m_spriteLerpStartPos[i]);


		//名前の背景の画像
		if (i == 0)
		{
			m_pointsUnit[0].SetScale(ResultSpriteConst::NO1_POINTS_SCALE);

			m_namePlate[0].Init("Assets/sprite/Result/No1PlayerFrame.DDS", ResultSpriteConst::NO1_NAME_PLATE_WIDTH, ResultSpriteConst::NO1_NAME_PLATE_HEIGHT);
			m_namePlate[0].SetPosition(m_spriteLerpStartPos[0]);
		}
		else
		{
			m_pointsUnit[0].SetScale(ResultSpriteConst::POINTS_SCALE);

			m_namePlate[i].Init("Assets/sprite/Result/LoserPlayerFrame.DDS", ResultSpriteConst::LOSER_NAME_PLATE_WIDTH, ResultSpriteConst::LOSER_NAME_PLATE_HEIGHT);
			m_namePlate[i].SetPosition(m_spriteLerpStartPos[i]);
		}

		m_knightFace[i].Update();
		m_gameRank[i].Update();
		m_namePlate[i].Update();
		m_pointsUnit[i].Update();
	}

	//Playerの名前
	//"Player"
	m_playerName.Init("Assets/sprite/Result/Player.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	m_playerName.SetPosition(m_spriteLerpStartPos[m_playerScore[0].Rank - 1]);
	m_playerName.SetScale(ResultSpriteConst::LOSER_PLAYER_NAME_SCALE);
	m_playerName.Update();
	//"CPU1"
	m_cpuName1.Init("Assets/sprite/Result/CPU1.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	m_cpuName1.SetPosition(m_spriteLerpStartPos[m_playerScore[1].Rank - 1]);
	m_cpuName1.Update();
	//"CPU2"
	m_cpuName2.Init("Assets/sprite/Result/CPU2.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	m_cpuName2.SetPosition(m_spriteLerpStartPos[m_playerScore[2].Rank - 1]);
	m_cpuName2.Update();
	//"CPU3"
	m_cpuName3.Init("Assets/sprite/Result/CPU3.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	m_cpuName3.SetPosition(m_spriteLerpStartPos[m_playerScore[3].Rank - 1]);
	m_cpuName3.Update();

	for (int i = 0; i < PLAYER; i++)
	{
		if (m_playerScore[i].Rank == 1)
		{
			switch (m_playerScore[i].NameNum)
			{
			case(1):
				m_playerName.SetScale(ResultSpriteConst::NO1_PLAYER_NAME_SCALE);
				m_playerName.Update();
				break;
			case(2):
				m_cpuName1.SetScale(ResultSpriteConst::NO1_PLAYER_NAME_SCALE);
				m_cpuName1.Update();
				break;
			case(3):
				m_cpuName2.SetScale(ResultSpriteConst::NO1_PLAYER_NAME_SCALE);
				m_cpuName2.Update();
				break;
			case(4):
				m_cpuName3.SetScale(ResultSpriteConst::NO1_PLAYER_NAME_SCALE);
				m_cpuName3.Update();
				break;
			}
		}

	}

	//選択のカーソル
	m_choiceCursor.Init("Assets/sprite/Select/pointer_black.DDS", 220.0f, 220.0f);
	m_choiceCursor.SetPosition(ResultSpriteConst::GO_TITLE_POS + ResultSpriteConst::GOTITLE_ADD_CURSOR_POS);
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
		switch (m_playerScore[i].Rank)
		{
		case 1:
			swprintf(Rank1, L"%2d", m_playerScore[i].Point);
			m_playerRank1.SetText(Rank1);
			m_playerRank1.SetPosition(m_lerpStartPos[i]);
			m_playerRank1.SetColor(g_vec4Black);
			m_playerRank1.SetScale(ResultSpriteConst::NO1_WORD_SCALE);
			m_playerRank1.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4White);
			break;
		case 2:
			swprintf(Rank2, L"%2d", m_playerScore[i].Point);
			m_playerRank2.SetText(Rank2);
			m_playerRank2.SetPosition(m_lerpStartPos[i]);
			m_playerRank2.SetColor(g_vec4Black);
			m_playerRank2.SetScale(ResultSpriteConst::LOSER_WORD_SCALE);
			m_playerRank2.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4White);
			break;
		case 3:
			swprintf(Rank3, L"%2d", m_playerScore[i].Point);
			m_playerRank3.SetText(Rank3);
			m_playerRank3.SetPosition(m_lerpStartPos[i]);
			m_playerRank3.SetColor(g_vec4Black);
			m_playerRank3.SetScale(ResultSpriteConst::LOSER_WORD_SCALE);
			m_playerRank3.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4White);
			break;
		case 4:
			swprintf(Rank4, L"%2d", m_playerScore[i].Point);
			m_playerRank4.SetText(Rank4);
			m_playerRank4.SetPosition(m_lerpStartPos[i]);
			m_playerRank4.SetColor(g_vec4Black);
			m_playerRank4.SetScale(ResultSpriteConst::LOSER_WORD_SCALE);
			m_playerRank4.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4White);
			break;
		}
	}
}

void Result::MoveLerp()
{
	//補完率を増加させる
	if (m_complement <= 1.0f)
	{
		m_complement += 0.02f;
	}
	else {
		//補完率が1を超えたらリセット
		m_complement = 0.0f;

		//次のステートに移行
		switch (m_change)
		{
		case enChange_1st:
			m_change = enChange_stop;
			m_drawSelectSpriteFlag = true;
			return;
			break;
		case enChange_2nd:
			m_change = enChange_1st;
			break;
		case enChange_3rd:
			m_change = enChange_2nd;
			break;
		case enChange_4th:
			m_change = enChange_3rd;
		}
	}

	MovePointFont();
	MoveName();
}

void Result::MovePointFont()
{
	switch (m_change)
	{
	case enChange_1st:
		m_lerpMoving[m_change].Lerp(m_complement, m_lerpStartPos[m_change], m_lerpMoveEnd[m_change]);
		m_playerRank1.SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::NO1_FONT_POINT_ADD_POS);

		m_namePlate[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::NO1_NAME_PLATE_ADD_POS);
		m_pointsUnit[0].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::No1POINTS_ALIGN);

		m_gameRank[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::GAMERANK_ADD_POS);
		break;

	case enChange_2nd:
		m_lerpMoving[m_change].Lerp(m_complement, m_lerpStartPos[m_change], m_lerpMoveEnd[m_change]);
		m_playerRank2.SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::FONT_POINT_ADD_POS);

		m_namePlate[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::LOSER_NAME_PLATE_ADD_POS);
		m_pointsUnit[1].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::POINTS_ALIGN);

		m_gameRank[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::GAMERANK_ADD_POS);
		break;

	case enChange_3rd:
		m_lerpMoving[m_change].Lerp(m_complement, m_lerpStartPos[m_change], m_lerpMoveEnd[m_change]);
		m_playerRank3.SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::FONT_POINT_ADD_POS);

		m_namePlate[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::LOSER_NAME_PLATE_ADD_POS);
		m_pointsUnit[2].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::POINTS_ALIGN);

		m_gameRank[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::GAMERANK_ADD_POS);
		break;

	case enChange_4th:
		m_lerpMoving[m_change].Lerp(m_complement, m_lerpStartPos[m_change], m_lerpMoveEnd[m_change]);
		m_playerRank4.SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::FONT_POINT_ADD_POS);

		m_namePlate[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::LOSER_NAME_PLATE_ADD_POS);
		m_pointsUnit[3].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::POINTS_ALIGN);

		m_gameRank[m_change].SetPosition(m_lerpMoving[m_change] + ResultSpriteConst::GAMERANK_ADD_POS);
		break;
	}

	for (int i = 0; i < m_pointsUnit.size(); i++)
	{
		m_namePlate[i].Update();
		m_pointsUnit[i].Update();
		m_gameRank[i].Update();
	}
}

void Result::MoveName()
{
	//現在のステートが何位の移動を行うものか判定
	for (int i = 0; i < PLAYER; i++)
	{
		if (m_playerScore[i].Rank - 1 == m_change)
		{
			m_nowMoveRank = m_change;
			m_nowMoveCharacter = m_playerScore[i].NameNum;
			break;
		}
	}

	m_spriteLerpMoving[m_nowMoveRank].Lerp(m_complement, m_spriteLerpStartPos[m_nowMoveRank], m_spriteLerpMoveEnd[m_nowMoveRank]);	

	switch (m_nowMoveCharacter)
	{
	case(1):
		if (m_playerScore[0].Rank != 1)
		{
			m_playerName.SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::LOSER_PLAYER_NAME_ALIGN);
			m_knightFace[0].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::KNIGHT_FACE_ADD_POS);
			m_knightFace[0].SetScale(ResultSpriteConst::LOSER_KNIGHT_FACE_SCALE);
		}
		else
		{
			m_playerName.SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::PLAYER_NAME_ALIGN);
			m_knightFace[0].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::NO1_KNIGHT_FACE_ADD_POS);
		}
		break;
	case(2):
		m_cpuName1.SetPosition(m_spriteLerpMoving[m_nowMoveRank]);
		if (m_playerScore[1].Rank != 1)
		{
			m_knightFace[1].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::KNIGHT_FACE_ADD_POS);
			m_knightFace[1].SetScale(ResultSpriteConst::LOSER_KNIGHT_FACE_SCALE);
		}
		else {
			m_knightFace[1].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::NO1_KNIGHT_FACE_ADD_POS);
		}
		break;
	case(3):
		m_cpuName2.SetPosition(m_spriteLerpMoving[m_nowMoveRank]);
		if (m_playerScore[2].Rank != 1)
		{
			m_knightFace[2].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::KNIGHT_FACE_ADD_POS);
			m_knightFace[2].SetScale(ResultSpriteConst::LOSER_KNIGHT_FACE_SCALE);
		}
		else {
			m_knightFace[2].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::NO1_KNIGHT_FACE_ADD_POS);
		}
		break;
	case(4):
		m_cpuName3.SetPosition(m_spriteLerpMoving[m_nowMoveRank]);
		if (m_playerScore[3].Rank != 1)
		{
			m_knightFace[3].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::KNIGHT_FACE_ADD_POS);
			m_knightFace[3].SetScale(ResultSpriteConst::LOSER_KNIGHT_FACE_SCALE);
		}
		else {
			m_knightFace[3].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::NO1_KNIGHT_FACE_ADD_POS);
		}
		break;
	}

	for (int i = 0; i < m_knightFace.size(); i++)
	{
		m_knightFace[i].Update();
	}

	m_playerName.Update();
	m_cpuName1.Update();
	m_cpuName2.Update();
	m_cpuName3.Update();
}

void Result::Select()
{
	if (g_pad[0]->IsTrigger(enButtonLeft) && m_selectFlag == true)
	{
		m_selectFlag = false;
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Title_Choise);
		se->SetVolume(m_seVolume);
		se->Play(false);
	}
	if (g_pad[0]->IsTrigger(enButtonRight) && m_selectFlag == false)
	{
		m_selectFlag = true;
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Title_Choise);
		se->SetVolume(m_seVolume);
		se->Play(false);
	}

	switch (m_selectFlag)
	{
	case false:
		m_cursor = enCursorPos_title;
		m_goTitleSelect.SetMulColor(m_alphaColorSelect);
		m_gameoverST.SetMulColor(m_alphaColorUnSelect);

		//選択カーソルを"タイトルへ戻る"に合わせる
		m_choiceCursor.SetPosition(ResultSpriteConst::GO_TITLE_POS + ResultSpriteConst::GOTITLE_ADD_CURSOR_POS);
		break;
	case true:
		m_cursor = enCursorPos_exit;
		m_goTitleSelect.SetMulColor(m_alphaColorUnSelect);
		m_gameoverST.SetMulColor(m_alphaColorSelect);

		//選択カーソルを"ゲーム終了"に合わせる
		m_choiceCursor.SetPosition(ResultSpriteConst::GAME_FINISH_POS + ResultSpriteConst::GAME_FINISH_ADD_CURSOR_POS);
		break;
	}

	//リザルト画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_cursor == enCursorPos_title)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Title_Choise);
		se->SetVolume(m_seVolume);
		se->Play(false);
		Tittle* tittle = NewGO<Tittle>(0, "tittle");
		DeleteGO(this);
		DeleteGO(m_bgm);
	}
	//ゲームを終了
	if (g_pad[0]->IsTrigger(enButtonA) && m_cursor == enCursorPos_exit)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Title_Choise);
		se->SetVolume(m_seVolume);
		se->Play(false);
		g_gameLoop.m_isLoop = false;
	}
	m_goTitleSelect.Update();
	m_gameoverST.Update();
	m_choiceCursor.Update();
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);	//背景
	m_resultLogo.Draw(rc);		//リザルトロゴ

	//名前の背景画像
	//ポイントの単位
	for (int i = 0; i < PLAYER; i++)
	{
		m_namePlate[i].Draw(rc);
		m_pointsUnit[i].Draw(rc);
	}
	//剣士の顔
	for (int i = 0; i < m_knightFace.size(); i++)
	{
		m_knightFace[i].Draw(rc);
		m_gameRank[i].Draw(rc);
	}

	m_playerName.Draw(rc);		//プレイヤー
	m_cpuName1.Draw(rc);		//CPU1
	m_cpuName2.Draw(rc);		//CPU2
	m_cpuName3.Draw(rc);		//CPU3

	m_playerRank1.Draw(rc);		//プレイヤーのポイント
	m_playerRank2.Draw(rc);		//CPU1のポイント
	m_playerRank3.Draw(rc);		//CPU2のポイント
	m_playerRank4.Draw(rc);		//CPU3のポイント


	//線形補間による順位表示が終了したら表示
	if (m_drawSelectSpriteFlag)
	{
		m_goTitle.Draw(rc);			//"タイトルへ戻る"非選択
		m_gameover.Draw(rc);		//"ゲーム終了"非選択
		m_goTitleSelect.Draw(rc);	//"タイトルへ戻る"選択
		m_gameoverST.Draw(rc);		//"ゲーム終了"選択
		m_choiceCursor.Draw(rc);	//選択中のものを表す剣
	}
}