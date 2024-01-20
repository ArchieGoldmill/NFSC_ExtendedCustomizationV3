#pragma once
#include "eSolid.h"
#include "Hashes.h"
#include "Func.h"
#include "eLightMaterial.h"
#include "ReplacementTextureEntry.h"
#include "Node.h"
#include "eView.h"

struct eModel : bNode<eModel>
{
	Hash NameHash;
	eSolid* Solid;
	ReplacementTextureEntry* ReplacementTextureTable;
	short ReplacementTextureCount;
	short Pad;

	eModel()
	{
		FUNC(0x00412230, void, __thiscall, _ctor, eModel*);
		_ctor(this);
	}

	void Render(eView* view, D3DXMATRIX* matrix, void* light, int flags)
	{
		FUNC(0x00729320, void, __thiscall, _Render, eView*, eModel*, D3DXMATRIX*, void*, int, int, int);
		_Render(view, this, matrix, light, flags, 0, 0);
	}

	void GetBoundingBox(D3DXVECTOR3* a, D3DXVECTOR3* b)
	{
		FUNC(0x005589C0, void, __thiscall, _GetBoundingBox, eModel*, D3DXVECTOR3*, D3DXVECTOR3*);
		_GetBoundingBox(this, a, b);
	}

	void ReplaceLightMaterial(Hash materialName)
	{
		this->ReplaceLightMaterial(materialName, eLightMaterial::Get(materialName, 0));
	}

	void ReplaceLightMaterial(Hash materialName, eLightMaterial* newMaterial)
	{
		FUNC(0x0055C0B0, void, __thiscall, _ReplaceLightMaterial, eModel*, Hash, eLightMaterial*);
		_ReplaceLightMaterial(this, materialName, newMaterial);
	}

	void AttachReplacementTextureTable(ReplacementTextureEntry* replaceNodes, int count)
	{
		FUNC(0x005588D0, void, __thiscall, _AttachReplacementTextureTable, eModel*, ReplacementTextureEntry*, int, int);
		_AttachReplacementTextureTable(this, replaceNodes, count, 0);
	}

	void Init(Hash hash)
	{
		FUNC(0x0055E7E0, void, __thiscall, _Init, eModel*, Hash);
		_Init(this, hash);
	}

	void UnInit()
	{
		FUNC(0x0055BF70, void, __thiscall, _Init, eModel*);
		_Init(this);
	}
};