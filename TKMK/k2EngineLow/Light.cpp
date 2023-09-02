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
        //ディレクションライトの色
        SetDirLightColor(DIRECTION_LIGHT_COLOR);

        //ディレクションライトの方向
        Vector3 direction = DIRECTION_LIGHT_DIRECTION;
        direction.Normalize();
        SetDirLightDirection(direction);

        //環境光の設定
        SetAmbient(AMBIENT_LIGHT_COLOR);

        //視点座標を設定
        SetEyePos(g_camera3D->GetPosition());
    }
}