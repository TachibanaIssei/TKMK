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

	/// <summary>
	/// フェード画像が黒かTip画像か
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
	/// フェードイン。
	/// </summary>
	void StartFadeIn(const float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full,
		const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black);

	/// <summary>
	/// フェードアウト。
	/// </summary>
	void StartFadeOut(const float speed, const EnFadeSpriteType spriteType = enFadeSpriteType_Full,
		const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black);

	/// <summary>
	/// フェード中？
	/// </summary>
	/// <returns>フェード中ならtrue。</returns>
	const bool IsFade(const EnFadeSpriteType spriteType = enFadeSpriteType_Full, const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black) const
	{
		if (spriteCategory == enFadeSpriteCategory_Black) return m_blackFadeState[spriteType] != enFadeState_fadeIdle;
		else if (spriteCategory == enFadeSpriteCategory_Tip) return m_tipFadeState != enFadeState_fadeIdle;
	}
	/// <summary>
	/// α値を取得。
	/// </summary>
	/// <returns>α値。</returns>
	const float GetCurrentAlpha(const EnFadeSpriteType spriteType = enFadeSpriteType_Full,const EnFadeSpriteCategory spriteCategory = enFadeSpriteCategory_Black) const
	{
		if (spriteCategory == enFadeSpriteCategory_Black) return m_blackSpriteCurrentAlpha[spriteType];
		else if (spriteCategory == enFadeSpriteCategory_Tip) return m_tipSpriteCurrentAlpha;
	}

private:
	/// <summary>
	/// 画面全体フェードの更新処理
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
	std::array<SpriteRender, m_fadeSpriteCount> m_blackSprite;					//スプライト
	std::array<float, m_fadeSpriteCount> m_blackSpriteCurrentAlpha = {};		//現在のα値。ここを変更すると画像が透明になる。
	std::array<float, m_fadeSpriteCount> m_blackFadeSpeed = { 1.0f,1.0f,1.0f };	//フェードの速度
};