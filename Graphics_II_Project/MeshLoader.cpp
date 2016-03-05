#include "MeshLoader.h"
#include <fstream>

MeshLoader::MeshLoader()
{
}


MeshLoader::~MeshLoader()
{
}

void MeshLoader::LoadOBJFromFile(const wchar_t * filePath, std::vector<Vertex_m>& verts, std::vector<UINT>& indices)
{
	std::ifstream fin(filePath);
	if (!fin)return;


	std::vector<XMFLOAT3> v_pos;
	std::vector<XMFLOAT3> v_normal;
	std::vector<XMFLOAT3> v_texCoord;
	std::vector<UINT> v_offset;
	char buffer[256];
	std::string check;
	std::string header;
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 texCoord;

	//load pos
	while (fin.peek() != 'v')
	{
		fin.getline(buffer, INT32_MAX, '\n');
	}
	
	do
	{
		fin.getline(buffer, INT32_MAX, ' ');
		check = buffer;
		if (check != "v")break;
		fin >> pos.x;
		fin >> pos.y;
		fin >> pos.z;
		pos.z *= -1;
		v_pos.push_back(pos);
		fin.ignore(INT32_MAX, '\n');
	} while (fin.peek() == 'v');


	if (check != "vn" && check != "vt")
	{
		while (fin.peek() != 'v')
		{
			fin.getline(buffer, INT32_MAX, '\n');
		}
		fin.getline(buffer, INT32_MAX, ' ');
		check = buffer;
	}

	if (check == "vn")
	{
		do
		{
			fin >> normal.x;
			fin >> normal.y;
			fin >> normal.z;
			normal.z *= -1;
			v_normal.push_back(normal);
			fin.ignore(INT32_MAX, '\n');

			fin.getline(buffer, INT32_MAX, ' ');
			check = buffer;

		} while (check == "vn");

		if (check != "vt")
		{
			while (char g = fin.peek() != 'v')
			{
				fin.getline(buffer, INT32_MAX, '\n');
			}
			fin.getline(buffer, INT32_MAX, ' ');
			check = buffer;
		}

		do
		{
			fin >> texCoord.x;
			fin >> texCoord.y;
			texCoord.y *= -1;
			v_texCoord.push_back(texCoord);
			fin.ignore(INT32_MAX, '\n');

			fin.getline(buffer, INT32_MAX, ' ');
			check = buffer;
		} while (check == "vt");
	}
	else if (check == "vt")
	{
		do
		{
			fin >> texCoord.x;
			fin >> texCoord.y;
			texCoord.y *= -1;
			v_texCoord.push_back(texCoord);
			fin.ignore(INT32_MAX, '\n');

			fin.getline(buffer, INT32_MAX, ' ');
			check = buffer;
		} while (check == "vt");

		if (check != "vn")
		{
			while (char g = fin.peek() != 'v')
			{
				fin.getline(buffer, INT32_MAX, '\n');
			}
			fin.getline(buffer, INT32_MAX, ' ');
			check = buffer;
		}

		do
		{
			fin >> normal.x;
			fin >> normal.y;
			fin >> normal.z;
			normal.z *= -1;
			v_normal.push_back(normal);
			fin.ignore(INT32_MAX, '\n');

			fin.getline(buffer, INT32_MAX, ' ');
			check = buffer;

		} while (check == "vn");
	}

	if (check != "f")
	{
		while (fin.peek() != 'f')
		{
			fin.getline(buffer, INT32_MAX, '\n');
		}
		fin.getline(buffer, INT32_MAX, ' ');
		check = buffer;
	}

	Vertex_m v;
	UINT index = 0;


	do
	{
		int p1, t1, n1;
		fin >> p1;
		fin.get();
		fin >> t1;
		fin.get();
		fin >> n1;
		v.pos = v_pos[--p1];
		v.uv0 = v_texCoord[--t1];
		v.normal = v_normal[--n1];
		verts.push_back(v);
		indices.push_back(index++);

		int p2, t2, n2;
		fin >> p2;
		fin.get();
		fin >> t2;
		fin.get();
		fin >> n2;

		v.pos = v_pos[--p2];
		v.uv0 = v_texCoord[--t2];
		v.normal = v_normal[--n2];
		verts.push_back(v);
		indices.push_back(index++);

		int p3, t3, n3;
		fin >> p3;
		fin.get();
		fin >> t3;
		fin.get();
		fin >> n3;

		v.pos = v_pos[--p3];
		v.uv0 = v_texCoord[--t3];
		v.normal = v_normal[--n3];
		verts.push_back(v);
		indices.push_back(index++);

		char i = fin.peek();
		if (fin.peek() != '\n')
		{
			fin.get();
			if (fin.peek() != '\n')
			{
				int p4, t4, n4;
				fin >> p4;
				fin.get();
				fin >> t4;
				fin.get();
				fin >> n4;

				v.pos = v_pos[--p4];
				v.uv0 = v_texCoord[--t4];
				v.normal = v_normal[--n4];
				verts.push_back(v);
				indices.push_back(index - 1);
				indices.push_back(index);
				indices.push_back(index - 3);
				index++;
			}
		}

		fin.ignore(INT32_MAX, '\n');
		fin.getline(buffer, INT32_MAX, ' ');
		check = buffer;
	} while (check == "f");
	XMMATRIX identity = XMMatrixIdentity();
	for (auto o : verts)
	{
		XMVECTOR newN = XMLoadFloat3(&o.normal);

		if (XMVector3Dot(newN, identity.r[1]).m128_f32[0] == 1)
		{
			XMStoreFloat3(&o.tengent, XMVector3Cross(identity.r[0], newN));
		}
		else
		{
			XMStoreFloat3(&o.tengent, XMVector3Cross(newN, identity.r[1]));
		}
	}

}
