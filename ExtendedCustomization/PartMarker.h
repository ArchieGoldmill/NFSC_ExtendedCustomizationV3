#pragma once
#include "Slots.h"
#include "D3DWrapper.h"
#include "IPartMarker.h"

class PartMarker : public IPartMarker
{
private:
	D3D::Matrix* marker;
	D3DXVECTOR3 scale;
	
public:
	PartMarker(Slot slot, D3D::Matrix* marker, D3DXVECTOR3 scale)
	{
		this->slot = slot;
		this->marker = marker;
		this->scale = scale;
	}

	D3D::Matrix* Get(D3D::Matrix* matrix)
	{
		auto scale = D3D::Matrix::FromScale(this->scale);
		D3D::Matrix::Multiply(&this->state, &scale, this->marker);
		D3D::Matrix::Multiply(&this->state, &this->state, matrix);

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