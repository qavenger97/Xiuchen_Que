struct VS_OUT
{
	float4 pos: SV_POSITION;
	float2 uv : TEXCOORD0;
};

VS_OUT main( float4 pos : POSITION )
{
	VS_OUT output = (VS_OUT)0;
	output.pos = float4(pos.xy,0,1);
	output.uv = pos.zw;
	return output;
}