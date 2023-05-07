#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace ShadowConst
{
	const int	RENDER_TARGET_WIDTH		= 10000;		//�����_�����O�^�[�Q�b�g�̕�
	const int	RENDER_TARGET_HEIGHT	= 10000;		//�����_�����O�^�[�Q�b�g�̍���

	const float LIGHT_CAMERA_VIEW_ANGLE = 60.0f;	//���C�g�J�����̃r���[�A���O��
}

void nsK2EngineLow::Shadow::Init()
{
	InitRenderTarget();
	InitLightCamera();
	m_shadowBlur.Init(&m_shadowMap.GetRenderTargetTexture());
}

void nsK2EngineLow::Shadow::Render(RenderContext& rc)
{
	UpdateLightCamera();

	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);
	//�e���f����`��
	g_renderingEngine->ShadowModelRendering(rc, m_lightCamera);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

	m_shadowBlur.ExecuteOnGPU(rc, 5.0f);
}

void nsK2EngineLow::Shadow::InitRenderTarget()
{
	//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g
	m_shadowMap.Create(
		ShadowConst::RENDER_TARGET_WIDTH,
		ShadowConst::RENDER_TARGET_HEIGHT,
		1,
		1,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		m_clearColor
	);
}

void nsK2EngineLow::Shadow::InitLightCamera()
{
	m_lightCamera.SetPosition(m_lightCameraPosition);
	m_lightCamera.SetTarget(0, 0, 0);
	m_lightCamera.SetUp(1, 0, 0);
	m_lightCamera.SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_VIEW_ANGLE));
	m_lightCamera.Update();
}

void nsK2EngineLow::Shadow::UpdateLightCamera()
{
	//���C�g�J�����̈ʒu���v�Z����
	Vector3 lightCamPos = g_camera3D->GetTarget();
	lightCamPos.x += m_lightCameraPosition.x;
	lightCamPos.y += m_lightCameraPosition.y;
	lightCamPos.z += m_lightCameraPosition.z;

	m_lightCamera.SetPosition(lightCamPos);
	m_lightCamera.SetTarget(g_camera3D->GetTarget());
	m_lightCamera.Update();
}
