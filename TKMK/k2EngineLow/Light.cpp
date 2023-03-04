#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::SceneLight::Init()
{
    m_light.directionalLight.color.x = 1.2f;
    m_light.directionalLight.color.y = 1.2f;
    m_light.directionalLight.color.z = 1.2f;

    m_light.directionalLight.direction.x = 1.0f;
    m_light.directionalLight.direction.y = -1.0f;
    m_light.directionalLight.direction.z = -1.0f;
    m_light.directionalLight.direction.Normalize();

    m_light.ambientLight.x = 0.1f;
    m_light.ambientLight.y = 0.1f;
    m_light.ambientLight.z = 0.1f;
    m_light.cameraEyePos = g_camera3D->GetPosition();
}
