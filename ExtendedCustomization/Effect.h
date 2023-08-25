#pragma once
#include "Node.h"

struct Effect : bTNode<Effect>
{
	int pad[2];
	D3DXMATRIX LocalMatrix;
	void* AcidEffect;
	Hash Key;

	Effect(D3DXMATRIX* matrix)
	{
		this->LocalMatrix = *matrix;
		this->AcidEffect = 0;
		this->Key = 0;
	}

	void Update(int a1, Hash emitter, int a3, float a4, int a5)
	{
		sizeof(Effect);
		FUNC(0x007BF2A0, void, __thiscall, _Update, Effect*, int, int, int, float, int);
		_Update(this, a1, emitter, a3, a4, a5);
	}
};