#pragma once
#include "define.h"
#include <vector>
class MeshLoader
{
public:
	MeshLoader();
	~MeshLoader();
	static void LoadOBJFromFile(const wchar_t* filePath, std::vector<Vertex_m>& verts, std::vector<UINT>& indices);
private:

};

