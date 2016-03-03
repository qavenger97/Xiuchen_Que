#pragma pack_matrix(row_major)

struct INPUT
{
	float3 coordinate : POSITION;
	float3 normal :		TEXCOORD0;
	float2 uvw	  :		TEXCOORD1;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal			   : NORMAL;
	float2 uv				   : TEXCOORD0;
};

// TODO: PART 3 STEP 2a
cbuffer WVP : register( b0 )
{
	float4x4 worldViewProj;
};

OUTPUT main( INPUT input )
{
	OUTPUT output = (OUTPUT)0;
	float4 coord = float4(input.coordinate, 1);
	output.pos = mul(coord, worldViewProj);
	output.uv = input.uvw.xy;
	output.normal = input.normal;
	return output;
}