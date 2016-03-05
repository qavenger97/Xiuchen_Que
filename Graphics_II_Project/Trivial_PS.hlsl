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

struct SpotLight
{
	float4 color;
	float3 pos;
	float range;
	float3 dir;
	float innerAtt;
	float outerAtt;
	float intensity;
	float2 pad;
};

cbuffer Light : register(b0)
{
	DirectionalLight sun;
	PointLight pointLight;
	SpotLight spotLight;
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

void ComputeDirectionalLight(DirectionalLight light, float3 surfaceNormal, out float4 diffuseColor)
{
	diffuseColor = float4(0, 0, 0, 0);
	float3 dir = -light.dir;
	diffuseColor = saturate(dot(dir, surfaceNormal))*light.intensity * light.color;
}

void ComputePointLight(PointLight light, float3 surfacePos, float3 surfaceNormal, out float4 diffuseColor)
{
	diffuseColor = float4(0, 0, 0, 0);
	float3 dir = light.pos - surfacePos;
	float len = length(dir);
	if (len > light.att)return;
	dir /= len;
	float surfaceRatio = max(0,dot(dir, surfaceNormal));
	float att = 1 - clamp(len / light.att, 0, 1);
	diffuseColor = surfaceRatio * light.intensity * att * light.color;
}

void ComputeSpotLight(SpotLight light, float3 surfacePos, float3 surfaceNormal, out float4 diffuseColor)
{
	diffuseColor = float4(0, 0, 0, 0);
	float3 dir = light.pos - surfacePos;
	float len = length(dir);
	//if (len > light.range)return;
	dir /= len;

	float spotRatio = max(0, (dot(-dir, light.dir)));
	float LightRatio = max(0, dot(dir, surfaceNormal));
	float attD = 1 - clamp(len / light.range, 0, 1);
	float attR = 1 - saturate((light.innerAtt - spotRatio) / (light.innerAtt - light.outerAtt));
	float spotFactor = (spotRatio > light.outerAtt) ? 1 : 0;
	diffuseColor = spotFactor * LightRatio * light.color * attR * attD * light.intensity;
}

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
	float4 diffuseLight_d;
	ComputeDirectionalLight(sun, nrm, diffuseLight_d);
	//PointLight diffuse
	float4 diffuseLight_p;
	ComputePointLight(pointLight, input.posWorld, nrm, diffuseLight_p);
	//SpotLight diffuse
	float4 diffuseLight_s;
	ComputeSpotLight(spotLight, input.posWorld, nrm, diffuseLight_s);

	float4 diffuseLight = diffuseLight_d + diffuseLight_p + diffuseLight_s;
	//specluar
	float4 specularLight = float4(0, 0, 0, 0);
	return ambientLight + float4( (texColor * diffuseLight).rgb,texColor.a) + specularLight;

	//return tex.Sample(filter, input.uv);
}