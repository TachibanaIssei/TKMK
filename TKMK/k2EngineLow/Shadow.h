#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		void Render(RenderContext& rc);

		Texture& GetShadowMapTexture(int number = 0)
		{
			return m_shadowMap[number].GetRenderTargetTexture();
		}

		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

	private:
		void InitRenderTarget();
		void InitLightCamera();

		void UpdateLightCamera();

	private:
		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		RenderTarget m_shadowMap[2];
		Camera m_lightCamera;

		Vector3 m_lightCameraPosition = Vector3(0.0f,1200.0f,150.0f);
	};
}