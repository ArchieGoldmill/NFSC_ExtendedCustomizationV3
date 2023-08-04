#pragma once
#include "FeCustomizeMainMenu.h"
#include "FeCustomizePartsMenu.h"
#include "PartList.h"
#include "FeCustomizeVisualsMenu.h"
#include "MenuCameras.h"


void InitMenu()
{
	InitFeCustomizeMainMenu();
	InitFeCustomizePartsMenu();
	InitFeCustomizeVisualsMenu();
	InitPartList();
	InitMenuCameras();

	if (g_Config.UnhardcodeBodykitNames)
	{
		injector::WriteMemory<BYTE>(0x0085FAC2, 0xFF);
	}

	// Thanks to rx
	injector::WriteMemory(0x00865919, g_Config.PreviewDebounceTime);
}