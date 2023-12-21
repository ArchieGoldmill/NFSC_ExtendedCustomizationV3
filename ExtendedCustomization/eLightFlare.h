#pragma once
#include "Node.h"
#include "Math.h"

struct eLightFlare : bTNode<eLightFlare>
{
	Hash NameHash;
	D3DCOLOR ColourTint;
	D3DXVECTOR3 Position;
	float ReflectPosZ;
	D3DXVECTOR3 Direction;
	int Type;
	int Flags;
};