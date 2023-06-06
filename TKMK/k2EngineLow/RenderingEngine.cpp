#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

void nsK2EngineLow::RenderingEngine::Init()
{
	InitViewPorts();
	InitRenderTargets();
	m_shadow.Init();
	m_postEffect.Init(m_mainRenderTarget);
	InitCopyToFrameBufferSprite();

	m_sceneLight.Init();
}

void nsK2EngineLow::RenderingEngine::InitRenderTargets()
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

void nsK2EngineLow::RenderingEngine::InitCopyToFrameBufferSprite()
{
	SpriteInitData spriteInitData;

	//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = m_mainRenderTarget.GetWidth();
	spriteInitData.m_height = m_mainRenderTarget.GetHeight();

	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//�X�v���C�g�������
	m_copyToFrameBufferSprite.Init(spriteInitData);
}

void nsK2EngineLow::RenderingEngine::InitViewPorts()
{
	//���ꂪ������
	m_viewPorts[0].Width = FRAME_BUFFER_W / 2;   //��ʂ̉��T�C�Y
	m_viewPorts[0].Height = FRAME_BUFFER_H / 2;   //��ʂ̏c�T�C�Y
	m_viewPorts[0].TopLeftX = 0;   //��ʍ����x���W
	m_viewPorts[0].TopLeftY = 0;   //��ʍ����y���W
	m_viewPorts[0].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
	m_viewPorts[0].MaxDepth = 1.0f;   //�[�x�l�̍ő�l

	//���ꂪ�������
	m_viewPorts[1].Width = FRAME_BUFFER_W / 2;   //��ʂ̉��T�C�Y
	m_viewPorts[1].Height = FRAME_BUFFER_H / 2;   //��ʂ̏c�T�C�Y
	m_viewPorts[1].TopLeftX = 0;   //��ʍ����x���W
	m_viewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;   //��ʍ����y���W
	m_viewPorts[1].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
	m_viewPorts[1].MaxDepth = 1.0f;   //�[�x�l�̍ő�l

	//���ꂪ�E����
	m_viewPorts[2].Width = FRAME_BUFFER_W / 2;   //��ʂ̉��T�C�Y
	m_viewPorts[2].Height = FRAME_BUFFER_H / 2;   //��ʂ̏c�T�C�Y
	m_viewPorts[2].TopLeftX = FRAME_BUFFER_W / 2;   //��ʍ����x���W
	m_viewPorts[2].TopLeftY = 0;   //��ʍ����y���W
	m_viewPorts[2].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
	m_viewPorts[2].MaxDepth = 1.0f;   //�[�x�l�̍ő�l

	//���ꂪ�E�����
	m_viewPorts[3].Width = FRAME_BUFFER_W / 2;   //��ʂ̉��T�C�Y
	m_viewPorts[3].Height = FRAME_BUFFER_H / 2;   //��ʂ̏c�T�C�Y
	m_viewPorts[3].TopLeftX = FRAME_BUFFER_W / 2;   //��ʍ����x���W
	m_viewPorts[3].TopLeftY = FRAME_BUFFER_H / 2;   //��ʍ����y���W
	m_viewPorts[3].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
	m_viewPorts[3].MaxDepth = 1.0f;   //�[�x�l�̍ő�l
}

void nsK2EngineLow::RenderingEngine::ModelRendering(RenderContext& rc)
{
	for (auto& modelObj : m_modelList) {
		modelObj->OnRenderModel(rc);
	}
}

void nsK2EngineLow::RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera)
{
	for (auto& modelObj : m_modelList) {
		modelObj->OnRenderShadowModel(rc,camera);
	}
}

void nsK2EngineLow::RenderingEngine::SpriteRendering(RenderContext& rc, bool drawTiming = false)
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

void nsK2EngineLow::RenderingEngine::FontRendering(RenderContext& rc)
{
	for (auto& fontObj : m_fontList)
	{
		fontObj->OnRenderFont(rc);
	}
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	SetEyePos(g_camera3D->GetPosition());

	m_shadow.Render(rc);

	//���C�������_�����O�^�[�Q�b�g�ɕύX
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	rc.ClearRenderTargetView(m_mainRenderTarget);

	for (int i = 0; i < m_viewPortCount; i++)
	{
		rc.SetViewport(m_viewPorts[i]);
		//���f����`��
		ModelRendering(rc);
	}

		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		m_postEffect.Render(rc, m_mainRenderTarget);
	}
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