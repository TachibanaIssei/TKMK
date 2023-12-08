#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"

namespace nsK2EngineLow
{
    void SceneLight::Init()
    {
        // ‘¾—zŒõ
        m_light.directionalLight[0].color.x = 1.2f;
        m_light.directionalLight[0].color.y = 1.2f;
        m_light.directionalLight[0].color.z = 1.2f;

        m_light.directionalLight[0].direction.x = 1.0f;
        m_light.directionalLight[0].direction.y = -1.0f;
        m_light.directionalLight[0].direction.z = -1.0f;
        m_light.directionalLight[0].direction.Normalize();
        m_light.directionalLight[0].castShadow = true;

        // ŠÂ‹«Œõ
        m_light.ambientLight.x = 0.1f;
        m_light.ambientLight.y = 0.1f;
        m_light.ambientLight.z = 0.1f;

        // ƒJƒƒ‰ˆÊ’u
        m_light.eyePos = g_camera3D[0]->GetPosition();
    }
}