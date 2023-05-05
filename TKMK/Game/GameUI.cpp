#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Actor.h"
#include "KnightPlayer.h"
#include "WizardPlayer.h"
#include "Player.h"
#include "Fade.h"

namespace
{
	const int Characters = 4;

	const Vector2 GAUGE_PIVOT = Vector2(0.5f, 0.5f);				//ゲージのピボット
	const Vector2 HPGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//HPゲージのピボット
	const Vector2 EXPERIENCEGAUGE_PIVOT = Vector2(0.0f, 0.5f);				//経験値ゲージのピボット

	const Vector3 STATUS_BAR_POS = Vector3(-450.0f, -500.0f, 0.0f);	//ステータスバーポジション
	const Vector3 TIME_POS = Vector3(0.0,470.0f, 0.0f);	//制限時間の座標
	const Vector3 TIME_FONT_POS = Vector3(-80.0, 500.0f, 0.0f);	//制限時間の座標

	const Vector3 HP_BAR_POS = Vector3(-670.0f, -480.0f, 0.0f);	//HPバーポジション
	const Vector3 HP_BAR_FLONT_POS = Vector3(-960.0f, -480.0f, 0.0f);	//HPバーの表のポジション

	const float HP_BAR_WIDTH = 1400.0f;     //HPバーの長さ
	const float HP_BAR_HIGHT = 200.0f;      //HPバーの高さ
	const Vector3 HP_BAR_SIZE = Vector3(HP_BAR_WIDTH, HP_BAR_HIGHT, 0.0f);  //HPバーのサイズ

	const Vector3 Skill_Pos = Vector3(520.0f, -280.0f, 0.0f);   //スキルアイコンポジション
	const Vector3 Ult_Pos = Vector3(470.0f, -445.0f, 0.0f);     //必殺技アイコンポジション

	const Vector3 LV_NUBER_POS = Vector3(780.0f, -360.0f, 0.0f);
	const Vector3 LvPos = Vector3(640.0f, -310.0f, 0.0f);       //Lv
	const Vector3 MaxLvPos = Vector3(920.0f, -400.0f, 0.0f);       // /10

	const Vector3 FLAME_POS = Vector3(920.0f,-480.0f,0.0f);    //レベルや経験値のフレーム

	const float DownPointPosY = 100.0f;

	const Vector3 EXPERIENCE_POS = Vector3(750.0f, -500.0f, 0.0f);  //経験値テーブル

	const float EXPBAR_WIDTH = 300.0f;
	const float EXPBAR_HEIGHT = 70.0f;
	const Vector3 EXPERIENCE_BAR_POS = Vector3(750.0f, -500.0f, 0.0f);	//経験値バーの座標
	const Vector3 EXPBAR_SIZE = Vector3(EXPBAR_WIDTH, EXPBAR_HEIGHT, 0.0f);	//経験値バーのサイズ

	const Vector3 UPTOLEVEL_POS = Vector3(820.0f, -480.0f, 0.0f);		//レベルアップまでに必要な経験値の量

	const Vector3 RESPWANCOUNT_POS = Vector3(0.0f, 0.0f, 0.0f);		//リスポーンした後のカウント

	const Vector3 SmallScale = Vector3(0.1f, 0.1f, 0.0f);

	const Vector3 FightSmallScale = Vector3(0.5f, 0.5f, 0.0f);

	const Vector3 RespawnInPos = Vector3(0.0f, 300.0f, 0.0f);		//Respawn inの座標

	const Vector3 RespawnCountPos = Vector3(0.0f, -200.0f, 0.0f);		//の座標

	const Vector3 ADDPOINTPOS = Vector3(20.0f, 10.0f, 0.0f);
}
GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	player = FindGO<Player>("player");
	fade = FindGO<Fade>("fade");
	//キャラのアイコン
	//ブルー
	m_CharIcon[0].Init("Assets/sprite/gameUI/Knight_Blue.DDS", 80.0f, 80.0f);
	//レッド
	m_CharIcon[1].Init("Assets/sprite/gameUI/Knight_Red.DDS", 80.0f, 80.0f);
	//グリーン
	m_CharIcon[2].Init("Assets/sprite/gameUI/Knight_Green.DDS", 80.0f, 80.0f);
	//イエロー
	m_CharIcon[3].Init("Assets/sprite/gameUI/Knight_Yellow.DDS", 80.0f, 80.0f);

	//ポイント関連
	{
		//キャラのポイントを表示
		m_Actors = m_game->GetActors();
		int num = 0;
		for (auto actor: m_Actors)
		{
			//ポイントを表示
			m_PointFont[num].SetPosition(PointPos[num]);
			m_PointFont[num].SetScale(1.3f);
			m_PointFont[num].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			m_PointFont[num].SetRotation(0.0f);
			m_PointFont[num].SetShadowParam(true, 2.0f, g_vec4Black);

			//プレイヤーが剣士なら
			if (actor->IsMatchName(knightname))
			{
				//アイコンを剣士にする(ブルー)
				
				m_CharIcon[num].SetPosition(CharIconPos[num]);
				//フレームをプレイヤー用にする
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame_player.DDS", 300.0f, 100.0f);
			}
			//プレイヤーが魔法使いなら
			else if (actor->IsMatchName(wizardname))
			{
				//アイコンを魔法使いにする
				
				//フレームをプレイヤー用にする
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame_player.DDS", 300.0f, 100.0f);
			}
			//それ以外(AI)なら
			else
			{
				m_CharIcon[num].SetPosition(CharIconPos[num]);
				m_PointFlame[num].Init("Assets/sprite/gameUI/pointFlame.DDS", 300.0f, 100.0f);
			}
			
			m_PointFlame[num].SetPosition(PointFlamePos[num]);
			m_PointFlame[num].SetScale(1.0f, 1.0f, 1.0f);
			m_PointFlame[num].Update();
			//
			m_CharIcon[num].Update();

			num++;
		}

	}
	
	//リスポーン関連
	{
		//Respawn inの画像
		m_RespawnIn.Init("Assets/sprite/gameUI/RespawnIn.DDS", 900.0f, 200.0f);
		m_RespawnIn.SetPosition(RespawnInPos);
		m_RespawnIn.SetScale(Vector3::One);

		//リスポーンの背景の画像
		m_Respawn_Back.Init("Assets/sprite/gameUI/Respawn_back.DDS", 1920, 1080.0f);
		m_Respawn_Back.SetPosition(Vector3::Zero);
		m_Respawn_Back.SetScale(Vector3::One);

		//リスポーンのカウントダウンの画像
		m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
		m_RespawnCountNumber.SetPosition(RespawnCountPos);
		m_RespawnCountNumber.SetScale(Vector3::One);

		m_RespawnIn.Update();
		m_Respawn_Back.Update();
		m_RespawnCountNumber.Update();
	}

	//スタートまでのカウントダウン
	m_CountNumper.Init("Assets/sprite/gameUI/count3.DDS", 1920.0f, 1080.0f);
	m_CountNumper.SetPosition(Vector3::Zero);
	m_CountNumper.SetScale(m_gameCountScale);
	m_CountNumper.Update();

	//右下のフレーム
	{
		//レベルや経験値のフレーム
		m_Flame.Init("Assets/sprite/gameUI/LevelBar.DDS", 1200.0f, 500.0f);
		m_Flame.SetPosition(FLAME_POS);
		m_Flame.SetScale(1.0, 1.0, 1.0);
		
		//経験値のフレーム
		m_ExperienceFlame.Init("Assets/sprite/gameUI/ExperienceBar.DDS", 600.0f, 120.0f);
		m_ExperienceFlame.SetPosition(EXPERIENCE_POS);
		m_ExperienceFlame.SetScale(0.5, 0.5, 1.0);
		
		//経験値バーの表ピボットにする
		m_EXPBerPos = EXPERIENCE_BAR_POS;
		m_ExperienceBar_flont.Init("Assets/sprite/gameUI/ExperienceBar_front.DDS", EXPBAR_WIDTH, EXPBAR_HEIGHT);
		m_ExperienceBar_flont.SetPosition(m_EXPBerPos);
		//m_ExperienceBar_flont.SetPivot(EXPERIENCEGAUGE_PIVOT);
		m_ExperienceBar_flont.SetScale(0.5, 50.0, 1.0);

		//経験値バーの裏
		m_ExperienceBar_back.Init("Assets/sprite/gameUI/ExperienceBar_back.DDS", 600.0f, 120.0f);
		m_ExperienceBar_back.SetPosition(EXPERIENCE_POS);
		m_ExperienceBar_back.SetScale(0.5, 0.5, 1.0);

		//レベルアップまでに必要な経験値の量
		m_ExpFont.SetPosition(UPTOLEVEL_POS);
		m_ExpFont.SetScale(1.0f);
		m_ExpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_ExpFont.SetRotation(0.0f);
		m_ExpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//Lvの画像を読み込む
		m_Lv.Init("Assets/sprite/gameUI/Lv.DDS", 196.0f, 150.0f);
		m_Lv.SetPosition(LvPos);
		m_Lv.SetScale(0.4, 0.4, 1.0);

		//Lv1の画像を読み込む
		m_LvNumber.Init("Assets/sprite/gameUI/Lv1.DDS", 150.0f, 150.0f);
		m_LvNumber.SetPosition(LV_NUBER_POS);
		m_LvNumber.SetScale(1.4, 1.4, 1.0);

		// /10の画像を読み込む
		m_MaxLv.Init("Assets/sprite/gameUI/maxLv.DDS", 196.0f, 150.0f);
		m_MaxLv.SetPosition(MaxLvPos);
		m_MaxLv.SetScale(0.5, 0.5, 1.0);

		//スキルのアイコン
		m_SkillRender.Init("Assets/sprite/gameUI/skil_Icon.DDS", 162, 162);
		m_SkillRender.SetPosition(Skill_Pos);
		m_SkillRender.SetScale(1.1, 1.1);
		//必殺技のアイコン
		m_UltRender.Init("Assets/sprite/gameUI/Ult_Icon.DDs", 162, 162);
		m_UltRender.SetPosition(Ult_Pos);
		m_UltRender.SetScale(1.2, 1.2);

		m_Lv.Update();
		m_LvNumber.Update();
		m_MaxLv.Update();
		m_Flame.Update();
		m_ExperienceFlame.Update();
		m_ExperienceBar_flont.Update();
		m_ExperienceBar_back.Update();
		m_SkillRender.Update();
		m_UltRender.Update();
	}

	//HP関連
	{
		//HPのフォント
		m_HpFont.SetPosition(-650.0f, -465.0f, 0.0f);
		m_HpFont.SetScale(1.0f);
		m_HpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_HpFont.SetRotation(0.0f);
		m_HpFont.SetShadowParam(true, 2.0f, g_vec4Black);

		//HPゲージ裏の画像を読み込む
		m_statusBar.Init("Assets/sprite/gameUI/HPBar_HP_back.DDS", 600.0f, 120.0f);
		m_statusBar.SetPosition(HP_BAR_POS);
		m_statusBar.SetScale(1.0, 0.7, 1.0);

		//HPゲージの表の画像を読み込む
		m_hpBar.Init("Assets/sprite/gameUI/HPBar_HP.DDS", 580.0f, 80.0f);
		//ピボットを設定する
		m_hpBar.SetPivot(HPGAUGE_PIVOT);
		m_hpBar.SetPosition(HP_BAR_FLONT_POS);

		//HPゲージのフレームの画像を読み込む
		m_HPFrame.Init("Assets/sprite/gameUI/HPBar_flame.DDS", 600.0f, 120.0f);
		m_HPFrame.SetPosition(HP_BAR_POS);
		m_HPFrame.SetScale(1.0, 0.7, 1.0);

		//更新処理
		m_statusBar.Update();
		m_HPFrame.Update();
		m_hpBar.Update();
	}

	//制限時間と獲得ポイント
	{
		m_TimeAndPointRender.Init("Assets/sprite/gameUI/timer.DDS", 1100.0f, 400.0f);
		m_TimeAndPointRender.SetPosition(TIME_POS);
		m_TimeAndPointRender.SetScale(0.35, 0.3, 0.3);

		//フォントの設定。
		m_time_left.SetPosition(TIME_FONT_POS);
		//フォントの大きさを設定。
		m_time_left.SetScale(1.6f);
		//フォントの色を設定。
		m_time_left.SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_time_left.SetShadowParam(true, 2.0f, g_vec4Black);

		m_TimeAndPointRender.Update();
	}

	m_GameUIState = m_GameStartState;

	return true;
}

void GameUI::Update()
{
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState== m_PauseState) {
		return;
	}

	//ゲームのステートがgameStartなら
	if (m_game->NowGameState() == 0)
	{
		CountDown();
	}
	
	//プレイヤーがリスポーン待機中なら
	if (player->CharGetRespawnTime() > 0)
	{
		RespawnCountDown();
	}
	
	//gameクラスのスタートのフラグが立っている間処理を行わない
	if (m_GameUIState == m_GameStartState) {
		return;
	}

	CharPoint();
	
	//制限時間の表示
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d:%02d", int(m_game->GetMinutesTimer()),int(m_game->GetSecondsTimer()));
	//表示するテキストを設定。
	m_time_left.SetText(wcsbuf);
	
	EXPBar();

	HPBar();
}

//ゲームスタートのカウントダウン
void GameUI::CountDown()
{
	//カウントダウン
	//int COUNTDOWNTIMER = m_game->CountDownMinutes();
	int StartCountDown= m_game->CountDownMinutes();

	if(m_game->CountDownMinutes() <= 0)
	{
		m_CountNumper.Init("Assets/sprite/gameUI/fight!.DDS", 1920.0f, 1080.0f);
		//m_gameCountScale = Vector3(5.0f, 5.0f, 0.0f);
		m_Color = 1.0f;
		m_fightFlag = true;
	}

	if (oldtStartCount != StartCountDown)
	{
		switch (StartCountDown)
		{
		case 1:
			m_CountNumper.Init("Assets/sprite/gameUI/count1.DDS", 1920.0f, 1080.0f);
			m_gameCountScale = Vector3(0.2f, 0.2f, 0.0f);
			m_Color = 1.0f;
			break;
		case 2:
			m_CountNumper.Init("Assets/sprite/gameUI/count2.DDS", 1920.0f, 1080.0f);
			m_gameCountScale = Vector3(0.2f, 0.2f, 0.0f);
			m_Color = 1.0f;
			break;
		default:
			break;
		}
	}
	//画像がFight!でないかつスケールが100以下なら
	else if(m_fightFlag == false&&m_gameCountScale.x<100.0f)
	{
		//徐々に文字を大きくする
		m_gameCountScale += SmallScale;
		//少しずつ透明にする
		m_Color -= 0.02f;
		
		m_CountNumper.SetScale(m_gameCountScale);
		//
		m_CountNumper.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_Color));
	}

	//画像がFight!なら
	if(m_fightFlag==true)
	{
		if (m_FightScale.x < m_gameCountScale.x)
		{
			//徐々に文字を小さくする
			m_gameCountScale -= FightSmallScale;
		}
		
		m_CountNumper.SetScale(m_gameCountScale);
		//
		m_CountNumper.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_Color));
	}

	
	m_CountNumper.Update();

	oldtStartCount = StartCountDown;
}

//リスポーンするまでのカウントダウン
void GameUI::RespawnCountDown()
{
	//カウントダウン
	int RESPAWNCOUNTDOWN = player->CharGetRespawnTime();

	if (oldRespawnCount != RESPAWNCOUNTDOWN)
	{
		switch (RESPAWNCOUNTDOWN)
		{
		case 0:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut0.DDS", 300, 500.0f);
			//画面を暗くしてゆく
			fade->StartFadeIn(2.0f);
			break;
		case 1:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut1.DDS", 300, 500.0f);
			break;
		case 2:
			m_RespawnCountNumber.Init("Assets/sprite/gameUI/RespawnConut2.DDS", 300, 500.0f);
			break;
		default:
			break;
		}
	}

	oldRespawnCount = RESPAWNCOUNTDOWN;

	m_RespawnCountNumber.Update();
}

//プレイヤーのHPの表示の処理
void GameUI::HPBar()
{
	int HP = player->CharSetHp();
	int MaxHP = player->CharSetMaxHp();
	wchar_t hp[255];
	swprintf_s(hp, 255, L"%d/%d", HP, MaxHP);
	m_HpFont.SetText(hp);

	Vector3 HpScale = Vector3::One;
	//HPバーの減っていく割合。
	HpScale.x = (float)player->CharSetHp() / (float)player->CharSetMaxHp();
	m_hpBar.SetScale(HpScale);

	m_hpBar.Update();
}

//制限時間の表示の処理
void GameUI::Timer()
{
		//0秒以下なら
		if (SecondsTimer <= 0) {
			//1分減らす
			MinutesTimer--;
			//もし0分なら、秒も0にする
			if (MinutesTimer < 0) {
				SecondsTimer = 0.0f;
				MinutesTimer = 0.0f;
				GameEndFlag = true;
			}
			//60秒に戻す
			else SecondsTimer = 60.0f;
		}
		else SecondsTimer -= g_gameTime->GetFrameDeltaTime();
	
}

//プレイヤーの経験値の表示の処理todo
void GameUI::EXPBar()
{
	//経験値の表示
	Vector3 EXPScale = Vector3::One;
	//プレイヤーの経験値を取得
	float nowEXP = player->CharSetEXP();
	//今の経験値テーブルを取得
	float nowEXPTable = player->CharSetEXPTable();
	//前のレベルの経験値テーブルを取得
	float oldEXPTable = player->CharSetOldEXPTable();

	//最終的な経験値テーブル
	float finalEXPTable = nowEXPTable - oldEXPTable;
	//最終的な経験値
	float finalEXP = nowEXP - oldEXPTable;

	//HPバーの増えていく割合。
	EXPScale.x = (float)finalEXP / (float)finalEXPTable;
	m_ExperienceBar_flont.SetScale(EXPScale);

	//EXPバー画像を左寄せに表示する
	Vector3 BerSizeSubtraction = HPBerSend(EXPBAR_SIZE, EXPScale);	//画像の元の大きさ

	if (finalEXPTable != oldEXPTable)
	{
		m_EXPBerPos.x = EXPERIENCE_BAR_POS.x;
	}

	//経験値の量が変わったときだけ
	if (finalEXP != oldEXP)
	{
		m_EXPBerPos.x -= BerSizeSubtraction.x;
	}
	
	
	m_ExperienceBar_flont.SetPosition(Vector3(m_EXPBerPos.x, m_EXPBerPos.y, 0.0f));
	//m_ExperienceBar_flont.SetScale(EXPScale);
	m_ExperienceBar_flont.Update();


	//レベルアップまでに必要な経験値の量
	int UpToLevel = nowEXPTable - nowEXP;
	wchar_t UTL[255];
	swprintf_s(UTL, 255, L"%d", UpToLevel);
	m_ExpFont.SetText(UTL);

	oldEXP = finalEXP;
	oldEXPTable = finalEXPTable;
}

//
Vector3& GameUI::HPBerSend(Vector3 size, Vector3 scale)
{
	Vector3 expBerSize = size;								//画像の元の大きさ
	Vector3 changeBerSize = Vector3::Zero;					//画像をスケール変換したあとの大きさ
	Vector3 BerSizeSubtraction = Vector3::Zero;				//画像の元と変換後の差

	changeBerSize.x = expBerSize.x * scale.x;
	BerSizeSubtraction.x = expBerSize.x - changeBerSize.x;
	BerSizeSubtraction.x /= 2.0f;

	return BerSizeSubtraction;
}

//キャラのポイントと。ポイントが一番多いキャラに王冠マークをつける表示の処理
void GameUI::CharPoint()
{
	//キャラのポイントを表示
	m_Actors = m_game->GetActors();

	int num = 0;
	for (auto actor:m_Actors)
	{
		charPoint[num] = actor->GetPoint();

		//ポイントの表示
		int POINT = charPoint[num];
		wchar_t P[255];
		swprintf_s(P, 255, L"%dp", POINT);
		m_PointFont[num].SetText(P);


		if (MaxPoint <= charPoint[num])
		{
			m_PointFont[num].SetScale(1.8f);
			Vector3 FontPos;
			FontPos= ADDPOINTPOS + PointPos[num];
			m_PointFont[num].SetPosition(FontPos);

			m_PointFlame[num].SetScale(1.7f,1.3f,0.0f);
			m_PointFlame[num].Update();
			MaxPoint = charPoint[num];
		}
		else
		{
			m_PointFont[num].SetScale(1.3f);
			m_PointFont[num].SetPosition(PointPos[num]);
			m_PointFlame[num].SetScale(1.0f, 1.0f, 0.0f);
			m_PointFlame[num].Update();
		}


		num++;
	}

	
	//誰に王冠マークつけるか決める
}

void GameUI::Render(RenderContext& rc)
{
	//gameクラスのポーズのフラグが立っている間処理を行わない
	if (m_GameUIState != m_PauseState && m_GameUIState != m_GameStartState) {
		//レベルや経験値のフレーム
		m_Flame.Draw(rc);
		//経験値の裏
		m_ExperienceBar_back.Draw(rc);
		//経験値の表 変動する
		m_ExperienceBar_flont.Draw(rc);
		//経験値フレーム
		m_ExperienceFlame.Draw(rc);
		//
		m_ExpFont.Draw(rc);
		
		m_HpNameFont.Draw(rc);

		m_TimeAndPointRender.Draw(rc);

		m_time_left.Draw(rc);

		m_statusBar.Draw(rc);
		m_hpBar.Draw(rc);
		
		m_HPFrame.Draw(rc);
		m_SkillRender.Draw(rc);
		m_UltRender.Draw(rc);

		m_HpFont.Draw(rc);
		m_Lv.Draw(rc);
		m_LvNumber.Draw(rc);
		m_MaxLv.Draw(rc);
		

		//ポイントを描画
		int num = 0;
		for (auto actor:m_Actors) {
			m_PointFlame[num].Draw(rc);
			m_PointFont[num].Draw(rc);
			m_CharIcon[num].Draw(rc);
			num++;
		}
		
		//リスポーンするまでの時間
		if (player->CharGetRespawnTime() > 0)
		{
			m_Respawn_Back.Draw(rc);
			m_RespawnIn.Draw(rc);
			m_RespawnCountNumber.Draw(rc);
		}
	}
	else
	{
		if (m_game->NowGameState() == 0) {
			m_CountNumper.Draw(rc);
		}
	}
	
}
