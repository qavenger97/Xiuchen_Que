texture2D tex : register(t0);
SamplerState filter : register(s0);

struct INPUT
{
	float4 projectedCoordinate : SV_POSITION;
	float4 color			   : COLOR;
	float2 uv				   : TEXCOORD0;
	float2 index			   : TEXCOORD1;
};

float4 main( INPUT input ) : SV_TARGET
{
	return tex.Sample(filter, input.uv);
}