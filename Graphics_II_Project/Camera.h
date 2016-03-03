#pragma once
#include "define.h"
class Skybox;
class Camera
{
private:
	XMFLOAT4X4 transform;
	XMFLOAT4X4 proj;
	WORD mouseX;
	WORD mouseY;
	Skybox* skybox = nullptr;
private:
	void Forward(float speed);
	void Strafe(float speed);
	void Fly(float speed);
	void Yaw(float speed);
	void Pitch(float speed);
public:
	Camera();
	~Camera();
	void SetCubemap(ID3D11Device* gfx, const wchar_t* filePath = L"");
	void SetProjection(float FOV, float width, float height, float nearZ, float farZ);
	void Update(float dt);
	void OnMouseDown(WPARAM btnState, WORD x, WORD y);
	void OnMouseUp(WPARAM btnState, WORD x, WORD y);
	void OnMouseMove(WPARAM btnState, WORD x, WORD y);
	void DrawSkybox(ID3D11DeviceContext* gfx);
	XMMATRIX GetPos()const;
	XMMATRIX GetProjectionMatrix()const;
	XMMATRIX GetViewMatrix()const;
	XMMATRIX GetViewProjectionMatrix()const;
	XMMATRIX GetViewMatrixInverse()const;
public:
};
