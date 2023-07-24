#pragma once
#include "Func.h"
#include "FEObject.h"

class FEButton :public FEObject
{
public:
	static char GetLastButton(char* packageFilename)
	{
		FUNC(0x00571300, int, __cdecl, _GetLastButton, char*);
		return _GetLastButton(packageFilename);
	}
};