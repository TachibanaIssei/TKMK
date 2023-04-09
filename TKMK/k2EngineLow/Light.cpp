#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::SceneLight::Init()
{
    //�f�B���N�V�������C�g�̏�����
    InitDirectionLight();
    InitPointLight();
    InitSpotLight();

    //�����̐ݒ�
    Vector4 ambient = { 0.3f,0.3f,0.3f,1.0f };
    SetAmbient(ambient);

    //�������C�g�̐ݒ�
    Vector3 groundColor = { 0.17f, 0.15f, 0.1255f };
    Vector3 skyColor = { 0.0116f, 0.0766f, 0.0766f };
    Vector3 groundNormal = Vector3{ 0.0f, 1.0f, 0.0f };
    SetHemiLight(groundColor, skyColor, groundNormal);

    m_light.cameraEyePos = g_camera3D->GetPosition();
}

void nsK2EngineLow::SceneLight::InitDirectionLight()
{
    //���ׂẴf�B���N�V�������C�g������Ȃ��悤�ɂ���
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHT; i++)
    {
        SetDirectionLight(i, g_vec3Zero, g_vec3Zero);
    }

    Vector3 dir = { 0.0f,-1.0f,-1.0f };
    dir.Normalize();

    Vector4 color = { 0.5f,0.5f,0.5f,1.0f };

    //0�Ԗڂ̃f�B���N�V�������C�g��������悤�ɂ���
    SetDirectionLight(0, dir, color);
}

void nsK2EngineLow::SceneLight::InitPointLight()
{
    for (int i = 0; i < MAX_POINT_LIGHT; i++)
    {
        SetPointLightPosition(g_vec3Zero, i);
        SetPointLightColor(g_vec3Zero, i);
        SetPointLightAttn(g_vec3Zero, i);
        UnUsePointLight(i);
    }
}

void nsK2EngineLow::SceneLight::InitSpotLight()
{
    for (int i = 0; i < MAX_SPOT_LIGHT; i++)
    {
        SetSpotLightPosition(g_vec3Zero, i);
        SetSpotLightColor(g_vec3Zero, i);
        SetSpotLightAttn(g_vec3Zero, i);
        UnUseSpotLight(i);
    }
}
