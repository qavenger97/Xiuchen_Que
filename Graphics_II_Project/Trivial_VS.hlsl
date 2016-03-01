#pragma pack_matrix(row_major)

struct INPUT
{
	float3 coordinate : POSITION;
	float3 uvw	  :		TEXCOORD0;
	float3 normal :		TEXCOORD1;
};

struct OUTPUT
{
	float4 projectedCoordinate : SV_POSITION;
	float4 color			   : COLOR;
	float2 uv				   : TEXCOORD0;
	float2 index			   : TEXCOORD1;
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
	output.projectedCoordinate = mul(coord, worldViewProj);
	output.uv = input.uvw.xy;

	output.color = (float4)1;
	return output;
}