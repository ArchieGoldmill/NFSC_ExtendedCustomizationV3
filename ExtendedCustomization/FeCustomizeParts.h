#pragma once
#include "FEString.h"
#include "Slots.h"
#include "Func.h"

class FeCustomizeParts
{
public:
	BYTE unk1[0x2C8];
	FEString* Header;
	BYTE unk2[0x8];
	bool IsAutosculpt;
	char WheelFilter;

	void AddMenuOption(Hash headerName, Slot slot, bool isAutosculpt, int filter)
	{
		FUNC(0x0085FE30, void, __thiscall, _AddMenuOption, FeCustomizeParts*, Hash, Slot, bool, int);
		_AddMenuOption(this, headerName, slot, isAutosculpt, filter);
	}
};