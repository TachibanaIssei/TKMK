#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		void Render(RenderContext& rc);

		const Texture& GetShadowMapTexture()const
		{
			return m_shadowMap.GetRenderTargetTexture();
		}

		const Camera& GetLightCamera()const
		{
			return m_lightCamera;
		}

	private:
		void InitRenderTarget();
		void InitLightCamera();

		void UpdateLightCamera();

	private:
		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		RenderTarget m_shadowMap;
		Camera m_lightCamera;

		Vector3 m_lightCameraPosition = Vector3(0.0f,1200.0f,150.0f);
	};
}