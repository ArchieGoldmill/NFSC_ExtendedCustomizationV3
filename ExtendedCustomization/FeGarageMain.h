#pragma once
#include "RideInfo.h"
#include "FECustomizationRecord.h"

class FeGarageMain
{
public:
	void HandleSpecialCustomization(RideInfo* rideInfo, FECustomizationRecord* record)
	{
		static auto _HandleSpecialCustomization = (int(__thiscall*)(FeGarageMain*, RideInfo*, FECustomizationRecord*))0x008597F0;
		_HandleSpecialCustomization(this, rideInfo, record);
	}

	void InstallPart(RideInfo* rideInfo, FECustomizationRecord* record, Slot slot, bool setOnly, const char* format, ...)
	{
		static auto _InstallPart = (void(__cdecl*)(FeGarageMain*, RideInfo*, FECustomizationRecord*, Slot, bool setOnly, const char*, ...))0x0084F040;
		va_list args;
		va_start(args, format);
		_InstallPart(this, rideInfo, record, slot, setOnly, format, args);
		va_end(args);
	}
};