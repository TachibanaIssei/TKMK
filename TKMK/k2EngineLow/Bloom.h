#pragma once
namespace nsK2EngineLow {
	class Bloom : public Noncopyable
	{
	public:
		void Init(RenderTarget& mainRenderTarget);
		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		/// <summary>
		/// レンダリングターゲットの初期化
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
		RenderTarget				m_luminnceRenderTarget;	//輝度抽出用レンダリングターゲット
		Sprite						m_luminanceSprite;		//輝度抽出画像
		Sprite						m_finalSprite;			//最終画像
		std::array<GaussianBlur,4>	m_gaussianBlur;			//ガウシアンブラー
	};
}