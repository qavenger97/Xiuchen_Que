#pragma pack_matrix(row_major)

struct GSINPUT
{
	float4 center : SV_POSITION;
	float3 extent : TEXCOORD0;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
};

cbuffer WVP : register(b0)
{
	float4x4 viewInverse;
	float4x4 proj;
	float4x4 view;
};

[maxvertexcount(16)]
void main(
	point GSINPUT input[1],
	inout LineStream< GSOutput > output
)
{
	float4 pos = input[0].center;
	float3 e = input[0].extent;

	float npeX = pos.x - e.x;
	float ppeX = pos.x + e.x;
	float npeY = pos.y - e.y;
	float ppeY = pos.y + e.y;
	float npeZ = pos.z - e.z;
	float ppeZ = pos.z + e.z;

	GSOutput verts[8] = {
		(GSOutput)0,
		(GSOutput)0,
		(GSOutput)0,
		(GSOutput)0,
		(GSOutput)0,
		(GSOutput)0,
		(GSOutput)0,
		(GSOutput)0
	};

	verts[0].pos.x = npeX;
	verts[0].pos.y = ppeY;
	verts[0].pos.z = npeZ;
	verts[0].pos.w = 1;

	verts[1].pos.x = npeX;
	verts[1].pos.y = ppeY;
	verts[1].pos.z = ppeZ;
	verts[1].pos.w = 1;

	verts[2].pos.x = ppeX;
	verts[2].pos.y = ppeY;
	verts[2].pos.z = ppeZ;
	verts[2].pos.w = 1;

	verts[3].pos.x = ppeX;
	verts[3].pos.y = ppeY;
	verts[3].pos.z = npeZ;
	verts[3].pos.w = 1;

	verts[4].pos.x = npeX;
	verts[4].pos.y = npeY;
	verts[4].pos.z = npeZ;
	verts[4].pos.w = 1;

	verts[5].pos.x = npeX;
	verts[5].pos.y = npeY;
	verts[5].pos.z = ppeZ;
	verts[5].pos.w = 1;

	verts[6].pos.x = ppeX;
	verts[6].pos.y = npeY;
	verts[6].pos.z = ppeZ;
	verts[6].pos.w = 1;

	verts[7].pos.x = ppeX;
	verts[7].pos.y = npeY;
	verts[7].pos.z = npeZ;
	verts[7].pos.w = 1;

	for (uint i = 0; i < 8; i++)
	{
		float4 wvp = mul(verts[i].pos, viewInverse);
		verts[i].pos = mul(wvp, proj);
	}

	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
	output.Append(verts[3]);
	output.Append(verts[0]);
	output.Append(verts[4]);
	output.Append(verts[5]);
	output.Append(verts[6]);
	output.Append(verts[7]);
	output.Append(verts[4]);
	output.RestartStrip();
	output.Append(verts[1]);
	output.Append(verts[5]);
	output.RestartStrip();
	output.Append(verts[2]);
	output.Append(verts[6]);
	output.RestartStrip();
	output.Append(verts[3]);
	output.Append(verts[7]);
}