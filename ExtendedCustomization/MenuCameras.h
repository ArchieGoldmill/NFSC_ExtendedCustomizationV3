#pragma once
#include "Feature.h"
#include "FeCustomizeParts.h"
#include "Slots.h"
#include "Config.h"
#include "FrontEndRenderingCar.h"

void __stdcall GetCameraScreenName(Slot slot, int maxChars, char* screenName)
{
	auto carId = FrontEndRenderingCar::GetCarId();
	auto part = g_Config.GetPart(slot, carId);
	strcpy_s(screenName, maxChars, part.Camera.c_str());
}

void __declspec(naked) GetCameraScreenNameCave()
{
	__asm
	{
		mov ecx, [ecx + 0x30];
		push[esp + 4];
		push eax;
		push ecx;
		call GetCameraScreenName;

		ret 8;
	}
}

void InitMenuCameras()
{
	injector::MakeJMP(0x00840073, GetCameraScreenNameCave);
}