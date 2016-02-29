#include "SamplerStates.h"



SamplerStates::SamplerStates()
{
}

void SamplerStates::CreateSamplerStates(ID3D11Device * gfx)
{
	D3D11_SAMPLER_DESC spd = {};
	spd.Filter = D3D11_FILTER_ANISOTROPIC;
	spd.MaxAnisotropy = 4;
	spd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	spd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	spd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	gfx->CreateSamplerState(&spd, &AnisotroicSampler);
	spd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	gfx->CreateSamplerState(&spd, &LinearSampler);
}


SamplerStates::~SamplerStates()
{
	SAFE_RELEASE(LinearSampler);
	SAFE_RELEASE(AnisotroicSampler);
}
