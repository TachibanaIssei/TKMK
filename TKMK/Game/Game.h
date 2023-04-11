#pragma once

#include "Level3DRender.h"

class BackGround;
class Result;
class Tittle;
class GameCamera;
class KnightPlayer;
class Neutral_Enemy;
class GameUI;
class Map;
class KnightAI;
class WizardPlayer;
class Player;
class CharUltFlag;
class Actor;
class Lamp;

class Game : public IGameObject
{
public:
	Game();
	~Game();

	enum EnGameState {
		enGameState_Start,
		enGameState_Battle,
		enGameState_Pause,
		enGamestate_End,
		enGameState_Rezult,
		enGameState_Num,
	};

	bool Start();
	void Update();
	void BattleStart();
	void Battle();
	void Pause();
	void End();
	void GameState();
	
	/// <summary>
	/// ゲーム中に再生される音の読み込み
	/// </summary>
	void SetMusic();

	/// <summary>
	/// 効果音の音量を返す
	/// </summary>
	/// <returns>効果音の音量</returns>
	float SetSoundEffectVolume()
	{
		return SoundEffectVolume;
	}

	/// <summary>
	/// 中立の敵のリスポーン
	/// </summary>
	void Respawn();

	/// <summary>
	/// リスポーンする座標の設定
	/// </summary>
	void SetEnemyRespawnPos();

	//リスポーンする座標を格納する
	void SetRespawnPosition(Vector3 pos, Quaternion rot, int number) {
		EnemyRespawnPosition[number] = pos;
		EnemyReapawnPot[number] = rot;
	};

	void Render(RenderContext& rc);

	/// <summary>
	/// 中立の敵の情報をリストから削除する
	/// </summary>
	/// <param name="enemy">消したい中立の敵</param>
	void RemoveEnemyFromList(Neutral_Enemy* enemy)
	{
		std::vector<Neutral_Enemy*>::iterator it = std::find(
			m_neutral_Enemys.begin(), // 中立の敵のリストの最初
			m_neutral_Enemys.end(),   // 中立の敵のリストの最後
			enemy                     // 消したい中立の敵
		);
		if (it != m_neutral_Enemys.end()) {
			m_neutral_Enemys.erase(it);
		}
	}
	//中立の敵のリストを返す
	std::vector<Neutral_Enemy*>& GetNeutral_Enemys() {
		return m_neutral_Enemys;
	}

	//Actorのリストを返す
	std::vector<Actor*>& GetActors() {
		return m_Actors;
	}

private:
	/// <summary>
	/// 中立の敵の名前を設定する
	/// </summary>
	char* CreateEnemyName() {
		enemyNum++;
		swprintf_s(name_t, 255, L"Neutral_Enemy%d", enemyNum);
		enemyName = (char*)name_t;
		wcstombs(enemyName, name_t, sizeof(name_t));

		return enemyName;
	}
	/// <summary>
	/// 中立の敵の生成
	/// </summary>
	void CreateEnemy(Vector3 pos, Quaternion rot);

	//ゲームのステート
	EnGameState m_GameState = enGameState_Start;

	ModelRender m_modelRender;
	Level3DRender m_level3DRender;
	Level3DRender m_Enemylevel;
	FontRender m_fontRender;

	Quaternion m_rotation = Quaternion::Identity;
	Quaternion m_sRotation = Quaternion::Identity;
	Vector3 EnemyRespawnPosition[9];
	Quaternion EnemyReapawnPot[9];
	SpriteRender m_Pause_Front;    //�|�[�Y���
	SpriteRender m_Pause_Back;     //�|�[�Y�̗����

	BackGround* m_backGround = nullptr;
	Result* m_rezult=nullptr;
	Tittle* m_tittle = nullptr;
	GameCamera* m_gamecamera = nullptr;
	GameUI* m_gameUI = nullptr;
	KnightPlayer* m_knightplayer = nullptr;
	KnightAI* m_KnightAI = nullptr;
	Neutral_Enemy* neutral_Enemy = nullptr;
	Map* m_Map = nullptr;
	SoundSource* m_bgm = nullptr;	//
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	CharUltFlag* charUltFlag = nullptr;
	Lamp* lamp = nullptr;

	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	std::vector<Actor*> m_Actors;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	float m_spriteAlpha = 0.0f;

	int enemyNumber = 0;

	int SearchRespawnPosNumber = 0;
	int RandamRespawnPosNumber;
	bool EnemyRespawnFlag[10];

	float m_Timer = 0.0f;

	//BGMの初期音量
	float BGMVolume = 1.0f;
	//効果音の初期音量
	float SoundEffectVolume = 1.0f;

	//プレイヤーの使うキャラの番号
	//０…剣士
	//１…魔法使い
	//２…ゾンビ
	//３…魔物
	int SelectCharNumber = 1;


	int enemyNum = 0;
	char* enemyName;
	wchar_t name_t[255];
};

