#pragma once
#include "RideInfo.h"
#include "FECustomizationRecord.h"

struct FeGarageMain
{
	void HandleSpecialCustomization(RideInfo* rideInfo, FECustomizationRecord* record)
	{
		static auto _HandleSpecialCustomization = (int(__thiscall*)(FeGarageMain*, RideInfo*, FECustomizationRecord*))0x008597F0;
		_HandleSpecialCustomization(this, rideInfo, record);
	}

	static inline auto InstallPart = (void(__cdecl*)(FeGarageMain*, RideInfo*, FECustomizationRecord*, Slot, bool setOnly, const char*, ...))0x0084F040;
};