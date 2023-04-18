#pragma once

//class KnightPlayer;
//class WizardPlayer;
class Player;
class Game;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	enum GameUIState
	{
		m_GameState,
		m_PauseState,
		m_BattleEndState,
	};
	GameUIState m_GameUIState;

	bool Start();
	void Update();

	void SetSGame(Game* Cgame)
	{
		m_game = Cgame;
	}

	/// <summary>
	/// �v���C���[�̃��x���ɍ��킹�ă��x���̉摜��ύX����
	/// </summary>
	/// <param name="lv">�v���C���[�̌��݂̃��x��</param>
	void LevelFontChange(int lv)
	{
		switch (lv)
		{
		case 1:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv10.DDS", 150.0f, 150.0f);
			break;
		case 2:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv2.DDS", 150.0f, 150.0f);
			break;
		case 3:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv3.DDS", 150.0f, 150.0f);
			break;
		case 4:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv4.DDS", 150.0f, 150.0f);
			break;
		case 5:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv5.DDS", 150.0f, 150.0f);
			break;
		case 6:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv6.DDS", 150.0f, 150.0f);
			break;
		case 7:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv7.DDS", 150.0f, 150.0f);
			break;
		case 8:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv8.DDS", 150.0f, 150.0f);
			break;
		case 9:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv9.DDS", 150.0f, 150.0f);
			break;
		case 10:
			m_LvNumber.Init("Assets/sprite/gameUI/Lv10.DDS", 150.0f, 150.0f);
			break;
		default:
			break;
		}
	}

	void Render(RenderContext& rc);

	/// HP�o�[�̕\��
    /// </summary>
	void HPBar();

	/// <summary>
	/// GamwUI�̃X�e�[�g��ύX
	/// </summary>
	/// <param name="gamescene">�ύX�������X�e�[�g�̖��O</param>
	void SetGameUIState(GameUIState gamescene) {
		m_GameUIState = gamescene;

	}

	void Timer();

	/// <summary>
	/// �Q�[�W�����񂹂��鏈��
	/// </summary>
	/// <param name="size">�摜�̌��̑傫��</param>
	/// <param name="scale">���݂̃X�P�[���{��</param>
	/// <returns>�ϊ��O�ƕϊ���̍�</returns>
	//Vector3 HPBerSend(Vector3 size, Vector3 scale)
	//{
	//	Vector3 hpBerSize = size;								//�摜�̌��̑傫��
	//	Vector3 changeBerSize = Vector3::Zero;					//�摜���X�P�[���ϊ��������Ƃ̑傫��
	//	Vector3 BerSizeSubtraction = Vector3::Zero;				//�摜�̌��ƕϊ���̍�

	//	changeBerSize.x = hpBerSize.x * scale.x;
	//	BerSizeSubtraction.x = hpBerSize.x - changeBerSize.x;
	//	BerSizeSubtraction.x /= 2.0f;

	//	return BerSizeSubtraction;
	//}
private:
	FontRender m_LevelFont;
	FontRender m_LevelNameFont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_AtkFont;
	FontRender m_SpeedFont;

	FontRender m_PointFont;

	/*KnightPlayer* m_knightplayer=nullptr;
	WizardPlayer* wizardPlayer = nullptr;*/
	Player* player = nullptr;
	Game* m_game = nullptr;
	//UI
	SpriteRender			m_hpBar;				//HP�o�[�̉摜
	SpriteRender			m_statusBar;			//�X�e�[�^�X�̉摜
	SpriteRender			m_HPFrame;		//�v���C���[�̊�摜�̘g
	SpriteRender			m_playerFaceBack;		//�v���C���[�̊�摜�̔w�i
	SpriteRender            m_SkillRender;          //�X�L���A�C�R��
	SpriteRender            m_UltRender;            //�K�E�A�C�R��
	SpriteRender            m_TimeAndPointRender;   //�������ԂƊl���|�C���g
	SpriteRender            m_Lv;
	SpriteRender            m_LvNumber;
	SpriteRender            m_MaxLv;
	SpriteRender            m_Flame;                //�������ԂƊl���|�C���g��HP�o�[�̉摜��
	SpriteRender            m_Point;                //�|�C���g
	SpriteRender            m_ExperienceFlame;        //�o���l�̃t���[��
	SpriteRender            m_ExperienceBar_flont;    //�o���l�o�[�̕\
	
	Vector2				m_HPBerPos = Vector2::Zero;				//HP�o�[�̃|�W�V����
	Vector2				m_HPWindowPos = Vector2::Zero;			//HP�g�̃|�W�V����
	Vector2				m_HPBackPos = Vector2::Zero;			//HP�w�i�̃|�W�V����
	
	
	FontRender m_time_left;

	//�b���v��^�C�}�[
	float SecondsTimer=0.0f;
	//�����v��^�C�}�[
	float MinutesTimer=5.0f;
	float m_timer = 300.0f;

	bool GameEndFlag=false;
	//int LEVEL;
};

