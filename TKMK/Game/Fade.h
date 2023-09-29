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

	/// <summary>
	/// �t�F�[�h�摜������Tip�摜��
	/// </summary>
	enum EnFadeSpriteCategory
	{
		enFadeSpriteCategory_Black,
		enFadeSpriteCategory_Tip
	};

	Fade();
	~Fade();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �t�F�[�h�C���B
	/// </summary>
	void StartFadeIn(const float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full,
		const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black);

	/// <summary>
	/// �t�F�[�h�A�E�g�B
	/// </summary>
	void StartFadeOut(const float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full,
		const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black);

	/// <summary>
	/// �t�F�[�h���H
	/// </summary>
	/// <returns>�t�F�[�h���Ȃ�true�B</returns>
	const bool IsFade(const EnFadeSpriteType spriteType = enFadeSpriteType_Full, const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black) const
	{
		if (spriteCategory == enFadeSpriteCategory_Black) return m_blackFadeState[spriteType] != enFadeState_fadeIdle;
		else if (spriteCategory == enFadeSpriteCategory_Tip) return m_tipFadeState != enFadeState_fadeIdle;
	}
	/// <summary>
	/// ���l���擾�B
	/// </summary>
	/// <returns>���l�B</returns>
	const float GetCurrentAlpha(const EnFadeSpriteType spriteType = enFadeSpriteType_Full,const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black) const
	{
		if (spriteCategory == enFadeSpriteCategory_Black) return m_blackSpriteCurrentAlpha[spriteType];
		else if (spriteCategory == enFadeSpriteCategory_Tip) return m_tipSpriteCurrentAlpha;
	}

private:
	/// <summary>
	/// ��ʑS�̃t�F�[�h�̍X�V����
	/// </summary>
	void FadeUpdate();

private:
	static const int m_fadeSpriteCount = enFadeSpriteType_Num;
	SpriteRender m_tipSprite;
	EnFadeSpriteCategory m_fadeSpriteCategory = enFadeSpriteCategory_Black;
	EnFadeState m_tipFadeState = enFadeState_fadeIdle;
	float m_tipSpriteCurrentAlpha = 0.0f;
	float m_tipFadeSpeed = 1.0f;
	std::array<EnFadeState, m_fadeSpriteCount> m_blackFadeState = { enFadeState_fadeIdle, enFadeState_fadeIdle, enFadeState_fadeIdle };
	std::array<SpriteRender, m_fadeSpriteCount> m_blackSprite;					//�X�v���C�g
	std::array<float, m_fadeSpriteCount> m_blackSpriteCurrentAlpha = {};		//���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
	std::array<float, m_fadeSpriteCount> m_blackFadeSpeed = { 1.0f,1.0f,1.0f };	//�t�F�[�h�̑��x
};