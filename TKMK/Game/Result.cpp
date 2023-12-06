#include "stdafx.h"
#include "Result.h"
#include "Tittle.h"
#include "Game.h"
#include "Fade.h"
#include "system/system.h"
#include "Effect.h"
#include "graphics/effect/EffectEmitter.h"


namespace ResultSpriteConst
{
	const float COMPLEMENT = 0.03f;											//補完を進める値

	const Vector3 GOTITLE_ADD_CURSOR_POS = Vector3(-330.0f, 0.0f, 0.0f);		//"タイトルに戻る"の画像の横に出す剣の画像に加算する値
	const Vector3 GAME_FINISH_ADD_CURSOR_POS = Vector3(-280.0f, 0.0f, 0.0f);	//"ゲーム終了"の画像の横に出す剣の画像に加算する値

	const Vector3 GO_TITLE_POS = Vector3(-400.0f, -450.0f, 0.0f);				//"タイトルに戻る"の位置
	const Vector3 GAME_FINISH_POS = Vector3(425.0f, -450.0f, 0.0f);				//"ゲームを終了"の位置

	const Vector3 RESULT_LOGO_POS = Vector3(0.0f, 400.0f, 0.0f);				//リザルトのロゴの座標

	const Vector3 POINTS_ALIGN = { 280.0f,-135.0f,0.0f };						//ポイントの"p"を揃える
	const Vector3 No1POINTS_ALIGN = { 280.0f,-148.0f,0.0f };					//一番上のポイントの"p"を揃える
	const Vector3 POINTS_SCALE = { 1.0f,1.0f,1.0f };							//"p"の画像の拡大率
	const Vector3 NO1_POINTS_SCALE = { 1.2f,1.2f,1.0f };						//一位の"p"の画像の拡大率

	const Vector3 FONT_POINT_ADD_POS = { 110.0f,-52.0f,0.0f };					//ポイント数のフォントの位置
	const Vector3 NO1_FONT_POINT_ADD_POS = { 100.0f,-43.0f,0.0f };				//一番上のポイント数のフォントの位置

	const Vector3 PLAYER_NAME_ALIGN = { 20.0f,0.0f,0.0f };						//プレイヤー名をCPUに揃えるために加算する値
	const Vector3 LOSER_PLAYER_NAME_ALIGN = { 0.0f,-3.0f,0.0f };				//プレイヤー名をCPUに揃えるために加算する値

	const Vector3 NO1_PLAYER_NAME_SCALE = { 1.5f,1.5f,1.0f };					//一位の名前の拡大率
	const Vector3 LOSER_PLAYER_NAME_SCALE = { 1.3f,1.3f,1.0f };					//それ以外の名前の拡大率

	const float NAME_WIDTH = 203.0f;											//プレイヤーとCPUの名前の幅
	const float NAME_HEIGHT = 64.0f;											//プレイヤーとCPUの名前の高さ

	const Vector3 KNIGHT_FACE_ADD_POS = { -210.0f,0.0f,0.0f };					//剣士の顔の位置を調整するために加算する座標
	const Vector3 NO1_KNIGHT_FACE_ADD_POS = { -210.0f,3.0f,0.0f };				//剣士の顔の位置を調整するために加算する座標
	const Vector3 LOSER_KNIGHT_FACE_SCALE = { 0.8f,0.8f,1.0f };					//剣士の顔の拡大率
	const float KNIGHT_FACE_WIDTH = 88.0f;										//剣士の顔の幅
	const float KNIGHT_FACE_HEIGHT = 122.0f;									//剣士の顔の高さ

	const float POINTS_UNIT_WIDTH = 54.0f;										//ポイントの単位の幅
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

	const float SE_DRAM_JAAN_VOLUME = 1.0f;		//ドラムの"ジャーン"の音量
	const float SE_DRAM_ROLL_VOLUME = 1.0f;		//ドラムロールの音量

	const Vector3 KNIGHT_POS = { -80.0f,0.0f,-150.0f };		//剣士の座標
	const float KNIGHT_ROT = 150.0f;						//剣士の回転
	const Vector3 BACK_MODEL_POS = { 0.0f,0.0f,0.0f };		//背景の座標

	const std::array<Vector3, 4> KNIGHTS_POS = {
		KNIGHT_POS,
		KNIGHT_POS + Vector3(-80.0f,0.0f,60.0f),
		KNIGHT_POS + Vector3(-5.0f,0.0f,60.0f),
		KNIGHT_POS + Vector3(40.0f,0.0f,60.0f),
	};

	const Vector3 FIREWORKS_EFFECT_POS = { -80.0f,0.0f,200.0f };

	const Vector3 SKYCUBE_POS = { 0.0f,-900.0f,-900.0f };					//スカイキューブの座標

	const Vector3	CAMERA_TARGET_POS = Vector3(0.0f, 50.0f, -150.0f);		//カメラのターゲット
	const Vector3	CAMERA_POSITION = Vector3(0.0f, 60.0f, -280.0f);		//カメラの座標
}

Result::Result()
{

}

Result::~Result()
{
	if (m_fireWorksPlayFlag && GameObjectManager::GetInstance()->IsActive()) {
		m_fireWorks->Stop();
	}

	DeleteGO(m_skyCube);
}

bool Result::Start()
{
	//音の読み込み
	g_soundEngine->ResistWaveFileBank(enSound_ResultBGM1, "Assets/sound/resultBGM/Result1.wav");
	g_soundEngine->ResistWaveFileBank(enSound_ResultBGM2, "Assets/sound/resultBGM/Result2.wav");
	g_soundEngine->ResistWaveFileBank(enSound_ResultBGM3, "Assets/sound/resultBGM/Result3.wav");
	g_soundEngine->ResistWaveFileBank(enSound_ResultDramRoll, "Assets/sound/resultBGM/dram_roll.wav");
	g_soundEngine->ResistWaveFileBank(enSound_ResultDramJaan, "Assets/sound/resultBGM/dram_jaan.wav");

	m_fade = FindGO<Fade>("fade");
	Game* game = FindGO<Game>("game");
	m_gameMode = g_renderingEngine->GetGameMode();
	g_renderingEngine->SetGameModeToRenderingEngine(RenderingEngine::enGameMode_SoloPlay);

	m_fade->StartFadeOut(1.0f);

	game->GetActorPoints(charPoints.data());

	int i, j, k, l, m;
	//ポイントを代入
	for (i = 0; i < 4; i++)
	{
		m_playerScore[i] = { charPoints[i],i + 1 };
	}

	//順位付け
	for (j = 0; j < m_maxPlayer; j++)
	{
		for (k = 0; k < m_maxPlayer; k++)
		{
			if (m_playerScore[j].m_point < m_playerScore[k].m_point)
			{
				m_playerScore[j].Rank += 1;
			}
		}
	}

	for (l = 0; l < m_maxPlayer; l++)
	{
		for (m = 0; m < m_maxPlayer; m++)
		{
			if (m_playerScore[l].m_point == m_playerScore[m].m_point)
			{
				if (m_playerScore[l].NameNum < m_playerScore[m].NameNum)
				{
					m_playerScore[m].Rank += 1;
				}

			}
		}
	}

	DeleteGO(game);

	//エフェクトの読み込み
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FireWorks, u"Assets/effect/Neutral_Enemy/fireworks.efk");

	SetCamera();

	Vector3 dir = { 0.1f,-1.0f,0.5f };
	dir.Normalize();
	Vector3 color = { 0.7f,0.7f,0.7f };
	g_renderingEngine->SetDirectionLight(0, dir, color);
	Vector3 ambient = { 0.6f,0.6f,0.6f };
	g_renderingEngine->SetAmbient(ambient);

	InitSprite();
	InitModel();
	InitSkyCube();

	m_bgm = NewGO<SoundSource>(0);

	SetCharacterState();

	SoundSource* dramJaan = NewGO<SoundSource>(0);
	dramJaan->Init(enSound_ResultDramRoll);
	dramJaan->SetVolume(ResultSpriteConst::SE_DRAM_ROLL_VOLUME);
	dramJaan->Play(false);

	return true;
}

void Result::Update()
{
	PlayAnimation();
	m_camera.Update();

	for (int i = 0; i < m_maxPlayer; i++)
	{
		m_knightModel[i].Update();
	}

	//最初の処理
	if (m_change == enChange_first)
	{
		Rank();
		if (m_fade->GetCurrentAlpha() <= 0.0f)
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

void Result::InitNameSprite()
{
	m_playerName.Init("Assets/sprite/gameUI/P1.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);

	if (m_gameMode == RenderingEngine::enGameMode_SoloPlay)
	{
		m_cpuName1.Init("Assets/sprite/gameUI/CPU1.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName2.Init("Assets/sprite/gameUI/CPU2.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName3.Init("Assets/sprite/gameUI/CPU3.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	}
	else if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
	{
		m_cpuName1.Init("Assets/sprite/gameUI/P2.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName2.Init("Assets/sprite/gameUI/CPU1.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName3.Init("Assets/sprite/gameUI/CPU2.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	}
	else if (m_gameMode == RenderingEngine::enGameMode_TrioPlay)
	{
		m_cpuName1.Init("Assets/sprite/gameUI/P2.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName2.Init("Assets/sprite/gameUI/P3.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName3.Init("Assets/sprite/gameUI/CPU1.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	}
	else if (m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
	{
		m_cpuName1.Init("Assets/sprite/gameUI/P2.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName2.Init("Assets/sprite/gameUI/P3.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
		m_cpuName3.Init("Assets/sprite/gameUI/P4.DDS", ResultSpriteConst::NAME_WIDTH, ResultSpriteConst::NAME_HEIGHT);
	}

	//"Player"
	m_playerName.SetPosition(m_spriteLerpStartPos[m_playerScore[0].Rank - 1]);
	m_playerName.SetScale(ResultSpriteConst::LOSER_PLAYER_NAME_SCALE);
	m_playerName.Update();
	//"CPU1"
	m_cpuName1.SetPosition(m_spriteLerpStartPos[m_playerScore[1].Rank - 1]);
	m_cpuName1.SetScale(ResultSpriteConst::LOSER_PLAYER_NAME_SCALE);
	m_cpuName1.Update();
	//"CPU2"
	m_cpuName2.SetPosition(m_spriteLerpStartPos[m_playerScore[2].Rank - 1]);
	m_cpuName2.SetScale(ResultSpriteConst::LOSER_PLAYER_NAME_SCALE);
	m_cpuName2.Update();
	//"CPU3"
	m_cpuName3.SetPosition(m_spriteLerpStartPos[m_playerScore[3].Rank - 1]);
	m_cpuName3.SetScale(ResultSpriteConst::LOSER_PLAYER_NAME_SCALE);
	m_cpuName3.Update();

	for (int i = 0; i < m_maxPlayer; i++)
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
}

void Result::InitSprite()
{
	//Resultの初期化
	m_spriteRender.Init("Assets/sprite/Result/titleBack_break.DDS", 1920.0f, 1080.0f);
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

	for (int i = 0; i < m_maxPlayer; i++)
	{
		//剣士の顔の初期化
		switch (i)
		{
		case(0):
			m_knightFace[i].Init("Assets/sprite/Result/PlayerFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
			m_knightFace[i].SetPosition(m_spriteLerpStartPos[i]);

			m_gameRank[i].Init("Assets/sprite/Result/Number_1st.DDS", ResultSpriteConst::GAMERANK_WIDTH, ResultSpriteConst::GAMERANK_HEIGHT);
			m_gameRank[i].SetPosition(m_spriteLerpStartPos[i]);
			break;
		case(1):
			m_knightFace[i].Init("Assets/sprite/Result/KnightRedFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
			m_knightFace[i].SetPosition(m_spriteLerpStartPos[i]);

			m_gameRank[i].Init("Assets/sprite/Result/Number_2nd.DDS", ResultSpriteConst::GAMERANK_WIDTH, ResultSpriteConst::GAMERANK_HEIGHT);
			m_gameRank[i].SetPosition(m_spriteLerpStartPos[i]);
			break;
		case(2):
			m_knightFace[i].Init("Assets/sprite/Result/KnightYellowFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
			m_knightFace[i].SetPosition(m_spriteLerpStartPos[i]);

			m_gameRank[i].Init("Assets/sprite/Result/Number_3rd.DDS", ResultSpriteConst::GAMERANK_WIDTH, ResultSpriteConst::GAMERANK_HEIGHT);
			m_gameRank[i].SetPosition(m_spriteLerpStartPos[i]);
			break;
		case(3):
			m_knightFace[i].Init("Assets/sprite/Result/KnightGreenFace.DDS", ResultSpriteConst::KNIGHT_FACE_WIDTH, ResultSpriteConst::KNIGHT_FACE_HEIGHT);
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

	InitNameSprite();

	//選択のカーソル
	m_choiceCursor.Init("Assets/sprite/Select/pointer_black.DDS", 220.0f, 220.0f);
	m_choiceCursor.SetPosition(ResultSpriteConst::GO_TITLE_POS + ResultSpriteConst::GOTITLE_ADD_CURSOR_POS);
	m_choiceCursor.SetScale(0.6f, 0.6f, 0.6f);
	Quaternion rot;
	rot.SetRotationDegZ(225.0f);
	m_choiceCursor.SetRotation(rot);
	m_choiceCursor.Update();
}

void Result::InitModel()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Knight/Knight_idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Win].Load("Assets/animData/Knight/Knight_Victory.tka");
	m_animationClips[enAnimationClip_Win].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Lose].Load("Assets/animData/Knight/Knight_Defeat.tka");
	m_animationClips[enAnimationClip_Lose].SetLoopFlag(true);
	m_animationClips[enAnimationClip_4th].Load("Assets/animData/Knight/Knight_Loser.tka");
	m_animationClips[enAnimationClip_4th].SetLoopFlag(true);

	//地面
	m_backGround.Init("Assets/modelData/background/stadium_ground.tkm");
	m_backGround.SetPosition(ResultSpriteConst::BACK_MODEL_POS);

	//壁
	m_backWall.Init("Assets/modelData/background/stadium05_Wall.tkm");
	m_backWall.SetPosition(ResultSpriteConst::BACK_MODEL_POS);

	m_knightModel[0].Init("Assets/modelData/character/Knight/Knight_Blue2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_knightModel[1].Init("Assets/modelData/character/Knight/Knight_Red2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_knightModel[2].Init("Assets/modelData/character/Knight/Knight_Yellow2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_knightModel[3].Init("Assets/modelData/character/Knight/Knight_Green2.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	//剣士
	Quaternion rot;
	rot.SetRotationDegY(ResultSpriteConst::KNIGHT_ROT);
	for (int i = 0; i < m_maxPlayer; i++)
	{
		//正面を向く
		m_knightModel[i].SetRotation(rot);
		m_knightModel[i].SetScale(1.3f, 1.3f, 1.3f);

		if (m_playerScore[i].Rank == 1)
		{
			m_knightModel[i].SetPosition(ResultSpriteConst::KNIGHTS_POS[0]);
		}
		else if (m_playerScore[i].Rank == 2)
		{
			m_knightModel[i].SetPosition(ResultSpriteConst::KNIGHTS_POS[1]);
		}
		else if (m_playerScore[i].Rank == 3)
		{
			m_knightModel[i].SetPosition(ResultSpriteConst::KNIGHTS_POS[2]);
		}
		else
		{
			m_knightModel[i].SetPosition(ResultSpriteConst::KNIGHTS_POS[3]);
		}
	}

	m_backGround.Update();
	m_backWall.Update();
	for (int i = 0; i < m_maxPlayer; i++)
	{
		m_knightModel[i].Update();
	}
}

void Result::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->SetPosition(ResultSpriteConst::SKYCUBE_POS);
	m_skyCube->SetScale(600.0f);
}

void Result::PlayEffect()
{
	//花火エフェクト再生
	m_fireWorks = NewGO<EffectEmitter>(0);
	m_fireWorks->Init(EnEFK::enEffect_FireWorks);
	m_fireWorks->SetScale(Vector3::One * 15.0f);
	m_fireWorks->SetPosition(ResultSpriteConst::FIREWORKS_EFFECT_POS);
	m_fireWorks->Play();
	m_fireWorks->Update();
}

void Result::SetCamera()
{
	m_camera.Init(*g_camera3D[0], 1.0f, true, 10.0f);

	m_camera.SetTarget(ResultSpriteConst::CAMERA_TARGET_POS);
	m_camera.SetPosition(ResultSpriteConst::CAMERA_POSITION);
	m_camera.Refresh();
	m_camera.Update();
}

void Result::SetCharacterState()
{
	bool winBGMFlag = false;
	for (int i = 0; i < m_gameMode; i++)
	{
		if (m_playerScore[i].Rank == 1)
		{
			m_bgm->Init(enSound_ResultBGM1);
			m_stayCharaState[i] = enCharacterState_Win;
			m_fireWorksPlayFlag = true;
			winBGMFlag = true;
		}
		else if (m_playerScore[i].Rank == 4)
		{
			m_bgm->Init(enSound_ResultBGM3);
			m_stayCharaState[i] = enCharacterState_4th;
		}
		else
		{
			m_bgm->Init(enSound_ResultBGM2);
			m_stayCharaState[i] = enCharacterState_Lose;
		}
	}

	int playerCount = 3;
	if (m_gameMode != RenderingEngine::enGameMode_SoloPlay)
	{
		if (winBGMFlag)
		{
			m_bgm->Init(enSound_ResultBGM1);
		}
		else
		{
			m_bgm->Init(enSound_ResultBGM3);
		}

		if (m_gameMode == RenderingEngine::enGameMode_DuoPlay)
		{
			playerCount = 2;
		}
		else if (m_gameMode == RenderingEngine::enGameMode_TrioPlay)
		{
			playerCount = 3;
		}
		else if (m_gameMode == RenderingEngine::enGameMode_QuartetPlay)
		{
			playerCount = 4;
		}
	}

	//CPUのステートを設定
	for (int i = playerCount; i < m_maxPlayer; i++)
	{
		if (m_playerScore[i].Rank == 1)
		{
			m_stayCharaState[i] = enCharacterState_Win;
		}
		else if (m_playerScore[i].Rank == 4)
		{
			m_stayCharaState[i] = enCharacterState_4th;
		}
		else
		{
			m_stayCharaState[i] = enCharacterState_Lose;
		}
	}
}

void Result::Rank()
{
	wchar_t Rank1[256];
	wchar_t Rank2[256];
	wchar_t Rank3[256];
	wchar_t Rank4[256];

	for (int i = 0; i < m_maxPlayer; i++)
	{
		switch (m_playerScore[i].Rank)
		{
		case 1:
			swprintf(Rank1, L"%2d", m_playerScore[i].m_point);
			m_playerRank1.SetText(Rank1);
			m_playerRank1.SetPosition(m_lerpStartPos[i]);
			m_playerRank1.SetColor(g_vec4White);
			m_playerRank1.SetScale(ResultSpriteConst::NO1_WORD_SCALE);
			m_playerRank1.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4Black);
			break;
		case 2:
			swprintf(Rank2, L"%2d", m_playerScore[i].m_point);
			m_playerRank2.SetText(Rank2);
			m_playerRank2.SetPosition(m_lerpStartPos[i]);
			m_playerRank2.SetColor(g_vec4White);
			m_playerRank2.SetScale(ResultSpriteConst::LOSER_WORD_SCALE);
			m_playerRank2.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4Black);
			break;
		case 3:
			swprintf(Rank3, L"%2d", m_playerScore[i].m_point);
			m_playerRank3.SetText(Rank3);
			m_playerRank3.SetPosition(m_lerpStartPos[i]);
			m_playerRank3.SetColor(g_vec4White);
			m_playerRank3.SetScale(ResultSpriteConst::LOSER_WORD_SCALE);
			m_playerRank3.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4Black);
			break;
		case 4:
			swprintf(Rank4, L"%2d", m_playerScore[i].m_point);
			m_playerRank4.SetText(Rank4);
			m_playerRank4.SetPosition(m_lerpStartPos[i]);
			m_playerRank4.SetColor(g_vec4White);
			m_playerRank4.SetScale(ResultSpriteConst::LOSER_WORD_SCALE);
			m_playerRank4.SetShadowParam(true, ResultSpriteConst::POINT_FONT_SHADOW_OFFSET, g_vec4Black);
			break;
		}
	}
}

void Result::MoveLerp()
{
	//補完率を増加させる
	if (m_complement <= 1.0f)
	{
		m_complement += ResultSpriteConst::COMPLEMENT;
	}
	else {
		//補完率が1を超えたらリセット
		m_complement = 0.0f;

		//一位以外の画像のとき"ジャン"の効果音を再生
		if (m_change != enChange_1st)
		{
			SoundSource* dramJaan = NewGO<SoundSource>(0);
			dramJaan->Init(enSound_ResultDramJaan);
			dramJaan->SetVolume(ResultSpriteConst::SE_DRAM_JAAN_VOLUME);
			dramJaan->Play(false);
		}

		//次のステートに移行
		switch (m_change)
		{
		case enChange_1st:
			m_change = enChange_stop;
			m_drawSelectSpriteFlag = true;
			m_charaState = m_stayCharaState;

			//プレイヤーが一位の場合、花火エフェクトを再生する
			if (m_fireWorksPlayFlag) {
				PlayEffect();
			}

			//BGM再生
			m_bgm->Play(true);
			m_bgm->SetVolume(m_bgmVolume);
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
			break;
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
	for (int i = 0; i < m_maxPlayer; i++)
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
		m_playerName.SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::LOSER_PLAYER_NAME_ALIGN);
		if (m_playerScore[0].Rank != 1)
		{
			m_knightFace[0].SetPosition(m_spriteLerpMoving[m_nowMoveRank] + ResultSpriteConst::KNIGHT_FACE_ADD_POS);
			m_knightFace[0].SetScale(ResultSpriteConst::LOSER_KNIGHT_FACE_SCALE);
		}
		else
		{
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

void Result::PlayAnimation()
{
	for (int i = 0; i < m_maxPlayer; i++)
	{
		switch (m_charaState[i])
		{
		case(enCharacterState_Idle):
			m_knightModel[i].PlayAnimation(enAnimationClip_Idle, 0.1f);
			break;
		case(enCharacterState_Win):
			m_knightModel[i].PlayAnimation(enAnimationClip_Win, 0.5f);
			break;
		case(enCharacterState_Lose):
			m_knightModel[i].PlayAnimation(enAnimationClip_Lose, 0.5f);
			break;
		case(enCharacterState_4th):
			m_knightModel[i].PlayAnimation(enAnimationClip_4th, 0.5f);
			break;
		}
	}
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
		se->Init(enSound_TitleOK);
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
		se->Init(enSound_TitleOK);
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
	m_backGround.Draw(rc);
	m_backWall.Draw(rc);

	//m_spriteRender.Draw(rc);	//背景
	m_resultLogo.Draw(rc);		//リザルトロゴ

	//名前の背景画像
	//ポイントの単位
	for (int i = 0; i < m_maxPlayer; i++)
	{
		m_knightModel[i].Draw(rc);
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