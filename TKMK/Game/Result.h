#pragma once

#include <array>

#define PLAYER 4
#define MOVE 4

class Tittle;
class Fade;

class Result :public IGameObject
{
public:
	Result();
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//Result�̃V�[���ϊ�
	enum EnChange {
		enChange_1st = 0,
		enChange_2nd = 1,
		enChange_3rd = 2,
		enChange_4th = 3,
		enChange_first,		//�ŏ��̏���
		enChange_move,		//���`�⊮��
		enChange_stop,		//�I�������
		enChange_Num,
	};

	//�J�[�\���̏ꏊ
	enum EnCursorPos {
		enCursorPos_title,	//�^�C�g���ɖ߂�̏ꏊ
		enCursorPos_exit,	//�Q�[���I���̏ꏊ
		enCursorPos_Num,
	};
	
	//�e�v���C���[�̃X�R�A��ۑ����邽�߂̂���
	struct Score {
		int Point = 0;		//�l���|�C���g
		int NameNum = 1;	
		int Rank = 1;		//����
	};

private:
	void InitSprite();
	void Rank();
	void MoveLerp();
	void MovePointFont();
	void Select();
	void MoveName();


private:
	int charPoints[PLAYER];		//�v���C���[�̃|�C���g
	int titleScene = 1;			//�^�C�g���̃V�[���ԍ�
	int m_nowMoveRank = 0;
	int m_nowMoveCharacter = 0;
	int m_timer = 0;			//�^�C�}�[

	float m_complement = 0.0f;	//���`��Ԃ̕⊮��
	float WORD_SCALE = 3.0f;
	float m_bgmVolume = 0.4f;
	float m_seVolume = 0.6f;

	bool m_selectFlag			= false;	//�I�����ڂ̐؂�ւ��t���O
	bool m_drawSelectSpriteFlag = false;	//�I�����ڂ̕\���̃t���O

	Score m_playerScore[PLAYER];

	ModelRender m_No1Charctar;		//��ʂ̃L�����̃��f��

	FontRender	m_playerRank1;
	FontRender	m_playerRank2;
	FontRender	m_playerRank3;
	FontRender	m_playerRank4;
	EnChange	m_change = enChange_first;
	EnCursorPos m_cursor;

	SpriteRender	m_spriteRender;		//�w�i
	SpriteRender	m_goTitle;			//"�^�C�g���֖߂�"��I��
	SpriteRender	m_goTitleSelect;	//"�^�C�g���֖߂�"�I��
	SpriteRender	m_gameover;			//"�Q�[���I��"��I��
	SpriteRender	m_gameoverST;		//"�^�C�g���֖߂�"�I��
	SpriteRender	m_playerName;		//"Player"���O���Ԃ肾����ς���
	SpriteRender	m_cpuName1;			//CPU1
	SpriteRender	m_cpuName2;			//CPU2
	SpriteRender	m_cpuName3;			//CPU3
	SpriteRender	m_resultLogo;		//���U���g�̃��S

	SpriteRender m_choiceCursor;		//�I�����̃J�[�\��

	Tittle* tittle = nullptr;
	SoundSource* m_bgm = nullptr;
	Fade* fade = nullptr;

	Vector3 PointRight = { 200.0f,85.0f,0.0f };

	//���`��Ԃł����܂œ�����
	std::array<Vector3,MOVE> m_lerpMoveEnd = {		//����
		Vector3(0.0f, 160.0f, 0.0f),		//�P��
		Vector3(0.0f, 20.0f, 0.0f),			//�Q��
		Vector3(0.0f, -120.0f, 0.0f),		//�R��
		Vector3(0.0f, -255.0f, 0.0f)		//�S��
	};

	//���`��Ԃœ������Ƃ��̍��W
	std::array<Vector3, MOVE> m_lerpMoving = {		//����
		Vector3(g_vec3Zero),		//�P��
		Vector3(g_vec3Zero),		//�Q��
		Vector3(g_vec3Zero),		//�R��
		Vector3(g_vec3Zero)			//�S��
	};

	//���`��ԑO�̍��W
	std::array<Vector3, MOVE> m_lerpStartPos = {	//����
		Vector3(-2500.0f, 160.0f, 0.0f),	//�P��
		Vector3(-2500.0f, 20.0f, 0.0f),		//�Q��
		Vector3(-2500.0f, -120.0f, 0.0f),	//�R��
		Vector3(-2500.0f, -255.0f, 0.0f)	//�S��
	};

	//�����x
	Vector4 m_alphaColorUnSelect	= { 0.0f,0.0f,0.0f,0.0f };	//����
	Vector4 m_alphaColorSelect		= { g_vec4White };			//�s����
};