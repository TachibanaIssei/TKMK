#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

void nsK2EngineLow::Bloom::Init(RenderTarget& mainRenderTarget)
{
	InitRenderTarget(mainRenderTarget);
	InitLuminanceSprite(mainRenderTarget);
	InitGaussianBlur();
	InitFinalSprite(mainRenderTarget);
}

void nsK2EngineLow::Bloom::InitRenderTarget(RenderTarget& mainRenderTarget)
{
	//�P�x���o�p�̃����_�����O�^�[�Q�b�g
	m_luminnceRenderTarget.Create(
		mainRenderTarget.GetWidth(),				//���C�������_�����O�^�[�Q�b�g�Ɠ���
		mainRenderTarget.GetHeight(),				//���C�������_�����O�^�[�Q�b�g�Ɠ���
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);
}

void nsK2EngineLow::Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
{
	SpriteInitData luminanceSpriteInitData;
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect/postEffect.fx";
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

	//�X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
	luminanceSpriteInitData.m_width = m_luminnceRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = m_luminnceRenderTarget.GetHeight();

	//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w��
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//�X�v���C�g������������
	m_luminanceSprite.Init(luminanceSpriteInitData);
}

void nsK2EngineLow::Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
{
	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();

	//mainRenderTarget�Ƃ��Ȃ����ƍ���
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

	finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//���Z�����ŕ`�悷��̂Ńu�����f�B���O���[�h�����Z�ɂ���
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//�X�v���C�g��������
	m_finalSprite.Init(finalSpriteInitData);
}

void nsK2EngineLow::Bloom::InitGaussianBlur()
{
	m_gaussianBlur.Init(&m_luminnceRenderTarget.GetRenderTargetTexture());
}

void nsK2EngineLow::Bloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
	rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);
	rc.SetRenderTargetAndViewport(m_luminnceRenderTarget);
	rc.ClearRenderTargetView(m_luminnceRenderTarget);

	//�P�x���o���s��
	m_luminanceSprite.Draw(rc);

	//�����_�����O�G���W���ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

	//�K�E�V�A���u���[�����s����
	m_gaussianBlur.ExecuteOnGPU(rc, 20);

	//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);

	//�ŏI����
	m_finalSprite.Draw(rc);

	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}
