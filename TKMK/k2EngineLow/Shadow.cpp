#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	namespace
	{
		const int RENDER_TARGET_WIDTH = 16000;	//�����_�����O�^�[�Q�b�g�̕�
		const int RENDER_TARGET_HEIGHT = 16000;	//�����_�����O�^�[�Q�b�g�̍���
		const float LIGHT_CAMERA_WIDTH = 5000.0f;
		const float LIGHT_CAMERA_HEIGHT = 5000.0f;
		const float LIGHT_CAMERA_ANGLE = 70.0f;	//���C�g�J�����̃A���O��
		const Vector3 LIGHT_CAMERA_UP = Vector3(1.0f, 0.0f, 0.0f);
	}

	void nsK2EngineLow::Shadow::Init()
	{
		InitRenderTarget();
		InitLightCamera();
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
	}

	void nsK2EngineLow::Shadow::InitRenderTarget()
	{
		//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g
		m_shadowMap.Create(
			RENDER_TARGET_WIDTH,
			RENDER_TARGET_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			m_clearColor
		);
	}

	void nsK2EngineLow::Shadow::InitLightCamera()
	{
		m_lightCamera.SetPosition(m_lightCameraPosition);
		m_lightCamera.SetTarget(Vector3::Zero);
		m_lightCamera.SetUp(LIGHT_CAMERA_UP);
		m_lightCamera.SetViewAngle(Math::DegToRad(LIGHT_CAMERA_ANGLE));
		//�e�������Ȃ��悤�ɂ��邽�߂ɃJ�����𕽍s���e�ɂ���
		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(LIGHT_CAMERA_WIDTH);
		m_lightCamera.SetHeight(LIGHT_CAMERA_HEIGHT);
		m_lightCamera.Update();
	}

	void nsK2EngineLow::Shadow::UpdateLightCamera()
	{
		//���C�g�J�����̈ʒu���v�Z����
		Vector3 lightCamPos = g_camera3D->GetTarget();
		lightCamPos.x += m_lightCameraPosition.x;
		lightCamPos.y = m_lightCameraPosition.y;
		lightCamPos.z += m_lightCameraPosition.z;

		m_lightCamera.SetPosition(lightCamPos);
		m_lightCamera.SetTarget(g_camera3D->GetTarget());
		m_lightCamera.Update();
	}
}