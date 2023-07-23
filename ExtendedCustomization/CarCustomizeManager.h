#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Func.h"

class CarCustomizeManager
{
public:
	DBCarPart* GetInstalledPart(Slot slot)
	{
		FUNC(0x0083F850, DBCarPart*, __thiscall, _GetInstalledPart, CarCustomizeManager*, Slot);
		return _GetInstalledPart(this, slot);
	}

	int* IsPartTypeInCart(Slot slot)
	{
		FUNC(0x00850190, int*, __thiscall, _IsPartTypeInCart, CarCustomizeManager*, Slot);
		return _IsPartTypeInCart(this, slot);
	}

	DBCarPart* GetCurrentPart(Slot slot)
	{
		auto cartPart = this->IsPartTypeInCart(slot);
		if (cartPart)
		{
			auto listPart = (StandardSelectablePart*)cartPart[3];
			return listPart->Part;
		}

		return this->GetInstalledPart(slot);
	}

	static CarCustomizeManager* Get()
	{
		return (CarCustomizeManager*)0x00BBABA8;
	}
};