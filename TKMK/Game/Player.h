#pragma once
#include "KnightPlayer.h"
#include "WizardPlayer.h"


#include "Status.h"

class Status;
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

	//プレイヤーのステートを
	enum EnPlayerStateChange
	{
		enPause,
		enGame,
	};
	EnPlayerStateChange m_playerStateChange;
	
	/// <summary>
	/// プレイヤーを生成する
	/// </summary>
	void CreaetPlayer();

	/// <summary>
	/// 生成するキャラを選ぶ
	/// </summary>
	/// <param name="number">キャラの番号</param>
	void CharSelect(int number);

	/// <summary>
	/// キャラの座標を取得
	/// </summary>
	/// <returns>選択されているキャラの座標</returns>
	Vector3 GetCharPosition()const;

	/// <summary>
	/// キャラの現在のレベルを返す
	/// </summary>
	/// <returns>選択されたキャラの現在のレベルを返す関数</returns>
	int CharSetLevel()const;

	/// <summary>
	/// キャラの現在のヒットポイントを返す
	/// </summary>
	/// <returns>選択されたキャラの現在のヒットポイントを返す関数</returns>
	int CharSetHp()const;

	/// <summary>
	/// キャラの最大ヒットポイントを返す
	/// </summary>
	/// <returns>選択されたキャラの最大ヒットポイントを返す関数</returns>
	int CharSetMaxHp()const;

	/// <summary>
	/// キャラの回転量を返す
	/// </summary>
	/// <returns>選択されたキャラの回転量を返す関数</returns>
	Quaternion CharSetRot()const;
	
	/// <summary>
	/// キャラの前方向を返す
	/// </summary>
	/// <returns>選択されたキャラの前方向を返す関数	</returns>
	Vector3 CharSetForward()const;

	/// <summary>
	/// キャラのスプライトフラグを返す
	/// </summary>
	/// <returns>選択されたキャラのスプライトフラグを返す関数</returns>
	bool CharSetSpriteFlag()const;

	/// <summary>
	/// プレイヤーのポイントを返す
	/// </summary>
	/// <returns>プレイヤーの現在のポイント</returns>
	int CharSetPoint()const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	float CharGetRespawnTime()const;

	Actor* GetPlayerActor() {
		return playerActor;
	}

	int CharSetEXP() const;

	int CharSetEXPTable() const;

	//プレイヤーのステートを取得
	//auto CharGetState() const;
	auto CharGetState() const
	{
		return playerActor->NowCharState();
	}
private:

	Game* game = nullptr;
	GameUI* gameUI = nullptr;
	KnightPlayer* knightPlayer = nullptr;
	WizardPlayer* wizardPlayer = nullptr;
	Status m_Status;

	Actor* playerActor = nullptr;

};

