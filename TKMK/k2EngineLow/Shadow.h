#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		void Render(RenderContext& rc);

		/// <summary>
		/// �V���h�E�}�b�v�e�N�X�`�����擾����
		/// </summary>
		/// <param name="number">�擾�������V���h�E�}�b�v�̔ԍ�</param>
		Texture& GetShadowMapTexture(int number)
		{
			return m_shadowMap[number].GetRenderTargetTexture();
		}

		/// <summary>
		/// ���C�g�J�������擾����
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