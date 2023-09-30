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
	/// <summary>
	/// �Q�[�W�����񂹂��鏈��
	/// </summary>
	/// <param name="size">�摜�̌��̑傫��</param>
	/// <param name="scale">���݂̃X�P�[���{��</param>
	/// <returns>�ϊ��O�ƕϊ���̍�</returns>
	Vector3 LeftAlignHPBar(const Vector3& size, const Vector3& scale);

private:
	SpriteRender		m_HP_Bar;		//HP�o�[�摜
	SpriteRender		m_HP_Frame;		//HP�g�摜
	SpriteRender		m_HP_Back;		//HP�w�i�摜
	FontRender			m_fontLv;
	Vector2				m_HPBer_Pos = Vector2::Zero;		//HP�o�[�̃|�W�V����
	Vector2				m_HPWindow_Pos = Vector2::Zero;		//HP�g�̃|�W�V����
	Vector2				m_HPBack_Pos = Vector2::Zero;		//HP�w�i�̃|�W�V����
	Vector2				m_levelFontPos = Vector2::Zero;		//HP�o�[�̉��ɕ\�����郌�x��

	int					m_viewportNo = 0;					//�r���[�|�[�g�̔ԍ�
	bool				m_drawFlag = false;
};

