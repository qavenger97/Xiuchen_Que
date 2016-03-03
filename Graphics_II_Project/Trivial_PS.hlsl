texture2D tex : register(t0);
SamplerState filter : register(s0);

struct DirectionalLight
{
	float3 dir;
	float intensity;
	float4 color;
};

cbuffer Light : register(b0)
{
	DirectionalLight sun;
}

struct INPUT
{
	float4 pos				   : SV_POSITION;
	float3 normal			   : NORMAL;
	float2 uv				   : TEXCOORD0;

};

float4 main( INPUT input ) : SV_TARGET
{
	float3 ldir = -sun.dir;
	float3 nrm = input.normal;
	float4 texColor = tex.Sample(filter, input.uv);
	return float4((max(0, dot(ldir, nrm)) * sun.color * texColor * sun.intensity).rgb,texColor.a);
	//return tex.Sample(filter, input.uv);
}