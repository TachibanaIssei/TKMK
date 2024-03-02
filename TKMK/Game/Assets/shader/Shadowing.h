// シャドウイング関係の処理。

#ifndef _SHADOWING_H_
#define _SHADOWING_H_

#include "Shadowing_const.h"
#include "PBRLighting_const.h"

static const int INFINITY = 40.0f;

#include "Sampler.h"

// チェビシェフの不等式を利用して、影になる可能性を計算する。
float Chebyshev(float2 moments, float depth)
{
    if (depth <= moments.x) {
		return 0.0;
	}
    // 遮蔽されているなら、チェビシェフの不等式を利用して光が当たる確率を求める
    float depth_sq = moments.x * moments.x;
    // このグループの分散具合を求める
    // 分散が大きいほど、varianceの数値は大きくなる
    float variance = moments.y - depth_sq;
    // このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める
    float md = depth - moments.x;
    // 光が届く確率を求める
    float lit_factor = variance / (variance + md * md);
    float lig_factor_min = 0.3f;
    // 光が届く確率の下限以下は影になるようにする。
    lit_factor = saturate((lit_factor - lig_factor_min) / (1.0f - lig_factor_min));
    // 光が届く確率から影になる確率を求める。
    return 1.0f - lit_factor;
}

float CalcShadowRate(
    Texture2D<float4> shadowMap[MAX_VIEWPORT][NUM_SHADOW_MAP],
    float4x4 mlvp[MAX_VIEWPORT][NUM_SHADOW_MAP], 
    float3 worldPos, 
    int cameraNumber,
    int isSoftShadow,
    int gameMode
)
{
    float shadow = 0.0f;
    int maxShadowMapCount = NUM_SHADOW_MAP;

    //2人プレイの場合は軽量化のためにカスケードシャドウを2枚だけにする
    if(gameMode == 2) maxShadowMapCount = 2;
    else if(gameMode >= 3) maxShadowMapCount = 1;

    for(int cascadeIndex = 0; cascadeIndex < maxShadowMapCount; cascadeIndex++)
    {
        float4 posInLVP = mul( mlvp[cameraNumber][cascadeIndex], float4( worldPos, 1.0f ));
        float2 shadowMapUV = posInLVP.xy / posInLVP.w;
        float zInLVP = posInLVP.z / posInLVP.w;
        shadowMapUV *= float2(0.5f, -0.5f);
        shadowMapUV += 0.5f;
        // シャドウマップUVが範囲内か判定
        if(shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
            && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f
            && zInLVP < 0.98f && zInLVP > 0.02f)
        {
            // シャドウマップから値をサンプリング
            float4 shadowValue;
            if(cameraNumber == 0) shadowValue = shadowMap[0][cascadeIndex].Sample(Sampler, shadowMapUV);
            else if(cameraNumber == 1) shadowValue = shadowMap[1][cascadeIndex].Sample(Sampler, shadowMapUV);
            else if(cameraNumber == 2) shadowValue = shadowMap[2][cascadeIndex].Sample(Sampler, shadowMapUV);
            else shadowValue = shadowMap[3][cascadeIndex].Sample(Sampler, shadowMapUV);

            zInLVP -= 0.001f;
            float pos = exp(INFINITY * zInLVP);
            if( isSoftShadow ){
                // ソフトシャドウ。
                shadow = Chebyshev(shadowValue.xy, pos);
            }else if(pos >= shadowValue.r ){
                // ハードシャドウ。
                shadow = 1.0f;
            }
            break;
        }
    }
    return shadow;
}

#endif