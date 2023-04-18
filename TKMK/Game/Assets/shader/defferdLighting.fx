/*!
 * @brief	ディファードライティング用のシェーダー。
 */

////////////////////////////////////////////////
// 構造体。
////////////////////////////////////////////////
struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

//ディレクションライト構造体
struct DirectionLight
{
    float3 direction;   //ライトの方向
    float4 color;       //ライトの色
};

//ポイントライト構造体
struct PointLight
{
    float3  position;   //ライトの座標
    int     isUse;      //使用中フラグ
    float4  color;      //ライトの色
    float3  range;      //xがライトの影響範囲、yが影響範囲に累乗するパラメータ
};

//スポットライト構造体
struct SpotLight
{
    float3  position;    //ライトの座標
    float3  angle;       //照射角度
    float4  color;       //色
    float3  range;       //xがライトの影響範囲、yが影響範囲に累乗するパラメータ
    int isUse;           //使用中フラグ
    float3  direction;   //照射方向
};

//半球ライト構造体
struct  HemisphereLight
{
    float3 groundColor;     //照り返しのライト
    int isUse;
    float3 skyColor;        //天球ライト
    float3 groundNormal;    //地面の法線
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer LightCB : register(b1)
{
    DirectionLight  directionLight;
    PointLight      pointLight;
    SpotLight       spotLight;
    HemisphereLight hemisphereLight;
    float3          CameraEyePos;       //カメラの座標
    float4          ambient;            //環境光
    Matrix          viewProjInvMatrix;  // ビュープロジェクション行列の逆行列。
};

////////////////////////////////////////////////
// ワールド変数
////////////////////////////////////////////////
Texture2D<float4> albedoTexture : register(t0); // アルベド
Texture2D<float4> normalTexture : register(t1); // 法線
Texture2D<float4> depthTexture  : register(t2); // 深度値

sampler Sampler : register(s0);

////////////////////////////////////////////////
// 関数宣言
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float4 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float4 lightColor, float3 worldPos, float3 normal,float2 uv);
float3 CalcLigFromDirectionLight(PSInput psIn,float3 normal, float3 worldPos);

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

float4 PSMain(PSInput In) : SV_Target0
{
    // G-Bufferの内容を使ってライティング
    float4 albedo = albedoTexture.Sample(Sampler, In.uv);
    float3 normal = normalTexture.Sample(Sampler, In.uv).rgb;
    normal = (normal * 2.0f) - 1.0f;
    float3 worldPos = depthTexture.Sample(Sampler,In.uv).xyz;

    // ディレクションライトを計算
    float3 directionLig = 0.0f;
    directionLig = CalcLigFromDirectionLight(In, normal, worldPos);


    float3 finalLight = directionLig + ambient.rgb;

    float4 finalColor = albedo;
    finalColor.xyz *= finalLight;

    return finalColor;
}

/////////////////////////////////////////////////////////////////////////
//  Lambert拡散反射を計算
/////////////////////////////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float4 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;

    // 内積の値を0以上の値にする
    t = max(0.0f, t);

    // 拡散反射光を計算する
    return lightColor.rgb * t;
}

/////////////////////////////////////////////////////////////////////////
//  Phong鏡面反射を計算
/////////////////////////////////////////////////////////////////////////
float3 CalcPhongSpecular(float3 lightDirection, float4 lightColor, float3 worldPos, float3 normal,float2 uv)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = normalize(CameraEyePos - worldPos);

    // 鏡面反射の強さを求める
    float t = max(0.0f,dot(toEye, refVec));

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);
    t *= 10.0f;

    // 鏡面反射光を求める
    float3 specularLig = lightColor.rgb * t;
    
    //スペキュラマップからスペキュラ反射の強さをサンプリング
    // float specPower = g_specularMap.Sample(g_sampler, uv).r;
    // specularLig *= specPower * 2.0f;
    
    return specularLig;
}

/////////////////////////////////////////////////////////////////////////
//  ディレクションライトを計算
/////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDirectionLight(PSInput In,float3 normal, float3 worldPos)
{
    // ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(directionLight.direction, directionLight.color, normal);

    // ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(
            directionLight.direction, directionLight.color, worldPos.xyz, normal, In.uv);
    
    // //サーフェイスの法線と光の入射方向に依存するリムの強さ
    // float power1 = 1.0f - max(0.0f, dot(directionLight.direction, psIn.normal));
    // //サーフェイスの法線と視線の方向に依存するリムの強さ
    // float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
    // //最終的なリムの強さ
    // float limPower = power1 * power2;
    // //指数関数的にする
    // limPower = pow(limPower, 1.3f);

    //最終的なディレクションライトを求める
    float3 finalDirectionLig = diffDirection + specDirection;
    
    return finalDirectionLig;
}