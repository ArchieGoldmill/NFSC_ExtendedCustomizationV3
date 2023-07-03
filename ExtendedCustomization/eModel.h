#pragma once
#include "eSolid.h"
#include "Hashes.h"
#include "Func.h"

class eModel
{
public:
	eModel* Prev;
	eModel* Next;
	Hash NameHash;
	eSolid* Solid;

	void Render(int view, D3D::Matrix* matrix, void* light, int flags)
	{
		FUNC(0x00729320, void, __thiscall, _Render, int, eModel*, D3D::Matrix*, void*, int, int, int);
		_Render(view, this, matrix, light, flags, 0, 0);
	}
};