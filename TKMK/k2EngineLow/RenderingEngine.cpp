#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {
	namespace
	{
		const int MAX_MODEL = 60;
		const int MAX_SPRITE = 100;
		const int MAX_FONT = 30;
	}

	RenderingEngine::RenderingEngine()
	{
		m_modelList.reserve(MAX_MODEL);
		m_spriteList.reserve(MAX_SPRITE);
		m_laterSpriteList.reserve(MAX_SPRITE);
		m_fontList.reserve(MAX_FONT);
	}

	RenderingEngine::~RenderingEngine()
	{
	}

	void RenderingEngine::Init()
	{
		InitRenderTargets();
		m_shadow.Init();
		m_postEffect.Init(m_mainRenderTarget);
		InitCopyToFrameBufferSprite();

		m_sceneLight.Init();
	}

	void RenderingEngine::InitRenderTargets()
	{
		//���C�������_�����O�^�[�Q�b�g
		m_mainRenderTarget.Create(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitCopyToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();

		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		//�X�v���C�g��������
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::ModelRendering(RenderContext& rc)
	{
		for (auto& modelObj : m_modelList) {
			modelObj->OnRenderModel(rc);
		}
	}

	void RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera)
	{
		for (auto& modelObj : m_modelList) {
			modelObj->OnRenderShadowModel(rc, camera);
		}
	}

	void RenderingEngine::SpriteRendering(RenderContext& rc, const bool drawTiming = false)
	{
		if (drawTiming) {
			for (auto& spriteObj : m_laterSpriteList)
			{
				spriteObj->OnRenderSprite(rc);
			}
		}
		else {
			for (auto& spriteObj : m_spriteList)
			{
				spriteObj->OnRenderSprite(rc);
			}
		}
	}

	void RenderingEngine::FontRendering(RenderContext& rc)
	{
		for (auto& fontObj : m_fontList)
		{
			fontObj->OnRenderFont(rc);
		}
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		SetEyePos(g_camera3D->GetPosition());

		m_shadow.Render(rc);

		//���C�������_�����O�^�[�Q�b�g�ɕύX
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);

		//���f����`��
		ModelRendering(rc);

		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		m_postEffect.Render(rc, m_mainRenderTarget);

		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyToFrameBufferSprite.Draw(rc);

		//�X�v���C�g��`��
		SpriteRendering(rc);
		//������`��
		FontRendering(rc);
		//�X�v���C�g�𕶎��̏�ɕ`�悷��
		SpriteRendering(rc, true);

		m_modelList.clear();
		m_spriteList.clear();
		m_laterSpriteList.clear();
		m_fontList.clear();
	}
}