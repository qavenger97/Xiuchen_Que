#include "Camera.h"
#include "Skybox.h"

Camera::Camera() : mouseX(0), mouseY(0)
{
	XMMATRIX view = XMMatrixIdentity();
	view.r[3] = XMVectorSet(0, 0, -1, 1);
	XMMATRIX projMatrix = XMLoadFloat4x4(&proj);
	XMStoreFloat4x4(&viewProj, XMMatrixInverse(nullptr, view) * projMatrix);
	XMStoreFloat4x4(&transform, view);
}

Camera::~Camera()
{
	if (skybox)
		delete skybox;
}


void Camera::SetCubemap(ID3D11Device* gfx, const wchar_t * filePath)
{
	if (skybox) { delete skybox; skybox = nullptr; }
	skybox = new Skybox(filePath);
	skybox->Create(gfx);
}

void Camera::SetProjection(float FOV, float width, float height, float nearZ, float farZ)
{
	XMStoreFloat4x4(&proj, XMMatrixPerspectiveFovLH(FOV, width / height, nearZ, farZ));
}

void Camera::Update(float dt)
{
	if (GetAsyncKeyState('W'))
	{
		Forward(dt);
	}

	if (GetAsyncKeyState('S'))
	{
		Forward(-dt);
	}

	if (GetAsyncKeyState('A'))
	{
		Strafe(-dt);
	}

	if (GetAsyncKeyState('D'))
	{
		Strafe(dt);
	}

	if (GetAsyncKeyState('F'))
	{
		Fly(dt);
	}

	if (GetAsyncKeyState('C'))
	{
		Fly(-dt);
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		Yaw(dt);
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		Yaw(-dt);
	}

	if (GetAsyncKeyState(VK_UP))
	{
		Pitch(dt);
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		Pitch(-dt);
	}

	XMMATRIX trans = XMLoadFloat4x4(&transform);
	XMMATRIX projection = XMLoadFloat4x4(&proj);
	XMStoreFloat4x4(&viewProj, XMMatrixInverse(nullptr, trans) * projection);
}

void Camera::OnMouseDown(WPARAM btnState, WORD x, WORD y)
{
	mouseX = x;
	mouseY = y;
}

void Camera::OnMouseUp(WPARAM btnState, WORD x, WORD y)
{
	mouseX = x;
	mouseY = y;
}

void Camera::OnMouseMove(WPARAM btnState, WORD x, WORD y)
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

void Camera::Forward(float speed)
{
	XMVECTOR vel = XMVectorSet(0, 0, speed, 1);
	XMMATRIX mat = XMLoadFloat4x4(&transform);
	XMVECTOR pos = mat.r[3];

	mat.r[3] = XMVectorSet(0, 0, 0, 1);
	vel = XMVector3Transform(vel, mat);
	vel.m128_f32[3] = 0;
	mat.r[3] = pos + vel;
	XMStoreFloat4x4(&transform, mat);
}

void Camera::Strafe(float speed)
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

void Camera::Fly(float speed)
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

void Camera::Yaw(float speed)
{
	XMMATRIX rotY = XMMatrixRotationY(speed);
	XMMATRIX trans = XMLoadFloat4x4(&transform);
	XMVECTOR pos = trans.r[3];
	trans.r[3] = XMVectorSet(0, 0, 0, 1);
	trans = trans*rotY;
	trans.r[3] = pos;
	XMStoreFloat4x4(&transform, trans);
}

void Camera::Pitch(float speed)
{
	XMMATRIX rotX = XMMatrixRotationX(speed);
	XMMATRIX trans = XMLoadFloat4x4(&transform);
	trans = rotX*trans;
	XMStoreFloat4x4(&transform, trans);
}

void Camera::DrawSkybox(ID3D11DeviceContext * gfx)
{
	skybox->Draw(gfx);
}

XMMATRIX Camera::GetPos() const
{
	XMMATRIX trans = XMLoadFloat4x4(&transform);
	XMMATRIX pos = XMMatrixTranslationFromVector(trans.r[3]);
	return pos;
}
