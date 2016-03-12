#include "Camera.h"
#include "Skybox.h"

Camera::Camera() : mouseX(0), mouseY(0)
{
	XMMATRIX view = XMMatrixIdentity();
	view.r[3] = XMVectorSet(0, 2, 1, 1);
	view = XMMatrixRotationX(0.5f) * XMMatrixRotationY(XM_PI) * view;
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
	float aspectRatio = width / height;
	XMStoreFloat4x4(&proj, XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ));
	BoundingFrustum::CreateFromMatrix(frustum, XMLoadFloat4x4(&proj));
	
	frustum.Transform(frustum, GetViewMatrixInverse());
}

void Camera::Update(float dt)
{
	if (GetAsyncKeyState(VK_SHIFT))dt *= 10;
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


	if (GetAsyncKeyState(VK_SPACE))
	{
		Fly(dt);
	}


	if (GetAsyncKeyState(VK_CONTROL))
	{
		Fly(-dt);
	}
	XMStoreFloat4(&frustum.Orientation, XMVector4Normalize(XMLoadFloat4(&frustum.Orientation)));
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
	if (btnState & VK_LBUTTON)
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


	pos = XMLoadFloat3(&frustum.Origin);
	pos += vel;
	XMStoreFloat3(&frustum.Origin, pos);
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


	pos = XMLoadFloat3(&frustum.Origin);
	pos += vel;
	XMStoreFloat3(&frustum.Origin, pos);
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

	pos = XMLoadFloat3(&frustum.Origin);
	pos += vel;
	XMStoreFloat3(&frustum.Origin, pos);
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


	XMVECTOR r = XMQuaternionRotationMatrix(rotY);
	XMVECTOR q = XMLoadFloat4(&frustum.Orientation);
	XMStoreFloat4(&frustum.Orientation, XMQuaternionMultiply(q, r));
}

void Camera::Pitch(float speed)
{
	XMMATRIX rotX = XMMatrixRotationX(speed);
	XMMATRIX trans = XMLoadFloat4x4(&transform);
	trans = rotX*trans;
	XMStoreFloat4x4(&transform, trans);


	XMVECTOR r = XMQuaternionRotationMatrix(rotX);
	XMVECTOR q = XMLoadFloat4(&frustum.Orientation);
	XMStoreFloat4(&frustum.Orientation, XMQuaternionMultiply(r, q));
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

XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMLoadFloat4x4(&proj);
}

XMMATRIX Camera::GetViewMatrix() const
{
	return XMMatrixInverse(0,(XMLoadFloat4x4(&transform)));
}

XMMATRIX Camera::GetViewProjectionMatrix() const
{
	return XMMatrixInverse(0, (XMLoadFloat4x4(&transform))) * XMLoadFloat4x4(&proj);
}

XMMATRIX Camera::GetViewMatrixInverse() const
{
	return XMLoadFloat4x4(&transform);
}

