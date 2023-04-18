#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

void nsK2EngineLow::RenderingEngine::Init()
{
	InitRenderTargets();
	m_shadow.Init();
	m_sceneLight.Init();
	//m_postEffect.Init(m_mainRenderTarget);
	InitSprits();

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

	//�A���x�h�J���[�������ݗp
	m_albedRenderTarget.Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT
	);

	//�@���������ݗp
	m_normalRenderTarget.Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN
	);

	//�[�x�������ݗp
	m_depthRenderTarget.Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);
}

void nsK2EngineLow::RenderingEngine::InitSprits()
{
	///////////////////////////////////////////////////
	///�t���[���o�b�t�@�ɃR�s�[����悤�̃X�v���C�g��������
	///////////////////////////////////////////////////
	SpriteInitData spriteInitData;

	//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = m_mainRenderTarget.GetWidth();
	spriteInitData.m_height = m_mainRenderTarget.GetHeight();

	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//�X�v���C�g��������
	m_copyToFrameBufferSprite.Init(spriteInitData);

	///////////////////////////////////////////////////
	///�f�B�t�@�[�h���C�e�B���O�p�̃X�v���C�g��������
	///////////////////////////////////////////////////
	SpriteInitData defferdLightingSprInitData;
	//��ʑS�̂Ƀ����_�����O����̂ŕ��ƍ����̓t���[���o�b�t�@�Ɠ���
	defferdLightingSprInitData.m_width = FRAME_BUFFER_W;
	defferdLightingSprInitData.m_height = FRAME_BUFFER_H;

	//�g�p����e�N�X�`��
	defferdLightingSprInitData.m_textures[0] = &m_albedRenderTarget.GetRenderTargetTexture();
	defferdLightingSprInitData.m_textures[1] = &m_normalRenderTarget.GetRenderTargetTexture();
	defferdLightingSprInitData.m_textures[2] = &m_depthRenderTarget.GetRenderTargetTexture();

	defferdLightingSprInitData.m_fxFilePath = "Assets/shader/defferdLighting.fx";
	defferdLightingSprInitData.m_expandConstantBuffer = (void*)&GetSceneLight();
	defferdLightingSprInitData.m_expandConstantBufferSize = sizeof(GetSceneLight());

	m_defferdLightingSpr.Init(defferdLightingSprInitData);
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

void nsK2EngineLow::RenderingEngine::SpriteRendering(RenderContext& rc)
{
	for (auto& spriteObj : m_spriteList)
	{
		spriteObj->OnRenderSprite(rc);
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
	m_sceneLight.SetViewProjInvMatrix(g_camera3D->GetViewProjectionMatrixInv());

	//m_shadow.Render(rc);

	//�����_�[�^�[�Q�b�g��G-Buffer�ɕύX���ď�������
	RenderTarget* rts[] = {
		&m_albedRenderTarget,
		&m_normalRenderTarget,
		&m_depthRenderTarget
	};

	rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
	rc.SetRenderTargets(ARRAYSIZE(rts), rts);
	rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);
	
	ModelRendering(rc);

	rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

	//�����_�����O����t���[���o�b�t�@�ɖ߂�
	g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
	m_defferdLightingSpr.Draw(rc);

	////���C�������_�����O�^�[�Q�b�g�ɕύX
	//rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//rc.ClearRenderTargetView(m_mainRenderTarget);

	////���f����`��
	//ModelRendering(rc);

	////�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	//rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	//m_postEffect.Render(rc, m_mainRenderTarget);

	/*rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);*/
	//m_copyToFrameBufferSprite.Draw(rc);

	//�X�v���C�g��`��
	SpriteRendering(rc);
	//������`��
	FontRendering(rc);

	m_modelList.clear();
	m_spriteList.clear();
	m_fontList.clear();
}