#include "k2EngineLowPreCompile.h"
#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow
{
	void CascadeShadowMapMatrix::CalcLightViewProjectionCropMatrix(
		const Vector3 lightDirection, 
		const std::array<float,NUM_SHADOW_MAP> cascadeAreaRateTbl,
		const int cameraNumber
	)
	{
		Camera lightCamera;
		Vector3 lightPos;
		float distLig = g_camera3D[cameraNumber]->GetFar() * 0.1f;
		lightPos = lightDirection * -distLig;
		lightCamera.SetPosition(lightPos);
		lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
		//�������ݒ�
		if (fabsf(lightDirection.y) > 0.9999f)
		{
			//�قڐ^��A�^�������Ă���
			lightCamera.SetUp(g_vec3AxisX);
		}
		else
		{
			lightCamera.SetUp(g_vec3AxisY);
		}
		lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		lightCamera.SetWidth(2500.0f);
		lightCamera.SetHeight(2500.0f);
		lightCamera.SetNear(1.0f);
		lightCamera.SetFar(g_camera3D[cameraNumber]->GetFar()*0.65f);
		lightCamera.Update();

		const auto& lvpMatrix = lightCamera.GetViewProjectionMatrix();

		float cascadeAreaTbl[NUM_SHADOW_MAP] = {
			g_camera3D[cameraNumber]->GetFar() * cascadeAreaRateTbl[SHADOW_MAP_AREA_NEAR],		//�߉e���f���ő�[�x�l
			g_camera3D[cameraNumber]->GetFar() * cascadeAreaRateTbl[SHADOW_MAP_AREA_MIDDLE],	//���e���f���ő�[�x�l
			g_camera3D[cameraNumber]->GetFar()													//���e���f���ő�[�x�l
		};

		//�J�����̑O�����ƉE�����A����������߂�
		const auto& cameraForward = g_camera3D[cameraNumber]->GetForward();
		const auto& cameraRight = g_camera3D[cameraNumber]->GetRight();
		//�O�ςŏ���������߂�
		Vector3 cameraUp;
		cameraUp.Cross(cameraForward, cameraRight);

		//��ԋ߂��G���A�̍ŏ��[�x�l�̓J�����̃j�A�N���b�v
		float nearDepth = g_camera3D[cameraNumber]->GetNear();
		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
		{
			float nearY = tanf(g_camera3D[cameraNumber]->GetViewAngle() * 0.5f) * nearDepth;
			float nearX = nearY * g_camera3D[cameraNumber]->GetAspect();
			float farY = tanf(g_camera3D[cameraNumber]->GetViewAngle() * 0.5f) * cascadeAreaTbl[areaNo];
			float farX = farY * g_camera3D[cameraNumber]->GetAspect();
			Vector3 nearPos = g_camera3D[cameraNumber]->GetPosition() + cameraForward * nearDepth;
			Vector3 farPos = g_camera3D[cameraNumber]->GetPosition() + cameraForward * cascadeAreaTbl[areaNo];
			//8���_�����߂�
			Vector3 vertex[8];
			//�ߕ��ʂ̒��_
			vertex[0] += nearPos + cameraUp * nearY + cameraRight * nearX;
			vertex[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;
			vertex[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;
			vertex[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;
			//�����ʂ̒��_
			vertex[4] += farPos + cameraUp * farY + cameraRight * farX;
			vertex[5] += farPos + cameraUp * farY + cameraRight * -farX;
			vertex[6] += farPos + cameraUp * -farY + cameraRight * farX;
			vertex[7] += farPos + cameraUp * -farY + cameraRight * -farX;

			//8���_�����C�g�r���[�v���W�F�N�V������Ԃɕϊ����āA
			//8���_�̍ő�l�ƍŏ��l�����߂�
			Vector3 vMax, vMin;
			vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			vMin = { FLT_MAX ,FLT_MAX ,FLT_MAX };
			for (auto& v : vertex)
			{
				lvpMatrix.Apply(v);
				vMax.Max(v);
				vMin.Min(v);
			}

			//�N���b�v�s������߂�
			float xScale = 2.0f / (vMax.x - vMin.x);
			float yScale = 2.0f / (vMax.y - vMin.y);
			float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
			float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;
			Matrix clopMatrix;
			clopMatrix.m[0][0] = xScale;
			clopMatrix.m[1][1] = yScale;
			clopMatrix.m[3][0] = xOffset;
			clopMatrix.m[3][1] = yOffset;

			m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;

			//���̃G���A�̋ߕ��ʂ܂ł̋�����������
			nearDepth = cascadeAreaTbl[areaNo];
		}
	}
}