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
	float4x4	skyMatrix;
	float4x4	viewProj;
};

OUTPUT main(INPUT input)
{
	OUTPUT output;
	float4 pos = float4(input.pos, 1);
	output.pos = mul(mul(pos, skyMatrix), viewProj).xyww;

	//output.lookup = mul(pos, world).xyz;
	output.lookup = input.pos;

	return output;
}