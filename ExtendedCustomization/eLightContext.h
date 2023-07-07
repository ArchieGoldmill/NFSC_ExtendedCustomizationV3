#pragma once
#include <d3dx9.h>
#include "Game.h"

class eLightContext
{
private:
	float data[0x74];

public:
	eLightContext()
	{
		memset(this, 0, 0x1D0);
	}
};