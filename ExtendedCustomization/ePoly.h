#pragma once
#include "Math.h"
#include "Func.h"
#include "TextureInfo.h"

class ePoly
{
public:
	D3DXVECTOR4 Vertices[4];
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

	void Render(TextureInfo* textureInfo, D3DXMATRIX* matrix)
	{
		FUNC(0x0070F000, void, __thiscall, _Render, int, ePoly*, TextureInfo*, D3DXMATRIX*, int, int);
		_Render(0, this, textureInfo, matrix, 0, 0);
	}

	void SetColor(Color color)
	{
		for (int i = 0; i < 4; i++)
		{
			this->Colours[i] = color.Color;
		}
	}
};