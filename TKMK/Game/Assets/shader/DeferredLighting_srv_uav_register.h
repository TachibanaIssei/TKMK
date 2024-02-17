///////////////////////////////////////
// ディファードライティングで使用されるモデルシェーダーのSRV_UAVレジスタ設定
///////////////////////////////////////

#ifndef _DEFERREDLIGHTING_SRV_UAV_REGISTER_H_
#define _DEFERREDLIGHTING_SRV_UAV_REGISTER_H_

#include "Shadowing_const.h"

Texture2D<float4> albedoTexture : register(t0);                  // アルベドテクスチャ
Texture2D<float4> normalTexture : register(t1);                  // ノーマルテクスチャ
Texture2D<float4> metallicShadowSmoothTexture : register(t2);    // r:メタリック,b:シャドウパラム,a:スムース
Texture2D<float4> g_shadowMap[MAX_VIEWPORT][NUM_SHADOW_MAP] : register(t3);    //シャドウマップ(t3~使用)

#endif