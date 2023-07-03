#pragma once
#include "Game.h"

class FECarRecord
{
public:
	static FECarRecord* Get()
	{
		FECarRecord** gFECarRecord = (FECarRecord**)0x00BBABAC;
		return *gFECarRecord;
	}

	CarType GetType()
	{
		static auto _GetType = (CarType(__thiscall*)(FECarRecord*))0x004AE150;
		return _GetType(this);
	}
};