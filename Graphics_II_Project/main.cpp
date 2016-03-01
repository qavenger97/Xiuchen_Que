// D3D LAB 1a "Line Land".
// Author: L.Norri CD DRX, FullSail University

// Introduction:
// Welcome to the first lab of the Direct3D Graphics Programming class.
// This is the ONLY guided lab in this course! 
// Future labs will demand the habits & foundation you develop right now!  
// It is CRITICAL that you follow each and every step. ESPECIALLY THE READING!!!

// TO BEGIN: Open the word document that acompanies this lab and start from the top.

//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************

#include <iostream>
#include <ctime>
#include "define.h"
using namespace std;

// BEGIN PART 1
// TODO: PART 1 STEP 1a
#include <d3d11.h>
//#include <d3dcompiler.h>

// TODO: PART 1 STEP 1b
// TODO: PART 2 STEP 6
#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include "Grid_VS.csh"
#include "Grid_PS.csh"
#include "DDSTextureLoader.h"
#define BACKBUFFER_WIDTH	500
#define BACKBUFFER_HEIGHT	500
#define NUM_VERT 16
//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************
Camera camera;

class ObjectBase
{
public:
	ObjectBase(){}
	ObjectBase(int numVerts) :numVerts(numVerts)
	{
		v = new Vertex[numVerts];
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&transform, I);
	}

	void Create(ID3D11Device* gfx)
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_IMMUTABLE;

		bd.ByteWidth = sizeof(Vertex)*numVerts;
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

	}

	void Draw(ID3D11DeviceContext* gfx)
	{
		UINT stride = sizeof Vertex;
		UINT offset = 0;

		gfx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		gfx->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		gfx->DrawIndexed(numIndices, 0, 0);

	}
	virtual ~ObjectBase()
	{
		if(pIndexBuffer)pIndexBuffer->Release();
		if(pVertexBuffer)pVertexBuffer->Release();
		delete[] indices;
		delete[] v;
	}
public:
	XMFLOAT4X4 transform;
protected:
	Vertex* v = 0;
	UINT* indices = 0;
	ID3D11Buffer* pVertexBuffer = 0;
	ID3D11Buffer* pIndexBuffer= 0;
	UINT numVerts;
	UINT numIndices;
};

class Star : public ObjectBase
{
public:
	Star(float thickness = 0.4f, float radiusOut = 1, float radiusIn = 0.5f) :ObjectBase(22)
	{
		float da = 0.1f;
		const float PI_2 = 6.28318531f;
		float halfthickness = thickness*0.5f;
		numIndices = 40 * 3;
		indices = new UINT[numIndices];
		v[0].pos.x = 0;
		v[0].pos.y = 0;
		v[0].pos.z = halfthickness;
		v[0].color.x = 0.2f;
		v[0].color.y = 0.4f;
		v[0].color.z = 1;
		v[11].pos.x = 0;
		v[11].pos.y = 0;
		v[11].pos.z = -halfthickness;
		v[11].color.x = 1;
		v[11].color.y = 0.4f;
		v[11].color.z = 0.2f;

		for (int c = 0; c < 5; c++)
		{
			int i = (c << 1) | 1;
			float angle1 = da*PI_2*(c<<1);
			float angle2 = da*PI_2*i;
			v[i    ].pos.x = cosf(angle1) * radiusOut;
			v[i    ].pos.y = sinf(angle1) * radiusOut;
			v[i    ].pos.z = halfthickness;

			v[i + 1].pos.x = cosf(angle2) * radiusIn;
			v[i + 1].pos.y = sinf(angle2) * radiusIn;
			v[i + 1].pos.z = halfthickness;

			v[i + 11].pos.x = cosf(angle1) * radiusOut;
			v[i + 11].pos.y = sinf(angle1) * radiusOut;
			v[i + 11].pos.z = -halfthickness;

			v[i + 12].pos.x = cosf(angle2) * radiusIn;
			v[i + 12].pos.y = sinf(angle2) * radiusIn;
			v[i + 12].pos.z = -halfthickness;

			float color = (1) * 0.2f;

			v[i].color.x = color;
			v[i + 1].color.x = color;
			v[i + 11].color.x = color;
			v[i + 12].color.x = color;

			color = (2) * 0.2f;
			v[i].color.y = color;
			v[i + 1].color.y = color;
			v[i + 11].color.y = color;
			v[i + 12].color.y = color;

			color = (3) * 0.2f;
			v[i].color.z = color;
			v[i + 1].color.z = color;
			v[i + 11].color.z = color;
			v[i + 12].color.z = color;
		}

		for (UINT c = 0; c < 10; c++)
		{
			UINT i = c * 3;
			indices[i    ] = 0;
			indices[i + 1] = c + 1;
			indices[i + 2] = c + 2;

			UINT i2 = (c + 10) * 3;
			indices[i2    ] = 11;
			indices[i2 + 1] = c + 13;
			indices[i2 + 2] = c + 12;

			UINT i3 = 59 + c*6;
			indices[i3 + 1] = c + 1;
			indices[i3 + 2] = c + 12;
			indices[i3 + 3] = (c + 2) == 11? 1 : c+2;
			indices[i3 + 4] = c + 12;
			indices[i3 + 5] = (c + 13) < numVerts? c+13: 12;
			indices[i3 + 6] = (c + 2) == 11 ? 1 : c + 2;
		}
		indices[29] = 1;
		indices[58] = 12;

	}
};

class Cube
{
public:
	Cube()
	{
		numVerts = 8;
		numIndices = 36;
	}
	void Create(ID3D11Device* gfx)
	{
		XMFLOAT3 v[] =
		{
			XMFLOAT3(-1,-1,-1),
			XMFLOAT3(-1,+1,-1),
			XMFLOAT3(+1,+1,-1),
			XMFLOAT3(+1,-1,-1),
			XMFLOAT3(-1,-1,+1),
			XMFLOAT3(-1,+1,+1),
			XMFLOAT3(+1,+1,+1),
			XMFLOAT3(+1,-1,+1)
		};

		UINT indices[] =
		{
			0,1,2,
			0,2,3,
			4,6,5,
			4,7,6,
			4,5,1,
			4,1,0,
			3,2,6,
			3,6,7,
			1,5,6,
			1,6,2,
			4,0,3,
			4,3,7
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

	}

	void Draw(ID3D11DeviceContext* gfx)
	{
		UINT stride = sizeof XMFLOAT3;
		UINT offset = 0;
		
		
		gfx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		gfx->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		gfx->DrawIndexed(numIndices, 0, 0);

	}

	~Cube()
	{
		if (pSRV)pSRV->Release();
		if (pIndexBuffer)pIndexBuffer->Release();
		if (pVertexBuffer)pVertexBuffer->Release();
	}
private:
	ID3D11Buffer* pVertexBuffer = 0;
	ID3D11Buffer* pIndexBuffer = 0;
	ID3D11ShaderResourceView* pSRV = 0;
	UINT numVerts;
	UINT numIndices;
};

struct Grid
{
	XMFLOAT4 v[84];
	UINT index[84];
	ID3D11Buffer* pVertexBuffer = 0;
	ID3D11Buffer* pIndexBuffer = 0;
	ID3D11Buffer* pConstantBuffer = 0;
	ID3D11InputLayout* pLayout = 0;
	ID3D11VertexShader* vs = 0;
	ID3D11PixelShader* ps = 0;
	ID3D11RasterizerState* pRasterizerState = 0;
	XMFLOAT4X4 transform;
	bool bMSAA = false;
	Grid()
	{
		XMStoreFloat4x4(&transform, XMMatrixIdentity());
		transform.m[0][0] = transform.m[1][1] = transform.m[2][2] = 4;
		transform.m[3][3] = 1;
		for (int i = 0; i < 21; i++)
		{
			float r = 1.0f / 20;
			UINT vvB = i + 21;
			UINT vhL = i + 42;
			UINT vhR = i + 63;

			v[vvB].x = v[i].x = -.5f + r*i;
			v[vvB].z = -.5f;
			v[i].z = .5f;

			v[vhL].x = -.5f;
			v[vhR].x = .5f;
			v[vhR].z = v[vhL].z = -.5f + r*i;

			v[i].y = v[vvB].y = v[vhR].y = v[vhL].y = 0;
			UINT l = i << 2;
			index[l] = i;
			index[l+1] = vvB;
			index[l+2] = vhL;
			index[l+3] = vhR;
		}
	}	
	void Create(ID3D11Device* gfx)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(XMFLOAT4) * 84;
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = v;
		gfx->CreateBuffer(&bd, &sd, &pVertexBuffer);

		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(UINT) * 84;
		sd.pSysMem = index;
		gfx->CreateBuffer(&bd, &sd, &pIndexBuffer);

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(XMFLOAT4X4);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		gfx->CreateBuffer(&bd, 0, &pConstantBuffer);

		gfx->CreateVertexShader(Grid_VS,ARRAYSIZE(Grid_VS) ,0 , &vs);
		gfx->CreatePixelShader(Grid_PS, ARRAYSIZE(Grid_PS), 0, &ps);
		D3D11_RASTERIZER_DESC rasterDesc = {};
		rasterDesc.AntialiasedLineEnable = true;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		gfx->CreateRasterizerState(&rasterDesc, &pRasterizerState);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0
		};
		gfx->CreateInputLayout(layout, ARRAYSIZE(layout), Grid_VS, ARRAYSIZE(Grid_VS), &pLayout);
	}
	~Grid()
	{
		if (pRasterizerState)pRasterizerState->Release();
		if (ps)ps->Release();
		if (vs)vs->Release();
		if (pLayout)pLayout->Release();
		if (pConstantBuffer)pConstantBuffer->Release();
		if (pVertexBuffer)pVertexBuffer->Release();
		if (pIndexBuffer)pIndexBuffer->Release();
	}
	void Draw(ID3D11DeviceContext* gfx)
	{
		UINT stride = sizeof XMFLOAT4;
		UINT offset = 0;

		gfx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		gfx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		gfx->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		gfx->VSSetShader(vs, 0, 0);

		D3D11_MAPPED_SUBRESOURCE mr = {};

		XMMATRIX world = XMLoadFloat4x4(&transform) * XMLoadFloat4x4(&viewProj);

		gfx->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
		memcpy(mr.pData, &world, sizeof(XMFLOAT4X4));
		gfx->Unmap(pConstantBuffer, 0);
		gfx->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		gfx->PSSetShader(ps, 0, 0);
		gfx->IASetInputLayout(pLayout);
		gfx->RSSetState(bMSAA ? pRasterizerState : nullptr);
		gfx->DrawIndexed(84, 0, 0);
	}
	
	void ToogleState(ID3D11DeviceContext* gfx)
	{
		bMSAA = !bMSAA;
	}
};

class DEMO_APP
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	HRESULT rs;
	ID3D11Device* pDevice = 0;
	ID3D11DeviceContext* pDeviceContext = 0;
	IDXGISwapChain* pSwapChain = 0;
	ID3D11RenderTargetView* pRTV = 0;
	ID3D11DepthStencilView* pDSV = 0;
	D3D11_VIEWPORT viewPort;

	ID3D11Buffer* pVertexBuffer = 0;
	ID3D11Buffer* pIndexBuffer = 0;
	ID3D11Buffer* pConstantBuffer = 0;
	ID3D11Buffer* pDepthBuffer = 0;
	
	ID3D11RasterizerState* pRasterizerState = 0;
	ID3D11RasterizerState* pRasterizerState_back = 0;
	ID3D11BlendState* pOverlay = 0;
	ID3D11SamplerState* pSamplarState = 0;

	ID3D11InputLayout* pLayout = 0;
	ID3D11VertexShader* vs = 0;
	ID3D11PixelShader* ps = 0;

	ID3D11ShaderResourceView* pSRV = 0;
	ID3D11ShaderResourceView* pSRV1 = 0;

	ConstantPerObject cb;

	float numPic = 0;

	Grid grid;
	Star star;
	Cube cube;
	// BEGIN PART 5
	// TODO: PART 5 STEP 1
	// TODO: PART 2 STEP 4
	
	// BEGIN PART 3
	// TODO: PART 3 STEP 1

	// TODO: PART 3 STEP 2b
	
	// TODO: PART 3 STEP 4a
	float nearPlane = 0.1f;
	float farPlane = 1000;
	float FOV = 1.57079633f;
public:
	// BEGIN PART 2
	// TODO: PART 2 STEP 1
	
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
	void Resize(WORD width, WORD height);
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP* g_myApp;
DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{
	// ****************** BEGIN WARNING ***********************// 
	// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY! 
	if (!g_myApp)g_myApp = this;
	application = hinst;
	appWndProc = proc;
	timer.Restart();
	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectXApplication", L"Lab 1a Line Land", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);
	//********************* END WARNING ************************//
	{
		// TODO: PART 1 STEP 3a
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.OutputWindow = window;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferDesc.Width = BACKBUFFER_WIDTH;
		sd.BufferDesc.Height = BACKBUFFER_HEIGHT;
		sd.SampleDesc.Count = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		XMStoreFloat4x4(&proj,XMMatrixPerspectiveFovLH(FOV, BACKBUFFER_HEIGHT / (float)BACKBUFFER_WIDTH, nearPlane, farPlane));
		UINT flag = 0;
#ifdef _DEBUG
		flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT rs = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, flag, 0, 0, D3D11_SDK_VERSION,
			&sd, &pSwapChain, &pDevice, 0, &pDeviceContext);
		if (rs != S_OK)
		{
			OutputDebugString(L"Craete swapchain and device failed\n");
		}

		// TODO: PART 1 STEP 4
		ID3D11Texture2D* backBuffer;
		rs = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		if (rs != S_OK)
		{
			OutputDebugString(L"Get Backbuffer failed\n");
		}
		// TODO: PART 1 STEP 5
		ZeroMemory(&viewPort, sizeof(viewPort));
		viewPort.Height = BACKBUFFER_HEIGHT;
		viewPort.Width = BACKBUFFER_WIDTH;
		viewPort.MaxDepth = 1;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.CPUAccessFlags = 0;
		bd.BindFlags = D3D11_BIND_RENDER_TARGET;

		rs = pDevice->CreateRenderTargetView(backBuffer, 0, &pRTV);
		if (rs != S_OK)
		{
			OutputDebugString(L"Create RTV failed\n");
		}

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(cb);
		rs = pDevice->CreateBuffer(&bd, 0, &pConstantBuffer);
		if (rs != S_OK)
		{
			OutputDebugString(L"Create Triangle ConstantBuffer Failed");
		}

		backBuffer->Release();
	}

	ID3D11Texture2D* depthBuffer = 0;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.Height = BACKBUFFER_HEIGHT;
	td.Width = BACKBUFFER_WIDTH;
	td.ArraySize = 1;
	td.MipLevels = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.SampleDesc.Count = 1;
	rs = pDevice->CreateTexture2D(&td, 0, &depthBuffer);
	if (rs != S_OK)
	{
		OutputDebugString(L"Create Depth Buffer failed\n");
	}

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	pDevice->CreateRasterizerState(&rasterDesc, &pRasterizerState);

	rasterDesc.CullMode = D3D11_CULL_FRONT;
	pDevice->CreateRasterizerState(&rasterDesc, &pRasterizerState_back);

	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(svd));
	svd.Format = DXGI_FORMAT_D32_FLOAT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	rs = pDevice->CreateDepthStencilView(depthBuffer, &svd, &pDSV);
	if (rs != S_OK)
	{
		OutputDebugString(L"Create Depth Stencil View failed\n");
	}
	if (depthBuffer) depthBuffer->Release();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0,
		"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0,
		"TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0
	};


	rs = pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), Trivial_VS, ARRAYSIZE(Trivial_VS), &pLayout);
	if (rs != S_OK)
	{
		OutputDebugString(L"Create Vertex Shader InputLayout failed\n");
	}

	rs = pDevice->CreateVertexShader(Trivial_VS, ARRAYSIZE(Trivial_VS), 0, &vs);
	if (rs != S_OK)
	{
		OutputDebugString(L"Create Vertex Shader failed\n");
	}
	rs = pDevice->CreatePixelShader(Trivial_PS, ARRAYSIZE(Trivial_PS), 0, &ps);
	if (rs != S_OK)
	{
		OutputDebugString(L"Create Pixel Shader failed\n");
	}

	D3D11_BLEND_DESC blendDesc = {};

	blendDesc.RenderTarget[0].BlendEnable = true;

	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	pDevice->CreateBlendState(&blendDesc, &pOverlay);

	CreateDDSTextureFromFile(pDevice, L"chest_base.dds", nullptr, &pSRV);
	CreateDDSTextureFromFile(pDevice, L"numbers_test.dds", nullptr, &pSRV1);

	{
		D3D11_SAMPLER_DESC spd = {};
		spd.Filter = D3D11_FILTER_ANISOTROPIC;
		spd.MaxAnisotropy = 4;
		spd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		spd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		spd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		pDevice->CreateSamplerState(&spd, &pSamplarState);
	}
	
	float factor[] = { 1,1,1,1 };
	pDeviceContext->OMSetBlendState(pOverlay, factor, 0xffffffff);
	pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
	pDeviceContext->RSSetViewports(1, &viewPort);
	pDeviceContext->PSSetShaderResources(0, 1, &pSRV1);
	pDeviceContext->PSSetSamplers(0, 1, &pSamplarState);
	grid.Create(pDevice);
	cube.Create(pDevice);
	camera.Update();
	star.Create(pDevice);
}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	timer.Signal();
	numPic += (float)timer.Delta();
	if (numPic >= 4)numPic = 0;
	camera.Update();
	static const float clearColor[] = { 0.1f, 0.1f, 1.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRTV, clearColor);
	pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->IASetInputLayout(pLayout);
	pDeviceContext->VSSetShader(vs, 0, 0);
	pDeviceContext->PSSetShader(ps, 0, 0);

	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(ms));

	XMStoreFloat4x4(&cb.worldMatrix, XMLoadFloat4x4(&viewProj));
	
	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &cb, sizeof(cb));
	pDeviceContext->Unmap(pConstantBuffer, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

	pDeviceContext->RSSetState(pRasterizerState_back);
	cube.Draw(pDeviceContext);


	static float angle = 0;
	angle += 0.0001f;
	XMMATRIX trans = XMMatrixRotationY(angle) * XMLoadFloat4x4(&viewProj);
	XMStoreFloat4x4(&cb.worldMatrix, trans);


	if (GetAsyncKeyState('1'))
		pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	if (GetAsyncKeyState('2'))
		pDeviceContext->PSSetShaderResources(0, 1, &pSRV1);


	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &cb, sizeof(cb));
	pDeviceContext->Unmap(pConstantBuffer, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);


	if (GetAsyncKeyState('3') & 0x01)
	{
		grid.ToogleState(pDeviceContext);
	}

	pDeviceContext->RSSetState(pRasterizerState);
	star.Draw(pDeviceContext);
	grid.Draw(pDeviceContext);
	pSwapChain->Present(0, 0);
	return true; 
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{
	if (pRasterizerState_back)pRasterizerState_back->Release();
	if (pSamplarState)pSamplarState->Release();
	if (pSRV1)pSRV1->Release();
	if (pSRV)pSRV->Release();
	if (pLayout)pLayout->Release();
	if (ps)ps->Release();
	if (vs)vs->Release();
	if (pOverlay)pOverlay->Release();
	if (pRasterizerState)pRasterizerState->Release();
	if (pDepthBuffer)pDepthBuffer->Release();
	if (pConstantBuffer)pConstantBuffer->Release();
	if (pIndexBuffer)pIndexBuffer->Release();
	if (pVertexBuffer) pVertexBuffer->Release();
	if (pDSV)pDSV->Release();
	if (pRTV)pRTV->Release();
	if (pSwapChain)pSwapChain->Release();
	if (pDeviceContext)pDeviceContext->Release();
	if (pDevice)pDevice->Release();
	UnregisterClass( L"DirectXApplication", application ); 
	return true;
}

void DEMO_APP::Resize(WORD width, WORD height)
{
	if (pDevice)
	{
		viewPort.Height = height;
		viewPort.Width = width;
		if (pDSV)pDSV->Release();
		if (pRTV)pRTV->Release();
		if (pSwapChain)pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

		pDevice->CreateRenderTargetView(pBackBuffer, 0, &pRTV);

		if (pBackBuffer)pBackBuffer->Release();

		D3D11_TEXTURE2D_DESC td = {};
		td.ArraySize = 1;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.Height = height;
		td.Width = width;
		td.Format = DXGI_FORMAT_D32_FLOAT;
		td.MipLevels = 1;
		td.SampleDesc.Count = 1;
		td.Usage = D3D11_USAGE_DEFAULT;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		ID3D11Texture2D* depthBuffer;
		pDevice->CreateTexture2D(&td, 0, &depthBuffer);
		pDevice->CreateDepthStencilView(depthBuffer, &dsvd, &pDSV);
		depthBuffer->Release();
		pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
		pDeviceContext->RSSetViewports(1, &viewPort);
		XMStoreFloat4x4(&proj, XMMatrixPerspectiveFovLH(FOV, width / (float)height, nearPlane, farPlane));
	}
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY!
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow );						   
LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wparam, LPARAM lparam );		
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance,(WNDPROC)WndProc);	
    MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT && myApp.Run() )
    {	
	    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        { 
            TranslateMessage( &msg );
            DispatchMessage( &msg ); 
        }
    }
	myApp.ShutDown(); 
	return 0; 
}
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if(GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;

	//if (GetAsyncKeyState('A'))
	//{
	//	camera.angleY -= (float)timer.Delta() * 50;
	//}

	//if (GetAsyncKeyState('D'))
	//{
	//	camera.angleY += (float)timer.Delta() * 50;
	//}
	//if (GetAsyncKeyState('E'))
	//{
	//	camera.angleX -= (float)timer.Delta() * 50;
	//}
	//if (GetAsyncKeyState('F'))
	//{
	//	camera.angleX += (float)timer.Delta() * 50;
	//}
    switch ( message )
    {
        case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;
		case WM_MOUSEMOVE:
			camera.OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			camera.OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONDOWN:
			camera.OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_SIZE:
			g_myApp->Resize(LOWORD(lParam), HIWORD(lParam));
			break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//