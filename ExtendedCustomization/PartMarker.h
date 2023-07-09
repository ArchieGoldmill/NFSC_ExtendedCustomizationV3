#pragma once
#include "Slots.h"
#include "D3DWrapper.h"
#include "IPartMarker.h"

class PartMarker : public IPartMarker
{
private:
	D3D::Matrix* marker;
	
public:
	PartMarker(Slot slot, D3D::Matrix* marker)
	{
		this->slot = slot;
		this->marker = marker;
	}

	D3D::Matrix* Get(D3D::Matrix* matrix)
	{
		D3D::Matrix::Multiply(&this->state, marker, matrix);

		return &this->state;
	}

	bool IsSlot(Slot slot)
	{
		if (this->slot == slot)
		{
			return true;
		}

		return false;
	}
};