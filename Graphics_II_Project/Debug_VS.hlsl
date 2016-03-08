#pragma pack_matrix(row_major)

struct INPUT
{
	float3 center		: POSITION;
	float3 extent		: TEXCOORD0;
};

cbuffer WVP : register(b0)
{
	float4x4 viewInverse;
	float4x4 proj;
	float4x4 view;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float3 extent : TEXCOORD0;
};

OUTPUT main( INPUT input )
{
	OUTPUT output;
	output.pos = float4(input.center,1);
	return output;
}