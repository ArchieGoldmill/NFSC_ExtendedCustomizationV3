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

	void Update(D3DXMATRIX* matrix, VltHash emitter, float a3, float intensity, D3DXVECTOR3* velocity)
	{
		sizeof(Effect);
		FUNC(0x007BF2A0, void, __thiscall, _Update, Effect*, D3DXMATRIX*, VltHash, float, float, D3DXVECTOR3*);
		_Update(this, matrix, emitter, a3, intensity, velocity);
	}
};