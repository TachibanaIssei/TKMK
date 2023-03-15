#pragma once

#include "Level3DRender.h"

class BackGround;
class Result;
class Tittle;
class GameCamera;
class KnightBase;
class KnightPlayer;
class Neutral_Enemy;
class GameUI;
class Map;
class KnightAI;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Pause();
	void GameState();

	

	void Render(RenderContext& rc);

	enum EnGameState {
		enGameState_Start,
		enGameState_Battle,
		enGameState_Pause,
		enGamestate_End,
		enGameState_Rezult,
		enGameState_Num,
	};
	

private:
	

	EnGameState m_GameState = enGameState_Start;


	//AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_modelRender;
	Level3DRender m_level3DRender;
	Level3DRender m_Enemylevel;
	FontRender m_fontRender;

	Quaternion m_rotation = Quaternion::Identity;
	Quaternion m_sRotation = Quaternion::Identity;

	SpriteRender m_Pause_Front;    //�|�[�Y���
	SpriteRender m_Pause_Back;     //�|�[�Y�̗����


	BackGround* m_backGround = nullptr;
	Result* m_rezult=nullptr;
	Tittle* m_tittle = nullptr;
	GameCamera* m_gamecamera = nullptr;
	KnightBase* m_knightbase = nullptr;
	GameUI* m_gameUI = nullptr;
	KnightPlayer* m_knightplayer = nullptr;
	KnightAI* m_KnightAI = nullptr;
	Neutral_Enemy* m_Neutral_Enemy = nullptr;
	Map* m_Map = nullptr;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	float m_spriteAlpha = 0.0f;


	//���U���g��ʂ��̃t���O
	bool RezultFlag = false;
	//�|�[�Y��ʂ��̃t���O
	bool PauseOpenFlag = false;
	bool PauseCloseFlag = true;
};

