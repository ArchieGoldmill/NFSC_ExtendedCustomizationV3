#pragma once
#include "FeCustomizeMainMenu.h"
#include "FeCustomizePartsMenu.h"
#include "PartList.h"
#include "FeCustomizeVisualsMenu.h"
#include "MenuCameras.h"
#include "CarbonSlot.h"

void InitMenu()
{
	InitFeCustomizeMainMenu();
	InitFeCustomizePartsMenu();
	InitFeCustomizeVisualsMenu();
	InitPartList();
	InitMenuCameras();
	InitCarbonSlot();

	if (g_Config.BodykitNames)
	{
		injector::WriteMemory<BYTE>(0x0085FAC2, 0xFF);
	}

	// Thanks to rx
	injector::WriteMemory(0x00865919, g_Config.PartPreviewDelay);
}