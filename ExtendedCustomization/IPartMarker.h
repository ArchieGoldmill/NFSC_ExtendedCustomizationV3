#pragma once
#include "Math.h"
#include "Slots.h"

class IPartMarker
{
protected:
	D3DXMATRIX state;
	Slot slot;

public:
	virtual D3DXMATRIX* Get(D3DXMATRIX* matrix) = 0;
};