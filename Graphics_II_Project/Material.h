#pragma once
#include "define.h"
class Material
{
public:
	Material();
	~Material();
private:
	XMFLOAT3 ambientColor;
	XMFLOAT3 diffuseColor;
	XMFLOAT3 specularColor;
	FLOAT	 specularPower;
};

