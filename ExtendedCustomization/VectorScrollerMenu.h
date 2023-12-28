#pragma once
#include "ScrollOption.h"
#include "FEString.h"
#include "FEPackage.h"

class VectorScroller
{
private:
	int vTable;
public:
	ScrollOption* Next;
	ScrollOption* Prev;
	ScrollOption* Current;
private:
	int unk1[0x46];
};

struct ScreenConstructorData
{
	char* PackageFilename;
	FEPackage* pPackage;
	int Arg;
};

class MenuScreen
{
private:
	int vTable;
public:
	int mPlaySound;
	int mDirectionForNextSound;
	char* PackageFilename;
	ScreenConstructorData ConstructData;
	int IsGarageScreen;
private:
	int data[2];
};

class VectorScrollerMenu : public MenuScreen
{
public:
	VectorScroller Options;
	int bWasLeftMouseDown;
	int pLastSelection;
	int pTitleName;
	int pTitleNameShadow;
	FEString* pTitleDesc;

	void* AddOption(TextOption* textOption)
	{
		FUNC(0x005B2320, void*, __thiscall, _AddOption, VectorScrollerMenu*, TextOption*);
		return _AddOption(this, textOption);
	}

	void RefreshHeader()
	{
		static auto _RefreshHeader = (void(__thiscall*)(VectorScrollerMenu*))0x005A32C0;
		_RefreshHeader(this);
	}

	void FinishAddingOptions()
	{
		static auto _FinishAddingOptions = (void(__thiscall*)(VectorScrollerMenu*))0x005C5EF0;
		_FinishAddingOptions(this);
	}

	void SetTitleName(Hash title)
	{
		static auto _SetTitleName = (void(__thiscall*)(VectorScrollerMenu*, Hash))0x005A3710;
		_SetTitleName(this, title);
	}

	void SetInitialOption(int n)
	{
		static auto _SetInitialOption = (void(__thiscall*)(VectorScrollerMenu*, int))0x005C5ED0;
		_SetInitialOption(this, n);
	}
};