/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};

cbuffer SpriteCB : register(b1){
	bool grayScale;
}

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	//�J���[�e�N�X�`���B
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain( PSInput In ) : SV_Target0
{
	if(grayScale)
	{
		float4 color = colorTexture.Sample(Sampler, In.uv) * mulColor;

		float Y = color.r * 0.29891f + color.g * 0.58661f + color.b * 0.11448f;
		color.r = Y;
		color.g = Y;
		color.b = Y;

		return color;
	}
	else
	{
		return colorTexture.Sample(Sampler, In.uv) * mulColor;
	}
}