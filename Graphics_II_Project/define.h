#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if(x){x->Release(); x = nullptr;}
#endif // !SAFE_RELEASE(X) 


struct ConstantPerObject
{
	XMFLOAT4X4 worldMatrix;
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct Vertex_m
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv0;
};

static float DegreeToRadian(float degree)
{
	return degree * 0.017453292f;
}

static float RadianToDegree(float radian)
{
	return radian * 57.29577951f;
}