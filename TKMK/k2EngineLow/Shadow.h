#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		void Render(RenderContext& rc);

		/// <summary>
		/// シャドウマップテクスチャを取得する
		/// </summary>
		/// <param name="number">取得したいシャドウマップの番号</param>
		Texture& GetShadowMapTexture(int number)
		{
			return m_shadowMap[number].GetRenderTargetTexture();
		}

		/// <summary>
		/// ライトカメラを取得する
		/// </summary>
		Camera& GetLightCamera(int number)
		{
			return m_lightCamera[number];
		}

	private:
		void InitRenderTarget();
		void InitLightCamera();

		void UpdateLightCamera();

	private:
		static const int m_maxViewPort = 4;
		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		RenderTarget m_shadowMap[m_maxViewPort];
		Camera m_lightCamera[m_maxViewPort];

		Vector3 m_lightCameraPosition = Vector3(0.0f,1200.0f,150.0f);
	};
}