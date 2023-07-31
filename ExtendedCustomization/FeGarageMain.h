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

	static void UnInstall(RideInfo* rideInfo, FECustomizationRecord* record, Slot slot)
	{
		FUNC(0x0083EEB0, void, __stdcall, _UnInstall, RideInfo*, FECustomizationRecord*, Slot);
		_UnInstall(rideInfo, record, slot);
	}
};