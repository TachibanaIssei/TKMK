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
    float3	lightDirection;	//ライトの方向
    float3	lightColor;		//ライトの色
    float3	CameraEyePos;	//カメラの座標を追加
    float3	ptPosition;		//ポイントライトの位置
    float3	ptColor;		//ポイントライトの色
    float   ptRange;        //ポイントライトの影響範囲
}

////////////////////////////////////////////////
// 関数宣言
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);

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
	
	//ディレクションライトの拡散反射光
    float3 diffDirection = CalcLambertDiffuse(
        lightDirection,
        lightColor,
        psIn.normal);
	
	//ディレクションライトの鏡面反射光
    float3 specDirection = CalcPhongSpecular(
        lightDirection,
        lightColor,
        psIn.worldPos,
        psIn.normal
        );
	
    //ポイントライト
    //ポイントライトの光の向き
    float3 ligDir = psIn.worldPos - ptPosition;
    ligDir = normalize(ligDir);
    //拡散反射光
    float3 diffPoint = CalcLambertDiffuse(
        ligDir,
        ptColor,
        psIn.normal
    );
    //鏡面反射光
    float3 specPoint = CalcPhongSpecular(
        ligDir,
        ptColor,
        psIn.worldPos,
        psIn.normal
    );
    //ポイントライトとの距離計算
    float3 distance = length(psIn.worldPos - ptPosition);
    //影響率は距離に比例して小さくなる
    float affect = 1.0f - 1.0f / ptRange * distance;
    //影響力がマイナスにならないようにする
    affect = max(0.0f, affect);
    //影響を指数関数的にする
    affect = pow(affect, 3.0f);
    //減衰率を乗算して影響を弱める
    diffPoint *= affect;
    specPoint *= affect;
    
    //2つの反射光を合算して最終的な反射光を求める
    float3 diffuseLight = diffPoint + diffDirection;
    float3 specularLight = specPoint + specDirection;
    
	//拡散反射光と鏡面反射光を合成する
    float3 light = diffuseLight + specularLight;
	
	//環境光
    light.x += 0.6f;
    light.y += 0.6f;
    light.z += 0.6f;
	
    albedoColor.xyz *= light;
	
	return albedoColor;
}

/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;

    // 内積の値を0以上の値にする
    t = max(0.0f, t);

    // 拡散反射光を計算する
    return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = CameraEyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);

    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);

    // 鏡面反射光を求める
    return lightColor * t;
}
