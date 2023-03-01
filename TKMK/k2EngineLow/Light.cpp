#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::Light::Init()
{
	//ライトの方向
	m_directionLight.lightDirection = Vector3(1.0f,-1.0f,-1.0f);
	//ライトの色
	m_directionLight.ligColor = Vector3(0.5f,0.5f,0.5f);
	//視点の位置を設定
	m_directionLight.CameraEyePos = g_camera3D->GetPosition();
}
