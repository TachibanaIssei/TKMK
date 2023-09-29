#include "stdafx.h"
#include "Tittle.h"
#include "CharacterSelect.h"
#include "Game.h"
#include "Fade.h"

namespace TitleConst
{
	const Vector3 START_IMAGE_SCALE = Vector3(0.4f, 0.4f, 1.0f);

	const Vector3 START_CURSOR_POS = { -730.0f,-130.0f,0.0f };			//スタート選択時の剣のカーソルの位置
	const Vector3 HOWTOPLAY_CURSOR_POS = { -830.0f,-245.0f,0.0f };		//遊び方選択時の剣のカーソルの位置
}

Tittle::Tittle()
{

}

Tittle::~Tittle()
{

}

bool Tittle::Start()
{
	InitSprite();
	ResistSound();

	m_bgm = NewGO<SoundSource>(0);

	return true;
}

void Tittle::Update()
{
	Scene();
	FadeSprite();

	m_timer++;

	SpriteUpdate();
}

void Tittle::InitSprite()
{
	//選択のカーソル
	m_choiceCursor.Init("Assets/sprite/Select/pointer_black.DDS", 220.0f, 220.0f);
	m_choiceCursor.SetPosition(TitleConst::START_CURSOR_POS);
	m_choiceCursor.SetScale(0.6f, 0.6f, 0.6f);
	Quaternion rot;
	rot.SetRotationDegZ(225.0f);
	m_choiceCursor.SetRotation(rot);
	m_choiceCursor.Update();

	//背景の初期化
	m_spriteRender.Init("Assets/sprite/Title/first_title/titleBack.DDS", 1920.0f, 1080.0f);
	m_spriteRender.SetPosition(g_vec3Zero);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();

	//PressAButton
	m_pressAButton.Init("Assets/sprite/Title/first_title/PressAButton.DDS", 700.0f, 150.0f);
	m_pressAButton.SetPosition(0.0f, -300.0f, 0.0f);
	m_pressAButton.Update();

	//titleのロゴ
	m_titleLogo.Init("Assets/sprite/Title/first_title/gameTitleLogo_SWORDARENA.DDS", 1200.0f, 200.0f);
	m_titleLogo.SetPosition(m_titleLogoPosition);
	m_titleLogo.SetMulColor(m_color);
	m_titleLogo.SetScale(m_titleLogoScale);
	m_titleLogo.Update();

	//titleの剣(右)
	m_titleswordwhite.Init("Assets/sprite/Title/first_Title/sword_right.DDS", 300.0f, 400.0f);
	m_titleswordwhite.SetPosition(m_Toprightfirstposition);
	m_titleswordwhite.SetScale(g_vec3One);
	m_titleswordwhite.Update();

	//titleの剣(左)
	m_titleswordbrack.Init("Assets/sprite/Title/first_Title/sword_left.DDS", 300.0f, 400.0f);
	m_titleswordbrack.SetPosition(m_Topleftfirstposition);
	m_titleswordbrack.SetScale(g_vec3One);
	m_titleswordbrack.Update();

	//火花
	m_fire.Init("Assets/sprite/Title/first_title/fireFlower_SWORDARENA.DDS", 1500.0f, 400.0f);
	m_fire.SetPosition(m_titleLogoPosition);
	m_fire.SetMulColor(m_color);
	m_fire.SetScale(g_vec3One);
	m_fire.Update();

	//Selectの初期化
	//"START"非選択
	m_start.Init("Assets/sprite/Title/ModeScene/START_white.DDS", 300.0f, 90.0f);
	m_start.SetPosition(m_firstPosition);
	m_start.SetScale(g_vec3One);
	m_start.Update();
	//"START"選択
	m_startST.Init("Assets/sprite/Title/ModeScene/START_color.DDS", 300.0f, 90.0f);
	m_startST.SetPosition(m_firstPosition);
	m_startST.SetScale(g_vec3One);
	m_startST.Update();
	//"HOWTOPLAY"非選択
	m_operation.Init("Assets/sprite/Title/ModeScene/HOWTOPLAY_white.DDS", 500.0f, 90.0f);
	m_operation.SetPosition(m_firstPosition);
	m_operation.SetScale(g_vec3One);
	m_operation.Update();
	//"HOWTOPLAY"選択
	m_operationST.Init("Assets/sprite/Title/ModeScene/HOWTOPLAY_color.DDS", 500.0f, 90.0f);
	m_operationST.SetPosition(m_firstPosition);
	m_operationST.SetMulColor(m_colorST);
	m_operationST.SetScale(g_vec3One);
	m_operationST.Update();
	//"OPTION"非選択
	/*m_option.Init("Assets/sprite/Title/ModeScene/OPTION_white.DDS", 370.0f, 90.0f);
	m_option.SetPosition(m_firstPosition);
	m_option.SetScale(g_vec3One);
	m_option.Update();*/
	//"OPTION"選択
	/*m_optionST.Init("Assets/sprite/Title/ModeScene/OPTION_color.DDS", 370.0f, 90.0f);
	m_optionST.SetPosition(m_firstPosition);
	m_optionST.SetMulColor(m_colorST);
	m_optionST.SetScale(g_vec3One);
	m_optionST.Update();*/

	//操作説明画像
	m_operationPic.Init("Assets/sprite/Title/nonGame_howTo.DDS", 1920.0f, 1080.0f);
	m_operationPic.SetPosition(g_vec3Zero);
	m_operationPic.SetScale(g_vec3One);
	m_operationPic.Update();

	m_operationInGame.Init("Assets/sprite/Title/Game_howTo.DDS", 1920.0f, 1080.0f);
	m_operationInGame.SetPosition(g_vec3Zero);
	m_operationInGame.Update();

	m_ghostExplanation.Init("Assets/sprite/Title/ghost_explanation.DDS", 1920.0f, 1080.0f);
	m_ghostExplanation.SetPosition(g_vec3Zero);
	m_ghostExplanation.Update();

	//キャラクター説明画像
	//セレクト画像
	m_Opchoice.Init("Assets/sprite/CharaExplanation/CharaOpSelect.DDS", 1000.0f, 100.0f);
	m_Opchoice.SetPosition(m_firstPosition);
	m_Opchoice.SetScale(g_vec3One);
	m_Opchoice.Update();
	//剣士
	m_KnightOp.Init("Assets/sprite/CharaExplanation/KnightOP.DDS", 1920.0f, 1080.0f);
	m_KnightOp.SetPosition(m_firstPosition);
	m_KnightOp.SetScale(g_vec3One);
	m_KnightOp.Update();
	//魔法使い
	m_WizardOp.Init("Assets/sprite/CharaExplanation/wizardOP.DDS", 1920.0f, 1080.0f);
	m_WizardOp.SetPosition(m_firstPosition);
	m_WizardOp.SetScale(g_vec3One);
	m_WizardOp.Update();

	//Selectのイラスト
	//"START"の時のイラスト
	m_iluststart.Init("Assets/sprite/Title/ModeScene/StageImage.DDS", 1901.0f, 664.0f);
	m_iluststart.SetPosition(m_RightfirstPosition);
	m_iluststart.SetScale(TitleConst::START_IMAGE_SCALE);
	m_iluststart.Update();
	//"START"の時の説明文
	m_iluststartOp.Init("Assets/sprite/Title/ModeScene/Start_Setumei.DDS", 850.0f, 110.0f);
	m_iluststartOp.SetPosition(m_RightfirstPosition);
	m_iluststartOp.SetScale(g_vec3One);
	m_iluststartOp.Update();
	//"HOWTOPLAY"の時のイラスト
	m_ilusthowtoplay.Init("Assets/sprite/Title/ModeScene/llust_HowToPlay.DDS", 500.0f, 500.0f);
	m_ilusthowtoplay.SetPosition(m_RightfirstPosition);
	m_ilusthowtoplay.SetScale(g_vec3One);
	m_ilusthowtoplay.Update();
	//"HOWTOPLAY"の時の説明文
	m_ilusthowtoplayOp.Init("Assets/sprite/Title/ModeScene/HowToPlay_Setumei.DDS", 700.0f, 100.0f);
	m_ilusthowtoplayOp.SetPosition(m_RightfirstPosition);
	m_ilusthowtoplayOp.SetScale(g_vec3One);
	m_ilusthowtoplayOp.Update();
	//"OPTION"の時のイラスト
	m_ilustoption.Init("Assets/sprite/Title/ModeScene/llust_Option.DDS", 500.0f, 500.0f);
	m_ilustoption.SetPosition(m_RightfirstPosition);
	m_ilustoption.SetScale(g_vec3One);
	m_ilustoption.Update();
	//"OPTION"の時の説明文
	m_ilustoptionOp.Init("Assets/sprite/Title/ModeScene/Option_Setumei.DDS", 550.0f, 100.0f);
	m_ilustoptionOp.SetPosition(m_RightfirstPosition);
	m_ilustoptionOp.SetScale(g_vec3One);
	m_ilustoptionOp.Update();
}

void Tittle::ResistSound()
{
	//BGMの設定
	g_soundEngine->ResistWaveFileBank(enSound_Title_Fanfare, "Assets/sound/titleBGM/fanfare1.wav");
	g_soundEngine->ResistWaveFileBank(enSound_Title_BGM, "Assets/sound/titleBGM/titleBGM2.wav");
	//選択音
	g_soundEngine->ResistWaveFileBank(enSound_Title_Choise, "Assets/sound/sentaku/sentaku4.wav");
	//斬撃音
	g_soundEngine->ResistWaveFileBank(enSound_Title_Slash2, "Assets/sound/titleBGM/titleSE/zangeki2.wav");
	g_soundEngine->ResistWaveFileBank(enSound_Title_Slash1, "Assets/sound/titleBGM/titleSE/zangeki1.wav");
	//キャラ決定音
	g_soundEngine->ResistWaveFileBank(enSound_OK, "Assets/sound/sentaku/kettei3.wav");
	//キャンセル音
	g_soundEngine->ResistWaveFileBank(enSound_Cancel, "Assets/sound/sentaku/cancel.wav");
	//タイトル画面決定音
	g_soundEngine->ResistWaveFileBank(enSound_TitleOK, "Assets/sound/titleBGM/titleketei.wav");
}

void Tittle::SpriteUpdate()
{
	m_choiceCursor.Update();
	m_spriteRender.Update();
	m_titleLogo.Update();
	m_pressAButton.Update();
	m_titleLogo.Update();
	m_fire.Update();
	m_start.Update();
	m_operation.Update();
	//m_option.Update();
	m_operationPic.Update();
	m_iluststart.Update();
	m_iluststartOp.Update();
	m_ilusthowtoplay.Update();
	m_ilusthowtoplayOp.Update();
	m_ilustoption.Update();
	m_ilustoptionOp.Update();
	m_titleswordwhite.Update();
	m_titleswordbrack.Update();
}

//シーンセレクト
void Tittle::Scene()
{
	//もし最初のPressAの画面だったら
	if (m_titleScene == enTitleScene_PressAScene)
	{
		m_fadeSeem = false;
		if (m_linear == enLinear_sword1)
		{
			if (swordright >= 1.1f)
			{
				swordright -= 0.11;
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(enSound_Title_Slash1);
				se->SetVolume(SEVolume);
				se->Play(false);
				m_linear = enLinear_sword2;
			}

			//線形補間
			m_swordright.Lerp(swordright, m_Toprightfirstposition, m_swordPosition);
			//線形補完したものをSetPositionに入れる
			m_titleswordwhite.SetPosition(m_swordright);

			m_RightPos = m_swordright;

			//補完率
			swordright += 0.07f;
		}

		else if (m_linear == enLinear_sword2)
		{
			if (swordleft >= 1.1f)
			{
				swordleft -= 0.11;
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(enSound_Title_Slash1);
				se->SetVolume(SEVolume);
				se->Play(false);
				m_linear = enLinear_delay;
			}
			//線形補間
			m_swordleft.Lerp(swordleft, m_Topleftfirstposition, m_swordPosition);
			//線形補完したものをSetPositionに入れる
			m_titleswordbrack.SetPosition(m_swordleft);

			m_LeftPos = m_swordleft;

			//補完率
			swordleft += 0.07f;
		}
		else if (m_linear == enLinear_delay)
		{
			if (delaytime > 20)
			{
				m_linear = enLinear_Logo;
			}
			//遅らせる時間
			delaytime++;
		}
		else if (m_linear == enLinear_Logo)
		{
			if (firstLogo > 0.8f)
			{
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(enSound_Title_Slash2);
				se->SetVolume(SEVolume);
				se->Play(false);
				m_linear = enLinear_End;
			}
			m_titlefadeSeem = true;
		
			//線形補間
			m_firsttitleScale.Lerp(firstLogo, m_firsttitleScale, m_titleLogoScale);
			m_fireScale.Lerp(firstLogo, m_firsttitleScale, m_titleLogoScale);

			//線形補間したものをSetPositionに入れる
			m_titleLogo.SetScale(m_firsttitleScale);
			m_fire.SetScale(m_fireScale);
			//補完率
			firstLogo += 0.03f;
		}
		else if(m_linear == enLinear_End)
		{
			m_fadeSeem = true;
			m_titleanim = true;
		}
		//タイトル画面の時の場所に移動する
		/*m_titleLogo.SetPosition(m_titleLogoPosition);
		m_titleLogo.SetScale(m_titleLogoScale);*/
		m_fire.SetPosition(m_titleLogoPosition);
		//画面外の下に移動する
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		//m_option.SetPosition(m_firstPosition);
	}
	//もしSelect画面だったら
	if (m_titleScene == enTitleScene_Change)
	{
		if (LogoComplement <= 1.0f)
		{
			if (fadetime > 30)
			{
				m_fadeSeem = false;
			}
			if (m_timer % 2 == 0 && m_fadeSeem == false)
			{
				//線形補完
				m_operationPosition.Lerp(LogoComplement, m_firstPosition, m_Central);
				m_startPosition.Lerp(LogoComplement, m_firstPosition, m_Top);
				m_optionPosition.Lerp(LogoComplement, m_firstPosition, m_Under);
				m_LogoPosition.Lerp(LogoComplement, m_titleLogoPosition, m_selectLogoPosition + m_LeftLogo);
				m_LogoScale.Lerp(LogoComplement, m_titleLogoScale, m_selectLogoScale);
				m_fireScale.Lerp(LogoComplement, m_titlefireScale, m_selectfireScale);
				m_ilust.Lerp(LogoComplement, m_RightfirstPosition, m_selectilust);
				m_ilustOp.Lerp(LogoComplement, m_RightfirstPosition, m_selectilustOp);
				m_swordright.Lerp(LogoComplement, m_RightPos, m_selectLogoPosition);
				m_swordleft.Lerp(LogoComplement, m_LeftPos, m_selectLogoPosition);
				m_swordscale.Lerp(LogoComplement, m_titleswordscale, m_selectswordscale);
				//線形補完したものをSetPositionに入れる
				m_fire.SetPosition(m_LogoPosition);
				m_fire.SetScale(m_fireScale);
				m_operation.SetPosition(m_operationPosition);
				m_start.SetPosition(m_startPosition);
				//m_option.SetPosition(m_optionPosition);
				m_titleLogo.SetPosition(m_LogoPosition);
				m_titleLogo.SetScale(m_LogoScale);
				m_iluststart.SetPosition(m_ilust);
				m_iluststartOp.SetPosition(m_ilustOp);
				m_titleswordwhite.SetPosition(m_swordright);
				m_titleswordbrack.SetPosition(m_swordleft);
				m_titleswordwhite.SetScale(m_swordscale);
				m_titleswordbrack.SetScale(m_swordscale);

				//補完率
				LogoComplement += 0.05f;
			}
			fadetime += 1;
		}
	}
	if (m_titleScene == enTitleScene_Select)
	{
		//もし操作説明画面やキャラクター説明画面が見えず、またセレクト画面だったら表示する
		if (m_operationLook == enOperationLook_UnSeem || m_characterOpLook == enCharacterOpLook_UnSeem || LogoComplement < 1.0f)
		{
			m_titleLogo.SetPosition(m_selectLogoPosition + m_LeftLogo);
			m_start.SetPosition(m_Top);
			m_operation.SetPosition(m_Central);
			//m_option.SetPosition(m_Under);
			m_titleLogo.SetScale(m_selectLogoScale);
		}
		Select();
		Ilust();
		Operation();
		CharacterOp();
		//ゲーム画面への遷移
		if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Start) {
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_TitleOK);
			se->Play(false);
			se->SetVolume(1.0f);

			m_fade = FindGO<Fade>("fade");
			m_fade->StartFadeIn(1.0f);

			//game画面へ遷移
			CharacterSelect* characterSelect = NewGO<CharacterSelect>(0, "game");
			DeleteGO(this);
			DeleteGO(m_bgm);
		}
	}
	//最初の画面でAボタンを押されたら
	if (m_titleScene == enTitleScene_PressAScene && m_titleanim == true && g_pad[0]->IsTrigger(enButtonA))
	{
		//線形変換に移る
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_TitleOK);
		se->Play(false);
		se->SetVolume(1.0f);
		titleScene = 1;
		m_isWaitFadeout = true;
	}
	if (m_titleScene == enTitleScene_Change && LogoComplement > 1.0f)
	{
		//セレクト画面に移る
		titleScene = 2;
	}
	
	switch (titleScene)
	{
	case 0:
		m_titleScene = enTitleScene_PressAScene;
		m_isComplement = true;
		break;
	case 1:
		m_titleScene = enTitleScene_Change;
		if (m_bgm->IsPlaying() == false)
		{
			m_bgm->Init(enSound_Title_BGM);
			m_bgm->Play(true);
			m_bgm->SetVolume(0.5f);
		}
		m_isComplement = true;
		break;
	case 2:
		m_titleScene = enTitleScene_Select;
		m_isComplement = false;
		break;
	}
}

//セレクトカーソル
void Tittle::Select()
{
	if (m_operationLook == enOperationLook_UnSeem && m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			selectPosition++;
			if (selectPosition > 1)
				selectPosition = 0;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			selectPosition--;
			if (selectPosition < 0)
				selectPosition = 1;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
	}

	switch (selectPosition)
	{
	case 0:
		//"はじめる"
		m_tSelectPosition = enSelectPosition_Start;
		//色が付いた"Start"を持ってくる
		m_startST.SetPosition(m_Top);
		//それ以外は画面外にする
		m_operationST.SetPosition(m_firstPosition);
		//m_optionST.SetPosition(m_firstPosition);

		//剣のカーソルの位置を設定
		m_choiceCursor.SetPosition(TitleConst::START_CURSOR_POS);
		break;
	case 1:
		//"キャラクター説明"
		m_tSelectPosition = enSelectPosition_Operation;
		//色が付いた"HOWTOPLAY"を持ってくる
		m_operationST.SetPosition(m_Central);
		//それ以外は画面外にする
		m_startST.SetPosition(m_firstPosition);
		//m_optionST.SetPosition(m_firstPosition);

		//剣のカーソルの位置を設定
		m_choiceCursor.SetPosition(TitleConst::HOWTOPLAY_CURSOR_POS);
		break;
	case 2:
		//"操作説明"
		m_tSelectPosition = enSelectPosition_Option;
		//色が付いた"Option"を持ってくる
		//m_optionST.SetPosition(m_Under);
		//それ以外は画面外にする
		m_startST.SetPosition(m_firstPosition);
		m_operationST.SetPosition(m_firstPosition);
		break;
	}	

	m_start.Update();
	m_startST.Update();
	m_operation.Update();
	m_operationST.Update();
	//m_option.Update();
	//m_optionST.Update();
}
//操作説明
void Tittle::Operation()
{
	//説明画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonB) && m_operationLook == enOperationLook_Seem) {
		//キャンセル音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_Cancel);
		se->Play(false);
		se->SetVolume(1.0f);
		//説明画面を非表示
		m_operationLook = enOperationLook_UnSeem;
		m_operationPageNumber = enOperationpage_Num;
	}

	//説明画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Operation && m_operationLook == enOperationLook_UnSeem) {
		//決定音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_TitleOK);
		se->Play(false);
		se->SetVolume(1.0f);
		//説明画面を表示
		m_operationLook = enOperationLook_Seem;
		m_operationPageNumber = enOperationPage_nonGame;
	}
	
	//ページの切り替え処理
	if (m_operationLook == enOperationLook_Seem)
	{
		if (g_pad[0]->IsTrigger(enButtonLeft))
		{
			m_operationPageNumber--;
			if (m_operationPageNumber < 0)
			{
				m_operationPageNumber = 0;
				return;
			}

			//選択音
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonRight))
		{
			m_operationPageNumber++;
			if (m_operationPageNumber > 2)
			{
				m_operationPageNumber = 2;
				return;
			}

			//選択音
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
	}
	else
	{
		m_operationPageNumber = 3;
	}
	
}

//キャラクター説明
void Tittle::CharacterOp()
{
	//キャラクター説明画面からタイトルへの遷移
	if (g_pad[0]->IsTrigger(enButtonB) && m_characterOpLook == enCharacterOpLook_Seem)
	{
		//選択音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_TitleOK);
		se->Play(false);
		se->SetVolume(1.0f);
		//キャラクター説明画面を非表示
		m_characterOpLook = enCharacterOpLook_UnSeem;
	}

	//キャラクター説明画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Option && m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		//選択音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(enSound_TitleOK);
		se->Play(false);
		se->SetVolume(1.0f);
		//キャラクター説明画面を表示
		m_characterOpLook = enCharacterOpLook_Seem;
	}

	//表示中の処理
	if (m_characterOpLook == enCharacterOpLook_Seem)
	{
		m_fire.SetScale(0.6f, 1.0f, 0.0f);
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		//m_option.SetPosition(m_firstPosition);
		//十字キー左を押したら
		if (g_pad[0]->IsTrigger(enButtonLeft) && characterOpPosition != 0)
		{
			characterOpPosition--;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		//十字キー右を押したら
		if (g_pad[0]->IsTrigger(enButtonRight) && characterOpPosition < 3)
		{
			characterOpPosition++;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(enSound_Title_Choise);
			se->Play(false);
			se->SetVolume(1.0f);
		}

		switch (characterOpPosition)
		{
		case 0:
			m_characterOpPosition = enCharacterOpPosition_Knight;
			m_WizardOp.SetPosition(m_firstPosition);
			m_KnightOp.SetPosition(m_opPosition);
			break;
		case 1:
			m_characterOpPosition = enCharacterOpPosition_Wizard;
			m_KnightOp.SetPosition(m_firstPosition);
			m_WizardOp.SetPosition(m_opPosition);
			break;
		case 2:
			m_characterOpPosition = enCharacterOpPosition_Zombie;
			break;
		case 3:
			m_characterOpPosition = enCharacterOpPosition_Mitei;
			m_fire.SetPosition(m_MiteiCursor);
			break;
		}
		m_Opchoice.SetPosition(0.0f, 450.0f, 0.0f);
		m_start.Update();
		m_operation.Update();
		//m_option.Update();
	}
	//キャラ説明画面を非表示にするときの処理
	if (m_characterOpLook == enCharacterOpLook_UnSeem)
	{
		m_KnightOp.SetPosition(m_firstPosition);
		m_WizardOp.SetPosition(m_firstPosition);
		m_Opchoice.SetPosition(m_firstPosition);
	}
	m_KnightOp.Update();
	m_WizardOp.Update();
	m_Opchoice.Update();
	m_fire.Update();
}

//選択画面のイラスト
void Tittle::Ilust()
{
	switch (m_tSelectPosition)
	{
	case enSelectPosition_Start:
		//指定の位置に移動させる
		m_iluststart.SetPosition(m_selectilust);
		m_iluststartOp.SetPosition(m_selectilustOp);
		//それ以外は画面外に移動
		m_ilusthowtoplay.SetPosition(m_RightfirstPosition);
		m_ilusthowtoplayOp.SetPosition(m_RightfirstPosition);
		m_ilustoption.SetPosition(m_RightfirstPosition);
		m_ilustoptionOp.SetPosition(m_RightfirstPosition);
		break;
	case enSelectPosition_Operation:
		//指定の位置に移動させる
		m_ilusthowtoplay.SetPosition(m_selectilust);
		m_ilusthowtoplayOp.SetPosition(m_selectilustOp);
		//それ以外は画面外に移動
		m_iluststart.SetPosition(m_RightfirstPosition);
		m_iluststartOp.SetPosition(m_RightfirstPosition);
		m_ilustoption.SetPosition(m_RightfirstPosition);
		m_ilustoptionOp.SetPosition(m_RightfirstPosition);
		break;
	case enSelectPosition_Option:
		//指定の位置に移動させる
		m_ilustoption.SetPosition(m_selectilust);
		m_ilustoptionOp.SetPosition(m_selectilustOp);
		//それ以外は画面外に移動
		m_iluststart.SetPosition(m_RightfirstPosition);
		m_iluststartOp.SetPosition(m_RightfirstPosition);
		m_ilusthowtoplay.SetPosition(m_RightfirstPosition);
		m_ilusthowtoplayOp.SetPosition(m_RightfirstPosition);
		break;
		break;
	}
}
void Tittle::FadeSprite()
{
	if (m_fadeSeem == true)
	{
		//PressAのフェード
		if (m_isWaitFadeout)
		{
			//フェードの間隔（短い）
			m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
		}
		else
		{
			//フェードの間隔（長い）
			m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
		}
		m_pressAButton.SetMulColor({ 1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha)) });
	}
	else
	{
		//透明にして見えなくする
		m_pressAButton.SetMulColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	}
	if (m_titlefadeSeem == true)
	{
		m_titleLogo.SetMulColor(m_colorST);
		m_fire.SetMulColor(m_colorST);
	}
	else
	{
		m_titleLogo.SetMulColor(m_color);
		m_fire.SetMulColor(m_color);
	}
}

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_pressAButton.Draw(rc);
	m_iluststart.Draw(rc);
	m_iluststartOp.Draw(rc);
	m_ilusthowtoplay.Draw(rc);
	m_ilusthowtoplayOp.Draw(rc);
	m_ilustoption.Draw(rc);
	m_ilustoptionOp.Draw(rc);
	m_titleswordwhite.Draw(rc);
	m_titleswordbrack.Draw(rc);
	m_fire.Draw(rc);
	m_titleLogo.Draw(rc);
	m_start.Draw(rc);
	m_startST.Draw(rc);
	m_operation.Draw(rc);
	m_operationST.Draw(rc);
	//m_option.Draw(rc);
	//m_optionST.Draw(rc);
	//線形補間中は表示しない
	if(!m_isComplement) m_choiceCursor.Draw(rc);

	switch (m_operationPageNumber)
	{
	case(enOperationPage_nonGame):
		m_operationPic.Draw(rc);
		break;
	case(enOperationPage_InGame):
		m_operationInGame.Draw(rc);
		break;
	case(enOperationPage_Ghost):
		m_ghostExplanation.Draw(rc);
		break;
	default:
		break;
	}

	m_KnightOp.Draw(rc);
	m_WizardOp.Draw(rc);
	m_Opchoice.Draw(rc);
}
