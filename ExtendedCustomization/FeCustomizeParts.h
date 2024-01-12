#pragma once
#include "FEString.h"
#include "Slots.h"
#include "Func.h"
#include "VectorScrollerMenu.h"

struct Fe2DScrollerMenu : VectorScrollerMenu
{
	VectorScroller SubOptionsScroller;

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

	void ResetSubOptions(ScrollOption* scrollOption, bool a)
	{
		FUNC(0x005C6400, void, __thiscall, _ResetSubOptions, Fe2DScrollerMenu*, ScrollOption*, bool);
		_ResetSubOptions(this, scrollOption, a);
	}

	void RefreshHeader()
	{
		FUNC(0x005A4EE0, void, __thiscall, _RefreshHeader, Fe2DScrollerMenu*);
		_RefreshHeader(this);
	}
};

struct FeCustomizeParts : Fe2DScrollerMenu
{
	BYTE unk1[0x3C];
	FEString* Header;
	BYTE unk2[0x8];
	bool IsAutosculpt;
	char WheelFilter;
	bool HoodCarbonFilter;
	bool SpoilerCarbonFilter;
	bool RoofScoopCarbonFilter;

	void AddMenuOption(Hash headerName, Slot slot, bool isAutosculpt, int filter)
	{
		FUNC(0x0085FE30, void, __thiscall, _AddMenuOption, FeCustomizeParts*, Hash, Slot, bool, int);
		_AddMenuOption(this, headerName, slot, isAutosculpt, filter);
	}

	void PopulateSubOptions(ScrollOption* scrollOption, Slot slot, bool isCarbon, bool isAutosculpt, char innerRadius, int initialOption)
	{
		FUNC(0x0085F9B0, void, __thiscall, _PopulateSubOptions, FeCustomizeParts*, ScrollOption*, Slot, bool, bool, char, int);
		_PopulateSubOptions(this, scrollOption, slot, isCarbon, isAutosculpt, innerRadius, initialOption);
	}
};

struct cFEng
{
	void QueuePackageMessage(Hash hash, char* package, int a1)
	{
		FUNC(0x00598540, void, __thiscall, _QueuePackageMessage, cFEng*, Hash, char*, int);
		_QueuePackageMessage(this, hash, package, a1);
	}

	static cFEng* Instance()
	{
		return *((cFEng**)0x00A97A78);
	}
};