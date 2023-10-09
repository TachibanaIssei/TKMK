#pragma once

struct Status;
class Actor;

class EnemyHpBar : public IGameObject
{
public:
	EnemyHpBar() {};
	~EnemyHpBar();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Init(const int viewportNo = -1);

	/// <summary>
	/// HP�o�[�̍��W���v�Z����
	/// </summary>
	/// <param name="cameraNo">�J�����ԍ�</param>
	/// <param name="status">�X�e�[�^�X</param>
	/// <param name="characterPosition">�L�����N�^�[�̍��W</param>
	/// <param name="lv">�L�����N�^�[�̃��x��</param>
	void CalcHpBarPosition(const int cameraNo, Status* status,const Vector3& characterPosition,const int lv);
	/// <summary>
	/// �`��t���O��ݒ肷��
	/// </summary>
	/// <param name="flag"></param>
	void SetDrawFlag(const bool flag)
	{
		m_drawFlag = flag;
	}

private:
	//�G�L�����㕔�̃��x���摜��������
	void InitLvSprite();

	/// <summary>
	/// �Q�[�W�����񂹂��鏈��
	/// </summary>
	/// <param name="size">�摜�̌��̑傫��</param>
	/// <param name="scale">���݂̃X�P�[���{��</param>
	/// <returns>�ϊ��O�ƕϊ���̍�</returns>
	Vector3 LeftAlignHPBar(const Vector3& size, const Vector3& scale);

	/// <summary>
	/// �L�����N�^�[�̃��x����ݒ肷��
	/// </summary>
	/// <param name="level"></param>
	void SetCharacterLevel(const int level);

private:
	/// <summary>
	/// �L�����N�^�[�̃��x����m_lvSprite�̗v�f�w��Ɏg�p����
	/// </summary>
	enum EnCharacterLevel
	{
		enCharacterLevel_One = 0,
		enCharacterLevel_Two,
		enCharacterLevel_Three,
		enCharacterLevel_Four,
		enCharacterLevel_Five,
		enCharacterLevel_Six,
		enCharacterLevel_Seven,
		enCharacterLevel_Eight,
		enCharacterLevel_Nine,
		enCharacterLevel_Ten,
	};

private:
	const static int m_maxLevel = 10;
	std::array<SpriteRender, m_maxLevel> m_lvSprite;
	SpriteRender		m_hpBar;		//HP�o�[�摜
	SpriteRender		m_hpBack;		//HP�w�i�摜
	Vector2				m_hpBerPosition = Vector2::Zero;		//HP�o�[�̃|�W�V����
	Vector2				m_hpBackPosition = Vector2::Zero;		//HP�w�i�̃|�W�V����
	Vector2				m_levelSpritePosition = Vector2::Zero;
	EnCharacterLevel	m_level = enCharacterLevel_One;
	int					m_viewportNo = 0;					//�r���[�|�[�g�̔ԍ�
	bool				m_drawFlag = false;
};

