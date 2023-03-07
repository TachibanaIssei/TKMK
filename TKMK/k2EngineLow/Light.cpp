#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::SceneLight::Init()
{
    m_light.directionalLight.color.x = 0.5f;
    m_light.directionalLight.color.y = 0.5f;
    m_light.directionalLight.color.z = 0.5f;

    m_light.directionalLight.direction.x = 1.0f;
    m_light.directionalLight.direction.y = -1.0f;
    m_light.directionalLight.direction.z = -1.0f;
    m_light.directionalLight.direction.Normalize();

    m_light.ambientLight.x = 0.3f;
    m_light.ambientLight.y = 0.3f;
    m_light.ambientLight.z = 0.3f;

    m_light.hemisphereLight.groundColor = Vector3(0.17f, 0.15f, 0.1255f);
    m_light.hemisphereLight.skyColor = Vector3(0.0116f, 0.0766f, 0.0766f);
    m_light.hemisphereLight.groundNormal = Vector3(0.0f, 1.0f, 0.0f);

    m_light.cameraEyePos = g_camera3D->GetPosition();
}
