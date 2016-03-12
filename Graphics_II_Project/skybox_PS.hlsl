TextureCube tex : register(t3);
SamplerState filter : register(s0);

struct INPUT
{
	float4 pos	:	SV_POSITION;
	float3 lookup	:	POSITION;
};

float4 main(INPUT input) :SV_TARGET
{
	return tex.SampleLevel(filter, input.lookup, 4);
}