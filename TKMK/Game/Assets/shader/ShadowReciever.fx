/*!
 * @brief 影が落とされるモデル用のシェーダー
 */

///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////
// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;  // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD0;  // UV座標
    float3 tangent : TANGENT;   //接ベクトル
    float3 biNormal : BINORMAL; //従ベクトル
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;   // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL;     // 法線
    float3 tangent : TANGENT;   //接ベクトル
    float3 biNormal : BINORMAL; //従ベクトル
    float2 uv : TEXCOORD0;      // UV座標
    float3 worldPos:TEXCOORD1;  //ワールド座標
    float4 posInLVP:TEXCOORD2;  //ライトビュースクリーン空間でのピクセルの座標
};

//ディレクションライト構造体
struct DirectionLight
{
    float3 direction;   //ライトの方向
    float3 color;       //ライトの色
};

//ポイントライト構造体
struct PointLight
{
    float3  position;   //ライトの座標
    int     isUse;      //使用中フラグ
    float3  color;      //ライトの色
    float3  range;      //xがライトの影響範囲、yが影響範囲に累乗するパラメータ
};

//スポットライト構造体
struct SpotLight
{
    float3  position;    //ライトの座標
    float3  angle;       //照射角度
    float3  color;       //色
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

///////////////////////////////////////////////////
// 定数バッファ
///////////////////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// ライトビュープロジェクション行列の定数バッファーを定義
cbuffer LightCB : register(b1)
{
    DirectionLight  directionLight;
    PointLight      pointLight;
    SpotLight       spotLight;
    HemisphereLight hemisphereLight;
    float3          CameraEyePos;   //カメラの座標
    float3          ambient;        //環境光
    float4x4        mLVP;
}

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0);      // アルベドマップ
Texture2D<float4> g_normalMap : register(t1);           //法線マップ
Texture2D<float4> g_specularMap : register(t2);
Texture2D<float4> g_shadowMap : register(t10);  // シャドウマップ
sampler g_sampler : register(s0);               // サンプラーステート

////////////////////////////////////////////////
// 関数宣言
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float4 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float4 lightColor, float3 worldPos, float3 normal,float2 uv);
float3 CalcLigFromDirectionLight(SPSIn psIn,float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn,float3 normal);
float3 CalcLigFromSpotLight(SPSIn psIn,float3 normal);
float3 CalcLigFromHemisphereLight(SPSIn psIn);
float3 CalcNormalMap(SPSIn psIn);

/// <summary>
/// 影が落とされる3Dモデル用の頂点シェーダー
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    // 通常の座標変換
    SPSIn psIn;
    float4 worldPos = mul(mWorld, vsIn.pos);
    psIn.worldPos = worldPos;
    psIn.pos = mul(mView, worldPos);
    psIn.pos = mul(mProj, psIn.pos);

    psIn.uv = vsIn.uv;

    psIn.normal = mul(mWorld, vsIn.normal);
    psIn.normal = normalize(psIn.normal);

    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));

    // ライトビュースクリーン空間の座標を計算する
    psIn.posInLVP = mul(mLVP,worldPos);

    return psIn;
}

/// <summary>
/// 影が落とされる3Dモデル用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //法線を計算
    float3 normal = CalcNormalMap(psIn);
    //ディレクションライトによるライティングの計算
    float3 directionLig = CalcLigFromDirectionLight(psIn, normal);
    //ポイントライトによるライティングの計算
    float3 pointLig = { 0.0f, 0.0f, 0.0f };
    if (pointLight.isUse)
    {
        pointLig = CalcLigFromPointLight(psIn,normal);
    }
    //スポットライトによるライティングの計算
    float3 spotLig = { 0.0f, 0.0f, 0.0f };
    if (spotLight.isUse)
    {
        spotLig = CalcLigFromSpotLight(psIn,normal);
    }
    //半球ライトによるライティングの計算
    float3 hemiLight = { 0.0f, 0.0f, 0.0f };
    if (hemisphereLight.isUse)
    {
        hemiLight = CalcLigFromHemisphereLight(psIn);
    }
    //光の合成
    float3 light = directionLig + ambient;

    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

    //光を色に乗算する
    albedoColor.xyz *= light;

    // ライトビュースクリーン空間からUV空間に座標変換
    float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f,-0.5f);
    shadowMapUV += 0.5f;

    //ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;

    if(shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        // シャドウマップに描き込まれているZ値と比較する
        float zInShadowMap = g_shadowMap.Sample(g_sampler,shadowMapUV).r;
        if(zInLVP > zInShadowMap)
        {
            // 遮蔽されている
            albedoColor.xyz *= 0.7f;
        }
    }

    return albedoColor;
}

/////////////////////////////////////////////////////////////////////////
//  Lambert拡散反射を計算
/////////////////////////////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;

    // 内積の値を0以上の値にする
    t = max(0.0f, t);

    // 拡散反射光を計算する
    return lightColor * t;
}

/////////////////////////////////////////////////////////////////////////
//  Phong鏡面反射を計算
/////////////////////////////////////////////////////////////////////////
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal,float2 uv)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = CameraEyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);

    // 鏡面反射の強さを0～1の数値にする
    //t = saturate(t);
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);
      
    // 鏡面反射光を求める
    float specularLig = lightColor * t;
    
    //スペキュラマップからスペキュラ反射の強さをサンプリング
    float specPower = g_specularMap.Sample(g_sampler, uv).r;
    specularLig *= specPower * 1.1f;

    return specularLig;
}

/////////////////////////////////////////////////////////////////////////
//  ディレクションライトを計算
/////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDirectionLight(SPSIn psIn,float3 normal)
{
    // ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(directionLight.direction, directionLight.color, normal);

    // ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(
            directionLight.direction, directionLight.color, psIn.worldPos, normal,psIn.uv);
    
    return diffDirection + specDirection;
}
/////////////////////////////////////////////////////////////////////////
//  ポイントライトを計算
/////////////////////////////////////////////////////////////////////////
float3 CalcLigFromPointLight(SPSIn psIn,float3 normal)
{
    // このサーフェイスに入射しているポイントライトの光の向きを計算する
    float3 ligDir = psIn.worldPos - pointLight.position;

    // 正規化して大きさ1のベクトルにする
    ligDir = normalize(ligDir);

    // 減衰なしのLambert拡散反射光を計算する
    float3 diffPoint = CalcLambertDiffuse(
        ligDir,             // ライトの方向
        pointLight.color,   // ライトのカラー
        normal              // サーフェイスの法線
    );

    // 減衰なしのPhong鏡面反射光を計算する
    float3 specPoint = CalcPhongSpecular(
        ligDir,             // ライトの方向
        pointLight.color,   // ライトのカラー
        psIn.worldPos,      // サーフェイスのワールド座標
        normal,             // サーフェイスの法線
        psIn.uv
    );

    // 距離による影響率を計算する
    // ポイントライトとの距離を計算する
    float3 distance = length(psIn.worldPos - pointLight.position);

    // 影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / pointLight.range.x * distance;

    // 影響力がマイナスにならないように補正をかける
    affect = max(0.0f, affect);

    // 影響の仕方を指数関数的にする
    affect = pow(affect, pointLight.range.y);

    // 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
    diffPoint *= affect;
    specPoint *= affect;

    return diffPoint + specPoint;
}

/////////////////////////////////////////////////////////////////////////
//  スポットライトを計算
/////////////////////////////////////////////////////////////////////////
float3 CalcLigFromSpotLight(SPSIn psIn, float3 normal)
{
    //サーフェイスによる入射するスポットライトの光の向き
    float3 ligDir = psIn.worldPos - spotLight.position;
    ligDir = normalize(ligDir);
    //減衰なしのLambert拡散反射光を計算
    float3 diffSpotLight = CalcLambertDiffuse(
        ligDir,
        spotLight.color,
        normal
    );
    //減衰なしのPhong鏡面反射光を計算
    float3 specSpotLight = CalcPhongSpecular(
        ligDir,
        spotLight.color,
        psIn.worldPos,
        normal,
        psIn.uv
    );
    
    //距離による影響率を計算する
    float3 distance = length(psIn.worldPos - spotLight.position);
    float affect = 1.0f - 1.0f / spotLight.range.x * distance;
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }
    affect = pow(affect, spotLight.range.y);
    diffSpotLight *= affect;
    specSpotLight *= affect;
    
    float angle = dot(ligDir, spotLight.direction);
    angle = abs(acos(angle));
    affect = 1.0f - 1.0 / spotLight.angle.x * angle;
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }
    affect = pow(affect, spotLight.angle.y);
    diffSpotLight *= affect;
    specSpotLight *= affect;
    
    return diffSpotLight + specSpotLight;
}

/////////////////////////////////////////////////////////////////////////
// 半球ライトを計算
/////////////////////////////////////////////////////////////////////////
float3 CalcLigFromHemisphereLight(SPSIn psIn)
{
    float t = dot(psIn.normal, hemisphereLight.groundNormal);
    t = (t + 1.0f) / 2.0f;
    float3 hemiLight = lerp(hemisphereLight.groundColor, hemisphereLight.skyColor, t);
    return hemiLight;
}

/////////////////////////////////////////////////////////////////////////
//  法線を計算
/////////////////////////////////////////////////////////////////////////
float3 CalcNormalMap(SPSIn psIn)
{
    float3 normal = psIn.normal;
    //法線マップからタンジェントスペースの法線をサンプリング
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    //タンジェントスペースの法線を0～1の範囲から-1～1の範囲に復元
    localNormal = (localNormal - 0.5f) * 2.0f;
    //タンジェントスペースの法線をワールドスペースに変換
    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;
    
    return normal;
}
