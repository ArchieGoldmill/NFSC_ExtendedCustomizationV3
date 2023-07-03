#pragma once

class PVehicle
{
public:
	bool IsGlareOn(int num)
	{
		FUNC(0x006D8080, bool, __thiscall, _IsGlareOn, PVehicle*, int);
		return _IsGlareOn(this, num);
	}
};