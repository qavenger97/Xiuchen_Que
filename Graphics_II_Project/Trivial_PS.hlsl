texture2D tex : register(t0);
SamplerState filter : register(s0);

struct DirectionalLight
{
	float3 dir;
	float intensity;
	float4 color;
};

struct SpotLight
{
	float3 pos;
	float att;
	float4 color;
	float intensity;
	float3 pad;
};

cbuffer Light : register(b0)
{
	DirectionalLight sun;
	SpotLight spot;
}

struct INPUT
{
	float4 pos					: SV_POSITION;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD0;
	float3 posView				: TEXCOORD1;
};

float4 main( INPUT input ) : SV_TARGET
{
	//DirectionLight
	float3 ddir = -sun.dir;
	float3 nrm = input.normal;
	float4 texColor = tex.Sample(filter, input.uv);
	float4 totalLight = max(0, dot(ddir, nrm))* sun.intensity * sun.color;
	//SpotLight
	float3 sdir = spot.pos - input.posView;
	float len = length(sdir);
	float att = 1 - clamp(len / spot.att, 0, 1);
	sdir /= len;
	totalLight = max(0, dot(sdir, nrm))*spot.intensity*att*spot.color + totalLight;

	return float4( (texColor * totalLight ).rgb,texColor.a);

	//return tex.Sample(filter, input.uv);
}