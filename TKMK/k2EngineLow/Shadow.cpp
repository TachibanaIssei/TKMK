#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace ShadowConst
{
	const int RENDER_TARGET_WIDTH	= 10000;	//レンダリングターゲットの幅
	const int RENDER_TARGET_HEIGHT	= 10000;	//レンダリングターゲットの高さ

	const float LIGHT_CAMERA_WIDTH = 2500.0f;
	const float LIGHT_CAMERA_HEIGHT = 2500.0f;

	const float LIGHT_CAMERA_ANGLE	= 20.0f;	//ライトカメラのアングル
}

void nsK2EngineLow::Shadow::Init()
{
	InitRenderTarget();
	InitLightCamera();
}

void nsK2EngineLow::Shadow::Render(RenderContext& rc)
{
	UpdateLightCamera();

	for (int i = 0; i < MAX_VIEWPORT; i++)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap[i]);
		rc.SetRenderTargetAndViewport(m_shadowMap[i]);
		rc.ClearRenderTargetView(m_shadowMap[i]);
		//影モデルを描画
		g_renderingEngine->ShadowModelRendering(rc, m_lightCamera, i);
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap[i]);
	}
}

void nsK2EngineLow::Shadow::InitRenderTarget()
{
	//シャドウマップ描画用のレンダリングターゲット
	for (int i = 0; i < m_maxViewPort; i++)
	{
		m_shadowMap[i].Create(
			ShadowConst::RENDER_TARGET_WIDTH,
			ShadowConst::RENDER_TARGET_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			m_clearColor
		);
	}
}

void nsK2EngineLow::Shadow::InitLightCamera()
{
	m_lightCamera.SetPosition(m_lightCameraPosition);
	m_lightCamera.SetTarget(Vector3::Zero);
	m_lightCamera.SetUp(Vector3::Right);
	m_lightCamera.SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_ANGLE));
	
	//影が動かないようにするためにカメラを平行投影にする
	m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	m_lightCamera.SetWidth(ShadowConst::LIGHT_CAMERA_WIDTH);
	m_lightCamera.SetHeight(ShadowConst::LIGHT_CAMERA_HEIGHT);
	m_lightCamera.Update();
}

void nsK2EngineLow::Shadow::UpdateLightCamera()
{
	//ライトカメラの位置を計算する
	Vector3 lightCamPos = g_camera3D[RenderingEngine::enCameraDrawing_Left]->GetTarget();
	lightCamPos.x += m_lightCameraPosition.x;
	lightCamPos.y = m_lightCameraPosition.y;
	lightCamPos.z += m_lightCameraPosition.z;

	m_lightCamera.SetPosition(lightCamPos);
	m_lightCamera.SetTarget(g_camera3D[0]->GetTarget());
	m_lightCamera.Update();
}
