#pragma once
#include "Slots.h"
#include "Math.h"
#include "IPartMarker.h"
#include "CarRenderInfo.h"
#include "DBCarPart.h"

class PartMarker : public IPartMarker
{
protected:
	D3DXMATRIX* marker;
	D3DXVECTOR3 scale;
	Slot attachTo;

public:
	PartMarker(Slot slot, Slot attachTo, D3DXMATRIX* marker, D3DXVECTOR3 scale, CarRenderInfo* carRenderInfo)
	{
		this->slot = slot;
		this->attachTo = attachTo;
		this->marker = marker;
		this->scale = scale;
		this->carRenderInfo = carRenderInfo;
	}

	D3DXMATRIX* Get(D3DXMATRIX* matrix);

	bool IsSlot(Slot slot)
	{
		if (this->slot == slot)
		{
			return true;
		}

		return false;
	}
};