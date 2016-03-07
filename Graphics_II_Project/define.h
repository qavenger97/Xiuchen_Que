#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Material.h"
using namespace DirectX;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if(x){x->Release(); x = nullptr;}
#endif // !SAFE_RELEASE(X) 

struct ConstantPerObject
{
	XMFLOAT4X4 viewInverse;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT4X4 view;
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 uv0;
	XMFLOAT3 tengent;
};

struct InstanceData
{
	XMFLOAT4X4 world;
};

struct Light
{
	XMFLOAT4 pos; // w = index;
	XMFLOAT4 dir; // 
	XMFLOAT4 color; // w = intensity;
	XMFLOAT4 att; // x = inner, y = outer, z = range, w = type: 0 == Directional Light, 1 = PointLight, 2 = SpotLight;
};

struct Material
{
	XMFLOAT3 ambientColor;
	FLOAT	 specularPower;
	XMFLOAT3 diffuseColor;
	FLOAT	 fresnelPower;
	XMFLOAT3 specularColor;
	FLOAT	 fresnelIntensity;
	FLOAT	 heightOffset;
	XMFLOAT3 pad;
};

struct LightBuffer
{
	Light light[3];
	Material material;
};

static float DegreeToRadian(float degree)
{
	return degree * 0.017453292f;
}

static float RadianToDegree(float radian)
{
	return radian * 57.29577951f;
}