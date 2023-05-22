#pragma once

enum EnSound {
	enSound_PlayBattle = 0,			//戦闘中のBGM
	enSound_Sword_Skill,			//プレイヤーのスキルの音
	enSound_ComboONE,				//プレイヤーの1回目のコンボの音
	enSound_ComboTwo,				//プレイヤーの2回目のコンボの音
	enSound_ComboThree,				//プレイヤーの3回目コンボの音
	enSound_Hand,					//プレイヤーが手を上に上げるときの音
	enSound_Sword_Ult,				//プレイヤーの必殺の音
	enSound_Metal_Falling,			//プレイヤーが着地した時の音
	enSound_Knight_Receiving_Damage,//プレイヤーの被ダメ声
	enSound_Knight_Death,			//プレイヤーが死んだときの声
	enSound_Knight_Charge_Power,	//プレイヤーがパワーをためる音
	enSound_Enemy_Voice,			//中立の敵の鳴き声
	enSound_Enemy_Death,			//中立の敵が死んだときの音
	enSound_Rabbit_Spawn,			//ウサギ出現した時の音
	enSound_Rabbit_Death,			//ウサギが倒されたときの音
	enSound_Rabbit_FootSteps,		//ウサギの足音
	enSound_Level_UP,				//レベルアップした時の音
	enSound_Level_Down,				//レベルダウンした時の音
	enSound_Healing,				//HP回復した時の音
	enSound_Pause_Screen,			//ポーズ画面に入る時の音
	enSound_TitleOK,				//タイトル画面の決定音
	enSound_OK,						//キャラセレクト画面の決定音
	enSound_Title_BGM,              //タイトルのBGM
	enSound_Title_Choise,			//タイトル画面での選択音
	enSound_Title_Slash1,			//タイトル画面での斬撃音1
	enSound_Title_Slash2,			//タイトル画面での斬撃音2
	enSound_CountDown3,				//ゲームカウントダウン３
	enSound_CountDown2,				//ゲームカウントダウン２
	enSound_CountDown1,				//ゲームカウントダウン１
	enSound_CountFight,				//ゲームカウントダウンファイト
	enSound_Title_Fanfare,			//勝利した時BGM？
	enSound_GameEnd,
	enSound_Num,	
};
