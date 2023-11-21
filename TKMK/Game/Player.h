#pragma once
#include "KnightPlayer.h"
#include "WizardPlayer.h"


#include "Status.h"

struct Status;
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
	// TODO:publicになっているのでprivateに
	SelectCharctar m_selectCharctar;

	//プレイヤーのステートを
	enum EnPlayerStateChange
	{
		enPause,
		enGame,
	};
	// TODO:publicになっているのでprivateに
	EnPlayerStateChange m_playerStateChange;
	
	/// <summary>
	/// プレイヤーを生成する
	/// </summary>
	void CreatePlayer();

	/// <summary>
	/// 生成するキャラを選ぶ
	/// </summary>
	/// <param name="number">キャラの番号</param>
	void SelectCharcter(int number);

	/// <summary>
	/// キャラの座標を取得
	/// </summary>
	/// <returns>選択されているキャラの座標</returns>
	Vector3 GetCharcterPosition()const;

	/// <summary>
	/// キャラの現在のレベルを返す
	/// </summary>
	/// <returns>選択されたキャラの現在のレベルを返す関数</returns>
	int GetCharacterLevel()const;

	/// <summary>
	/// キャラの現在のヒットポイントを返す
	/// </summary>
	/// <returns>選択されたキャラの現在のヒットポイントを返す関数</returns>
	int GetCharacterHp()const;

	/// <summary>
	/// キャラの最大ヒットポイントを返す
	/// </summary>
	/// <returns>選択されたキャラの最大ヒットポイントを返す関数</returns>
	int GetCharcterMaxHp()const;

	/// <summary>
	/// キャラの回転量を返す
	/// </summary>
	/// <returns>選択されたキャラの回転量を返す関数</returns>
	Quaternion GetCharcterRotation()const;
	
	/// <summary>
	/// キャラの前方向を返す
	/// </summary>
	/// <returns>選択されたキャラの前方向を返す関数	</returns>
	Vector3 GetCharcterForward()const;

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
		return m_playerActor;
	}

	/// <summary>
	/// 剣士プレイヤーのポインタを返す
	/// </summary>
	KnightPlayer* GetKnightPlayer()
	{
		return m_knightPlayer;
	}

	int CharGetEXP() const;

	int CharSetEXPTable() const;

	int CharGetEXPTableForLevel(int Level) const;

	int CharGetSaveEXP() const;

	void CharResatSaveEXP(int num) const;

	float CharGetSkillCoolTimer() const;

	//プレイヤーのステートを取得
	//auto CharGetState() const;
	auto CharGetState() const
	{
		return m_playerActor->NowCharState();
	}

	/// <summary>
	/// プレイヤーの名前を取得する
	/// </summary>
	/// <returns></returns>
	const char* GetName() {
		return m_playerName;
	}
private:

	Game* m_game = nullptr;
	GameUI* m_gameUI = nullptr;
	KnightPlayer* m_knightPlayer = nullptr;
	WizardPlayer* m_wizardPlayer = nullptr;
	Status m_status;

	Actor* m_playerActor = nullptr;
	const char* m_playerName = nullptr;
};

