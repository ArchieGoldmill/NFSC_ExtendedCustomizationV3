#pragma once
#include "StandardSelectablePart.h"

struct AutosculptSelectablePart : StandardSelectablePart
{
	int Region;
	float* Zones;

	AutosculptSelectablePart(Slot slot, DBCarPart* part) : StandardSelectablePart(slot, part)
	{
		this->vTable = 0x009F9F48;
		this->Region = this->ConvertSlotToRegion(slot);
		this->Zones = 0;
	}

	int ConvertSlotToRegion(Slot slot)
	{
		auto _ConvertSlotToRegion = (int(__cdecl*)(Slot))0x00843180;
		return _ConvertSlotToRegion(slot);
	}

	void SetToCurrentPart()
	{
		FUNC(0x008434A0, void, __thiscall, _SetToCurrentPart, AutosculptSelectablePart*);
		_SetToCurrentPart(this);
	}

	static void GetPartsList(Slot slot, bNode<AutosculptSelectablePart*>* listHead, bool isCarbon, Hash brandName, int innerRadius)
	{
		FUNC(0x008534A0, void, __cdecl, _GetPartsList, Slot, bNode<AutosculptSelectablePart*>*, bool, Hash, int);
		_GetPartsList(slot, listHead, isCarbon, brandName, innerRadius);
	}

	static AutosculptSelectablePart* GetCurrent()
	{
		auto current = (AutosculptSelectablePart**)0x00BBB050;
		return *current;
	}
};