#pragma once
#include "Func.h"
#include "FEObject.h"

struct FEWideString
{
	int mpsString;
	int mulBufferLength;
};

class FEString : public FEObject
{
public:
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
};