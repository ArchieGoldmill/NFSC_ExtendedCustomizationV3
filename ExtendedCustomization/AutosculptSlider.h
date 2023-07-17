#pragma once
#include "Hashes.h"
#include "Func.h"

class AutosculptSlider
{
public:
	int Act(int a2, Hash action)
	{
		FUNC(0x0085B020, int, __thiscall, _Act, AutosculptSlider*, int, Hash);
		return _Act(this, a2, action);
	}
};