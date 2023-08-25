#pragma once
namespace nsK2EngineLow {
	class Bloom : public Noncopyable
	{
	public:
		void Init(RenderTarget& mainRenderTarget);
		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		/// <summary>
		/// �����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �P�x���o�p�̃X�v���C�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitLuminanceSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �{�P�摜�����Z��������X�v���C�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitFinalSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �K�E�V�A���u���[�̏�����
		/// </summary>
		void InitGaussianBlur();

	private:
		RenderTarget				m_luminnceRenderTarget;	//�P�x���o�p�����_�����O�^�[�Q�b�g
		Sprite						m_luminanceSprite;		//�P�x���o�摜
		Sprite						m_finalSprite;			//�ŏI�摜
		std::array<GaussianBlur,4>	m_gaussianBlur;			//�K�E�V�A���u���[
	};
}