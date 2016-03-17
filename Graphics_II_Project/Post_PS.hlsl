texture2D origin : register(t0);
texture2D depth : register(t1);
SamplerState filter : register(s0);

struct PS_IN
{
	float4 pos: SV_POSITION;
	float2 uv : TEXCOORD0;
};

cbuffer Wave : register(b0)
{
	float	frequency;
	float   speed;
	float   amplitude;
	float   pad;
}

float4 main(PS_IN input) : SV_TARGET
{
	float2 newUV = input.uv;
	newUV.y += sin(input.uv.x * frequency + speed) * amplitude;
	float4 depthValue = depth.Sample(filter, newUV);
	float4 output = origin.Sample(filter, newUV);
	

	return output;
}