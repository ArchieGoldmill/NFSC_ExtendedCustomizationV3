#pragma once
#include "Feature.h"
#include "Decals.h"

void __stdcall AfterInitializeEverything()
{
	if (g_Config.FixDecals)
	{
		FixRearDecals();
	}

	//LoadResourceFile("CARS\\TIRES\\GEOMETRY.BIN", 0, 0, 0, 0, 0, 0);
	//LoadResourceFile("CARS\\TIRES\\TEXTURES.BIN", 6, 0, 0, 0, 0, 0);
}

void __declspec(naked) AfterInitializeEverythingCave()
{
	__asm
	{
		call AfterInitializeEverything;
		ret;
	}
}

void InitInitializeEverythingHooks()
{
	injector::MakeJMP(0x006B7887, AfterInitializeEverythingCave);
}