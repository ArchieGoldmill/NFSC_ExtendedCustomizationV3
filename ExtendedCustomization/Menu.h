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

	// Unhardcode bodykit names
	injector::WriteMemory<BYTE>(0x0085FAC2, 0xFF);
}