#pragma once

class Tittle;

class Rezult:public IGameObject
{
public:
	Rezult();
	~Rezult();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	Tittle* tittle;

};

