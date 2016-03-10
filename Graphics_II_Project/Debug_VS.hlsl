#pragma pack_matrix(row_major)

struct INPUT
{
	float3 center		: POSITION;
	float3 extent		: TEXCOORD0;
};

struct OUTPUT
{
	float4 center : SV_POSITION;
	float3 extent : TEXCOORD0;
};

OUTPUT main( INPUT input )
{
	OUTPUT output;
	output.center = float4(input.center, 1);
	output.extent = input.extent;
	return output;
}