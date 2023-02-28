/*!
 * @brief	シンプルなモデルシェーダー。
 */


////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

cbuffer DirectionLightCB : register(b1)
{
    float3 lightDirection;	//ライトの方向
    float3 lightColor;		//ライトの色
    float3 CameraEyePos;	//カメラの座標を追加
}

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float2 uv 		: TEXCOORD0;	//UV座標。
    float3 normal	: NORMAL;
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float2 uv 			: TEXCOORD0;	//uv座標。
    float3 normal		: NORMAL;
    float3 worldPos		: TEXCOORD1;	//ワールド座標
	
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

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
/// 頂点シェーダーのコア関数。
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
	psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.normal = mul(m, vsIn.normal);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	psIn.uv = vsIn.uv;

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
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	
	//拡散反射光
	//内積の計算
    float t = dot(psIn.normal, lightDirection);
    //内積の結果に-1を乗算する
	t *= -1;
	
	if(t < 0.0f)
    {
        t = 0.0f;
    }
	
    float3 diffuseLight = lightColor * t;
	
	//鏡面反射光
	//反射ベクトル
    float3 refVec = reflect(lightDirection, psIn.normal);
	//サーフェイスから視点に向かうベクトル
    float3 toEyeVec = CameraEyePos - psIn.worldPos;
	//正規化
    toEyeVec = normalize(toEyeVec);
	
	//求めた2つのベクトルの内積
    t = dot(refVec, toEyeVec);
	//内積が負の場合は0.0fにする
	if(t<0.0f)
    {
        t = 0.0f;
    }
	//強さを絞る
    t = pow(t, 4.0f);
	//鏡面反射光を求める
    float3 specularLight = lightColor * t;
	
	
	//拡散反射光と鏡面反射光を合成する
    float3 light = diffuseLight + specularLight;
	
	//環境光
    light.x += 0.6f;
    light.y += 0.6f;
    light.z += 0.6f;
	
    albedoColor.xyz *= light;
	
	return albedoColor;
}
