#include "k2EngineLowPreCompile.h"
#include "Light.h"

namespace nsK2EngineLow
{
    namespace
    {
        const Vector3 DIRECTION_LIGHT_COLOR = { 0.5f,0.5f,0.5f };
        const Vector3 DIRECTION_LIGHT_DIRECTION = { 1.0f,-1.0f,-1.0f };
        const Vector3 AMBIENT_LIGHT_COLOR = { 0.3f,0.3f,0.3f };
    }

    void SceneLight::Init()
    {
        //�f�B���N�V�������C�g�̐F
        SetDirLightColor(DIRECTION_LIGHT_COLOR);

        //�f�B���N�V�������C�g�̕���
        Vector3 direction = DIRECTION_LIGHT_DIRECTION;
        direction.Normalize();
        SetDirLightDirection(direction);

        //�����̐ݒ�
        SetAmbient(AMBIENT_LIGHT_COLOR);

        //���_���W��ݒ�
        SetEyePos(g_camera3D->GetPosition());
    }
}