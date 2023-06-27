#pragma once
class Fade:public IGameObject
{
public:
	/// <summary>
	/// �t�F�[�h�̏��
	/// </summary>
	enum EnFadeState
	{
		enFadeState_fadeIn,		//�t�F�[�h�C��
		enFadeState_fadeOut,	//�t�F�[�h�A�E�g
		enFadeState_fadeIdle,	//�������Ȃ�(�ʏ펞)
	};

	/// <summary>
	/// �t�F�[�h�p�摜�̕���
	/// </summary>
	enum EnFadeSpriteType
	{
		enFadeSpriteType_Full = 0,	//�S���
		enFadeSpriteType_Left = 1,	//��
		enFadeSpriteType_Right = 2,	//�E
		enFadeSpriteType_Num
	};

	Fade();
	~Fade();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �t�F�[�h�C���B
	/// </summary>
	void StartFadeIn(float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full)
	{
		m_fadeSpeed[spriteType] = speed;
		m_fadeState[spriteType] = enFadeState_fadeIn;
	}
	/// <summary>
	/// �t�F�[�h�A�E�g�B
	/// </summary>
	void StartFadeOut(float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full)
	{
		m_fadeSpeed[spriteType] = speed;
		m_fadeState[spriteType] = enFadeState_fadeOut;
	}

	/// <summary>
	/// �t�F�[�h���H
	/// </summary>
	/// <returns>�t�F�[�h���Ȃ�true�B</returns>
	const bool IsFade(const EnFadeSpriteType spriteType = enFadeSpriteType_Full) const
	{
		return m_fadeState[spriteType] != enFadeState_fadeIdle;
	}
	/// <summary>
	/// ���l���擾�B
	/// </summary>
	/// <returns>���l�B</returns>
	const float GetCurrentAlpha(const EnFadeSpriteType spriteType = enFadeSpriteType_Full) const
	{
		return m_currentAlpha[spriteType];
	}

private:
	/// <summary>
	/// ��ʑS�̃t�F�[�h�̍X�V����
	/// </summary>
	void FadeUpdate();

private:
	static const int m_fadeSpriteCount = enFadeSpriteType_Num;
	std::array<EnFadeState, m_fadeSpriteCount> m_fadeState = { enFadeState_fadeIdle, enFadeState_fadeIdle, enFadeState_fadeIdle };
	std::array<SpriteRender, m_fadeSpriteCount> m_blackSprite;					//���E�p�̃X�v���C�g
	std::array<float, m_fadeSpriteCount> m_currentAlpha = { 0.0f,0.0f,0.0f };	//���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
	std::array<float, m_fadeSpriteCount> m_fadeSpeed = { 1.0f,1.0f,1.0f };		//�t�F�[�h�̑��x
};