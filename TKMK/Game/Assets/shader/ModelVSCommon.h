//���f���̒��_�V�F�[�_�[�֌W�̋��ʃw�b�_�[

///////////////////////////////////////
// �萔�o�b�t�@�B
///////////////////////////////////////
// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

////////////////////////////////////////////////
// �\����
////////////////////////////////////////////////

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
	float4 pos : POSITION;          //���_���W�B
	float3 normal : NORMAL;         //�@���B
	float2 uv : TEXCOORD0;          //UV���W�B
	float3 tangent  : TANGENT;      //�ڃx�N�g���B
	float3 biNormal : BINORMAL;     //�]�x�N�g���B
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
StructuredBuffer<float4x4> g_boneMatrix         : register(t3);	    //�{�[���s��B

///////////////////////////////////////
// �֐��錾
///////////////////////////////////////
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal);

////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////
/// <summary>
//�X�L���s����v�Z����B
/// </summary>
float4x4 CalcSkinMatrix(SVSIn skinVert)
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
/// ���[���h��Ԃ̒��_���W���v�Z����B
/// </summary>
/// <param name="vertexPos">���_���W</param>
/// <param name="mWorld">���[���h�s��</param>
float4 CalcVertexPositionInWorldSpace(float4 vertexPos, float4x4 mWorld)
{
	float4 pos;
	pos = mul(mWorld, vertexPos);  // ���f���̒��_�����[���h���W�n�ɕϊ�

	return pos;
}

/// <summary>
//  �@���}�b�v����@�����擾
/// </summary>
float3 GetNormalFromNormalMap(Texture2D<float4> normalMap, sampler Sampler,float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = normalMap.SampleLevel (Sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
    
    return newNormal;
}

/// <summary>
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, mWorld);
}

/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMainSkin( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, CalcSkinMatrix(vsIn));
}

/// <summary>
/// ���[���h�X�y�[�X�̖@���A�ڃx�N�g���A�]�x�N�g�����v�Z����B
/// </summary>
/// <param name="oNormal">�@���̏o�͐�</param>
/// <param name="oTangent">�ڃx�N�g���̏o�͐�</param>
/// <param name="oBiNormal">�]�x�N�g���̏o�͐�</param>
/// <param name="mWorld">���[���h�s��</param>
/// <param name="iNormal">�@��</param>
/// <param name="iTanget">�ڃx�N�g��</param>
/// <param name="iBiNormal">�]�x�N�g��</param>
void CalcVertexNormalTangentBiNormalInWorldSpace( 
	out float3 oNormal, 
	out float3 oTangent, 
	out float3 oBiNormal,
	float4x4 mWorld,
	float3 iNormal,
	float3 iTangent,
	float3 iBiNormal
)
{
	float3x3 m3x3 = (float3x3)mWorld;
	oNormal = normalize(mul(m3x3, iNormal));
	oTangent = normalize(mul(m3x3, iTangent));
	oBiNormal = normalize(mul(m3x3, iBiNormal));
}