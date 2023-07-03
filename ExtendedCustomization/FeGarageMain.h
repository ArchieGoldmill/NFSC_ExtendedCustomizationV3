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
};