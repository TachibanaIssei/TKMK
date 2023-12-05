#pragma once

#include "../k2EngineLow/mySourceCode/Camera/SpringCamera.h"

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
		int m_point = 0;		//�l���|�C���g
		int NameNum = 1;	
		int Rank = 1;		//����
	};

private:
	void InitNameSprite();
	void InitSprite();
	void InitModel();
	void InitSkyCube();
	void PlayEffect();
	void SetCamera();
	void SetCharacterState();
	void Rank();
	void MoveLerp();
	void MovePointFont();
	void Select();
	void MoveName();
	void PlayAnimation();

	enum EnAnimationClip
	{
		enAnimationClip_Idle,
		enAnimationClip_Win,
		enAnimationClip_Lose,
		enAnimationClip_4th,
		enAnimationClip_Num
	};

	enum EnCharacterState
	{
		enCharacterState_Idle,
		enCharacterState_Win,
		enCharacterState_Lose,
		enCharacterState_4th,
		enCharacterState_Num
	};

private:
	const static int m_maxPlayer = 4;

	SpringCamera m_camera;
	SkyCube* m_skyCube;					//�X�J�C�L���[�u
	std::array<ModelRender, m_maxPlayer> m_knightModel;	//���m�̃��f��
	ModelRender m_backGround;			//�n��
	ModelRender m_backWall;				//��
	EffectEmitter* m_fireWorks;
	RenderingEngine::EnGameMode	m_gameMode;
	AnimationClip m_animationClips[enAnimationClip_Num];
	std::array<EnCharacterState, m_maxPlayer> m_charaState = { enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle };
	std::array<EnCharacterState, m_maxPlayer> m_stayCharaState = { enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle,enCharacterState_Idle };

	std::array<int, m_maxPlayer> charPoints = {0,0,0,0};		//�v���C���[�̃|�C���g

	int titleScene = 1;			//�^�C�g���̃V�[���ԍ�
	int m_nowMoveRank = 0;
	int m_nowMoveCharacter = 0;
	int m_timer = 0;			//�^�C�}�[

	float m_complement = 0.0f;	//���`��Ԃ̕⊮��
	float m_bgmVolume = 0.4f;
	float m_seVolume = 1.0f;

	bool m_selectFlag			= false;	//�I�����ڂ̐؂�ւ��t���O
	bool m_drawSelectSpriteFlag = false;	//�I�����ڂ̕\���̃t���O
	bool m_fireWorksPlayFlag = false;		//�ԉ΃G�t�F�N�g�������t���O

	bool m_isInit = false;

	std::array<Score, m_maxPlayer> m_playerScore;

	ModelRender m_No1Charctar;		//��ʂ̃L�����̃��f��

	FontRender	m_playerRank1;
	FontRender	m_playerRank2;
	FontRender	m_playerRank3;
	FontRender	m_playerRank4;
	EnChange	m_change = enChange_first;
	EnCursorPos m_cursor;

	SpriteRender	m_spriteRender;		//�w�i
	SpriteRender	m_resultLogo;		//���U���g�̃��S
	SpriteRender	m_goTitle;			//"�^�C�g���֖߂�"��I��
	SpriteRender	m_goTitleSelect;	//"�^�C�g���֖߂�"�I��
	SpriteRender	m_gameover;			//"�Q�[���I��"��I��
	SpriteRender	m_gameoverST;		//"�^�C�g���֖߂�"�I��

	std::array<SpriteRender, 4> m_pointsUnit;	//"p"�̉摜
	std::array<SpriteRender, 4> m_knightFace;
	std::array<SpriteRender, 4> m_namePlate;	//���O�̔w�i�摜
	std::array<SpriteRender, 4> m_gameRank;		//���ʂ̉摜

	SpriteRender	m_playerName;		//"Player"
	SpriteRender	m_cpuName1;			//CPU1
	SpriteRender	m_cpuName2;			//CPU2
	SpriteRender	m_cpuName3;			//CPU3

	SpriteRender m_choiceCursor;		//�I�����̃J�[�\��

	Tittle* tittle = nullptr;
	SoundSource* m_bgm = nullptr;
	Fade* m_fade = nullptr;

	//�t�H���g�̐��`��ԑO�̍��W
	std::array<Vector3, m_maxPlayer> m_lerpStartPos = {	//����
		Vector3(-2100.0f, 300.0f, 0.0f),		//�P��
		Vector3(-2100.0f, 145.0f, 0.0f),		//�Q��
		Vector3(-2100.0f, 0.0f, 0.0f),		//�R��
		Vector3(-2100.0f, -145.0f, 0.0f)		//�S��
	};

	//�X�v���C�g�̐��`��ԑO�̍��W
	std::array<Vector3, m_maxPlayer> m_spriteLerpStartPos = {	//����
		Vector3(-2100.0f, 180.0f, 0.0f),		//�P��
		Vector3(-2100.0f, 25.0f, 0.0f),			//�Q��
		Vector3(-2100.0f, -117.0f, 0.0f),		//�R��
		Vector3(-2100.0f, -263.0f, 0.0f)		//�S��
	};

	//�t�H���g����`��Ԃł����܂œ�����
	std::array<Vector3, m_maxPlayer> m_lerpMoveEnd = {		//����
		Vector3(370.0f, 300.0f, 0.0f),		//�P��
		Vector3(370.0f, 145.0f, 0.0f),		//�Q��
		Vector3(370.0f, 4.0f, 0.0f),		//�R��
		Vector3(370.0f, -145.0f, 0.0f)		//�S��
	};

	//�X�v���C�g����`��Ԃł����܂œ�����
	std::array<Vector3, m_maxPlayer> m_spriteLerpMoveEnd = {		//����
		Vector3(300.0f, 180.0f, 0.0f),		//�P��
		Vector3(300.0f, 25.0f, 0.0f),		//�Q��
		Vector3(300.0f, -117.0f, 0.0f),		//�R��
		Vector3(300.0f, -265.0f, 0.0f)		//�S��
	};

	//�t�H���g����`��Ԃœ������Ƃ��̍��W
	std::array<Vector3, m_maxPlayer> m_lerpMoving = {		//����
		Vector3(g_vec3Zero),		//�P��
		Vector3(g_vec3Zero),		//�Q��
		Vector3(g_vec3Zero),		//�R��
		Vector3(g_vec3Zero)			//�S��
	};

	//�X�v���C�g����`��Ԃœ������Ƃ��̍��W
	std::array<Vector3, m_maxPlayer> m_spriteLerpMoving = {		//����
		Vector3(g_vec3Zero),		//�P��
		Vector3(g_vec3Zero),		//�Q��
		Vector3(g_vec3Zero),		//�R��
		Vector3(g_vec3Zero)			//�S��
	};

	//�����x
	Vector4 m_alphaColorUnSelect	= { 0.0f,0.0f,0.0f,0.0f };	//����
	Vector4 m_alphaColorSelect		= { g_vec4White };			//�s����
};