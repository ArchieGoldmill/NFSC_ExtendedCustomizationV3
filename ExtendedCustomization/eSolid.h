#pragma once
#include "Hashes.h"
#include "Func.h"
#include "PositionMarker.h"

struct eSolid;

struct eMorphTarget
{
	Hash NameHash;
	eSolid* Target;
	float BlendAmnt;
};

struct eSolid
{
	char unk[0x20];
	D3DXVECTOR3 BoundingBoxMin;
	void* TextureTable;
	D3DXVECTOR3 BoundingBoxMax;
	char unk_1[0x54];
	eMorphTarget* MorphTargetList;
	char unk_2[0xC];
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

	void PoseMorph()
	{
		FUNC(0x00558E10, void, __thiscall, _PoseMorph, eSolid*);
		_PoseMorph(this);
	}

	void GetBoundingBox(D3DXVECTOR3* a, D3DXVECTOR3* b);
};