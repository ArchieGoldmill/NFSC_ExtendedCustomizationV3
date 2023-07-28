#pragma once
#include "Feature.h"
#include "Game.h"
#include "CarPartDatabase.h"
#include "Hashes.h"
#include "Config.h"
#include "Func.h"

void FixRearDecals()
{
	DBCarPart* part = NULL;
	while (true)
	{
		part = CarPartDatabase::Instance->GetCarPart(Slot::DECAL_FRONT_WINDOW_TEX0, CarType(-1), part);
		if (part)
		{
			auto upgradeGroup = (UpgradeGroupAttribute*)part->GetAppliedAttributeParam(Hashes::PARTID_UPGRADE_GROUP);
			if (upgradeGroup)
			{
				if (upgradeGroup->Level == 33)
				{
					// Since all parts share the same reference to attribute we can change it once.
					upgradeGroup->Part = 0x68;
					upgradeGroup->Level = 32;
					break;
				}
			}
		}
	}

	Game::CarPartSlotMap[(int)Slot::DECAL_REAR_WINDOW_TEX0] = 0x68;
}

int __stdcall sub_714820(eSolid* solid, int a1, D3DXVECTOR3* a2, int a3)
{
	D3DXVECTOR3 newBounds[2];
	if (strstr(solid->Name, "DECAL_FRONT_WINDOW"))
	{
		newBounds[0] = a2[0];
		newBounds[1] = a2[1];
		newBounds[1].x += 0.5;
		newBounds[1].z += 0.1;

		a2 = newBounds;
	}

	if (strstr(solid->Name, "DECAL_REAR_WINDOW"))
	{
		newBounds[0] = a2[0];
		newBounds[1] = a2[1];
		newBounds[0].x -= 0.5;
		newBounds[1].z += 0.1;

		a2 = newBounds;
	}

	FUNC(0x714820, int, __cdecl, _sub_714820, int, D3DXVECTOR3*, int);
	return _sub_714820(a1, a2, a3);
}

void __declspec(naked) sub_714820_cave()
{
	static constexpr auto cExit = 0x00727A64;
	__asm
	{
		push[esp + 0x20];
		call sub_714820;
		jmp cExit;
	}
}

void InitDecals()
{
	if (g_Config.FixDecals)
	{
		// Get decal texture from texture name rather then part name
		injector::WriteMemory<unsigned short>(0x007D5C35, 0x4277);
	}

	if (g_Config.FixDecalsZOrder)
	{
		injector::MakeJMP(0x00727A5C, sub_714820_cave);
	}
}