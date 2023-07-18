#pragma once
#include "ColorData.h"
#include "Func.h"

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

	void WriteColor(int colorNum, RecordColorData hs, DBCarPart* part1, DBCarPart* part2)
	{
		FUNC(0x0049C5C0, void, __thiscall, _WriteColor, FECustomizationRecord*, int, RecordColorData, DBCarPart*, DBCarPart*);
		_WriteColor(this, colorNum, hs, part1, part2);
	}

	void GetColor(int colorNum, ColorData* paintData)
	{
		FUNC(0x0049C540, void, __thiscall, _GetColor, FECustomizationRecord*, int, ColorData*);
		_GetColor(this, colorNum, paintData);
	}

	ASColorData* GetASColorData(int num)
	{
		auto as = ((char*)this) + 0xCC;
		if (num == 0 || num == 1)
		{
			auto data = (ASColorData*)(as + ZoneColor1 * 44);
			return data + num;
		}
		if (num == 2 || num == 3)
		{
			auto data = (ASColorData*)(as + ZoneColor2 * 44);
			return data + (num - 2);
		}
		if (num == 4 || num == 5)
		{
			auto data = (ASColorData*)(as + ZoneColor3 * 44);
			return data + (num - 4);
		}

		return 0;
	}
};