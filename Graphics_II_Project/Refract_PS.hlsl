texture2D tex : register(t0);
SamplerState filter : register(s0);

struct INPUT
{
	float4 pos					: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
	float3 toEye				: TEXCOORD1;
	float3 posWorld				: TEXCOORD2;
	float3 tengent				: TEXCOORD3;
	float3 binormal				: TEXCOORD4;
	float3 eyeTengent			: TEXCOORD5;
};


float4 main(INPUT input) : SV_TARGET
{
	float3 toEye = normalize(input.toEye);
	float3 normal = normalize(input.normal);

	float3 lookup = refract(toEye, normal, 0.91f);
	float4 ref = float4(tex.Sample(filter, lookup.xy).xyz,1);

	return ref;
}