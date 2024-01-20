#pragma once
namespace nsK2EngineLow
{
	class CascadeShadowMapMatrix : public Noncopyable
	{
	public:
		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����N���b�v�s����v�Z����
		/// </summary>
		/// <param name="lightDirection">���C�g�̕���</param>
		void CalcLightViewProjectionCropMatrix(
			const Vector3 lightDirection,
			const std::array<float, NUM_SHADOW_MAP> cascadeAreaRateTbl,
			const int cameraNumber
		);
		/// <summary>
		/// �v�Z���ꂽ���C�g�r���[�v���W�F�N�V�����N���b�v�s����擾
		/// </summary>
		/// <param name="shadowMapNo">�V���h�E�}�b�v�̔ԍ�</param>
		/// <param name="cameraNumber">�`�撆�̃J�����̔ԍ�</param>
		/// <returns></returns>
		const Matrix& GetLightViewProjectionCropMatrix(const int shadowMapNo) const
		{
			return m_lvpcMatrix[shadowMapNo];
		}
	private:
		Matrix m_lvpcMatrix[NUM_SHADOW_MAP];	//���C�g�r���[�N���b�v�s��
	};
}