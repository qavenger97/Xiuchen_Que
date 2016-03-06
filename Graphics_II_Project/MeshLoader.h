#pragma once
#include "define.h"
#include <vector>
class MeshLoader
{
public:
	MeshLoader();
	~MeshLoader();
	static void LoadOBJFromFile(const wchar_t* filePath, std::vector<Vertex>& verts, std::vector<UINT>& indices, BoundingBox* boundingBox = nullptr);
private:
};

