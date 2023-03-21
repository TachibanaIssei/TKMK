#pragma once
#include "KnightPlayer.h"
#include "WizardPlayer.h"

class Game;
class GameUI;
class KnightPlayer;
class WizardPlayer;

class Player:public IGameObject/*,public KnightPlayer*/
{
public:
	Player();
	~Player();

	//キャラ選択
	enum SelectCharctar
	{
		enKnight,
		enWizard,
		enZombie,
		enMonster,
		enNum,
	};
	SelectCharctar m_selectCharctar;
	
	bool Start();

	/// <summary>
	/// 生成するキャラを選ぶ
	/// </summary>
	/// <param name="number">キャラの番号</param>
	void CharSelect(int number)
	{
		switch (number)
		{
		case 0:
			m_selectCharctar = enKnight;
			break;
		case 1:
			m_selectCharctar = enWizard;
			break;
		case 2:
			m_selectCharctar = enZombie;
			break;
		case 3:
			m_selectCharctar = enMonster;
			break;

		default:
			break;
		}
	}

	/// <summary>
	/// キャラの座標を取得
	/// </summary>
	/// <returns>選択されているキャラの座標</returns>
	Vector3 GetCharPosition()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->GetPosition();
			break;

		case enWizard:
			return wizardPlayer->GetPosition();
			break;

		case enZombie:
			break;

		case enMonster:
			break;

		default:
			break;
		}
	}

	/// <summary>
	/// キャラの現在のレベルを返す
	/// </summary>
	/// <returns>選択されたキャラの現在のレベルを返す関数</returns>
	int CharSetLevel()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->SetLevel();
			break;
		case enWizard:
			//wizardPlayer->Set
			break;
		case enZombie:
			break;
		case enMonster:
			break;
		
		default:
			break;
		}
	}

	/// <summary>
	/// キャラの現在のヒットポイントを返す
	/// </summary>
	/// <returns>選択されたキャラの現在のヒットポイントを返す関数</returns>
	int CharSetHp()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->SetHp();
			break;

		case enWizard:
			return wizardPlayer->SetHp();
			break;

		case enZombie:
			break;

		case enMonster:
			break;


		default:
			break;
		}
	}

	/// <summary>
	/// キャラの最大ヒットポイントを返す
	/// </summary>
	/// <returns>選択されたキャラの最大ヒットポイントを返す関数</returns>
	int CharSetMaxHp()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->SetMaxHp();
			break;

		case enWizard:
			//return wizardPlayer->SetMaxHp();
			break;

		case enZombie:
			break;

		case enMonster:
			break;


		default:
			break;
		}
	}

	/// <summary>
	/// キャラの回転量を返す
	/// </summary>
	/// <returns>選択されたキャラの回転量を返す関数</returns>
	Quaternion CharSetRot()const
	{
		switch (m_selectCharctar)
		{
		case enKnight:
			return knightPlayer->GetRot();
			break;

		case enWizard:
			//return wizardPlayer->GetRot();
			break;

		case enZombie:
			break;

		case enMonster:
			break;


		default:
			break;
		}
	}




	Game* game = nullptr;
	GameUI* gameUI = nullptr;
	KnightPlayer* knightPlayer = nullptr;
	WizardPlayer* wizardPlayer = nullptr;

	

};

