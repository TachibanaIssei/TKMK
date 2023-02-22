#pragma once
#include "ModelRender.h"

#include "Level3DRender.h"

class BackGround;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Num,
	};

	AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_modelRender;
	Level3DRender m_level3DRender;

	Quaternion m_rotation = Quaternion::Identity;

	BackGround* m_backGround = nullptr;
};

