#pragma once
#include "define.h"
#include <string>
class Skybox
{
public:
	Skybox(const wchar_t* filePath = L"");
	void Create(ID3D11Device* gfx);

	void Draw(ID3D11DeviceContext* gfx);

	~Skybox();
private:
	ID3D11Buffer* pVertexBuffer = 0;
	ID3D11Buffer* pIndexBuffer = 0;
	ID3D11ShaderResourceView* pSRV = 0;
	ID3D11RasterizerState* pState = 0;
	ID3D11InputLayout*	pLayout = 0;
	ID3D11VertexShader* pVS = 0;
	ID3D11PixelShader* pPS = 0;
	ID3D11DepthStencilState* pDSS = 0;
	UINT numVerts;
	UINT numIndices;
	std::wstring fileName;
};

