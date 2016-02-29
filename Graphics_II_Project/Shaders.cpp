#include "Shaders.h"



Shaders::Shaders()
{
}

void Shaders::Create(ID3D11Device * gfx, const void* VS_Code, const void* PS_Code, SIZE_T VS_Length, SIZE_T PS_Length)
{
	gfx->CreateVertexShader(VS_Code, VS_Length, nullptr, &pVS);
	gfx->CreatePixelShader(PS_Code, PS_Length, nullptr, &pPS);
}

void Shaders::SetShader(ID3D11DeviceContext * gfx)
{
	gfx->IASetInputLayout(pLayout);
	gfx->VSSetShader(pVS, nullptr, 0);
	gfx->PSSetShader(pPS, nullptr, 0);
}


Shaders::~Shaders()
{
	SAFE_RELEASE(pLayout);
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(pVS);
}
