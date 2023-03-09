#pragma once

class KnightPlayer;

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
	FontRender m_LevelNameFont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_AtkFont;
	FontRender m_SpeedFont;

	KnightPlayer* m_knightplayer=nullptr;
	//UI
	SpriteRender			m_hpBar;														//HPバーの画像
	SpriteRender			m_mpBar;														//MPバーの画像
	SpriteRender			m_statusBar;													//ステータスの画像
	SpriteRender			m_playerFaceFrame;												//プレイヤーの顔画像の枠
	SpriteRender			m_playerFaceBack;												//プレイヤーの顔画像の背景
	SpriteRender			m_playerFace;

	//int LEVEL;
};

