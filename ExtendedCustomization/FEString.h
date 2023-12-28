#pragma once
#include "Func.h"
#include "FEObject.h"

struct FEWideString
{
	int mpsString;
	int mulBufferLength;
};

struct FEString :  FEObject
{
	int pLabelName;
	Hash LabelHash;
	FEWideString WideString;
	int Format;
	int Leading;
	int MaxWidth;

	void SetLanguageHash(Hash hash)
	{
		FUNC(0x00571080, void, __cdecl, _SetLanguageHash, FEString*, Hash);
		_SetLanguageHash(this, hash);
	}

	FEString* Clone()
	{
		static auto _Clone = (FEString * (__thiscall*)(FEString*, int))0x005FF6D0;
		return _Clone(this, 0);
	}
};