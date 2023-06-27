#pragma once
class Fade:public IGameObject
{
public:
	/// <summary>
	/// フェードの状態
	/// </summary>
	enum EnFadeState
	{
		enFadeState_fadeIn,		//フェードイン
		enFadeState_fadeOut,	//フェードアウト
		enFadeState_fadeIdle,	//何もしない(通常時)
	};

	/// <summary>
	/// フェード用画像の分類
	/// </summary>
	enum EnFadeSpriteType
	{
		enFadeSpriteType_Full = 0,	//全画面
		enFadeSpriteType_Left = 1,	//左
		enFadeSpriteType_Right = 2,	//右
		enFadeSpriteType_Num
	};

	Fade();
	~Fade();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// フェードイン。
	/// </summary>
	void StartFadeIn(float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full)
	{
		m_fadeSpeed[spriteType] = speed;
		m_fadeState[spriteType] = enFadeState_fadeIn;
	}
	/// <summary>
	/// フェードアウト。
	/// </summary>
	void StartFadeOut(float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full)
	{
		m_fadeSpeed[spriteType] = speed;
		m_fadeState[spriteType] = enFadeState_fadeOut;
	}

	/// <summary>
	/// フェード中？
	/// </summary>
	/// <returns>フェード中ならtrue。</returns>
	const bool IsFade(const EnFadeSpriteType spriteType = enFadeSpriteType_Full) const
	{
		return m_fadeState[spriteType] != enFadeState_fadeIdle;
	}
	/// <summary>
	/// α値を取得。
	/// </summary>
	/// <returns>α値。</returns>
	const float GetCurrentAlpha(const EnFadeSpriteType spriteType = enFadeSpriteType_Full) const
	{
		return m_currentAlpha[spriteType];
	}

private:
	/// <summary>
	/// 画面全体フェードの更新処理
	/// </summary>
	void FadeUpdate();

private:
	static const int m_fadeSpriteCount = enFadeSpriteType_Num;
	std::array<EnFadeState, m_fadeSpriteCount> m_fadeState = { enFadeState_fadeIdle, enFadeState_fadeIdle, enFadeState_fadeIdle };
	std::array<SpriteRender, m_fadeSpriteCount> m_blackSprite;					//左右用のスプライト
	std::array<float, m_fadeSpriteCount> m_currentAlpha = { 0.0f,0.0f,0.0f };	//現在のα値。ここを変更すると画像が透明になる。
	std::array<float, m_fadeSpriteCount> m_fadeSpeed = { 1.0f,1.0f,1.0f };		//フェードの速度
};