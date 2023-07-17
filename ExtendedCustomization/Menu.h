#pragma once
#include "FeCustomizeMainMenu.h"
#include "FeCustomizePartsMenu.h"
#include "PartList.h"

void InitMenu()
{
	InitFeCustomizeMainMenu();
	InitFeCustomizePartsMenu();
	InitPartList();

	// Unhardcode bodykit names
	injector::WriteMemory<BYTE>(0x0085FAC2, 0xFF);
}