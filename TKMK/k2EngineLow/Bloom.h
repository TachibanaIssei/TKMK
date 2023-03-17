#pragma once
namespace nsK2EngineLow {
	class Bloom
	{
	public:
		void Init(RenderTarget& mainRenderTarget);

		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		/// <summary>
		/// レンダーターゲットの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 輝度抽出用のスプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitLuminanceSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// ボケ画像を加算合成するスプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitFinalSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// ガウシアンブラーの初期化
		/// </summary>
		void InitGaussianBlur();

	private:
		RenderTarget				m_luminnceRenderTarget;

		Sprite						m_luminanceSprite;
		Sprite						m_finalSprite;

		GaussianBlur				m_gaussianBlur[4];
	};
}