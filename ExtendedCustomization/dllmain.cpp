#include <Windows.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Config.h"

// Features
#include "Animations.h"
#include "Customization.h"
#include "Textures.h"
#include "Menu.h"

// Hooks
#include "CarRenderInfoHooks.h"
#include "UserInput.h"

void Init()
{
	InitConfig();

	InitAnimations();
	InitCustomization();
	InitTextures();
	InitUserInput();
	InitMenu();

	InitCarRenderInfoHooks();
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
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English nfsc.exe (6,88 MB (7.217.152 bytes)).", "NFSC - Style Point", MB_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}