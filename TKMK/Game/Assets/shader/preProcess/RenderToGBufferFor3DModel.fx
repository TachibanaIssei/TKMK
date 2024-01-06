///////////////////////////////////////
// 構造体。
///////////////////////////////////////

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       //座標。
    float3 normal : NORMAL;         //法線。
    float3 tangent  : TANGENT;      //接ベクトル。
    float3 biNormal : BINORMAL;     //従ベクトル。
    float2 uv : TEXCOORD0;          //UV座標。
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0;             // アルベド
    float4 normal : SV_Target1;             // 法線
    float4 metaricShadowSmooth : SV_Target2;  // メタリック、影パラメータ、スムース。rにメタリック、gに影パラメータ、aにスムース。
};

struct GBufferCB
{
    int drawCameraNumber;
};

cbuffer cb_0 : register(b1)
{
    GBufferCB gbufferCB;  // ライトデータ
};

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////

#include "../ModelVSCommon.h"

///////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      //アルベドマップ
Texture2D<float4> g_normal : register(t1);      //法線マップ
Texture2D<float4> g_spacular : register(t2);    //スペキュラマップ

///////////////////////////////////////
// サンプラーステート
///////////////////////////////////////
sampler g_sampler : register(s0);

///////////////////////////////////////
// 関数
///////////////////////////////////////



// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal)
{
    SPSIn psIn;
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal);
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

SPSOut PSMainCore( SPSIn psIn, int isShadowReciever)
{
    // G-Bufferに出力
    SPSOut psOut;
    // アルベドカラーと深度値を出力
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    
    clip(psOut.albedo.a - 0.2f);    // ピクセルキル

    psOut.albedo.w = psIn.pos.z;
    // 法線を出力
    psOut.normal.xyz = GetNormalFromNormalMap( 
        g_normal, g_sampler, psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv ) ;
    psOut.normal.w = gbufferCB.drawCameraNumber/10.0f;
    // メタリックスムースを出力。
    psOut.metaricShadowSmooth = g_spacular.Sample(g_sampler, psIn.uv);
    // 影パラメータ。
    psOut.metaricShadowSmooth.g = 255.0f * isShadowReciever;
    
    return psOut;
}

// モデル用のピクセルシェーダーのエントリーポイント
SPSOut PSMain(SPSIn psIn)
{
    return PSMainCore(psIn, 0);
}
SPSOut PSMainShadowReciever(SPSIn psIn)
{
    return PSMainCore(psIn, 1);
}