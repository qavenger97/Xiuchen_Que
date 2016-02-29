#pragma once
#include "define.h"
class Shaders
{
public:
	Shaders();
	void Create(ID3D11Device* gfx, const void* VS_Code, const void* PS_Code, SIZE_T VS_Length, SIZE_T PS_Length);
	void SetShader(ID3D11DeviceContext* gfx);
	~Shaders();
private:
	ID3D11VertexShader* pVS = nullptr;
	ID3D11PixelShader* pPS = nullptr;
	ID3D11InputLayout* pLayout = nullptr;
};

