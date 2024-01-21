#pragma once
#include "Feature.h"

void InitDecals()
{
	if (g_Config.FixDecals)
	{
		// Get decal texture from texture name rather then part name
		injector::WriteMemory<unsigned short>(0x007D5C35, 0x4277);

		Game::CarPartSlotMap[(int)Slot::DECAL_REAR_WINDOW_TEX0] = 0x68;
	}
}