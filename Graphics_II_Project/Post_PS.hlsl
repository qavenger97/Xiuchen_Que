texture2D origin : register(t0);
texture2D depth : register(t1);
SamplerState filter : register(s0);

struct PS_IN
{
	float4 pos: SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(PS_IN input) : SV_TARGET
{
	float2 newUV = input.uv;
	float4 depthValue = depth.Sample(filter, newUV);
	float4 output = origin.Sample(filter, newUV);
	if (depthValue.g > 0.1f)output *= 0.6f;
	return depthValue;
	return output;
}