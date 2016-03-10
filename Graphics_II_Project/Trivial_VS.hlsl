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
	float3 uv					: TEXCOORD0;
	float3 toEye				: TEXCOORD1;
	float3 posWorld				: TEXCOORD2;
	float3 tengent				: TEXCOORD3;
	float3 binormal				: TEXCOORD4;
	float3 eyeTengent			: TEXCOORD5;
};

// TODO: PART 3 STEP 2a
cbuffer WVP : register( b0 )
{
	float4x4 viewInverse;
	float4x4 proj;
	float4x4 view;
};

cbuffer InstanceData : register(b1)
{
	float4x4 world[100];
}

OUTPUT main( INPUT input, uint id : SV_InstanceID )
{
	OUTPUT output;
	float4 coord = float4(input.pos, 1);
	float4 wv = mul(coord, world[id]);
	wv = mul(wv, viewInverse);
	output.pos = mul(wv,proj);
	float3x3 world3x3 = (float3x3)world[id];
	output.posWorld = mul(coord, world[id]).xyz;
	output.uv = input.uvw;

	float3 normal = normalize(input.normal);
	float3 tengent = normalize(input.tengent);
	float3 biTengent = (cross(normal,tengent));

	output.normal = (mul(normal, world3x3));
	output.tengent = (mul(tengent, world3x3));
	output.binormal = (cross(output.normal, output.tengent));

	output.toEye = normalize(view[3].xyz - output.posWorld);

	float3x3 toTengent = float3x3(tengent, biTengent, normal);

	output.eyeTengent = normalize(mul(output.toEye, toTengent));
	output.eyeTengent.y *= -1;
	return output;
}
