#pragma once

#include "Level3DRender.h"

class BackGround;
class Result;
class Tittle;
class GameCamera;
//class KnightBase;
class KnightPlayer;
class Neutral_Enemy;
class GameUI;
class Map;
class KnightAI;
class WizardPlayer;
class Player;
class CharUltFlag;
class Actor;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Pause();
	void GameState();
	
	/// <summary>
	/// 
	/// </summary>
	void Respawn();

	/*std::vector<Neutral_Enemy*> GetNeutral_EnemyContaier() {
		return m_enemyCounter;
	}*/

	/// <summary>
	/// エネミーの数を減らす処理
	/// </summary>
	/// <returns>エネミーの数</returns>
	int SubNeutral_EnemyContaier() {
		return enemyNumber--;
	}

	/// <summary>
	/// エネミーの数を減らす処理
	/// </summary>
	/// <returns>エネミーの数</returns>
	int GetNeutral_EnemyContaier() {
		return enemyNumber;
	}

	/// <summary>
	/// 繝ｪ繧ｹ繝昴�繝ｳ縺吶ｋ蠎ｧ讓吶ｒ險ｭ螳
	/// </summary>
	/// <returns></returns>
	Vector3 SetEnemyRespawnPos();



	/// <summary>
	/// 繝ｪ繧ｹ繝昴�繝ｳ縺吶ｋ蠎ｧ讓吶ｒ譬ｼ邏阪☆繧
	/// </summary>
	/// <param name="pos">繧ｪ繝悶ず繧ｧ繧ｯ繝医�蠎ｧ讓</param>
	/// <param name="rot">繧ｪ繝悶ず繧ｧ繧ｯ繝医�蝗櫁ｻ｢驥</param>
	/// <param name="number">繧ｪ繝悶ず繧ｧ繧ｯ繝医�逡ｪ蜿ｷ</param>
	void SetRespawnPosition(Vector3 pos, Quaternion rot, int number) {
		EnemyRespawnPosition[number] = pos;
		EnemyReapawnPot[number] = rot;
	};

	void Render(RenderContext& rc);

	enum EnGameState {
		enGameState_Start,
		enGameState_Battle,
		enGameState_Pause,
		enGamestate_End,
		enGameState_Rezult,
		enGameState_Num,
	};
	//Enemy????
	std::vector<Neutral_Enemy*>& GetNeutral_Enemys() {
		return m_neutral_Enemys;
	}
	//Actorを返す
	std::vector<Actor*>& GetActors() {
		return m_Actors;
	}

private:
	/// <summary>
	/// 中立の敵の名前を作る
	/// </summary>
	char* CreateEnemyName() {
		enemyNum++;
		swprintf_s(name_t, 255, L"Neutral_Enemy%d", enemyNum);
		enemyName = (char*)name_t;
		wcstombs(enemyName, name_t, sizeof(name_t));

		return enemyName;
	}
	/// <summary>
	/// 中立の敵を作る
	/// </summary>
	void CreateEnemy(Vector3 pos, Quaternion rot);

	//
	EnGameState m_GameState = enGameState_Start;


	//AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_modelRender;
	Level3DRender m_level3DRender;
	Level3DRender m_Enemylevel;
	FontRender m_fontRender;

	Quaternion m_rotation = Quaternion::Identity;
	Quaternion m_sRotation = Quaternion::Identity;
	Vector3 EnemyRespawnPosition[9];          //繝ｬ繝吶Ν縺ｮ荳ｭ遶九�謨ｵ縺ｮ繝ｪ繧ｹ繝昴�繝ｳ縺吶ｋ蠎ｧ讓吶ｒ譬ｼ邏阪☆繧
	Quaternion EnemyReapawnPot[9];            //繝ｬ繝吶Ν縺ｮ荳ｭ遶九�謨ｵ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医�蝗櫁ｻ｢驥上ｒ譬ｼ邏阪☆繧
	SpriteRender m_Pause_Front;    //ポーズ画面
	SpriteRender m_Pause_Back;     //ポーズの裏画面

	BackGround* m_backGround = nullptr;
	Result* m_rezult=nullptr;
	Tittle* m_tittle = nullptr;
	GameCamera* m_gamecamera = nullptr;
	//KnightBase* m_knightbase = nullptr;
	GameUI* m_gameUI = nullptr;
	KnightPlayer* m_knightplayer = nullptr;
	KnightAI* m_KnightAI = nullptr;
	Neutral_Enemy* neutral_Enemy = nullptr;
	Map* m_Map = nullptr;
	SoundSource* m_bgm = nullptr;	//
	WizardPlayer* wizardPlayer = nullptr;
	Player* player = nullptr;
	CharUltFlag* charUltFlag = nullptr;

	std::vector<Neutral_Enemy*> m_neutral_Enemys;
	std::vector<Actor*> m_Actors;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	float m_spriteAlpha = 0.0f;

	int enemyNumber = 0;

	int SearchRespawnPos;       //繝ｪ繧ｹ繝昴�繝ｳ縺吶ｋ蠎ｧ讓吶�逡ｪ蜿ｷ繧呈爾縺

	//bool RespawnNumberBox[50];

	float m_Timer = 0.0f;

	//BGMの音量調整に使用する変数
	float musicVolume = 1.0f;

	//リザルト画面かのフラグ
	bool RezultFlag = false;
	//ポーズ画面かのフラグ
	bool PauseOpenFlag = false;
	bool PauseCloseFlag = true;


	//キャラの番号
	int SelectCharNumber = 1;


	int enemyNum = 0;
	char* enemyName;
	wchar_t name_t[255];
};

