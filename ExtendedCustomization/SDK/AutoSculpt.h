#pragma once
#include "Func.h"

class AutoSculpt
{
public:
	static int GetActiveZoneNum(int a1)
	{
		FUNC(0x007B9AE0, int, __stdcall, _GetActiveZoneNum, int);
		return _GetActiveZoneNum(a1);
	}
};