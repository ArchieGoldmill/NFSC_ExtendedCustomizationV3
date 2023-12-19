#pragma once
#include "Slots.h"
#include "Func.h"
#include "Game.h"
#include "ColorData.h"
#include "Constants.h"
#include "Math.h"
#include "AutoSculpt.h"
#include "VinylManager.h"

struct CarRenderInfo;
struct RideInfo
{
	CarType CarId;
	int unk1[17];
	Hash SkinHash;
	Hash WheelHash;
	Hash SpinnerHash;
	DBCarPart* Parts[(int)Slot::COUNT];
	bool PartsEnabled[(int)Slot::COUNT];
	unsigned short unk2[5];
	ColorMaterialData BodyMaterial;
	ColorMaterialData WheelMaterial;
	int unk3[2];
	int RenderUsage;
	int unk4;
	float RideHeight;
	RIVinyl Vinyls[20];
	int VinylMirror;
	BYTE VinylNum;
	BYTE unk5[3];
	AutoSculpt Autosculpt;
	CarRenderInfo* pCarRenderInfo;

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
		static auto _GetCarTypeName = (char* (__cdecl*)(CarType))0x007B0290;
		return _GetCarTypeName(this->CarId);
	}

	Hash GetCarTypeHash()
	{
		return StringHash(this->GetCarTypeName());
	}

	DBCarPart* GetVinylPart(int num)
	{
		static auto _GetVinylPart = (DBCarPart * (__thiscall*)(RideInfo*, int))0x007B0870;
		return _GetVinylPart(this, num);
	}

	ASColorData* GetCustomPaintData(int num)
	{
		if (num == 0 || num == 1)
		{
			auto as = (ASColorData*)&this->Autosculpt.Regions[ZoneColor1];
			return as + num;
		}
		if (num == 2 || num == 3)
		{
			auto as = (ASColorData*)&this->Autosculpt.Regions[ZoneColor2];
			return as + (num - 2);
		}
		if (num == 4 || num == 5)
		{
			auto as = (ASColorData*)&this->Autosculpt.Regions[ZoneColor3];
			return as + (num - 4);
		}

		return NULL;
	}

	bool IsPlayer()
	{
		return this->RenderUsage == 0;
	}

	void SetRandomParts(int a)
	{
		FUNC(0x007E1880, void, __thiscall, _SetRandomParts, RideInfo*, int);
		_SetRandomParts(this, a);
	}

	void SetRandomPart(Slot slot)
	{
		FUNC(0x007DA970, void, __thiscall, _SetRandomPart, RideInfo*, Slot, Hash);
		_SetRandomPart(this, slot, -1);
	}
};