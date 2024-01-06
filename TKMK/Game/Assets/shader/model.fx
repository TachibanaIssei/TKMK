/*!
 * @brief	シンプルなモデルシェーダー。
 */
////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float2 uv 			: TEXCOORD0;	//uv座標。
    float3 normal		: NORMAL;       //法線
    float3 tangent      : TANGENT;      //接ベクトル
    float3 biNormal     : BINORMAL;     //従ベクトル
    float3 worldPos		: TEXCOORD1;	//ワールド座標	
};

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////
#include "ModelVSCommon.h"

///////////////////////////////////////
// PBRライティング関係の定数
///////////////////////////////////////
#include "PBRLighting_const.h"

///////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////
#include "model_srv_uav_register.h"

///////////////////////////////////////
// PBRライティング
///////////////////////////////////////
#include "PBRLighting.h"

///////////////////////////////////////
// シャドウイング
///////////////////////////////////////
#include "Shadowing.h"

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal)
{
	SPSIn psIn;

	// 頂点座標をワールド座標系に変換する。
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
	psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
    
	// ワールド空間の法線、接ベクトル、従ベクトルを計算する。
	CalcVertexNormalTangentBiNormalInWorldSpace(
		psIn.normal,
		psIn.tangent,
		psIn.biNormal,
		mWorldLocal,
		vsIn.normal,
		vsIn.tangent,
		vsIn.biNormal
	);

	psIn.uv = vsIn.uv;
    
	return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMainCore( SPSIn In, uniform int isSoftShadow)
{	
    //アルベドカラーをサンプリング。
    float4 albedoColor = albedoTexture.Sample(Sampler, In.uv);
    //法線をサンプリング。
    float3 normal = GetNormalFromNormalMap(normalTexture, Sampler,In.normal,In.tangent,In.biNormal,In.uv);
    //ワールド座標をサンプリング。
    float3 worldPos = In.worldPos;
    //スペキュラカラーをサンプリング。
    float3 specColor = albedoColor.xyz;
    //金属度をサンプリング。
    float metaric = metallicShadowSmoothTexture.SampleLevel(Sampler, In.uv, 0).r;
    //スムース
    float smooth = metallicShadowSmoothTexture.SampleLevel(Sampler, In.uv, 0).a;
    
    return float4(normal.xyz,1.0f);

    //影生成用のパラメータ
    float shadowParam = 1.0f;
    
	// 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(light.eyeInfomation.eyePos[light.eyeInfomation.drawCameraNumber] - worldPos);

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
        )* ( 1.0f - shadow );
    }

	// 環境光による底上げ
    lig += light.ambientLight * albedoColor.xyz;

	float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    return float4(finalColor.xyz, albedoColor.a);
}

//ソフトシャドウを行うピクセルシェーダー
float4 PSMainSoftShadow(SPSIn In) : SV_Target0
{
    return PSMainCore( In, true);
}
//ハードシャドウを行うピクセルシェーダー。
float4 PSMainHardShadow(SPSIn In) : SV_Target0
{
    return PSMainCore( In, false);
} 

/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSShadowMapMain(SPSIn psIn) : SV_Target0
{
    //シャドウマップにZ値を書き込む
    return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
}