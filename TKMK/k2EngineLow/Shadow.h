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
		Texture& GetShadowMapTexture(int number = 0)
		{
			return m_shadowMap[number].GetRenderTargetTexture();
		}

		/// <summary>
		/// ライトカメラを取得する
		/// </summary>
		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

	private:
		void InitRenderTarget();
		void InitLightCamera();

		void UpdateLightCamera();

	private:
		static const int m_maxViewPort = 2;
		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		RenderTarget m_shadowMap[m_maxViewPort];
		Camera m_lightCamera;

		Vector3 m_lightCameraPosition = Vector3(0.0f,1200.0f,150.0f);
	};
}