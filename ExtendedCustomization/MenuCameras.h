#pragma once
#include "Feature.h"
#include "FeCustomizeParts.h"
#include "Slots.h"
#include "Config.h"
#include "FERenderingCar.h"
#include "SteerAngle.h"
#include "AutosculptSelectablePart.h"

void __stdcall GetCameraScreenName(Slot slot, int maxChars, char* screenName)
{
	if (slot == Slot_Tires)
	{
		SteerAngle.SetTarget(-30);
	}
	else if (slot == Slot::FRONT_WHEEL)
	{
		SteerAngle.SetTarget(30);
	}
	else
	{
		SteerAngle.Reset();
	}

	auto carId = FERenderingCar::GetCarId();
	auto part = g_Config.GetPart(slot, carId);
	strcpy_s(screenName, maxChars, part.Camera.c_str());
}

void __cdecl GetCameraScreenNameAS(char* dest, const char* camera, int count)
{
	SteerAngle.Reset();

	if (strcmp(camera, "FeAutosculptFrontBumper.fng") == 0)
	{
		auto part = AutosculptSelectablePart::GetCurrent();
		if (part)
		{
			auto partConfig = g_Config.GetPart(part->SlotId, FERenderingCar::GetCarId());
			if (partConfig.Camera.size())
			{
				strcpy_s(dest, count, partConfig.Camera.c_str());
				return;
			}
		}
	}

	strcpy_s(dest, count, camera);
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
	injector::MakeCALL(0x0084EB3A, GetCameraScreenNameAS, true);
}