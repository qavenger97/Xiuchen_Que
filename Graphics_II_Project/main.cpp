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

#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include "Grid_VS.csh"
#include "Grid_PS.csh"
#include "Debug_VS.csh"
#include "Debug_PS.csh"
#include "Debug_GS.csh"
#include "Post_VS.csh"
#include "Post_PS.csh"
#include "DDSTextureLoader.h"
#include "XTime.h"
#include "Camera.h"
#include "SamplerStates.h"
#include "MeshLoader.h"
#include "Material.h"
#define BACKBUFFER_WIDTH	500
#define BACKBUFFER_HEIGHT	500
#define NUM_VERT 16
//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************


XTime timer;
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
		v[0].pos.z = -halfthickness;
		v[0].normal.x = 0;
		v[0].normal.y = 0;
		v[0].normal.z = -1;
		v[0].uv0.x = 0.5f;
		v[0].uv0.y = 0.5f;
		v[11].pos.x = 0;
		v[11].pos.y = 0;
		v[11].pos.z = halfthickness;
		v[11].normal.x = 0;
		v[11].normal.y = 0;
		v[11].normal.z = 1;
		v[11].uv0.x = 0.5f;
		v[11].uv0.y = 0.5f;

		for (int c = 0; c < 5; c++)
		{
			int i = (c << 1) | 1;
			float angle1 = da*PI_2*(c<<1);
			float angle2 = da*PI_2*i;
			float cosA = cosf(angle1);
			float sinA = sinf(angle1);
			float cosB = cosf(angle2);
			float sinB = sinf(angle2);
			float r = radiusIn / radiusOut;
			v[i    ].pos.x = cosA * radiusOut;
			v[i    ].pos.y = sinA * radiusOut;
			v[i    ].pos.z = -halfthickness;
			v[i    ].normal.x = 0;
			v[i    ].normal.y = 0;
			v[i    ].normal.z = -1;
			v[i    ].uv0.x = cosA;
			v[i	   ].uv0.y = -sinA;

			v[i + 1].pos.x = cosB * radiusIn;
			v[i + 1].pos.y = sinB * radiusIn;
			v[i + 1].pos.z = -halfthickness;
			v[i + 1].normal.x = 0;
			v[i + 1].normal.y = 0;
			v[i + 1].normal.z = -1;
			v[i + 1].uv0.x = cosB * r;
			v[i + 1].uv0.y = -sinB * r;

			v[i + 11].pos.x = cosA * radiusOut;
			v[i + 11].pos.y = sinA * radiusOut;
			v[i + 11].pos.z = halfthickness;
			v[i + 11].normal.x = 0;
			v[i + 11].normal.y = 0;
			v[i + 11].normal.z = 1;
			v[i + 11].uv0.x = cosA;
			v[i + 11].uv0.y = -sinA;

			v[i + 12].pos.x = cosB * radiusIn;
			v[i + 12].pos.y = sinB * radiusIn;
			v[i + 12].pos.z = halfthickness;
			v[i + 12].normal.x = 0;
			v[i + 12].normal.y = 0;
			v[i + 12].normal.z = 1;
			v[i + 12].uv0.x = cosB * r;
			v[i + 12].uv0.y = -sinB * r;
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
		bd.ByteWidth = sizeof(ConstantPerObject);
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
		gfx->PSSetShader(ps, 0, 0);
		gfx->IASetInputLayout(pLayout);

		D3D11_MAPPED_SUBRESOURCE mr = {};

		XMMATRIX world = XMLoadFloat4x4(&transform) * camera.GetViewMatrix() * camera.GetProjectionMatrix();

		gfx->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
		memcpy(mr.pData, &world, sizeof(XMFLOAT4X4));
		gfx->Unmap(pConstantBuffer, 0);

		gfx->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		gfx->RSSetState(bMSAA ? pRasterizerState : nullptr);
		gfx->DrawIndexed(84, 0, 0);
	}
	
	void ToogleState(ID3D11DeviceContext* gfx)
	{
		bMSAA = !bMSAA;
	}
};

class Mesh
{
	ID3D11Buffer* pVertexBuffer = 0;
	ID3D11Buffer* pIndexBuffer = 0;
	ID3D11Buffer* pInstanceBuffer = 0;
	ID3D11Buffer* pConstantBuffer = 0;
	ID3D11Buffer* pDebugBox = 0;
	ID3D11InputLayout* pLayout = 0;
	ID3D11VertexShader* vs = 0;
	ID3D11PixelShader* ps = 0;
	ID3D11VertexShader* pDebug_VS = 0;
	ID3D11PixelShader* pDebug_PS = 0;
	ID3D11GeometryShader* pDebug_GS = 0;
	ID3D11InputLayout* pDebugLayout = 0;
	InstanceData transform[100];
	BoundingBox bounds[100];
	UINT numIndex;
	UINT numInstance;
	bool debug;
	float scale;
public:
	Mesh(float scale = 1):scale(scale)
	{
		numInstance = 100;
		debug = false;
	}

	void ToogleBoundingBox()
	{
		debug = !debug;
	}

	void Create(ID3D11Device* gfx, const wchar_t* filePath, float z = 0)
	{
		for(UINT y = 0; y < 10; y++)
		{
			for (UINT x = 0; x < 10; x++)
			{
				UINT i = y * 10 + x;
				XMMATRIX transformation = XMMatrixIdentity();
				transformation = transformation * XMMatrixScaling(scale, scale, scale);
				transformation = transformation * XMMatrixRotationY(DegreeToRadian(x*36.0f));
				XMStoreFloat4x4(&transform[i].world, transformation);
				transform[i].world.m[3][0] = x * 10.0f - 50;
				transform[i].world.m[3][2] = y * 10.0f - 50;
				transform[i].world.m[3][1] = z;
				XMStoreFloat4x4(&transform[i].worldI, XMMatrixInverse(0, XMLoadFloat4x4(&transform[i].world)));
			}
		}

		std::vector<Vertex> v;
		std::vector<UINT> index;
		BoundingBox bound;
		MeshLoader::LoadOBJFromFile(filePath, v, index, &bound);
		XMVECTOR c = XMLoadFloat3(&bound.Center) * scale;
		XMVECTOR e = XMLoadFloat3(&bound.Extents) * scale;
		for (UINT i = 0; i < numInstance; i++)
		{
			XMStoreFloat3(&bounds[i].Center, c + XMLoadFloat4x4(&transform[i].world).r[3]);
			XMStoreFloat3(&bounds[i].Extents, e);
		}
		

		numIndex = (UINT)index.size();

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(Vertex) * (UINT)v.size();
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = v.data();
		gfx->CreateBuffer(&bd, &sd, &pVertexBuffer);

		bd.ByteWidth = sizeof(BoundingBox) * numInstance;
		sd.pSysMem = &bounds[0];
		gfx->CreateBuffer(&bd, &sd, &pDebugBox);

		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(UINT) * (UINT)index.size();
		sd.pSysMem = index.data();
		gfx->CreateBuffer(&bd, &sd, &pIndexBuffer);

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(ConstantPerObject);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		gfx->CreateBuffer(&bd, 0, &pConstantBuffer);

		bd.ByteWidth = sizeof(InstanceData) * numInstance;
		gfx->CreateBuffer(&bd, 0, &pInstanceBuffer);

		gfx->CreateVertexShader(Trivial_VS, ARRAYSIZE(Trivial_VS), 0, &vs);
		gfx->CreatePixelShader(Trivial_PS, ARRAYSIZE(Trivial_PS), 0, &ps);
		gfx->CreateVertexShader(Debug_VS, ARRAYSIZE(Debug_VS), 0, &pDebug_VS);
		gfx->CreatePixelShader(Debug_PS, ARRAYSIZE(Debug_PS), 0, &pDebug_PS);
		gfx->CreateGeometryShader(Debug_GS, ARRAYSIZE(Debug_GS), 0, &pDebug_GS);


		D3D11_RASTERIZER_DESC rasterDesc = {};

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0,
			"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0,
			"TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0,
			"TEXCOORD", 2, DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0
		};
		gfx->CreateInputLayout(layout, ARRAYSIZE(layout), Trivial_VS, ARRAYSIZE(Trivial_VS), &pLayout);
		D3D11_INPUT_ELEMENT_DESC debugLayout[] =
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0,
			"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0,
		};
		gfx->CreateInputLayout(debugLayout, ARRAYSIZE(debugLayout), Debug_VS, ARRAYSIZE(Debug_VS), &pDebugLayout);
	}
	~Mesh()
	{
		SAFE_RELEASE(ps);
		SAFE_RELEASE(vs);
		SAFE_RELEASE(pLayout);
		SAFE_RELEASE(pDebugBox);
		SAFE_RELEASE(pDebug_GS);
		SAFE_RELEASE(pDebug_PS);
		SAFE_RELEASE(pDebug_VS);
		SAFE_RELEASE(pDebugLayout);
		SAFE_RELEASE(pInstanceBuffer);
		SAFE_RELEASE(pConstantBuffer);
		SAFE_RELEASE(pVertexBuffer);
		SAFE_RELEASE(pIndexBuffer);
	}
	void Draw(ID3D11DeviceContext* gfx)
	{
		UINT stride = sizeof Vertex;
		UINT offset = 0;

		gfx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ID3D11Buffer* buffer[] = pVertexBuffer, 
		gfx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		gfx->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		gfx->VSSetShader(vs, 0, 0);
		gfx->PSSetShader(ps, 0, 0);
		gfx->IASetInputLayout(pLayout);

		D3D11_MAPPED_SUBRESOURCE mr = {};
		ConstantPerObject cb;

		XMStoreFloat4x4(&cb.viewInverse, camera.GetViewMatrix());
		XMStoreFloat4x4(&cb.projectionMatrix, camera.GetProjectionMatrix());
		XMStoreFloat4x4(&cb.view, camera.GetViewMatrixInverse());

		gfx->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
		memcpy(mr.pData, &cb, sizeof(cb));
		gfx->Unmap(pConstantBuffer, 0);

		D3D11_MAPPED_SUBRESOURCE tr = {};
		gfx->Map(pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tr);
		//memcpy(mr.pData, transform, sizeof(transform));
		InstanceData* data = reinterpret_cast<InstanceData*>(tr.pData);
		UINT numVisInstance = 0;
		for (UINT i = 0; i < numInstance; i++)
		{
			if (camera.GetViewFrustum()->Intersects(bounds[i]))
			{
				data[numVisInstance++] = transform[i];
			}
		}

		gfx->Unmap(pInstanceBuffer, 0);
		gfx->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		gfx->VSSetConstantBuffers(1, 1, &pInstanceBuffer);
		gfx->DrawIndexedInstanced(numIndex, numVisInstance, 0, 0, 0);

		if (debug)
		{
			UINT stride_debug = sizeof BoundingBox;
			UINT offset_debug = 0;
			gfx->IASetInputLayout(pDebugLayout);

			gfx->VSSetShader(pDebug_VS, 0, 0);
			gfx->IASetVertexBuffers(0, 1, &pDebugBox,&stride_debug,&offset_debug);
			gfx->GSSetShader(pDebug_GS, 0, 0);
			gfx->PSSetShader(pDebug_PS, 0, 0);
			gfx->GSSetConstantBuffers(0, 1, &pConstantBuffer);
			gfx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			gfx->Draw(numInstance, 0);
			gfx->GSSetShader(nullptr,0,0);
		}
	}
};

class PostQuad
{
public:
	PostQuad(ID3D11Device* gfx)
	{
		XMFLOAT4 verts[4] = 
		{
			{-1,  1,  0,  0},
			{-1, -1,  0,  1},
			{ 1,  1,  1,  0},
			{ 1, -1,  1,  1}
		};

		UINT indices[] = 
		{
			0,1,2,
			2,1,3
		};
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(verts);
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = verts;
		gfx->CreateBuffer(&bd, &sd, &pVertexBuffer);

		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(indices);
		sd.pSysMem = indices;
		gfx->CreateBuffer(&bd, &sd, &pIndexBuffer);

		gfx->CreateVertexShader(Post_VS, sizeof(Post_VS), 0, &pVS);
		gfx->CreatePixelShader(Post_PS, sizeof(Post_PS), 0, &pPS);

		D3D11_INPUT_ELEMENT_DESC ied[] = 
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0, };
		gfx->CreateInputLayout(ied, ARRAYSIZE(ied), Post_VS, sizeof(Post_VS), &pLayout);
	}

	void Draw(ID3D11DeviceContext* gfx)
	{
		UINT stride = sizeof(XMFLOAT4);
		UINT offset = 0;
		gfx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		gfx->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		gfx->IASetInputLayout(pLayout);
		gfx->VSSetShader(pVS, 0, 0);
		gfx->PSSetShader(pPS, 0, 0);
		gfx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx->DrawIndexed(6, 0, 0);
	}
	~PostQuad()
	{
		SAFE_RELEASE(pPS);
		SAFE_RELEASE(pVS);
		SAFE_RELEASE(pLayout);
		SAFE_RELEASE(pIndexBuffer);
		SAFE_RELEASE(pVertexBuffer);
	}
private:
	ID3D11Buffer* pVertexBuffer = 0;
	ID3D11Buffer* pIndexBuffer = 0;
	ID3D11InputLayout* pLayout = 0;
	ID3D11VertexShader* pVS = 0;
	ID3D11PixelShader* pPS = 0;
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
	ID3D11RenderTargetView* pPostView = 0;
	ID3D11DepthStencilView* pDSV = 0;
	D3D11_VIEWPORT viewPort;

	//ID3D11Buffer* pVertexBuffer = 0;
	//ID3D11Buffer* pIndexBuffer = 0;
	ID3D11Buffer* pConstantBuffer = 0;
	ID3D11Buffer* pDepthBuffer = 0;
	ID3D11Buffer* pLightsBuffer = 0;
	ID3D11Buffer* pPostBuffer = 0;
	ID3D11RasterizerState* pRasterizerState = 0;
	ID3D11RasterizerState* pRasterizerState_back = 0;
	ID3D11BlendState* pOverlay = 0;
	ID3D11DepthStencilState* pStencilDisable = 0;
	ID3D11DepthStencilState* pStencilClip = 0;
	ID3D11DepthStencilState* pStencilDraw = 0;
	ID3D11InputLayout* pLayout = 0;
	ID3D11VertexShader* vs = 0;
	ID3D11PixelShader* ps = 0;
	ID3D11GeometryShader* gs = 0;
	ID3D11ShaderResourceView* pSRV = 0;
	ID3D11ShaderResourceView* pSRV1 = 0;
	ID3D11ShaderResourceView* pSRV2 = 0;
	ID3D11ShaderResourceView* pPostRsc = 0;
	ID3D11ShaderResourceView* pDepthRsc = 0;
	ID3D11ShaderResourceView* pFlatNormal = 0;
	SamplerStates samplers;
	ConstantPerObject cb;
	Wave wave;
	Mesh mesh;
	Mesh mesh1;
	Mesh hole;
	Mesh mesh2;
	Grid grid;
	Star star;
	LightBuffer lights;
	float nearPlane = 0.01f;
	float farPlane = 10000;
	float FOV = DegreeToRadian(75);
	WORD mouseX;
	WORD mouseY;
	BOOL animate = false;
	PostQuad* post;
private:
	void InitResources();
public:	
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
	void Resize(WORD width, WORD height);
	void OnMouseMove(WPARAM btnState, WORD x, WORD y);
};

//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP* g_myApp;

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc):mesh(0.1f), hole(0.03f), mesh1(0.1f), mesh2(0.02f)
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

	window = CreateWindow(L"DirectXApplication", L"Graphics II Project", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	ShowWindow(window, SW_SHOW);
	//********************* END WARNING ************************//
	{
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
		//sd.SampleDesc.Quality = 8;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		camera.SetProjection(FOV, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT, nearPlane, farPlane);
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

		ID3D11Texture2D* backBuffer;
		rs = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		if (rs != S_OK)
		{
			OutputDebugString(L"Get Backbuffer failed\n");
		}

		ZeroMemory(&viewPort, sizeof(viewPort));
		viewPort.Height = BACKBUFFER_HEIGHT;
		viewPort.Width = BACKBUFFER_WIDTH;
		viewPort.MaxDepth = 1;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.CPUAccessFlags = 0;


		rs = pDevice->CreateRenderTargetView(backBuffer, 0, &pRTV);
		if (rs != S_OK)
		{
			OutputDebugString(L"Create RTV failed\n");
		}

		D3D11_TEXTURE2D_DESC textDesc;
		backBuffer->GetDesc(&textDesc);

		if(backBuffer)backBuffer->Release();
		textDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		pDevice->CreateTexture2D(&textDesc, 0, &backBuffer);


		rs = pDevice->CreateRenderTargetView(backBuffer, 0, &pPostView);
		D3D11_SHADER_RESOURCE_VIEW_DESC srvD = {};
		srvD.Format = textDesc.Format;
		srvD.Texture2D.MipLevels = 1;
		srvD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		pDevice->CreateShaderResourceView(backBuffer,&srvD , &pPostRsc);
		if (backBuffer)backBuffer->Release();

		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(cb);
		rs = pDevice->CreateBuffer(&bd, 0, &pConstantBuffer);
		if (rs != S_OK)
		{
			OutputDebugString(L"Create Triangle ConstantBuffer Failed");
		}
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.ByteWidth = sizeof(lights);
		rs = pDevice->CreateBuffer(&bd, 0, &pLightsBuffer);
		
		bd.ByteWidth = sizeof(Wave);
		pDevice->CreateBuffer(&bd, 0, &pPostBuffer);
	}

	ID3D11Texture2D* depthBuffer = 0;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Format = DXGI_FORMAT_R24G8_TYPELESS;
	td.Height = BACKBUFFER_HEIGHT;
	td.Width = BACKBUFFER_WIDTH;
	td.ArraySize = 1;
	td.MipLevels = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
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
	rasterDesc.FrontCounterClockwise = TRUE;
	rasterDesc.MultisampleEnable = true;
	pDevice->CreateRasterizerState(&rasterDesc, &pRasterizerState);

	rasterDesc.CullMode = D3D11_CULL_FRONT;
	pDevice->CreateRasterizerState(&rasterDesc, &pRasterizerState_back);

	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(svd));
	svd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	rs = pDevice->CreateDepthStencilView(depthBuffer, &svd, &pDSV);

	if (rs != S_OK)
	{
		OutputDebugString(L"Create Depth Stencil View failed\n");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Texture2D.MipLevels = 1;
	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	pDevice->CreateShaderResourceView(depthBuffer, &srvd, &pDepthRsc);

	if (depthBuffer) depthBuffer->Release();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0,
		"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0,
		"TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0,
		"TEXCOORD", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0
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
	blendDesc.AlphaToCoverageEnable = true;
	pDevice->CreateBlendState(&blendDesc, &pOverlay);

	samplers.CreateSamplerStates(pDevice);
	D3D11_DEPTH_STENCIL_DESC cdsd = {};
	cdsd.DepthEnable = true;
	cdsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	cdsd.DepthFunc = D3D11_COMPARISON_LESS;
	cdsd.StencilEnable = true;
	cdsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	cdsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	cdsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	cdsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	cdsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	cdsd.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

	cdsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	cdsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	cdsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	cdsd.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	pDevice->CreateDepthStencilState(&cdsd, &pStencilClip);

	cdsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	cdsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	cdsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	cdsd.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
	pDevice->CreateDepthStencilState(&cdsd, &pStencilDraw);

	cdsd.StencilEnable = false;
	pDevice->CreateDepthStencilState(&cdsd, &pStencilDisable);


	InitResources();
	//pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
	pDeviceContext->RSSetViewports(1, &viewPort);
	pDeviceContext->PSSetSamplers(0, 1, samplers.GetAnisotroicSampler());
	
}

void DEMO_APP::InitResources()
{
	XMStoreFloat4x4(&cb.skyMatrix, XMMatrixIdentity());
	star.Create(pDevice);
	star.transform.m[3][0] = 2;
	grid.Create(pDevice);
	mesh.Create(pDevice, L"plane.obj");
	mesh1.Create(pDevice, L"plane.obj", 0.3f);
	hole.Create(pDevice, L"plane.obj", 0.3f);
	mesh2.Create(pDevice, L"plane.obj", 0.3f);
	camera.SetCubemap(pDevice, L"Cube_City.dds");

	CreateDDSTextureFromFile(pDevice, L"C.dds", nullptr, &pSRV);
	CreateDDSTextureFromFile(pDevice, L"N.dds", nullptr, &pSRV1);
	CreateDDSTextureFromFile(pDevice, L"R.dds", nullptr, &pSRV2);
	CreateDDSTextureFromFile(pDevice, L"FLAT_N.dds", nullptr, &pFlatNormal);

	lights.light[0].pos = XMFLOAT4(0, 0, 0, 0);
	lights.light[0].dir = XMFLOAT4(0, -1, 0, 0);
	//lights.light[0].color = XMFLOAT4(0.9f, 0.7f, 0.7f, 0);
	lights.light[0].color = XMFLOAT4(0.7f, 0.7f, 0.7f, .5f);
	lights.light[0].att = XMFLOAT4(0, 0, 0, 0);

	lights.light[1].pos = XMFLOAT4(0, 0.1f, 0, 1);
	//lights.light[1].color = XMFLOAT4(0.2f, 0.5f, 0.6f, 0);
	lights.light[1].color = XMFLOAT4(0.2f, 0.5f, 0.6f, 0.7f);
	lights.light[1].dir = XMFLOAT4(0, 0, 0, 0);
	lights.light[1].att = XMFLOAT4(0, 0, 10, 1);

	lights.light[2].pos = XMFLOAT4(0, 0, 0, 2);
	//lights.light[2].color = XMFLOAT4(1, 1, 1, 0);
	lights.light[2].color = XMFLOAT4(1, 1, 1, 1);
	lights.light[2].att = XMFLOAT4(0.99f, 0.92f, 20, 2);
	lights.material.fresnelIntensity = 0.9f;
	lights.material.fresnelPower = 1;
	lights.material.specularPower = 10;
	lights.material.heightOffset = 0.04f;
	post = new PostQuad(pDevice);

	wave.amplitude = 0.01f;
	wave.frequency = 50;
	wave.speed = 0;
}
bool DEMO_APP::Run()
{
	timer.Signal();
	//pDeviceContext->ClearState();
	pDeviceContext->OMSetRenderTargets(1, &pPostView, pDSV);
	pDeviceContext->PSSetSamplers(0, 1, samplers.GetAnisotroicSampler());
	pDeviceContext->RSSetViewports(1, &viewPort);
	float dt = (float)timer.Delta();
	camera.Update(dt);
	XMStoreFloat4(&lights.light[2].dir, camera.GetViewMatrixInverse().r[2]);
	XMStoreFloat4(&lights.light[2].pos, camera.GetPos().r[3]);
	lights.light[2].pos.w = 2;
	static float dir = 1;

	if (animate)
	{
		if (lights.material.fresnelIntensity <= -1)
		{
			dir = -1;
		}

		if (lights.material.fresnelIntensity >= 1)
		{
			dir = 1;
		}
		lights.material.fresnelIntensity -= dt*0.1f * dir;
	}
	

	D3D11_MAPPED_SUBRESOURCE ms = {};
	XMStoreFloat4x4(&cb.viewInverse, camera.GetPos() * camera.GetViewProjectionMatrix());
	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &cb, sizeof(cb));
	pDeviceContext->Unmap(pConstantBuffer, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	camera.DrawSkybox(pDeviceContext);

	pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->IASetInputLayout(pLayout);
	pDeviceContext->VSSetShader(vs, 0, 0);
	pDeviceContext->PSSetShader(ps, 0, 0);

	pDeviceContext->RSSetState(pRasterizerState);
	lights.skyMatrix = cb.skyMatrix;
	pDeviceContext->UpdateSubresource(pLightsBuffer, 0, 0, &lights, 0, 0);

	static float angle = 0;
	angle += 0.0001f;
	if (angle >= XM_2PI)angle = 0;
	XMMATRIX t = XMMatrixRotationY(angle) * XMLoadFloat4x4(&star.transform);
	XMStoreFloat4x4(&cb.viewInverse, t * camera.GetViewMatrix());
	XMStoreFloat4x4(&cb.projectionMatrix, camera.GetProjectionMatrix());
	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, &cb, sizeof(cb));
	pDeviceContext->Unmap(pConstantBuffer, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);


	pDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	pDeviceContext->PSSetShaderResources(1, 1, &pSRV1);
	pDeviceContext->PSSetShaderResources(2, 1, &pSRV2);

	pDeviceContext->PSSetConstantBuffers(0, 1, &pLightsBuffer);

	star.Draw(pDeviceContext);


	static const float factor[] = { 1,1,1,1 };
	pDeviceContext->OMSetBlendState(0, factor, 0xffffffff);
	pDeviceContext->OMSetDepthStencilState(pStencilDraw, 1);
	hole.Draw(pDeviceContext);

	pDeviceContext->OMSetBlendState(pOverlay, factor, 0xffffffff);
	pDeviceContext->OMSetDepthStencilState(pStencilClip, 1);
	mesh1.Draw(pDeviceContext);

	pDeviceContext->OMSetDepthStencilState(pStencilDisable, 0);
	mesh.Draw(pDeviceContext);

	//HandleControl
	{

		if (GetAsyncKeyState('1') & 0x01)
		{
			camera.SetCubemap(pDevice, L"Cube_Lake.dds");
		}
		if (GetAsyncKeyState('2') & 0x01)
		{
			camera.SetCubemap(pDevice, L"Cube_Desert.dds");
		}
		if (GetAsyncKeyState('3') & 0x01)
		{
			camera.SetCubemap(pDevice, L"Cube_Mountain.dds");
		}
		if (GetAsyncKeyState('4') & 0x01)
		{
			camera.SetCubemap(pDevice, L"Cube_City.dds");
		}

		//if (GetAsyncKeyState('3') & 0x01)
		//{
		//	grid.ToogleState(pDeviceContext);
		//}

		if (GetAsyncKeyState(VK_F2))
		{
			FOV += dt;
			camera.SetProjection(FOV, viewPort.Width, viewPort.Height, nearPlane, farPlane);
		}
		if (GetAsyncKeyState(VK_F1))
		{
			FOV -= dt;
			camera.SetProjection(FOV, viewPort.Width, viewPort.Height, nearPlane, farPlane);
		}

		if (GetAsyncKeyState('K')& 0x01)animate = !animate;

		if (GetAsyncKeyState(VK_LEFT))
		{	
			if (GetAsyncKeyState(VK_SHIFT))
			{
				wave.amplitude -= dt*0.01f;
			}
			else
			{
				XMVECTOR d = XMVectorSet(0, 0, 0, 0);
				XMVECTOR pos = XMLoadFloat4(&lights.light[1].pos);
				d.m128_f32[0] = -(float)timer.Delta() ;
				d = XMVector4Transform(d, camera.GetViewMatrixInverse());
				d.m128_f32[1] = 0;
				d.m128_f32[2] = 0;
				XMStoreFloat4(&lights.light[1].pos, d + pos);
				lights.light[1].pos.w = 1;
			}
		}

		if (GetAsyncKeyState('F') & 0x01)
		{
			lights.light[2].color.w = lights.light[2].color.w == 0 ? 1.0f : 0 ;
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{

			if (GetAsyncKeyState(VK_SHIFT))
			{
				wave.amplitude += dt*0.01f;
			}
			else
			{
				XMVECTOR d = XMVectorSet(0, 0, 0, 0);
				XMVECTOR pos = XMLoadFloat4(&lights.light[1].pos);
				d.m128_f32[0] = dt;
				d = XMVector4Transform(d, camera.GetViewMatrixInverse());
				d.m128_f32[1] = 0;
				d.m128_f32[2] = 0;
				XMStoreFloat4(&lights.light[1].pos, d + pos);
				lights.light[1].pos.w = 1;
			}
		}

		if (GetAsyncKeyState(VK_UP))
		{
			if (GetAsyncKeyState(VK_SHIFT))
			{
				wave.frequency+=dt*10;
			}
			else
			{
				lights.light[1].pos.z -= (float)timer.Delta() ;
			}
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			if (GetAsyncKeyState(VK_SHIFT))
			{
				wave.frequency-=dt*10;
			}
			else
			{
				lights.light[1].pos.z += dt;
			}
		}

		if (GetAsyncKeyState('5') & 0x01)
		{
			mesh.ToogleBoundingBox();
		}
	}
	//pDeviceContext->ClearState();
	wave.speed += dt;

	pDeviceContext->OMSetRenderTargets(1, &pRTV, nullptr);
	pDeviceContext->PSSetSamplers(0, 1, samplers.GetAnisotroicSampler());
	pDeviceContext->RSSetViewports(1, &viewPort);
	pDeviceContext->PSSetShaderResources(0, 1, &pPostRsc);
	pDeviceContext->PSSetShaderResources(1, 1, &pDepthRsc);
	pDeviceContext->PSSetConstantBuffers(0, 1, &pPostBuffer);
	pDeviceContext->UpdateSubresource(pPostBuffer, 0, 0, &wave, 0, 0);
	post->Draw(pDeviceContext);

	pDeviceContext->PSSetShaderResources(0, 1, &pPostRsc); 
	pDeviceContext->PSSetShaderResources(1, 1, &pFlatNormal);
	pDeviceContext->PSSetShaderResources(2, 1, &pSRV2);
	pDeviceContext->PSSetConstantBuffers(0, 1, &pLightsBuffer);
	pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
	mesh2.Draw(pDeviceContext);
	pSwapChain->Present(0, 0);
	pDeviceContext->ClearState();
	return true; 
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{
	if (post)delete post;
	if (pRasterizerState_back)pRasterizerState_back->Release();
	if (pSRV)pSRV->Release();
	if (pSRV1)pSRV1->Release();
	if (pSRV2)pSRV2->Release();
	if (pLayout)pLayout->Release();
	if (ps)ps->Release();
	if (vs)vs->Release();
	if (pOverlay)pOverlay->Release();
	if (pStencilDraw)pStencilDraw->Release();
	if (pStencilClip)pStencilClip->Release();
	if (pStencilDisable)pStencilDisable->Release();
	if (pFlatNormal)pFlatNormal->Release();
	if (pRasterizerState)pRasterizerState->Release();
	if (pPostBuffer)pPostBuffer->Release();
	if (pDepthBuffer)pDepthBuffer->Release();
	if (pLightsBuffer)pLightsBuffer->Release();
	if (pConstantBuffer)pConstantBuffer->Release();
	//if (pIndexBuffer)pIndexBuffer->Release();
	//if (pVertexBuffer) pVertexBuffer->Release();
	if (pPostRsc)pPostRsc->Release();
	if (pDepthRsc)pDepthRsc->Release();
	if (pDSV)pDSV->Release();
	if (pPostView)pPostView->Release();
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
		if (pPostView)pPostView->Release();
		if (pPostRsc)pPostRsc->Release();
		if (pDepthRsc)pDepthRsc->Release();
		if (pSwapChain)pSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

		pDevice->CreateRenderTargetView(pBackBuffer, 0, &pRTV);

		D3D11_TEXTURE2D_DESC textDesc;
		pBackBuffer->GetDesc(&textDesc);

		if (pBackBuffer)pBackBuffer->Release();
		textDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		pDevice->CreateTexture2D(&textDesc, 0, &pBackBuffer);


		rs = pDevice->CreateRenderTargetView(pBackBuffer, 0, &pPostView);
		D3D11_SHADER_RESOURCE_VIEW_DESC srvD = {};
		srvD.Format = textDesc.Format;
		srvD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvD.Texture2D.MipLevels = 1;
		pDevice->CreateShaderResourceView(pBackBuffer, &srvD, &pPostRsc);
		if (pBackBuffer)pBackBuffer->Release();


		D3D11_TEXTURE2D_DESC td = {};
		td.ArraySize = 1;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.Height = height;
		td.Width = width;
		td.Format = DXGI_FORMAT_R24G8_TYPELESS;
		td.MipLevels = 1;
		td.SampleDesc.Count = 1;
		td.Usage = D3D11_USAGE_DEFAULT;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		ID3D11Texture2D* depthBuffer;
		pDevice->CreateTexture2D(&td, 0, &depthBuffer);
		pDevice->CreateDepthStencilView(depthBuffer, &dsvd, &pDSV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		pDevice->CreateShaderResourceView(depthBuffer, &srvd, &pDepthRsc);
		depthBuffer->Release();
		//pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
		camera.SetProjection(FOV, width, height, nearPlane, farPlane);
		camera.Update((float)timer.Delta());
	}
}

void DEMO_APP::OnMouseMove(WPARAM btnState, WORD x, WORD y)
{
	if (btnState & VK_RBUTTON)
	{
		float dx = DegreeToRadian(x - (float)mouseX);
		float dy = DegreeToRadian(y - (float)mouseY);
		XMMATRIX rotX = XMMatrixRotationY(dx);
		XMMATRIX rotZ = XMMatrixRotationX(dy);
		XMMATRIX rot = rotX * rotZ;
		//XMVECTOR dir = XMLoadFloat4(&lights.light[0].dir);
		//dir = XMVector3Rotate(dir, XMQuaternionRotationMatrix(rot));
		//XMStoreFloat4(&lights.light[0].dir, dir);
		XMStoreFloat4x4(&cb.skyMatrix, XMLoadFloat4x4(&cb.skyMatrix) * rotX);
	}
	mouseX = x;
	mouseY = y;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
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

    switch ( message )
    {
        case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;
		case WM_MOUSEMOVE:
				camera.OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
				g_myApp->OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
			camera.OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_LBUTTONDOWN:
			camera.OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_SIZE:
			switch (wParam)
			{
			case SIZE_MAXIMIZED:
			case SIZE_RESTORED:
				g_myApp->Resize(LOWORD(lParam), HIWORD(lParam));
				break;
			default:
				break;
			}
			break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//