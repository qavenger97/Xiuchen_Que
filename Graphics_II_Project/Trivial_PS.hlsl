texture2D tex : register(t0);
texture2D normal : register(t1);
texture2D spec : register(t2);
textureCUBE envMap: register(t3);
SamplerState filter : register(s0);

struct Light
{
	float4 pos; // w = index;
	float4 dir; //
	float4 color; // w = intensity;
	float4 att; // x = inner, y = outer, z = range, w = type: 0 == Directional Light, 1 = PointLight, 2 = SpotLight;
};

struct Material
{
	float3 ambientColor;
	float	 specularPower;
	float3 diffuseColor;
	float	 fresnelPower;
	float3 specularColor;
	float	 fresnelIntensity;
	float	heightOffset;
	float3 pad;
};

cbuffer Light : register(b0)
{
	Light lights[3];
	Material material;
}

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

void ComputeSpecular(float4 lightColor, float3 surfacePos, float3 toEye, float3 surfaceNormal, float3 lightDir , out float4 specular)
{
	specular = float4(0, 0, 0, 0);
	float3 halfv = normalize(lightDir + toEye);
	float intensity = pow(saturate(dot(surfaceNormal, halfv)), material.specularPower);
	specular = float4(material.specularColor, 0) * lightColor * intensity * lightColor.w;
}


void ComputeDirectionalLight(Light light, float3 surfacePos, float3 toEye, float3 surfaceNormal, out float4 diffuse, out float4 specular)
{
	diffuse = float4(0, 0, 0, 0);
	specular = float4(0, 0, 0, 0);
	float3 dir = -light.dir.xyz;
	diffuse = saturate(dot(dir, surfaceNormal))*light.color.w * float4(light.color.xyz,0);
	ComputeSpecular(light.color, surfacePos, toEye, surfaceNormal, dir, specular);
}

void ComputePointLight(Light light, float3 surfacePos, float3 toEye, float3 surfaceNormal, out float4 diffuse, out float4 specular)
{
	diffuse = float4(0, 0, 0, 0);
	specular = float4(0, 0, 0, 0);
	float3 dir = light.pos.xyz - surfacePos;
	float len = length(dir);
	if (len > light.att.z)return;
	dir /= len;
	float surfaceRatio = max(0,dot(dir, surfaceNormal));
	float att = 1 - clamp(len / light.att.z, 0, 1);
	diffuse = surfaceRatio * light.color.w * att * float4(light.color.xyz, 0);
	ComputeSpecular(light.color, surfacePos, toEye, surfaceNormal, dir,  specular);
	specular *= att;
}

void ComputeSpotLight(Light light, float3 surfacePos, float3 toEye, float3 surfaceNormal, out float4 diffuse, out float4 specular)
{
	diffuse = float4(0, 0, 0, 0);
	specular = float4(0, 0, 0, 0);
	float3 dir = light.pos.xyz - surfacePos;
	float len = length(dir);
	if (len > light.att.z)return;
	dir /= len;

	float spotRatio = max(0, (dot(-dir, light.dir.xyz)));
	float LightRatio = max(0, dot(dir, surfaceNormal));
	float attD = 1 - clamp(len / light.att.z, 0, 1);
	float attR = 1 - saturate((light.att.x - spotRatio) / (light.att.x - light.att.y));
	float spotFactor = (spotRatio > light.att.y) ? 1 : 0;
	float att = attR * attD;
	diffuse = spotFactor * LightRatio * float4(light.color.xyz, 0) * att * light.color.w;
	ComputeSpecular(light.color, surfacePos, toEye, surfaceNormal, dir, specular);
	specular *= att;
}

void ComputeLight(Light light, float3 surfacePos, float3 toEye, float3 surfaceNormal, out float4 diffuse, out float4 specular)
{
	diffuse = float4(0, 0, 0, 0);
	specular = float4(0, 0, 0, 0);
	switch (light.att.w)
	{
	case 0:
		ComputeDirectionalLight(light, surfacePos, toEye, surfaceNormal, diffuse, specular);
		break;
	case 1:
		ComputePointLight(light, surfacePos, toEye, surfaceNormal, diffuse, specular);
		break;
	case 2:
		ComputeSpotLight(light, surfacePos, toEye, surfaceNormal, diffuse, specular);
		break;
	default:
		break;
	}
}

float4 main( INPUT input ) : SV_TARGET
{
	float height = 2 * (normal.Sample(filter, input.uv.xy).a - .5f);
	float2 newUV = normalize(input.eyeTengent).xy;
	//height = 0;
	//newUV.y *= -1;
	//float2 offsetUV = input.uv.xy;
	float2 offsetUV = height * newUV * material.heightOffset + input.uv.xy;

	float3 nrmT = normal.Sample(filter, offsetUV).xyz;
	float4 specT = spec.Sample(filter, offsetUV);
	float4 texColor = tex.Sample(filter, offsetUV);
	nrmT = normalize((nrmT  - .5f)*2);

	float3 nrn = normalize(input.normal);
	float3 ten = normalize(input.tengent);
	float3 bin = normalize(input.binormal);

	float4 specularLight_d = float4(0, 0, 0, 0);
	float4 specularLight_p = float4(0, 0, 0, 0);
	float4 specularLight_s = float4(0, 0, 0, 0);

	float3x3 TBN = float3x3(ten, bin, nrn);

	float3 surfaceNormal = mul(nrmT, TBN);

	float4 ambientLight = float4((material.ambientColor * texColor.xyz).rgb,0);

	float3 toEye = normalize(input.toEye);

	float4 envColor = envMap.Sample(filter, reflect(-toEye, surfaceNormal));

	float4 diffuseLight_d;
	ComputeLight(lights[0], input.posWorld, toEye, surfaceNormal, diffuseLight_d, specularLight_d);
	float4 diffuseLight_p;
	ComputeLight(lights[1], input.posWorld, toEye, surfaceNormal, diffuseLight_p, specularLight_p);
	float4 diffuseLight_s;
	ComputeLight(lights[2], input.posWorld, toEye, surfaceNormal, diffuseLight_s, specularLight_s);

	float4 diffuseLight = (diffuseLight_d + diffuseLight_p + diffuseLight_s) * float4(material.diffuseColor,0);

	float4 specularLight = (specularLight_d + specularLight_p + specularLight_s)*specT;

	float f = pow((1 - saturate(dot(toEye, surfaceNormal))), material.fresnelPower) * material.fresnelIntensity;
	float4 fresnel = float4(f, f, f, 0);
	//return envColor;
	//return float4(diffuseLight_p.xyz,1);
	//return ambientLight + float4(((texColor * diffuseLight)).rgb, texColor.a);
	//return ambientLight + float4(((texColor * diffuseLight) + specularLight).rgb, texColor.a);
	return ambientLight + float4((((texColor * diffuseLight) + specularLight) * envColor).rgb, texColor.a) + fresnel ;
}