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
	/// 
	/// </summary>
	void Respawn();

	/*std::vector<Neutral_Enemy*> GetNeutral_EnemyContaier() {
		return m_enemyCounter;
	}*/

	/// <summary>
	/// �G�l�~�[�̐���炷����
	/// </summary>
	/// <returns>�G�l�~�[�̐�</returns>
	int SubNeutral_EnemyContaier() {
		return enemyNumber--;
	}

	/// <summary>
	/// �G�l�~�[�̐���炷����
	/// </summary>
	/// <returns>�G�l�~�[�̐�</returns>
	int GetNeutral_EnemyContaier() {
		return enemyNumber;
	}

	void SetEnemyRespawnPos();

	void SetRespawnPosition(Vector3 pos, Quaternion rot, int number) {
		EnemyRespawnPosition[number] = pos;
		EnemyReapawnPot[number] = rot;
	};

	void Render(RenderContext& rc);

	void RemoveEnemyFromList(Neutral_Enemy* enemy)
	{
		std::vector<Neutral_Enemy*>::iterator it = std::find(
			m_neutral_Enemys.begin(), // ����J�n�ʒu����C�e���[�^
			m_neutral_Enemys.end(),   // ����I���ʒu����C�e���[�^
			enemy                     // ��������v�f
		);
		if (it != m_neutral_Enemys.end()) {
			m_neutral_Enemys.erase(it);
		}
	}
	//�x�N�^�^��Neutral_Enemy��Ԃ�
	std::vector<Neutral_Enemy*>& GetNeutral_Enemys() {
		return m_neutral_Enemys;
	}

	void Enemys()
	{
		m_neutral_Enemys.pop_back();
	}

	//Actor��Ԃ�
	std::vector<Actor*>& GetActors() {
		return m_Actors;
	}

	/// <summary>
	/// CharacterSelectで選んだキャラをセットする
	/// </summary>
	/// <param name="select">使うキャラナンバー</param>
	void SetCharacterSelect(int select)
	{
		SelectCharNumber = select;
	}

private:
	/// <summary>
	/// �����̓G�̖��O����
	/// </summary>
	char* CreateEnemyName() {
		enemyNum++;
		swprintf_s(name_t, 255, L"Neutral_Enemy%d", enemyNum);
		enemyName = (char*)name_t;
		wcstombs(enemyName, name_t, sizeof(name_t));

		return enemyName;
	}
	/// <summary>
	/// �����̓G����
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

	//BGM�̉��ʒ����Ɏg�p����ϐ�
	float musicVolume = 1.0f;

	//���U���g��ʂ��̃t���O
	bool RezultFlag = false;
	//�|�[�Y��ʂ��̃t���O
	bool PauseOpenFlag = false;
	bool PauseCloseFlag = true;

	//�L�����̔ԍ�
	int SelectCharNumber = 0;		//0剣士１魔法使い


	int enemyNum = 0;
	char* enemyName;
	wchar_t name_t[255];
};

