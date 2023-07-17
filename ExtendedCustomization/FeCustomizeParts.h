#pragma once
#include "FEString.h"
#include "Slots.h"
#include "Func.h"

class FeCustomizeParts
{
public:
	int unk1[0xB2];
	FEString* Header;
	int unk2[0x2];
	bool IsAutosculpt;
	char WheelFilter;

	void AddMenuOption(Hash headerName, Slot slot, bool isAutosculpt, int filter)
	{
		FUNC(0x0085FE30, void, __thiscall, _AddMenuOption, FeCustomizeParts*, Hash, Slot, bool, int);
		_AddMenuOption(this, headerName, slot, isAutosculpt, filter);
	}
};