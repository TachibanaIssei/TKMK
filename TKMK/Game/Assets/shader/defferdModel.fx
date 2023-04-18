/*!
 * @brief シンプルなモデル表示用のシェーダー
 */

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;

    SSkinVSIn skinVert;				//スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 worldPos :TEXCOORD1; //ワールド座標
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0; // アルベド
    float4 normal : SV_Target1; // 法線
    float4 worldPos : SV_Target2; //ワールド座標
};

// モデルテクスチャ
Texture2D<float4> g_texture : register(t0);
Texture2D<float4> g_normalMap : register(t1);
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

// サンプラーステート
sampler g_sampler : register(s0);

/////////////////////////////////////////////////////////////////////////
//  関数を宣言
/////////////////////////////////////////////////////////////////////////
float3 CalcNormalMap(SPSIn psIn);

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}

    psIn.pos = mul(m, vsIn.pos);        // 1. モデルの頂点をワールド座標系に変換
    psIn.worldPos = psIn.pos;           // ワールド座標を出力
    psIn.pos = mul(mView, psIn.pos);    // 2. ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos);    // 3. カメラ座標系からスクリーン座標系に変換
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.uv = vsIn.uv;
    
    psIn.tangent = normalize(mul(mWorld,vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld,vsIn.biNormal));

    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
SPSOut PSMain(SPSIn psIn)
{
    // G-Bufferに出力
    SPSOut psOut;

    // アルベドカラーを出力
    psOut.albedo = g_texture.Sample(g_sampler, psIn.uv);

    psIn.normal = CalcNormalMap(psIn);

    // 法線を出力
    // 出力は0～1に丸められてしまうのでマイナスの値が失われてしまう
    // なので-1～1を0～1に変換する
    // (-1 ～ 1) ÷ 2.0       = (-0.5 ～ 0.5)
    // (-0.5 ～ 0.5) + 0.5  = (0.0 ～ 1.0)
    psOut.normal.xyz = (psIn.normal / 2.0f) + 0.5f;
    psOut.normal.w = 0.0f;

    psOut.worldPos.xyz = psIn.worldPos;
    psOut.worldPos.w = 1.0f;

    return psOut;
}

/////////////////////////////////////////////////////////////////////////
//  法線を計算
/////////////////////////////////////////////////////////////////////////
float3 CalcNormalMap(SPSIn psIn)
{
    float3 localNormal = g_normalMap.Sample(g_sampler,psIn.uv).xyz;
    //タンジェントスペースの法線を0~1の範囲から-1~1の範囲に復元する
    localNormal = (localNormal - 0.5f) * 2.0f;

    float3 normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + psIn.normal * localNormal.z;

    return normal;
}
