#pragma once

//class KnightPlayer;
//class WizardPlayer;
class Player;
class Game;
class Actor;
class Fade;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	enum GameUIState
	{
		m_GameStartState,
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
			m_LvNumber.Init("Assets/sprite/gameUI/Lv1.DDS", 150.0f, 150.0f);
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
	/// 
	/// </summary>
	void CountDown();

	/// <summary>
	/// 
	/// </summary>
	void RespawnCountDown();

	/// <summary>
	/// 
	/// </summary>
	void EXPBar();

	/// <summary>
	/// 
	/// </summary>
	void CharPoint();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="size"></param>
	/// <param name="scale"></param>
	/// <returns></returns>
	Vector3& HPBerSend(Vector3 size, Vector3 scale);

private:
	FontRender m_ExpFont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_PointFont[4];

	Player* player = nullptr;
	Game* m_game = nullptr;
	Actor* actor = nullptr;
	Fade* fade = nullptr;

	std::vector<Actor*> m_Actors;

	//UI
	SpriteRender			m_hpBar;							//HP�o�[�̉摜
	SpriteRender			m_statusBar;						//�X�e�[�^�X�̉摜
	SpriteRender			m_HPFrame;							//�v���C���[�̊�摜�̘g
	SpriteRender			m_playerFaceBack;					//�v���C���[�̊�摜�̔w�i
	SpriteRender            m_SkillRender;						//�X�L���A�C�R��
	SpriteRender            m_UltRender;						//�K�E�A�C�R��
	SpriteRender            m_TimeAndPointRender;				//�������ԂƊl���|�C���g
	SpriteRender            m_Lv;
	SpriteRender            m_LvNumber;
	SpriteRender            m_MaxLv;
	SpriteRender            m_Flame;							//�������ԂƊl���|�C���g��HP�o�[�̉摜��
	SpriteRender            m_Point;							//�|�C���g
	SpriteRender            m_ExperienceFlame;					//�o���l�̃t���[��
	SpriteRender            m_ExperienceBar_flont;				//�o���l�o�[�̕\
	SpriteRender            m_ExperienceBar_back;				//�o���l�o�[�̗�
	SpriteRender			m_Crown;							//�|�C���g����ԑ����L�����ɂ��鉤���}�[�N
	SpriteRender			m_PointFlame[4];					//�|�C���g��\������t���[��
	SpriteRender            m_CharIcon[4];                      //�L�����̃A�C�R��
	SpriteRender			m_CountNumper;					//�J�E���g�_�E��
	SpriteRender			m_RespawnIn;						//Respawn in�̉摜
	SpriteRender			m_Respawn_Back;						//���X�|�[�����̔w�i
	SpriteRender			m_RespawnCountNumber;				//���X�|�[�����̃J�E���g�_�E��

	
	Vector2				m_HPBerPos = Vector2::Zero;				//HP�o�[�̃|�W�V����
	Vector2				m_HPWindowPos = Vector2::Zero;			//HP�g�̃|�W�V����
	Vector2				m_HPBackPos = Vector2::Zero;			//HP�w�i�̃|�W�V����

	Vector3				m_EXPBerPos = Vector3::Zero;				   //EXP�o�[�̃|�W�V����

	Vector3				m_gameCountScale = Vector3(0.2f,0.2f,0.0f);
	Vector3				m_FightScale = Vector3(1.0f, 1.0f, 0.0f);
	bool				m_fightFlag = false;
	float				m_GameCountScale = 20.0f;
	float               m_Color = 1.0f;

	Vector3 PointPos[4] = {
		Vector3(-850.0f, 150.0f, 0.0f),
		Vector3(-850.0f, 50.0f, 0.0f), 
		Vector3(-850.0f, -50.0f, 0.0f), 
		Vector3(-850.0f, -150.0f, 0.0f), 
	};															//�|�C���g

	Vector3 PointFlamePos[4] = {
		Vector3(-850.0f, 120.0f, 0.0f),
		Vector3(-850.0f, 20.0f, 0.0f),
		Vector3(-850.0f, -80.0f, 0.0f),
		Vector3(-850.0f, -180.0f, 0.0f),
	};															//�|�C���g

	Vector3 CharIconPos[4] = {
		Vector3(-920.0f, 120.0f, 0.0f),
		Vector3(-920.0f, 20.0f, 0.0f),
		Vector3(-920.0f, -80.0f, 0.0f),
		Vector3(-920.0f, -180.0f, 0.0f),
	};															//�A�C�R��
	
	Vector3 CrownPos[4] = {
		Vector3(-920.0f, 120.0f, 0.0f),
		Vector3(-920.0f, 20.0f, 0.0f),
		Vector3(-920.0f, -80.0f, 0.0f),
		Vector3(-920.0f, -180.0f, 0.0f),
	};															//�����}�[�N

	FontRender m_time_left;

	const char* knightname = "knightplayer";
	const char* wizardname = "wizardplayer";

	int oldtStartCount = 0;

	int oldRespawnCount = 0;

	//�b���v��^�C�}�[
	float SecondsTimer=0.0f;
	//�����v��^�C�}�[
	float MinutesTimer=5.0f;
	float m_timer = 300.0f;

	bool GameEndFlag=false;

	//�O�t���[���̌o���l
	int oldEXP=0;

	//�L�����̃|�C���g
	int charPoint[4];

	int MaxPoint = 0;

	


};

