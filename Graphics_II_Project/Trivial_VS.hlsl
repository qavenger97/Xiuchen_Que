#pragma pack_matrix(row_major)

struct INPUT
{
	float3 pos		:		POSITION;
	float3 normal	:		TEXCOORD0;
	float3 uvw		:		TEXCOORD1;
	float3 tengent	:		TEXCOORD2;
};

struct OUTPUT
{
	float4 pos					: SV_POSITION;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD0;
	float3 posWorld				: TEXCOORD1;
	float3 tengent				: TEXCOORD2;
	float3 binormal				: TEXCOORD3;
};

// TODO: PART 3 STEP 2a
cbuffer WVP : register( b0 )
{
	float4x4 worldView;
	float4x4 world;
	float4x4 proj;
};

OUTPUT main( INPUT input )
{
	OUTPUT output;
	float4 coord = float4(input.pos, 1);
	float4 wv = mul(coord, worldView);
	output.pos = mul(wv,proj);
	float3x3 world3x3 = (float3x3)world;
	output.posWorld = mul(input.pos, world3x3);
	output.uv = input.uvw.xy;
	output.normal = normalize(mul(input.normal, world3x3));
	/*if (dot(output.normal, float3(0, 1, 0))) output.tengent = cross(output.normal, float3(1, 0, 0));
	else
		output.tengent =  cross(output.normal, float3(0, 1, 0));*/
	output.tengent = normalize(mul(input.tengent, world3x3));
	output.binormal = cross(output.normal, output.tengent);
	return output;
}

//cross(float3(1, 0, 0),output.normal);