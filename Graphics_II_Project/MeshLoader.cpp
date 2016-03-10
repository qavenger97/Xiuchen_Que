#include "MeshLoader.h"
#include <fstream>

MeshLoader::MeshLoader()
{
}


MeshLoader::~MeshLoader()
{
}

void MeshLoader::LoadOBJFromFile(const wchar_t * filePath, std::vector<Vertex>& verts, std::vector<UINT>& indices, BoundingBox* boundingBox)
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

	Vertex v;
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

		XMVECTOR v0 = XMLoadFloat3(&verts[index - 3].pos);
		XMVECTOR v1 = XMLoadFloat3(&verts[index - 2].pos);
		XMVECTOR v2 = XMLoadFloat3(&verts[index - 1].pos);
		XMVECTOR w0 = XMLoadFloat3(&verts[index - 3].uv0);
		XMVECTOR w1 = XMLoadFloat3(&verts[index - 2].uv0);
		XMVECTOR w2 = XMLoadFloat3(&verts[index - 1].uv0);
		XMVECTOR n[] = {
			XMLoadFloat3(&verts[index - 3].normal) ,
			XMLoadFloat3(&verts[index - 2].normal) ,
			XMLoadFloat3(&verts[index - 1].normal) 
		};

		XMFLOAT3 vertEdge0; 
		XMFLOAT3 vertEdge1; 
		XMFLOAT3 texEdge0; 
		XMFLOAT3 texEdge1; 

		XMStoreFloat3(&vertEdge0, v1 - v0);
		XMStoreFloat3(&vertEdge1, v2 - v0);
		XMStoreFloat3(&texEdge0, w1 - w0);
		XMStoreFloat3(&texEdge1, w2 - w0);

		float ratio = 1.0f / (texEdge0.x*texEdge1.y - texEdge1.x*texEdge0.y);
		XMVECTOR uDirection = XMLoadFloat3(&XMFLOAT3(
			(texEdge1.y * vertEdge0.x - texEdge0.y * vertEdge1.x) * ratio,
			(texEdge1.y * vertEdge0.y - texEdge0.y * vertEdge1.y) * ratio,
			(texEdge1.y * vertEdge0.z - texEdge0.y * vertEdge1.z) * ratio
			));

		uDirection = XMVector3Normalize(uDirection);
		for (int i = 0; i < 3; i++)
		{
			float dot = XMVector3Dot(n[i], uDirection).m128_f32[0];
			XMStoreFloat3(&verts[index - (3 - i)].tengent, uDirection - dot * n[i]);
		}

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

				XMVECTOR v0 = XMLoadFloat3(&verts[index - 2].pos);
				XMVECTOR v1 = XMLoadFloat3(&verts[index - 1].pos);
				XMVECTOR v2 = XMLoadFloat3(&verts[index - 4].pos);
				XMVECTOR w0 = XMLoadFloat3(&verts[index - 2].uv0);
				XMVECTOR w1 = XMLoadFloat3(&verts[index - 1].uv0);
				XMVECTOR w2 = XMLoadFloat3(&verts[index - 4].uv0);
				XMVECTOR n[] = {
					XMLoadFloat3(&verts[index - 2].normal) ,
					XMLoadFloat3(&verts[index - 1].normal) ,
					XMLoadFloat3(&verts[index - 4].normal)
				};

				XMFLOAT3 vertEdge0;
				XMFLOAT3 vertEdge1;
				XMFLOAT3 texEdge0;
				XMFLOAT3 texEdge1;

				XMStoreFloat3(&vertEdge0, v1 - v0);
				XMStoreFloat3(&vertEdge1, v2 - v0);
				XMStoreFloat3(&texEdge0, w1 - w0);
				XMStoreFloat3(&texEdge1, w2 - w0);

				float ratio = 1.0f / (texEdge0.x*texEdge1.y - texEdge1.x*texEdge0.y);
				XMVECTOR uDirection = XMLoadFloat3(&XMFLOAT3(
					(texEdge1.y * vertEdge0.x - texEdge0.y * vertEdge1.x) * ratio,
					(texEdge1.y * vertEdge0.y - texEdge0.y * vertEdge1.y) * ratio,
					(texEdge1.y * vertEdge0.z - texEdge0.y * vertEdge1.z) * ratio
					));

				uDirection = XMVector3Normalize(uDirection);

				float dot = XMVector3Dot(n[0], uDirection).m128_f32[0];
				XMStoreFloat3(&verts[index - 2].tengent, uDirection - dot * n[0]);
				dot = XMVector3Dot(n[1], uDirection).m128_f32[0];
				XMStoreFloat3(&verts[index - 1].tengent, uDirection - dot * n[1]); 
				dot = XMVector3Dot(n[2], uDirection).m128_f32[0];
				XMStoreFloat3(&verts[index - 4].tengent, uDirection - dot * n[2]);
			}
		}

		fin.ignore(INT32_MAX, '\n');
		fin.getline(buffer, INT32_MAX, ' ');
		check = buffer;
	} while (check == "f");

	if (boundingBox)
	{
		XMVECTOR vmin = XMVectorSet(0, 0, 0, 0);
		XMVECTOR vmax = XMVectorSet(0, 0, 0, 0);
		for (size_t i = 0; i < v_pos.size(); i++)
		{
			vmin = XMVectorMin(vmin, XMLoadFloat3(&v_pos[i]));
			vmax = XMVectorMax(vmax, XMLoadFloat3(&v_pos[i]));
		}
		XMStoreFloat3(&(boundingBox->Center), 0.5f*(vmin + vmax));
		XMStoreFloat3(&(boundingBox->Extents), 0.5f*(vmax - vmin));
	}
}
