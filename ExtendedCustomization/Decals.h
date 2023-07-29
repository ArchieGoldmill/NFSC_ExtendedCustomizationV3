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
			auto attr = part->GetAppliedAttributeParam<UpgradeGroup>(Hashes::PARTID_UPGRADE_GROUP);
			if (attr)
			{
				if (attr->Value.Level == 33)
				{
					// Since all parts share the same reference to attribute we can change it once.
					attr->Value.Part = 0x68;
					attr->Value.Level = 32;
					break;
				}
			}
		}
	}

	Game::CarPartSlotMap[(int)Slot::DECAL_REAR_WINDOW_TEX0] = 0x68;
}

void InitDecals()
{
	if (g_Config.FixDecals)
	{
		// Get decal texture from texture name rather then part name
		injector::WriteMemory<unsigned short>(0x007D5C35, 0x4277);
	}
}