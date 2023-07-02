#pragma once

class FECarRecord
{
public:
	static FECarRecord* Get()
	{
		FECarRecord** gFECarRecord = (FECarRecord**)0x00BBABAC;
		return *gFECarRecord;
	}

	int GetType()
	{
		static auto _GetType = (int(__thiscall*)(FECarRecord*))0x004AE150;
		return _GetType(this);
	}
};