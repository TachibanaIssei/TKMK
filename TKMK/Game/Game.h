#pragma once
#include "ModelRender.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
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

	Quaternion m_rotation = Quaternion::Identity;
};

