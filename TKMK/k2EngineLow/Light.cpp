#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::Light::Init()
{
	//���C�g�̕���
	m_directionLight.lightDirection = Vector3(1.0f,-1.0f,-1.0f);
	//���C�g�̐F
	m_directionLight.ligColor = Vector3(0.5f,0.5f,0.5f);
	//���_�̈ʒu��ݒ�
	m_directionLight.CameraEyePos = g_camera3D->GetPosition();
}
