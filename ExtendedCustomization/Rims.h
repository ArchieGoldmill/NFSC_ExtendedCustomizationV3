#pragma once
#include "Feature.h"
#include "Slots.h"
#include "Constants.h"

void InitRims()
{
	// Disable masking
	injector::MakeNOP(0x007B2C90, 5);
	injector::MakeNOP(0x007C358F, 5);

	// Remove rim texture replacememnts
	injector::MakeNOP(0x007D9BAA, 6);

	// Make all wheels roated instead of mirrored on the other side of the car
	char makeWheelsRotated[3] = { 0xB0, 0x00, 0x90 };
	injector::WriteMemoryRaw(0x007E585D, makeWheelsRotated, 3);

	// Make rear wheels installable
	injector::WriteMemory(0x007D6C76, 5, true);
	Game::CarPartSlotMap[(int)Slot::REAR_WHEEL] = 0x54;

	Game::AutosculptRegionList[ZoneRearWheel] = Slot::REAR_WHEEL;
}