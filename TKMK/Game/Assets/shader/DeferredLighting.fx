///////////////////////////////////////
// PBR�x�[�X�̃f�B�t�@�[�h���C�e�B���O
///////////////////////////////////////

///////////////////////////////////////
// �\���́B
///////////////////////////////////////

//���_�V�F�[�_�[�ւ̓��͍\���́B
struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};
//�s�N�Z���V�F�[�_�[�ւ̓��͍\���́B
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};
///////////////////////////////////////
// PBR���C�e�B���O�֌W�̒萔
///////////////////////////////////////
#include "PBRLighting_const.h"

///////////////////////////////////////
// �萔�o�b�t�@�B
///////////////////////////////////////
#include "DeferredLighting_cav_register.h"

///////////////////////////////////////
// SRV_UAV�̃��W�X�^�ݒ�
///////////////////////////////////////
#include "DeferredLighting_srv_uav_register.h"

///////////////////////////////////////
// PBR���C�e�B���O
///////////////////////////////////////
#include "PBRLighting.h"

///////////////////////////////////////
// �V���h�E�C���O
///////////////////////////////////////
#include "Shadowing.h"

///////////////////////////////////////
// �֐�
///////////////////////////////////////

//���_�V�F�[�_�[�B
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

/*!
 * @brief	UV���W�Ɛ[�x�l���烏�[���h���W���v�Z����B
 *@param[in]	uv				uv���W
 *@param[in]	zInScreen		�X�N���[�����W�n�̐[�x�l
 *@param[in]	mViewProjInv	�r���[�v���W�F�N�V�����s��̋t�s��B
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
    // UV���W���e�r���[�|�[�g�̐��K�����W�n�ɕϊ�����
    uv += uvOffset[cameraNumber];
    uv *= 2.0f;
    
	screenPos.xy = (uv * float2(2.0f, -2.0f)) + float2( -1.0f, 1.0f);
	screenPos.z = zInScreen;
	
	float4 worldPos = mul(mViewProjInv, float4(screenPos, 1.0f));
	worldPos.xyz /= worldPos.w;
	return worldPos.xyz;
}

/*!
 * @brief	�f�B���N�V�������C�g�̔��ˌ����v�Z
 *@param[in]	normal			�T�[�t�F�C�X�̖@���B
 *@param[in]	toEye           �T�[�t�F�C�X���王�_�ւ̕����x�N�g��(�P�ʃx�N�g��)
 *@param[in]	albedoColor     �A���x�h�J���[
 *@param[in]    metaric         ���^���b�N
 *@param[in]    smooth          ���炩��
 *@param[in]    specColor       �X�y�L�����J���[
 *@param[in]    worldPos        �T�[�t�F�C�X�̃��[���h���W
 *@param[in]    isSoftShadow    �\�t�g�V���h�E�H
 *@param[in]    shadowParam     �V���h�E���V�[�o�[�t���O�B
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
    
    // �e�̗�������v�Z����B
    float shadow = 0.0f;
    if( light.directionalLight[0].castShadow == 1){
        //�e�𐶐�����Ȃ�B
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

//�s�N�Z���V�F�[�_�[�̃R�A
float4 PSMainCore(PSInput In, uniform int isSoftShadow) 
{
    //G-Buffer�̓��e���g���ă��C�e�B���O
    //�A���x�h�J���[���T���v�����O
    float4 albedoColor = albedoTexture.Sample(Sampler, In.uv);
    //�@�����T���v�����O
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    //�`�撆�̃J�����̔ԍ�
    int cameraNumber = round(normalTexture.Sample(Sampler,In.uv).a * 10.0f);

    //���[���h���W���T���v�����O
    float3 worldPos = CalcWorldPosFromUVZ(
        In.uv, 
        albedoColor.w, 
        light.eyeInfomation.mViewProjInv[cameraNumber],
        cameraNumber);
    //�X�y�L�����J���[���T���v�����O
    float3 specColor = albedoColor.xyz;
    //�����x���T���v�����O
    float metaric = metallicShadowSmoothTexture.SampleLevel(Sampler, In.uv, 0).r;
    //�X���[�X
    float smooth = metallicShadowSmoothTexture.SampleLevel(Sampler, In.uv, 0).a;

    //�e�����p�̃p�����[�^
    float shadowParam = metallicShadowSmoothTexture.Sample(Sampler, In.uv).g;

    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
    float3 toEye = normalize(light.eyeInfomation.eyePos[cameraNumber] - worldPos);
    
    // �f�B���N�V�������C�g���v�Z
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

    // �����ɂ���グ
    lig += light.ambientLight * albedoColor.xyz;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    return finalColor;
}

float4 PSMainSoftShadow(PSInput In) : SV_Target0
{
    return PSMainCore( In, true);
}
//�n�[�h�V���h�E���s���s�N�Z���V�F�[�_�[
float4 PSMainHardShadow(PSInput In) : SV_Target0
{
    return PSMainCore( In, false);
} 