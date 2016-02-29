#pragma pack_matrix(row_major)
struct INPUT
{
	float3 pos : POSITION;
};

struct OUTPUT
{
	float4 pos	:	SV_POSITION;
	float3 lookup	:	POSITION;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4	viewProj;
};

OUTPUT main(INPUT input)
{
	OUTPUT output;
	output.pos = mul(float4(input.pos, 1), viewProj).xyww;
	output.lookup = input.pos;
	return output;
}