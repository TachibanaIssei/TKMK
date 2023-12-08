#pragma once
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow
{
	class IRenderere;
	class ShadowMapRender : public Noncopyable
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="isSoftShadow"></param>
		void Init(const bool isSoftShadow);
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="ligNo">���C�g�ԍ�</param>
		/// <param name="lightDirection">���C�g�̕���</param>
		/// <param name="renderObjects">�`��I�u�W�F�N�g</param>
		void Render(
			RenderContext& rc,
			int ligNo,
			Vector3& lightDirection,
			std::vector<IRenderer*> renderObjects,
			const int cameraNumber
		);

		/// <summary>
			/// �V���h�E�}�b�v���擾
			/// </summary>
			/// <param name="areaNo">�G���A�ԍ�</param>
			/// <returns></returns>
		Texture& GetShadowMap(const int areaNo)
		{
			if (m_isSoftShadow)
			{
				return m_gausBlur[areaNo].GetBokeTexture();
			}
			return m_shadowMaps[areaNo].GetRenderTargetTexture();
		}
		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s����擾
		/// </summary>
		/// <param name="areaNo"></param>
		/// <returns></returns>
		const Matrix& GetLVPMatrix(const int areaNo) const
		{
			return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
		}

	private:
		CascadeShadowMapMatrix		m_cascadeShadowMapMatrix;										//�J�X�P�[�h�V���h�E�}�b�v�̍s��������I�u�W�F�N�g
		RenderTarget				m_shadowMaps[NUM_SHADOW_MAP];									//�����_�[�^�[�Q�b�g
		float						m_cascadeAreaRateArray[NUM_SHADOW_MAP] = { 0.05f, 0.3f,1.0f };	//�J�X�P�[�h�V���h�E�̋߁`�������̔䗦
		GaussianBlur				m_gausBlur[NUM_SHADOW_MAP];										//�V���h�E�}�b�v�Ƀu���[��������B�\�t�g�V���h�E�Ŏg�p
		bool						m_isSoftShadow = false;											//�\�t�g�V���h�E���s���H
	};
}