#pragma once

class Tittle;

class Result:public IGameObject
{
public:
	Result();
	~Result();
	void Update();
	bool Start();
	void Render(RenderContext& rc);

	Quaternion m_sRotation;
	SpriteRender m_spriteRender;
	Tittle* tittle=nullptr;

private:
	int charPoints[2];
	int titleScene = 1;

	FontRender moji;

};

