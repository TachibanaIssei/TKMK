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
	/// フェードイン。
	/// </summary>
	void StartFadeIn(float speed)
	{
		m_fadeSpeed = speed;
		m_FadeState = FadeIn;
	}
	/// <summary>
	/// フェードアウト。
	/// </summary>
	void StartFadeOut(float speed)
	{
		m_fadeSpeed = speed;
		m_FadeState = FadeOut;
	}
	/// <summary>
	/// フェード中＞？
	/// </summary>
	/// <returns>フェード中ならtrue。</returns>
	const bool IsFade() const
	{
		return m_FadeState != FadeIdle;
	}
	/// <summary>
	/// α値を取得。
	/// </summary>
	/// <returns>α値。</returns>
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
	float m_currentAlpha = 0.0f;		//現在のα値。ここを変更すると画像が透明になる。
	float m_fadeSpeed = 1.0f;
};

