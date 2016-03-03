texture2D tex : register(t0);
texture2D normal : register(t1);
SamplerState filter : register(s0);

struct DirectionalLight
{
	float3 dir;
	float intensity;
	float4 color;
};

struct PointLight
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
	PointLight pointLight;
}

struct INPUT
{
	float4 pos					: SV_POSITION;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD0;
	float3 posWorld				: TEXCOORD1; 
	float3 tengent				: TEXCOORD2;
	float3 binormal				: TEXCOORD3;
};

float4 main( INPUT input ) : SV_TARGET
{
	float3 nrmT = normal.Sample(filter, input.uv).xyz;
	nrmT = normalize((nrmT * 2 - 1.0f));

	float3 nrm = normalize(input.normal);
	float3 ten = normalize(input.tengent);
	float3 bin = normalize(input.binormal);

	float3x3 TBN = float3x3(ten, bin, nrm);


	nrm = mul(nrmT, TBN);
	float4 texColor = tex.Sample(filter, input.uv);
	float4 ambientLight = float4((0.1f * texColor).rgb,0);
	//DirectionLight diffuse
	float3 ddir = -sun.dir;
	float4 diffuseLight = saturate(dot(ddir, nrm)) * sun.intensity * sun.color;

	//PointLight diffuse
	float3 sdir = pointLight.pos - input.posWorld;
	float len = length(sdir);
	float att = 1 - clamp(len / pointLight.att, 0, 1);
	sdir /= len;
	diffuseLight = max(0, dot(sdir, nrm))*pointLight.intensity *att * pointLight.color + diffuseLight;

	//SpotLight diffuse
	//TODO: add spotLight

	//specluar
	float4 specularLight = float4(0, 0, 0, 0);

	return ambientLight + float4( (texColor * diffuseLight).rgb,texColor.a) + specularLight;

	//return tex.Sample(filter, input.uv);
}