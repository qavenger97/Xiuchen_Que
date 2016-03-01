#pragma once
#include "Math.h"
#include <DirectXMath.h>
#include "XTime.h"
using namespace DirectX;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(X) if(x){x->Release(); x = nullptr;}
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

XTime timer;

XMFLOAT4X4 proj;
XMFLOAT4X4 viewProj;

static float DegreeToRadian(float degree)
{
	return degree * 0.017453292f;
}

static float RadianToDegree(float radian)
{
	return radian * 57.29577951f;
}

struct Camera
{
private:
	XMFLOAT4X4 transform;
	WORD mouseX;
	WORD mouseY;


	void Forward(float speed)
	{
		XMVECTOR vel = XMVectorSet(0,0,speed,1);
		XMMATRIX mat = XMLoadFloat4x4(&transform);
		XMVECTOR pos = mat.r[3];

		mat.r[3] = XMVectorSet(0, 0, 0, 1);
		vel = XMVector3Transform(vel, mat);
		vel.m128_f32[3] = 0;
		mat.r[3] = pos + vel;
		XMStoreFloat4x4(&transform, mat);
	}

	void Strafe(float speed)
	{
		XMVECTOR vel = XMVectorSet(speed, 0, 0, 1);
		XMMATRIX mat = XMLoadFloat4x4(&transform);
		XMVECTOR pos = mat.r[3];

		mat.r[3] = XMVectorSet(0, 0, 0, 1);
		vel = XMVector3Transform(vel, mat);
		vel.m128_f32[3] = 0;
		mat.r[3] = pos + vel;
		XMStoreFloat4x4(&transform, mat);
	}

	void Fly(float speed)
	{
		XMVECTOR vel = XMVectorSet(0, speed, 0, 1);
		XMMATRIX mat = XMLoadFloat4x4(&transform);
		XMVECTOR pos = mat.r[3];

		mat.r[3] = XMVectorSet(0, 0, 0, 1);
		vel = XMVector3Transform(vel, mat);
		vel.m128_f32[3] = 0;
		mat.r[3] = pos + vel;
		XMStoreFloat4x4(&transform, mat);
	}

	void Yaw(float speed)
	{
		XMMATRIX rotY = XMMatrixRotationY(speed);
		XMMATRIX trans = XMLoadFloat4x4(&transform);
		XMVECTOR pos = trans.r[3];
		trans.r[3] = XMVectorSet(0, 0, 0, 1);
		trans = trans*rotY;
		trans.r[3] = pos;
		XMStoreFloat4x4(&transform, trans);
	}

	void Pitch(float speed)
	{
		XMMATRIX rotX = XMMatrixRotationX(speed);
		XMMATRIX trans = XMLoadFloat4x4(&transform);
		trans = rotX*trans;
		XMStoreFloat4x4(&transform, trans);
	}
public:	
	Camera() : mouseX(0), mouseY(0)
	{
		XMMATRIX view = XMMatrixIdentity();
		view.r[3] = XMVectorSet(0, 0, -1, 1);
		XMMATRIX projMatrix = XMLoadFloat4x4(&proj);
		XMStoreFloat4x4(&viewProj, XMMatrixInverse(nullptr, view) * projMatrix);
		XMStoreFloat4x4(&transform, view);
	}
	void Update()
	{
		if (GetAsyncKeyState('W'))
		{
			Forward((float)timer.Delta());
		}

		if (GetAsyncKeyState('S'))
		{
			Forward(-(float)timer.Delta());
		}

		if (GetAsyncKeyState('A'))
		{
			Strafe(-(float)timer.Delta());
		}

		if (GetAsyncKeyState('D'))
		{
			Strafe((float)timer.Delta());
		}

		if (GetAsyncKeyState('F'))
		{
			Fly((float)timer.Delta());
		}

		if (GetAsyncKeyState('C'))
		{
			Fly(-(float)timer.Delta());
		}

		if (GetAsyncKeyState(VK_LEFT))
		{
			Yaw((float)timer.Delta());
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			Yaw(-(float)timer.Delta());
		}

		if (GetAsyncKeyState(VK_UP))
		{
			Pitch((float)timer.Delta());
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			Pitch(-(float)timer.Delta());
		}

		XMMATRIX trans = XMLoadFloat4x4(&transform);
		XMMATRIX projection = XMLoadFloat4x4(&proj);
		XMStoreFloat4x4(&viewProj, XMMatrixInverse(nullptr, trans) * projection);
	}
	void OnMouseDown(WPARAM btnState, WORD x, WORD y)
	{
		mouseX = x;
		mouseY = y;
	}
	void OnMouseUp(WPARAM btnState, WORD x, WORD y)
	{
		mouseX = x;
		mouseY = y;
	}
	void OnMouseMove(WPARAM btnState, WORD x, WORD y)
	{
		if (btnState && MK_LBUTTON)
		{
			float dx = DegreeToRadian(x - (float)mouseX);
			float dy = DegreeToRadian(y - (float)mouseY);
			Yaw(dx);
			Pitch(dy);
		}
		mouseX = x;
		mouseY = y;
	}
};