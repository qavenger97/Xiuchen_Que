struct GSINPUT
{
	float4 pos : SV_POSITION;
	float3 extent : TEXCOORD0;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(
	point GSINPUT input[1],
	inout LineStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = (input[0].pos);
		output.Append(element);
	}
}