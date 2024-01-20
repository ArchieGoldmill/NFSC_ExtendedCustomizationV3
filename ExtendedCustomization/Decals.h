#pragma once
#include "Feature.h"

void InitDecals()
{
	if (g_Config.FixDecals)
	{
		// Get decal texture from texture name rather then part name
		injector::WriteMemory<unsigned short>(0x007D5C35, 0x4277);
	}
}