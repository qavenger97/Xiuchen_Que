#include "Skybox.h"
#include "DDSTextureLoader.h"
#include "skybox_VS.csh"
#include "skybox_PS.csh"

Skybox::Skybox(const wchar_t* filePath)
	:
	numVerts(24),numIndices(36),fileName(filePath)
{
}

void Skybox::Create(ID3D11Device * gfx)
{
	XMFLOAT3 v[] =
	{
		//Front Face
		XMFLOAT3(-.5f,-.5f,+.5f),
		XMFLOAT3(-.5f,+.5f,+.5f),
		XMFLOAT3(+.5f,+.5f,+.5f),
		XMFLOAT3(+.5f,-.5f,+.5f),
		//BackFace
		XMFLOAT3(-.5f,-.5f,-.5f),
		XMFLOAT3(-.5f,+.5f,-.5f),
		XMFLOAT3(+.5f,+.5f,-.5f),
		XMFLOAT3(+.5f,-.5f,-.5f),
		//TopFace
		XMFLOAT3(-.5f,+.5f,-.5f),
		XMFLOAT3(-.5f,+.5f,+.5f),
		XMFLOAT3(+.5f,+.5f,+.5f),
		XMFLOAT3(+.5f,+.5f,-.5f),
		//BottomFace
		XMFLOAT3(-.5f,-.5f,-.5f),
		XMFLOAT3(-.5f,-.5f,+.5f),
		XMFLOAT3(+.5f,-.5f,+.5f),
		XMFLOAT3(+.5f,-.5f,-.5f),
		//RightFace
		XMFLOAT3(+.5f,-.5f,-.5f),
		XMFLOAT3(+.5f,-.5f,+.5f),
		XMFLOAT3(+.5f,+.5f,+.5f),
		XMFLOAT3(+.5f,+.5f,-.5f),
		//LeftFace
		XMFLOAT3(-.5f,-.5f,-.5f),
		XMFLOAT3(-.5f,-.5f,+.5f),
		XMFLOAT3(-.5f,+.5f,+.5f),
		XMFLOAT3(-.5f,+.5f,-.5f)
	};

	UINT indices[] =
	{
		//FrontFace
		0,1,2,
		0,2,3,
		//BackFace
		4,6,5,
		4,7,6,
		//TopFace
		8,9,10,
		8,10,11,
		//BottomFace
		12,15,13,
		13,15,14,
		//RightFace
		16,17,18,
		16,18,19,
		//LeftFace
		20,23,21,
		21,23,22
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;

	bd.ByteWidth = sizeof(XMFLOAT3)*numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.pSysMem = v;
	HRESULT rs = gfx->CreateBuffer(&bd, &sd, &pVertexBuffer);
	if (rs != S_OK)
	{
		OutputDebugString(L"Create Triangle VertexBuffer Failed");
	}
	bd.ByteWidth = sizeof(UINT) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	sd.pSysMem = indices;
	rs = gfx->CreateBuffer(&bd, &sd, &pIndexBuffer);
	if (rs != S_OK)
	{
		OutputDebugString(L"Create Triangle IndexBuffer Failed");
	}

	CreateDDSTextureFromFile(gfx, fileName.c_str(), 0, &pSRV);

	D3D11_RASTERIZER_DESC rd = {};
	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;
	gfx->CreateRasterizerState(&rd, &pState);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0
	};

	gfx->CreateInputLayout(layout, ARRAYSIZE(layout), skybox_VS, sizeof(skybox_VS), &pLayout);

	gfx->CreateVertexShader(skybox_VS, sizeof(skybox_VS), nullptr, &pVS);
	gfx->CreatePixelShader(skybox_PS, sizeof(skybox_PS), nullptr, &pPS);

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	gfx->CreateDepthStencilState(&dsd, &pDSS);
}

void Skybox::Draw(ID3D11DeviceContext * gfx)
{
	UINT stride = sizeof XMFLOAT3;
	UINT offset = 0;
	gfx->OMSetDepthStencilState(pDSS, 0);
	gfx->RSSetState(pState);
	gfx->IASetInputLayout(pLayout);
	gfx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	gfx->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gfx->VSSetShader(pVS, 0, 0);
	gfx->PSSetShader(pPS, 0, 0);
	gfx->PSSetShaderResources(3, 1, &pSRV);
	gfx->DrawIndexed(numIndices, 0, 0);
	gfx->OMSetDepthStencilState(nullptr, 0);
}

Skybox::~Skybox()
{
	SAFE_RELEASE(pDSS);
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(pVS);
	SAFE_RELEASE(pLayout);
	SAFE_RELEASE(pSRV);
	SAFE_RELEASE(pState);
	SAFE_RELEASE(pIndexBuffer);
	SAFE_RELEASE(pVertexBuffer);
}
