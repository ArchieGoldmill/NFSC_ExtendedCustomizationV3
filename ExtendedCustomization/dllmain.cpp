#include <Windows.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Animations.h"
#include "Customization.h"
#include "Textures.h"
#include "Menu.h"
#include "ForceLodA.h"
#include "AutosculptOverrides.h"
#include "CarRenderInfoHooks.h"
#include "UserInput.h"
#include "Vinyls.h"
#include "Stance.h"
#include "ExhaustFX.h"
#include "Paints.h"
#include "RotorGlow.h"
#include "InitializeEverythingHooks.h"
#include "Wheels.h"
#include "LegacyTextures.h"
#include "LegacyMenu.h"
#include "LegacyCustomization.h"
#include "SteerAngle.h"
#include "Version.h"

void Init()
{
	InitVersion();

	InitConfig();
	SteerAngle = FESteerAngle();

	D3DXMatrixScaling(&CenterMarker, 1, 1, 1);

	InitAnimations();
	InitCustomization();
	InitTextures();
	InitUserInput();
	InitMenu();
	InitForceLodA();
	InitDecals();
	InitAutosculpt();
	InitNeon();
	InitVinyls();
	InitStance();
	InitExhaust();
	InitPaints();
	InitRotorGlow();
	InitWheels();

	Legacy::InitCustomization();
	Legacy::InitTextures();
	Legacy::InitMenu();

	InitCarRenderInfoHooks();
	InitInitializeEverythingHooks();
}

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x87E926) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			Init();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English nfsc.exe (6,88 MB (7.217.152 bytes)).", "Extended Customization", MB_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}