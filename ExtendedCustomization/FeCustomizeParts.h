#pragma once
#include "FEString.h"
#include "Slots.h"
#include "Func.h"

struct Fe2DScrollerMenu
{
	void SetInitialOption(int i)
	{
		FUNC(0x005C6560, void, __thiscall, _SetInitialOption, Fe2DScrollerMenu*, int);
		_SetInitialOption(this, i);
	}
	
	void FinishAddingOptions()
	{
		FUNC(0x005C65C0, void, __thiscall, _FinishAddingOptions, Fe2DScrollerMenu*);
		_FinishAddingOptions(this);
	}
};

struct FeCustomizeParts : Fe2DScrollerMenu
{
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