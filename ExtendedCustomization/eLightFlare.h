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

struct eLightFlareParams
{
	float MinSize;
	float MaxSize;
	float MaxColour[4];
	float Power;
	float ZBias;
	float MinScale;
	float MaxScale;
	int TextureID;

	static inline eLightFlareParams** List = (eLightFlareParams**)0x00A6BF88;
};