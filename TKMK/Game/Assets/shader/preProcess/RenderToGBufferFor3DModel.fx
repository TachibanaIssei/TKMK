///////////////////////////////////////
// �\���́B
///////////////////////////////////////

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION;       //���W�B
    float3 normal : NORMAL;         //�@���B
    float3 tangent  : TANGENT;      //�ڃx�N�g���B
    float3 biNormal : BINORMAL;     //�]�x�N�g���B
    float2 uv : TEXCOORD0;          //UV���W�B
};

// �s�N�Z���V�F�[�_�[����̏o��
struct SPSOut
{
    float4 albedo : SV_Target0;             // �A���x�h
    float4 normal : SV_Target1;             // �@��
    float4 metaricShadowSmooth : SV_Target2;  // ���^���b�N�A�e�p�����[�^�A�X���[�X�Br�Ƀ��^���b�N�Ag�ɉe�p�����[�^�Aa�ɃX���[�X�B
};

///////////////////////////////////////
// ���_�V�F�[�_�[�̋��ʏ������C���N���[�h����B
///////////////////////////////////////

#include "../ModelVSCommon.h"

///////////////////////////////////////
// �V�F�[�_�[���\�[�X
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      //�A���x�h�}�b�v
Texture2D<float4> g_normal : register(t1);      //�@���}�b�v
Texture2D<float4> g_spacular : register(t2);    //�X�y�L�����}�b�v

///////////////////////////////////////
// �T���v���[�X�e�[�g
///////////////////////////////////////
sampler g_sampler : register(s0);

///////////////////////////////////////
// �֐�
///////////////////////////////////////



// ���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal)
{
    SPSIn psIn;
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal);
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�

    // ���[���h��Ԃ̖@���A�ڃx�N�g���A�]�x�N�g�����v�Z����B
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
    // G-Buffer�ɏo��
    SPSOut psOut;
    // �A���x�h�J���[�Ɛ[�x�l���o��
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    
    clip(psOut.albedo.a - 0.2f);    // �s�N�Z���L��

    psOut.albedo.w = psIn.pos.z;
    // �@�����o��
    psOut.normal.xyz = GetNormalFromNormalMap( 
        g_normal, g_sampler, psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv ) ;
    psOut.normal.w = 1.0f;
    // ���^���b�N�X���[�X���o�́B
    psOut.metaricShadowSmooth = g_spacular.Sample(g_sampler, psIn.uv);
    // �e�p�����[�^�B
    psOut.metaricShadowSmooth.g = 255.0f * isShadowReciever;
    
    return psOut;
}

// ���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
SPSOut PSMain(SPSIn psIn)
{
    return PSMainCore(psIn, 0);
}
SPSOut PSMainShadowReciever(SPSIn psIn)
{
    return PSMainCore(psIn, 1);
}