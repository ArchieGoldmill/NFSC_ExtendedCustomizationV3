#pragma once
#include "Hashes.h"
#include "Func.h"
#include "PositionMarker.h"

class eSolid
{
public:
	char unk[0xA0];
	char Name[60];

	static eSolid* Find(Hash name)
	{
		FUNC(0x0055DF00, eSolid*, __cdecl, eFindSolid, Hash);
		return eFindSolid(name);
	}

	PositionMarker* GetPostionMarker(Hash name)
	{
		FUNC(0x00558D90, PositionMarker*, __thiscall, _GetPostionMarker, eSolid*, Hash);
		return _GetPostionMarker(this, name);
	}

	void GetBoundingBox(D3DXVECTOR3* a, D3DXVECTOR3* b);
};