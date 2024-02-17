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

namespace nsK2EngineLow
{
	void Shadow::Init()
	{
		InitRenderTarget();
		InitLightCamera();
	}

	void Shadow::Render(RenderContext& rc)
	{
		UpdateLightCamera();

		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap[i]);
			rc.SetRenderTargetAndViewport(m_shadowMap[i]);
			rc.ClearRenderTargetView(m_shadowMap[i]);
			//影モデルを描画
			g_renderingEngine->ShadowModelRendering(rc, m_lightCamera[i], i);
			rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap[i]);
		}
	}

	void Shadow::InitRenderTarget()
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

	void Shadow::InitLightCamera()
	{
		for (int i = 0; i < MAX_VIEWPORT; i++) {
			m_lightCamera[i].SetPosition(m_lightCameraPosition);
			m_lightCamera[i].SetTarget(Vector3::Zero);
			m_lightCamera[i].SetUp(Vector3::Right);
			m_lightCamera[i].SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_ANGLE));

			//影が動かないようにするためにカメラを平行投影にする
			m_lightCamera[i].SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
			m_lightCamera[i].SetWidth(ShadowConst::LIGHT_CAMERA_WIDTH);
			m_lightCamera[i].SetHeight(ShadowConst::LIGHT_CAMERA_HEIGHT);
			m_lightCamera[i].Update();
		}
	}

	void Shadow::UpdateLightCamera()
	{
		for (int i = 0; i < g_renderingEngine->GetGameMode(); i++)
		{
			//ライトカメラの位置を計算する
			Vector3 lightCamPos = g_camera3D[i]->GetTarget();
			lightCamPos.x += m_lightCameraPosition.x;
			lightCamPos.y = m_lightCameraPosition.y;
			lightCamPos.z += m_lightCameraPosition.z;

			m_lightCamera[i].SetPosition(lightCamPos);
			m_lightCamera[i].SetTarget(g_camera3D[i]->GetTarget());
		}
	}
}