#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Func.h"

struct CarCustomizeManager
{
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

	void HandleCart(int* carType, StandardSelectablePart* selectablePart)
	{
		FUNC(0x00850370, void, __thiscall, _HandleCart, CarCustomizeManager*, int*, StandardSelectablePart*);
		_HandleCart(this, carType, selectablePart);
	}

	bool IsInstalledPartCarbon(Slot slot)
	{
		FUNC(0x0083F650, bool, __thiscall, _IsInstalledPartCarbon, CarCustomizeManager*, Slot);
		return _IsInstalledPartCarbon(this, slot);
	}

	static CarCustomizeManager* Instance()
	{
		return (CarCustomizeManager*)0x00BBABA8;
	}
};