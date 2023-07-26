#pragma once
#include "FeCustomizeMainMenu.h"
#include "FeCustomizePartsMenu.h"
#include "PartList.h"
#include "FeCustomizeVisualsMenu.h"
#include "MenuCameras.h"
#include "PartWarning.h"

void InitMenu()
{
	InitFeCustomizeMainMenu();
	InitFeCustomizePartsMenu();
	InitFeCustomizeVisualsMenu();
	InitPartList();
	InitMenuCameras();
	Legacy::InitPartWarning();

	if (g_Config.UnhardcodeBodykitNames)
	{
		injector::WriteMemory<BYTE>(0x0085FAC2, 0xFF);
	}
}