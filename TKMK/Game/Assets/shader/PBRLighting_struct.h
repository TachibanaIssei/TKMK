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

struct EyeInfo
{
    float4x4 mViewProjInv[MAX_VIEWPORT];  // ビュープロジェクション行列の逆行列
    float3 eyePos[MAX_VIEWPORT];          // カメラの視点
    float pad;
    int drawCameraNumber;   //描画しているカメラの番号
};

struct LightCB
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float3 ambientLight;    // 環境光
    EyeInfo eyeInfomation;
    float4x4 mlvp[MAX_VIEWPORT][NUM_SHADOW_MAP];
    int gameMode;           //ゲームモード(0:1画面,2:2画面,4:4画面)
};

#endif