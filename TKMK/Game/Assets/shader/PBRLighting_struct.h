/////////////////////////////////////////////////////
// PBRライティング関係の構造体をまとめたヘッダーファイル
/////////////////////////////////////////////////////

#ifndef _PBRLIGHTING_STRUCT_H_
#define _PBRLIGHTING_STRUCT_H_

#include "Shadowing_const.h"

// ディレクションライト構造体。
struct DirectionalLight
{
    float3 direction;   // ライトの方向
    int castShadow;     // 影をキャストする？
    float4 color;       // ライトの色
};

struct LightCB
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float4x4 mViewProjInv;  // ビュープロジェクション行列の逆行列
    float3 eyePos;          // カメラの視点
    float3 ambientLight;    // 環境光
   // float4x4 mlvp[NUM_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP];
};

#endif