#pragma once
#include "Feature.h"
#include "Game.h"
#include "CarPartDatabase.h"
#include "Hashes.h"
#include "Config.h"

struct UpgradeGroupAttribute
{
	Hash Name;
	unsigned char Level;
	unsigned char Slot;
};

void __stdcall AfterInitializeEverything()
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
					upgradeGroup->Slot = 0x68;
					upgradeGroup->Level = 32;
					break;
				}
			}
		}
	}

	Game::CarPartSlotMap[(int)Slot::DECAL_REAR_WINDOW_TEX0] = 0x68;
}

void __declspec(naked) AfterInitializeEverythingCave()
{
	__asm
	{
		call AfterInitializeEverything;
		ret;
	}
}

void InitDecals()
{
	if (g_Config.FixDecals)
	{
		injector::MakeJMP(0x006B7887, AfterInitializeEverythingCave);

		// Get decal texture from texture name rather then part name
		injector::WriteMemory<unsigned short>(0x007D5C35, 0x4277);
	}
}