#pragma once

#include "Level3DRender.h"

class BackGround;
class Result;
class GameCamera;
class KnightBase;
class KnightPlayer;
class Neutral_Enemy;
//class GameUI;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();

	void TestPlayer();
	void OnAnimationEvent(const wchar_t* clipName,const wchar_t* eventName);

	void Render(RenderContext& rc);

private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};

	AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_modelRender;
	Level3DRender m_level3DRender;
	FontRender m_fontRender;

	Quaternion m_rotation = Quaternion::Identity;
	Quaternion m_sRotation = Quaternion::Identity;
	SpriteRender m_spriteRender;
	BackGround* m_backGround = nullptr;
	Result* m_rezult=nullptr;
	GameCamera* m_gamecamera = nullptr;
	KnightBase* m_knightbase = nullptr;
	//GameUI* m_gameUI = nullptr;
	KnightPlayer* m_knightplayer = nullptr;
	Neutral_Enemy* m_NE = nullptr;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	float m_spriteAlpha = 0.0f;
};

