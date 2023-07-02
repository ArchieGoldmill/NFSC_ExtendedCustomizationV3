#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Func.h"

struct AutosculptData
{
	float Zones[11];
	int Modified;
};

class CarRenderInfo;
class RideInfo
{
public:
	int CarId;
	int unk1[285];
	int RenderUsage;
	int unk2;
	float RideHeight;
	int unk3[223];
	AutosculptData AutosculptData[0x14];
	int unk4[12];
	CarRenderInfo* CarRenderInfo;

	DBCarPart* GetPart(Slot slot)
	{
		static auto _GetPart = (DBCarPart * (__thiscall*)(RideInfo*, Slot))0x007B06F0;
		return _GetPart(this, slot);
	}

	void SetPart(Slot slot, DBCarPart* part, bool updatePartsEnabled = true)
	{
		static auto _SetPart = (void(__thiscall*)(RideInfo*, Slot, DBCarPart * part, bool updatePartsEnabled))0x007D67A0;
		_SetPart(this, slot, part, updatePartsEnabled);
	}

	char* GetCarTypeName()
	{
		static auto _GetCarTypeName = (char* (__cdecl*)(int))0x007B0290;
		return _GetCarTypeName(this->CarId);
	}
};