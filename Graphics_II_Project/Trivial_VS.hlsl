#pragma pack_matrix(row_major)

struct INPUT
{
	float3 pos : POSITION;
	float3 normal :		TEXCOORD0;
	float2 uvw	  :		TEXCOORD1;
};

struct OUTPUT
{
	float4 pos					: SV_POSITION;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD0;
	float3 posView				: TEXCOORD1;
};

// TODO: PART 3 STEP 2a
cbuffer WVP : register( b0 )
{
	float4x4 worldView;
	float4x4 proj;
};

OUTPUT main( INPUT input )
{
	OUTPUT output;
	float4 coord = float4(input.pos, 1);
	float4 wv = mul(coord, worldView);
	output.pos = mul(wv,proj);
	output.posView = input.pos;
	output.uv = input.uvw.xy;
	output.normal = input.normal;
	return output;
}