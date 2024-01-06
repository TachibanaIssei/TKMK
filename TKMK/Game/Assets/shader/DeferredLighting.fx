///////////////////////////////////////
// PBRベースのディファードライティング
///////////////////////////////////////

///////////////////////////////////////
// 構造体。
///////////////////////////////////////

//頂点シェーダーへの入力構造体。
struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};
//ピクセルシェーダーへの入力構造体。
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};
///////////////////////////////////////
// PBRライティング関係の定数
///////////////////////////////////////
#include "PBRLighting_const.h"

///////////////////////////////////////
// 定数バッファ。
///////////////////////////////////////
#include "DeferredLighting_cav_register.h"

///////////////////////////////////////
// SRV_UAVのレジスタ設定
///////////////////////////////////////
#include "DeferredLighting_srv_uav_register.h"

///////////////////////////////////////
// PBRライティング
///////////////////////////////////////
#include "PBRLighting.h"

///////////////////////////////////////
// シャドウイング
///////////////////////////////////////
#include "Shadowing.h"

///////////////////////////////////////
// 関数
///////////////////////////////////////

//頂点シェーダー。
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

/*!
 * @brief	UV座標と深度値からワールド座標を計算する。
 *@param[in]	uv				uv座標
 *@param[in]	zInScreen		スクリーン座標系の深度値
 *@param[in]	mViewProjInv	ビュープロジェクション行列の逆行列。
 */
float3 CalcWorldPosFromUVZ( float2 uv, float zInScreen, float4x4 mViewProjInv, int cameraNumber )
{
	float3 screenPos;
    float2 uvOffset[MAX_VIEWPORT] = 
    {
        float2( 0.0f,  0.0f ),
        float2(-0.5f,  0.0f ),
        float2( 0.0f, -0.5f ),
        float2(-0.5f, -0.5f ),
    };
    // UV座標を各ビューポートの正規化座標系に変換する
    uv += uvOffset[cameraNumber];
    uv *= 2.0f;
    
	screenPos.xy = (uv * float2(2.0f, -2.0f)) + float2( -1.0f, 1.0f);
	screenPos.z = zInScreen;
	
	float4 worldPos = mul(mViewProjInv, float4(screenPos, 1.0f));
	worldPos.xyz /= worldPos.w;
	return worldPos.xyz;
}

/*!
 * @brief	ディレクションライトの反射光を計算
 *@param[in]	normal			サーフェイスの法線。
 *@param[in]	toEye           サーフェイスから視点への方向ベクトル(単位ベクトル)
 *@param[in]	albedoColor     アルベドカラー
 *@param[in]    metaric         メタリック
 *@param[in]    smooth          滑らかさ
 *@param[in]    specColor       スペキュラカラー
 *@param[in]    worldPos        サーフェイスのワールド座標
 *@param[in]    isSoftShadow    ソフトシャドウ？
 *@param[in]    shadowParam     シャドウレシーバーフラグ。
 */
float3 CalcDirectionLight(
    float3 normal, 
    float3 toEye, 
    float4 albedoColor, 
    float metaric, 
    float smooth, 
    float3 specColor,
    float3 worldPos,
    bool isSoftShadow,
    float shadowParam
){
    float3 lig = 0;
    
    // 影の落ち具合を計算する。
    float shadow = 0.0f;
    if( light.directionalLight[0].castShadow == 1){
        //影を生成するなら。
        shadow = CalcShadowRate( 
            g_shadowMap, 
            light.mlvp, 
            0, 
            worldPos, 
            isSoftShadow ) * shadowParam;
        }

    for(int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++)
    {        
        lig += CalcLighting(
            light.directionalLight[ligNo].direction,
            light.directionalLight[ligNo].color.xyz,
            normal,
            toEye,
            albedoColor,
            metaric,
            smooth,
            specColor
        ) * ( 1.0f - shadow );
    }
    return lig;
}

//ピクセルシェーダーのコア
float4 PSMainCore(PSInput In, uniform int isSoftShadow) 
{
    //G-Bufferの内容を使ってライティング
    //アルベドカラーをサンプリング
    float4 albedoColor = albedoTexture.Sample(Sampler, In.uv);
    //法線をサンプリング
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    //描画中のカメラの番号
    int cameraNumber = round(normalTexture.Sample(Sampler,In.uv).a * 10.0f);

    //ワールド座標をサンプリング
    float3 worldPos = CalcWorldPosFromUVZ(
        In.uv, 
        albedoColor.w, 
        light.eyeInfomation.mViewProjInv[cameraNumber],
        cameraNumber);
    //スペキュラカラーをサンプリング
    float3 specColor = albedoColor.xyz;
    //金属度をサンプリング
    float metaric = metallicShadowSmoothTexture.SampleLevel(Sampler, In.uv, 0).r;
    //スムース
    float smooth = metallicShadowSmoothTexture.SampleLevel(Sampler, In.uv, 0).a;

    //影生成用のパラメータ
    float shadowParam = metallicShadowSmoothTexture.Sample(Sampler, In.uv).g;

    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(light.eyeInfomation.eyePos[cameraNumber] - worldPos);
    
    // ディレクションライトを計算
    float3 lig = CalcDirectionLight(
        normal, 
        toEye, 
        albedoColor, 
        metaric, 
        smooth, 
        specColor,
        worldPos,
        isSoftShadow,
        shadowParam
    );

    // 環境光による底上げ
    lig += light.ambientLight * albedoColor.xyz;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    return finalColor;
}

float4 PSMainSoftShadow(PSInput In) : SV_Target0
{
    return PSMainCore( In, true);
}
//ハードシャドウを行うピクセルシェーダー
float4 PSMainHardShadow(PSInput In) : SV_Target0
{
    return PSMainCore( In, false);
} 