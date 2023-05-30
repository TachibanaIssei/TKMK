/*!
 * @brief	スプライト用のシェーダー
 */

cbuffer cb : register(b0){
	float4x4 mvp;
	float4 mulColor;
};

cbuffer SpriteCB : register(b1){
	bool grayScale;		//グレースケールするかどうか
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

float4 CalcGrayScale(float4 color);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain( PSInput In ) : SV_Target0
{
	float4 color = colorTexture.Sample(Sampler, In.uv) * mulColor;
	
	if(grayScale)
	{
		return CalcGrayScale(color);
	}

	return color;
}

float4 CalcGrayScale(float4 color)
{
	float Y = color.r * 0.29891f + color.g * 0.58661f + color.b * 0.11448f;
		color.r = Y;
		color.g = Y;
		color.b = Y;

		return color;
}