#pragma once
#include "Slots.h"
#include "Math.h"
#include "IPartMarker.h"

class PartMarker : public IPartMarker
{
private:
	D3DXMATRIX* marker;
	D3DXVECTOR3 scale;

public:
	PartMarker(Slot slot, D3DXMATRIX* marker, D3DXVECTOR3 scale)
	{
		this->slot = slot;
		this->marker = marker;
		this->scale = scale;
	}

	D3DXMATRIX* Get(D3DXMATRIX* matrix)
	{
		D3DXMATRIX scale;
		D3DXMatrixScaling(&scale, this->scale.x, this->scale.y, this->scale.z);
		D3DXMatrixMultiply(&this->state, &scale, this->marker);
		D3DXMatrixMultiply(&this->state, &this->state, matrix);

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