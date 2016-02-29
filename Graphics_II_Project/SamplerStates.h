#pragma once
#include "define.h"
class SamplerStates
{
public:
	SamplerStates();
	void CreateSamplerStates(ID3D11Device* gfx);
	~SamplerStates();
	inline ID3D11SamplerState* const* GetAnisotroicSampler()const {	return &AnisotroicSampler; }
	inline ID3D11SamplerState* const* GetLinearSampler()const { return &LinearSampler; }
private:
	ID3D11SamplerState* LinearSampler = nullptr;
	ID3D11SamplerState* AnisotroicSampler = nullptr;
};

