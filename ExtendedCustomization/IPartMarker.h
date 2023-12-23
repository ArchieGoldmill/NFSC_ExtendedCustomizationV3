#pragma once
#include "Math.h"
#include "Slots.h"
#include "CarRenderInfo.h"

class IPartMarker
{
protected:
	D3DXMATRIX state;
	Slot slot;
	CarRenderInfo* carRenderInfo;

public:
	virtual D3DXMATRIX* Get(D3DXMATRIX* matrix) = 0;
};

class IPartAnimation : public IPartMarker
{
protected:
	D3DXMATRIX* start;
	D3DXMATRIX* end;
	float amount;

public:
	virtual void Update() = 0;
	virtual bool IsSlot(Slot slot) = 0;
	virtual void SetTarget(float target) = 0;
	virtual void AddSubSlot(Slot slot) = 0;
	virtual D3DXMATRIX Calculate() = 0;

	bool IsEnded()
	{
		return this->amount == 1.0f;
	}
};