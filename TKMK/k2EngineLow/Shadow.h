#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		void Render(RenderContext& rc);

		Texture& GetShadowMapTexture()
		{
			return m_shadowMap.GetRenderTargetTexture();
		}

		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

		/// <summary>
		/// シャドウマップにブラーをかけたテクスチャを取得
		/// </summary>
		/// <returns></returns>
		Texture& GetShadowBlurBokeTexture()
		{
			return m_shadowBlur.GetBokeTexture();
		}

	private:
		void InitRenderTarget();
		void InitLightCamera();

		void UpdateLightCamera();

	private:
		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		RenderTarget m_shadowMap;
		Camera m_lightCamera;
		GaussianBlur m_shadowBlur;

		Vector3 m_lightCameraPosition = Vector3(0.0f,1000.0f,0.0f);
	};
}