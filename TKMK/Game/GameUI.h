#pragma once

class KnightBase;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	FontRender m_LevelFont;
	FontRender m_HpFont;
	FontRender m_AtkFont;
	FontRender m_SpeedFont;
	KnightBase* m_knightbase=nullptr;

	int LEVEL;
};

