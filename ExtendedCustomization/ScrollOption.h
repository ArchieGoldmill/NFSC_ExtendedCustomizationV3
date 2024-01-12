#pragma once
#include "FEObject.h"

struct ScrollOption
{
	int vTable;
	ScrollOption* Prev;
	ScrollOption* Next;
	Hash ObjHash;
	FEObject* FEngObject;
	float XPos;
	float YPos;
	int OriginalColor;
	bool IsGreyOut;
	bool IsFlashable;
	bool Locked;
	bool bClipped;
	float OrigWidth;
	float OrigHeight;
	Hash NameHash;
	Hash DescHash;
	int bIsTutorialAvailable;
	char* pTutorialMovieName;
	int ScrollType;
	int m_bEnabled;
	int m_SubType;
};

struct TextOption : ScrollOption
{
	FEObject* String;
	FEObject* Image;
	int Sytle;
	char Text[64];
	bool ShowImage;

	static TextOption* Create(Hash text, int num)
	{
		auto textOption = (TextOption*)j_malloc_0(0x98);
		static auto _TextOption = (void(__thiscall*)(TextOption*, Hash, int, char*, char*))0x005869C0;
		char a = 1, b = 0;
		_TextOption(textOption, text, num, &a, &b);
		return textOption;
	}
};

struct CarPartTextOption : TextOption
{
	StandardSelectablePart* SelectablePart;
};