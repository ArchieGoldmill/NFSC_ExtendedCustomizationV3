#pragma once
#include "D3DWrapper.h"
#include "Func.h"
#include "TextureInfo.h"

struct Vector3
{
	D3DXVECTOR3 V;
	float Padding;
};

class ePoly
{
public:
	bVector3 Vertices[4];
	D3DXVECTOR2 UVs[4];
	D3DXVECTOR2 UVsMask[4];
	D3DCOLOR Colours[4];
	char Flags;
	char Flailer;

	ePoly()
	{
		memset(this, 0, sizeof(ePoly));
		FUNC(0x00710B20, void, __thiscall, _ePoly, ePoly*);
		_ePoly(this);
		memset(this, 0, sizeof(D3DXVECTOR4) * 4);
	}

	void Render(TextureInfo* textureInfo, D3D::Matrix* matrix)
	{
		FUNC(0x0070F000, void, __thiscall, _Render, int, ePoly*, TextureInfo*, D3D::Matrix*, int, int);
		_Render(0, this, textureInfo, matrix, 0, 0);
	}

	//void TransformVertices(D3D::Matrix& matrix)
	//{
	//	for (auto& vert : this->Vertices)
	//	{
	//		matrix.Transform(vert);
	//	}
	//}

	//D3DXVECTOR3 GetNormal()
	//{
	//	auto v1 = D3DXVECTOR3(this->Vertices[0]);
	//	auto v2 = D3DXVECTOR3(this->Vertices[1]);
	//	auto v3 = D3DXVECTOR3(this->Vertices[2]);

	//	auto v1v2 = v2 - v1;
	//	auto v1v3 = v3 - v2;

	//	D3DXVECTOR3 normal;
	//	D3DXVec3Cross(&normal, &v1v2, &v1v3);

	//	D3DXVec3Normalize(&normal, &normal);

	//	return normal;
	//}

	//D3DXVECTOR3 GetCenter()
	//{
	//	D3DXVECTOR3 centerPoint;
	//	centerPoint.x = (this->Vertices[0].x + this->Vertices[2].x) / 2.0f;
	//	centerPoint.y = (this->Vertices[0].y + this->Vertices[2].y) / 2.0f;
	//	centerPoint.z = (this->Vertices[0].z + this->Vertices[2].z) / 2.0f;
	//	return centerPoint;
	//}
};