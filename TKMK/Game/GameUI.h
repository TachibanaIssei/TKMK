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
	/// HP�o�[�̕\��
/// </summary>
	void HPBar();

	/// <summary>
	/// �Q�[�W�����񂹂��鏈��
	/// </summary>
	/// <param name="size">�摜�̌��̑傫��</param>
	/// <param name="scale">���݂̃X�P�[���{��</param>
	/// <returns>�ϊ��O�ƕϊ���̍�</returns>
	Vector3 HPBerSend(Vector3 size, Vector3 scale)
	{
		Vector3 hpBerSize = size;								//�摜�̌��̑傫��
		Vector3 changeBerSize = Vector3::Zero;					//�摜���X�P�[���ϊ��������Ƃ̑傫��
		Vector3 BerSizeSubtraction = Vector3::Zero;				//�摜�̌��ƕϊ���̍�

		changeBerSize.x = hpBerSize.x * scale.x;
		BerSizeSubtraction.x = hpBerSize.x - changeBerSize.x;
		BerSizeSubtraction.x /= 2.0f;

		return BerSizeSubtraction;
	}
private:
	FontRender m_LevelFont;
	FontRender m_LevelNameFont;

	FontRender m_HpFont;
	FontRender m_HpNameFont;

	FontRender m_AtkFont;
	FontRender m_SpeedFont;

	KnightPlayer* m_knightplayer=nullptr;
	//UI
	SpriteRender			m_hpBar;				//HP�o�[�̉摜
	SpriteRender			m_statusBar;			//�X�e�[�^�X�̉摜
	SpriteRender			m_playerFaceFrame;		//�v���C���[�̊�摜�̘g
	SpriteRender			m_playerFaceBack;		//�v���C���[�̊�摜�̔w�i
	SpriteRender            m_SkillRender;          //�X�L���A�C�R��
	SpriteRender            m_UltRender;            //�K�E�A�C�R��
	SpriteRender            m_TimeAndPointRender;   //�������ԂƊl���|�C���g
	
	Vector2				m_HPBerPos = Vector2::Zero;				//HP�o�[�̃|�W�V����
	Vector2				m_HPWindowPos = Vector2::Zero;			//HP�g�̃|�W�V����
	Vector2				m_HPBackPos = Vector2::Zero;			//HP�w�i�̃|�W�V����
	
	
	FontRender m_time_left;

	//�b���v��^�C�}�[
	float SecondsTimer=0.0f;
	//�����v��^�C�}�[
	float MinutesTimer=5.0f;
	float m_timer = 300.0f;
	//int LEVEL;
};

