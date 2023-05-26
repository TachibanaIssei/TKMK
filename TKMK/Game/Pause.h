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
	/// �|�[�Y���̈ړ�����
	/// </summary>
	void PauseMove();

	/// <summary>
	/// �I�񂾔ԍ��ɑΉ������X�e�[�g�ɂ��鏈��
	/// </summary>
	void SelectMenu();

	/// <summary>
	/// ���j���[���̃X�e�[�g�̊Ǘ�
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

	SpriteRender m_Pause_Front;    //�|�[�Y�̃��C��
	SpriteRender m_Pause_Back;     //�w�i
	SpriteRender m_Menu_Back;        //Back
	SpriteRender m_Menu_HowToPlay;   //HowToPlay
	SpriteRender m_Menu_BGM;         //BGM
	SpriteRender m_Menu_SE;          //SE
	SpriteRender m_Menu_QuitGame;    //QuitGame
	SpriteRender m_Menu_BGM_Bar;   //SelectBar_BGM
	SpriteRender m_Menu_SE_Bar;   //SelectBar_SE
	SpriteRender m_Menu_Flame_BGM;   //SelectBar_BGM
	SpriteRender m_Menu_Flame_SE;   //SelectBar_SE
	SpriteRender m_operationPic;        //�������

	//���������ʂ̉摜���o�����̃t���O
	bool HowToPlaySpriteFlag = false;

	bool QuitGameFlag = false;

	//���j���[
	int MenuNumber = 0;
	int MenuNumber_old = 0;

	const float m_addVolume = 0.2f;
	const float m_subVolume = -0.2f;


};

