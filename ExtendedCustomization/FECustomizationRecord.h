#pragma once
#include "Game.h"

class FECustomizationRecord
{
public:
	static FECustomizationRecord* Get()
	{
		return (FECustomizationRecord*)0x00BBABC0;
	}

	bool SetInstalledPart(Slot slot, DBCarPart* part, CarType carId, bool setOnly)
	{
		static auto _SetInstalledPart = (bool(__thiscall*)(FECustomizationRecord*, Slot, DBCarPart*, CarType, bool))0x004BAC50;
		return _SetInstalledPart(this, slot, part, carId, setOnly);
	}

	DBCarPart* GetInstalledPart(CarType carId, Slot slot)
	{
		static auto _GetInstalledPart = (DBCarPart * (__thiscall*)(FECustomizationRecord*, CarType, Slot))0x004AE350;
		return _GetInstalledPart(this, carId, slot);
	}
};