#include "stdafx.h"
#include "Tittle.h"
#include "CharacterSelect.h"
#include "Game.h"

Tittle::Tittle()
{

}

Tittle::~Tittle()
{

}

bool Tittle::Start()
{
	//背景の初期化
	m_spriteRender.Init("Assets/sprite/Title/first_title/titleBack.DDS",1920.0f,1080.0f);
	m_spriteRender.SetPosition(0.0f, 0.0f, 0.0f);
	m_spriteRender.SetScale(g_vec3One);
	m_sRotation.SetRotationZ(0.0f);
	m_spriteRender.SetRotation(m_sRotation);
	m_spriteRender.Update();
	
	//titleのロゴ
	m_titleLogo.Init("Assets/sprite/Title/first_title/gameTitleLogo.DDS", 400.0f,200.0f);
	m_titleLogo.SetPosition(m_firstPosition);
	m_titleLogo.SetScale(m_titleLogoScale);
	m_titleLogo.Update();

	//火花
	m_fire.Init("Assets/sprite/Title/first_title/fireFlower.DDS", 1500.0f, 400.0f);
	m_fire.SetPosition(m_firstPosition);
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
	m_option.Init("Assets/sprite/Title/ModeScene/OPTION_white.DDS", 370.0f, 90.0f);
	m_option.SetPosition(m_firstPosition);
	m_option.SetScale(g_vec3One);
	m_option.Update();
	//"OPTION"選択
	m_optionST.Init("Assets/sprite/Title/ModeScene/OPTION_color.DDS", 370.0f, 90.0f);
	m_optionST.SetPosition(m_firstPosition);
	m_optionST.SetMulColor(m_colorST);
	m_optionST.SetScale(g_vec3One);
	m_optionST.Update();

	//操作説明画像
	m_operationPic.Init("Assets/sprite/Controller.DDS", 1920.0f, 1080.0f);
	m_operationPic.SetPosition(m_firstPosition);
	m_operationPic.SetScale(g_vec3One);
	m_operationPic.Update();

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
	m_iluststart.Init("Assets/sprite/Title/ModeScene/llust_Start.DDS", 500.0f, 600.0f);
	m_iluststart.SetPosition(m_RightfirstPosition);
	m_iluststart.SetScale(g_vec3One);
	m_iluststart.Update();
	//"START"の時の説明文
	m_iluststartOp.Init("Assets/sprite/Title/ModeScene/Start_Setumei.DDS", 850.0f, 100.0f);
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

	//BGMの設定
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/gameBGM/TitleBGM1.wav");
	//選択音
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/sentaku/sentaku4.wav");

	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(1);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.5f);

	return true;
}

void Tittle::Update()
{
	Scene();

	m_timer++;

	m_spriteRender.Update();
	m_titleLogo.Update();
	m_fire.Update();
	m_start.Update();
	m_operation.Update();
	m_option.Update();
	m_operationPic.Update();
	m_iluststart.Update();
	m_iluststartOp.Update();
	m_ilusthowtoplay.Update();
	m_ilusthowtoplayOp.Update();
	m_ilustoption.Update();
	m_ilustoptionOp.Update();
}

//シーンセレクト
void Tittle::Scene()
{
	//もし最初のPressAの画面だったら
	if (m_titleScene == enTitleScene_PressAScene)
	{
		//タイトル画面の時の場所に移動する
		m_titleLogo.SetPosition(m_titleLogoPosition);
		m_titleLogo.SetScale(m_titleLogoScale);
		m_fire.SetPosition(m_titleLogoPosition);
		//画面外の下に移動する
		m_start.SetPosition(m_firstPosition);
		m_operation.SetPosition(m_firstPosition);
		m_option.SetPosition(m_firstPosition);
	}
	//もしSelect画面だったら
	if (m_titleScene == enTitleScene_Change)
	{
		if (LogoComplement < 1.0f)
		{
			if (m_timer % 2 == 0)
			{
				//線形補完
				m_operationPosition.Lerp(LogoComplement, m_firstPosition, m_Central);
				m_startPosition.Lerp(LogoComplement, m_firstPosition, m_Top);
				m_optionPosition.Lerp(LogoComplement, m_firstPosition, m_Under);
				m_LogoPosition.Lerp(LogoComplement, m_titleLogoPosition, m_selectLogoPosition);
				m_LogoScale.Lerp(LogoComplement, m_titleLogoScale, m_selectLogoScale);
				m_fireScale.Lerp(LogoComplement, m_titlefireScale, m_selectfireScale);
				m_ilust.Lerp(LogoComplement, m_RightfirstPosition, m_selectilust);
				m_ilustOp.Lerp(LogoComplement, m_RightfirstPosition, m_selectilustOp);

				//線形補完したものをSetPositionに入れる
				m_fire.SetPosition(m_LogoPosition);
				m_fire.SetScale(m_fireScale);
				m_operation.SetPosition(m_operationPosition);
				m_start.SetPosition(m_startPosition);
				m_option.SetPosition(m_optionPosition);
				m_titleLogo.SetPosition(m_LogoPosition);
				m_titleLogo.SetScale(m_LogoScale);
				m_iluststart.SetPosition(m_ilust);
				m_iluststartOp.SetPosition(m_ilustOp);

				//補完率
				LogoComplement += 0.03f;

			}
		}
	}
	if (m_titleScene == enTitleScene_Select)
	{
		//もし操作説明画面やキャラクター説明画面が見えず、またセレクト画面だったら表示する
		if (m_operationLook == enOperationLook_UnSeem || m_characterOpLook == enCharacterOpLook_UnSeem || LogoComplement < 1.0f)
		{
			m_titleLogo.SetPosition(m_selectLogoPosition);
			m_start.SetPosition(m_Top);
			m_operation.SetPosition(m_Central);
			m_option.SetPosition(m_Under);
			m_titleLogo.SetScale(m_selectLogoScale);
		}
		Select();
		Ilust();
		Operation();
		CharacterOp();
		//ゲーム画面への遷移
		if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Start) {
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
			//game画面へ遷移
			CharacterSelect* characterSelect = NewGO<CharacterSelect>(0, "game");
			DeleteGO(this);
			DeleteGO(m_bgm);
		}
	}
	//最初の画面でAボタンを押されたら
	if (m_titleScene == enTitleScene_PressAScene && g_pad[0]->IsTrigger(enButtonA))
	{
		//線形変換に移る
		titleScene = 1;
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
		break;
	case 1:
		m_titleScene = enTitleScene_Change;
		break;
	case 2:
		m_titleScene = enTitleScene_Select;
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
			if (selectPosition > 2)
				selectPosition = 0;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			selectPosition--;
			if (selectPosition < 0)
				selectPosition = 2;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
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
		m_optionST.SetPosition(m_firstPosition);
		break;
	case 1:
		//"操作説明"
		m_tSelectPosition = enSelectPosition_Option;
		//色が付いた"Option"を持ってくる
		m_optionST.SetPosition(m_Under);
		//それ以外は画面外にする
		m_startST.SetPosition(m_firstPosition);
		m_operationST.SetPosition(m_firstPosition);
		break;
	case 2:
		//"キャラクター説明"
		m_tSelectPosition = enSelectPosition_Operation;
		//色が付いた"HOWTOPLAY"を持ってくる
		m_operationST.SetPosition(m_Central);
		//それ以外は画面外にする
		m_startST.SetPosition(m_firstPosition);
		m_optionST.SetPosition(m_firstPosition);
		break;
	}	

	m_start.Update();
	m_startST.Update();
	m_operation.Update();
	m_operationST.Update();
	m_option.Update();
	m_optionST.Update();
}
//操作説明
void Tittle::Operation()
{
	//説明画面からタイトル画面への遷移
	if (g_pad[0]->IsTrigger(enButtonB) && m_operationLook == enOperationLook_Seem) {
		//選択音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//説明画面を非表示
		m_operationLook = enOperationLook_UnSeem;
	}

	//説明画面への遷移
	if (g_pad[0]->IsTrigger(enButtonA) && m_tSelectPosition == enSelectPosition_Operation && m_operationLook == enOperationLook_UnSeem) {
		//選択音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(1.0f);
		//説明画面を表示
		m_operationLook = enOperationLook_Seem;
	}
	//操作画面を表示
	if (m_operationLook == enOperationLook_Seem)
	{
		m_operationPic.SetPosition(m_opPosition);
	}
	//操作画面を非表示
	if (m_operationLook == enOperationLook_UnSeem)
	{
		m_operationPic.SetPosition(m_firstPosition);
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
		se->Init(5);
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
		se->Init(5);
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
		m_option.SetPosition(m_firstPosition);
		//十字キー左を押したら
		if (g_pad[0]->IsTrigger(enButtonLeft) && characterOpPosition != 0)
		{
			characterOpPosition--;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}
		//十字キー右を押したら
		if (g_pad[0]->IsTrigger(enButtonRight) && characterOpPosition < 3)
		{
			characterOpPosition++;
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(5);
			se->Play(false);
			se->SetVolume(1.0f);
		}

		switch (characterOpPosition)
		{
		case 0:
			m_characterOpPosition = enCharacterOpPosition_Knight;
			m_fire.SetPosition(m_KnightCursor + m_charaLeftCursor);
			m_WizardOp.SetPosition(m_firstPosition);
			m_KnightOp.SetPosition(m_opPosition);
			break;
		case 1:
			m_characterOpPosition = enCharacterOpPosition_Wizard;
			m_fire.SetPosition(m_WizardCursor + m_charaLeftCursor);
			m_KnightOp.SetPosition(m_firstPosition);
			m_WizardOp.SetPosition(m_opPosition);
			break;
		case 2:
			m_characterOpPosition = enCharacterOpPosition_Zombie;
			m_fire.SetPosition(m_ZombieCursor + m_charaLeftCursor);
			break;
		case 3:
			m_characterOpPosition = enCharacterOpPosition_Mitei;
			m_fire.SetPosition(m_MiteiCursor + m_charaLeftCursor);
			break;
		}
		m_Opchoice.SetPosition(0.0f, 450.0f, 0.0f);
		m_start.Update();
		m_operation.Update();
		m_option.Update();
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

void Tittle::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_iluststart.Draw(rc);
	m_iluststartOp.Draw(rc);
	m_ilusthowtoplay.Draw(rc);
	m_ilusthowtoplayOp.Draw(rc);
	m_ilustoption.Draw(rc);
	m_ilustoptionOp.Draw(rc);
	m_fire.Draw(rc);
	m_titleLogo.Draw(rc);
	m_start.Draw(rc);
	m_startST.Draw(rc);
	m_operation.Draw(rc);
	m_operationST.Draw(rc);
	m_option.Draw(rc);
	m_optionST.Draw(rc);
	m_operationPic.Draw(rc);
	m_KnightOp.Draw(rc);
	m_WizardOp.Draw(rc);
	m_Opchoice.Draw(rc);
}
