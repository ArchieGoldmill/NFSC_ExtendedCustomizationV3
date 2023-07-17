#pragma once
#include "D3DWrapper.h"
#include "Slots.h"

class IPartMarker
{
protected:
	D3D::Matrix state;
	Slot slot;

public:
	virtual D3D::Matrix* Get(D3D::Matrix* matrix) = 0;
};