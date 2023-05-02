#pragma once
class Fade:public IGameObject
{
public:
	Fade();
	~Fade();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �t�F�[�h�C���B
	/// </summary>
	void StartFadeIn(float speed)
	{
		m_fadeSpeed = speed;
		m_FadeState = FadeIn;
	}
	/// <summary>
	/// �t�F�[�h�A�E�g�B
	/// </summary>
	void StartFadeOut(float speed)
	{
		m_fadeSpeed = speed;
		m_FadeState = FadeOut;
	}
	/// <summary>
	/// �t�F�[�h�����H
	/// </summary>
	/// <returns>�t�F�[�h���Ȃ�true�B</returns>
	const bool IsFade() const
	{
		return m_FadeState != FadeIdle;
	}
	/// <summary>
	/// ���l���擾�B
	/// </summary>
	/// <returns>���l�B</returns>
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

private:

	enum FadeState
	{
		FadeIn,
		FadeOut,
		FadeIdle,
	};
	FadeState m_FadeState = FadeIdle;

	SpriteRender BlackSprite;
	float m_currentAlpha = 0.0f;		//���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
	float m_fadeSpeed = 1.0f;
};

