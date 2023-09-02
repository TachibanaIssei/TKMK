#pragma once
class Fade :public IGameObject
{
public:
	enum EnFadeSprite
	{
		enFadeSprite_BlackSprite,
		enFadeSprite_TipSprite,
	};
public:
	Fade();
	~Fade();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// フェードイン。
	/// </summary>
	void StartFadeIn(const float speed, const EnFadeSprite fadeSprite)
	{
		m_fadeSpeed = speed;
		m_FadeState = FadeIn;
		m_enFadeSprite = fadeSprite;
	}
	/// <summary>
	/// フェードアウト。
	/// </summary>
	void StartFadeOut(const float speed)
	{
		m_fadeSpeed = speed;
		m_FadeState = FadeOut;
	}
	/// <summary>
	/// フェード中？
	/// </summary>
	/// <returns>フェード中ならtrue。</returns>
	const bool IsFade() const
	{
		return m_FadeState != FadeIdle;
	}
	/// <summary>
	/// α値を取得。
	/// </summary>
	/// <returns>α値</returns>
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

	FadeState m_FadeState = FadeIdle;	//フェードステート
	EnFadeSprite m_enFadeSprite = enFadeSprite_BlackSprite;

	SpriteRender BlackSprite;			//黒の画像
	SpriteRender TipSprite;				//Tipの画像
	float m_currentAlpha = 0.0f;		//現在のα値。ここを変更すると画像が透明になる。
	float m_fadeSpeed = 1.0f;			//フェードのスピード
};
