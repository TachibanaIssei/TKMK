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
	SpriteRender			m_hpBar;														//HP�o�[�̉摜
	SpriteRender			m_mpBar;														//MP�o�[�̉摜
	SpriteRender			m_statusBar;													//�X�e�[�^�X�̉摜
	SpriteRender			m_playerFaceFrame;												//�v���C���[�̊�摜�̘g
	SpriteRender			m_playerFaceBack;												//�v���C���[�̊�摜�̔w�i
	SpriteRender			m_playerFace;

	//int LEVEL;
};

