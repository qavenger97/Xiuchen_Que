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
};

// TODO: PART 3 STEP 2a
cbuffer WVP : register( b0 )
{
	float4x4 worldView;
	float4x4 world;
	float4x4 proj;
	float4x4 view;
};

OUTPUT main( INPUT input )
{
	OUTPUT output;
	float4 coord = float4(input.pos, 1);
	float4 wv = mul(coord, worldView);
	output.pos = mul(wv,proj);
	float3x3 world3x3 = (float3x3)world;
	output.posWorld = mul(coord, world).xyz;
	output.uv = input.uvw;
	output.normal = normalize(mul(input.normal, world3x3));

	output.tengent = normalize(mul(input.tengent, world3x3));
	output.binormal = cross(output.tengent, output.normal);

	output.toEye = normalize(view[3].xyz - output.posWorld);
	return output;
}
