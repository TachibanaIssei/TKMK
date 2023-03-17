#pragma once

class Game;

class Tittle:public IGameObject
{
public:
	Tittle();
	~Tittle();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	Game* game=nullptr;
	SoundSource* m_bgm = nullptr;

};

