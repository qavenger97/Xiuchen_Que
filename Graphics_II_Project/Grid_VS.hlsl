#pragma pack_matrix(row_major)
struct INPUT
{
	float4 pos : POSITION;
};

cbuffer WVP : register(b0)
{
	float4x4 skyMatrix;
	float4x4 wvp;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
};

OUTPUT main( INPUT input)
{
	OUTPUT output;
	output.pos = mul(input.pos, wvp);
	return output;
}