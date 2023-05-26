#pragma once

class Game;

class Pause:public IGameObject
{
public:

	Pause();
	~Pause();

	enum EnPauseMenu
	{
		enPauseMenuState_Back,
		enPauseMenuState_HowToPlay,
		enPauseMenuState_BGM,
		enPauseMenuState_SE,
		enPauseMenuState_QuitGame,
	};
	EnPauseMenu m_EnPauseMenu = enPauseMenuState_Back;

	bool Start();
	void Update();

	/// <summary>
	/// ポーズ時の移動処理
	/// </summary>
	void PauseMove();

	/// <summary>
	/// 選んだ番号に対応したステートにする処理
	/// </summary>
	void SelectMenu();

	/// <summary>
	/// メニュー時のステートの管理
	/// </summary>
	void MenuState();

	void Menu_Back();
	void Menu_HowToPlay();
	void Menu_BGM();
	void Menu_SE();
	void Menu_QuitGame();
	void Push_OK();

	void SetBGMBarScale();
	void SetSEBarScale();

	bool IsDeleteGame() {
		return QuitGameFlag;
	}

	void Render(RenderContext& rc);

private:
	Game* game = nullptr;

	SpriteRender m_Pause_Front;    //ポーズのメイン
	SpriteRender m_Pause_Back;     //背景
	SpriteRender m_Menu_Back;        //Back
	SpriteRender m_Menu_HowToPlay;   //HowToPlay
	SpriteRender m_Menu_BGM;         //BGM
	SpriteRender m_Menu_SE;          //SE
	SpriteRender m_Menu_QuitGame;    //QuitGame
	SpriteRender m_Menu_BGM_Bar;   //SelectBar_BGM
	SpriteRender m_Menu_SE_Bar;   //SelectBar_SE
	SpriteRender m_Menu_Flame_BGM;   //SelectBar_BGM
	SpriteRender m_Menu_Flame_SE;   //SelectBar_SE
	SpriteRender m_operationPic;        //操作説明

	//操作説明画面の画像を出すかのフラグ
	bool HowToPlaySpriteFlag = false;

	bool QuitGameFlag = false;

	//メニュー
	int MenuNumber = 0;
	int MenuNumber_old = 0;

	const float m_addVolume = 0.2f;
	const float m_subVolume = -0.2f;


};

