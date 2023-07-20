#pragma once
#include <string>
#include "Feature.h"
#include "Slots.h"
#include "Constants.h"
#include "AutosculptSelectablePart.h"
#include "AutoSculpt.h"
#include "AutosculptSlider.h"
#include "FrontEndRenderingCar.h"
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"

const char* __stdcall ConvertSlotToFng(Slot slot)
{
	const char* result;

	switch (slot)
	{
	case Slot::EXHAUST:
		result = "FeAutosculptExhaust.fng";
		break;
	case Slot::SPOILER:
		result = "FeAutosculptSpoiler.fng";
		break;
	case Slot::FRONT_BUMPER:
	case Slot::LICENSE_PLATE:
	case Slot_Stance:
	case Slot_Neon:
		result = "FeAutosculptFrontBumper.fng";
		break;
	case Slot::REAR_BUMPER:
		result = "FeAutosculptRearBumper.fng";
		break;
	case Slot::ROOF:
		result = "FeAutosculptTop.fng";
		break;
	case Slot::ROOFSCOOP:
		result = "FeAutosculptRoof.fng";
		break;
	case Slot::HOOD:
		result = "FeAutosculptHood.fng";
		break;
	case Slot::SKIRT:
		result = "FeAutosculptSkirt.fng";
		break;
	case Slot::FRONT_WHEEL:
		result = "FeAutosculptWheel.fng";
		break;
	case Slot::REAR_WHEEL:
		result = "FeAutosculptWheel.fng";
		break;
	default:
		result = 0;
		break;
	}

	return result;
}

int __fastcall ConvertSlotToRegion(Slot slot)
{
	int result;

	switch (slot)
	{
	case Slot::EXHAUST:
		result = ZoneExhaust;
		break;
	case Slot::SPOILER:
		result = ZoneSpoiler;
		break;
	case Slot::FRONT_BUMPER:
		result = ZoneFrontBumper;
		break;
	case Slot::REAR_BUMPER:
		result = ZoneRearBumper;
		break;
	case Slot::ROOF:
		result = ZoneRoof;
		break;
	case Slot::ROOFSCOOP:
		result = ZoneRoofScoop;
		break;
	case Slot::HOOD:
		result = ZoneHood;
		break;
	case Slot::SKIRT:
		result = ZoneSkirt;
		break;
	case Slot::FRONT_WHEEL:
		result = ZoneFrontWheel;
		break;
	case Slot::REAR_WHEEL:
		result = ZoneRearWheel;
		break;
	case Slot::LICENSE_PLATE:
		result = ZoneLicenseplate;
		break;
	case Slot_Stance:
		result = ZoneStance;
		break;
	case Slot_Neon:
		result = ZoneNeon;
		break;
	default:
		result = 21;
		break;
	}

	return result;
}

int __fastcall ConvertSlotToState(Slot partId)
{
	int result;

	switch (partId)
	{
	case Slot::EXHAUST:
		result = 7;
		break;
	case Slot::SPOILER:
		result = 10;
		break;
	case Slot::FRONT_BUMPER:
	case Slot::LICENSE_PLATE:
	case Slot_Stance:
	case Slot_Neon:
		result = 4;
		break;
	case Slot::REAR_BUMPER:
		result = 5;
		break;
	case Slot::ROOF:
		result = 12;
		break;
	case Slot::ROOFSCOOP:
		result = 9;
		break;
	case Slot::HOOD:
		result = 8;
		break;
	case Slot::SKIRT:
		result = 6;
		break;
	case Slot::FRONT_WHEEL:
		result = 11;
		break;
	case Slot::REAR_WHEEL:
		result = 11;
		break;
	default:
		result = -1;
		break;
	}

	return result;
}

int __stdcall GetActiveZoneNum(int param)
{
	auto current = AutosculptSelectablePart::GetCurrent();
	if (current)
	{
		auto slot = current->SlotId;
		if (slot == Slot_Stance || slot == Slot_Neon || slot == Slot::LICENSE_PLATE)
		{
			return current->Part->GetAppliedAttributeIParam(Hashes::MORPHTARGET_NUM, 0);
		}
	}

	return AutoSculpt::GetActiveZoneNum(param);
}

int __fastcall AutosculptSlider_Act(AutosculptSlider* slider, int, int a2, Hash action)
{
	int result = slider->Act(a2, action);

	auto currentSelectablePart = AutosculptSelectablePart::GetCurrent();
	if (currentSelectablePart)
	{
		auto slot = currentSelectablePart->SlotId;
		auto carRenderInfo = FrontEndRenderingCar::Get()->RideInfo.CarRenderInfo;

		if (slot == Slot_Stance)
		{
			carRenderInfo->UpdateWheelYRenderOffset();
		}

		if (slot == Slot::LICENSE_PLATE)
		{
			carRenderInfo->Extras->Textures.UpdateLicensePlate();
		}
	}

	return result;
}

Hash __cdecl ZoneName(char* str, int num)
{
	auto current = AutosculptSelectablePart::GetCurrent();
	if (current)
	{
		Hash zoneHash = current->Part->GetAppliedAttributeIParam(Hashes::ZONE_HASH, 0);
		if (zoneHash)
		{
			return StringHash1(std::to_string(num).c_str(), zoneHash);
		}
	}

	return FromIndex(str, num);
}

void __fastcall InitAutosculptSlider(int* _this, int param, float a2, float max, int a4, float amount)
{
	static auto cSlider_InitValues = (void(__thiscall*)(int*, float, float, int, float))0x005733E0;
	cSlider_InitValues(_this, a2, max, a4, amount);
	auto current = AutosculptSelectablePart::GetCurrent();
	if (current)
	{
		int num = _this[0x12] + 1;
		float* _fthis = (float*)_this;

		float amount = current->Part->GetAppliedAttributeFParam(Hashes::ZONE_AMOUNT, 1.0f);
		amount = current->Part->GetAppliedAttributeFParam(FromIndex("ZONE_AMOUNT%d", num), amount);

		_fthis[0x8] = amount;
		_fthis[0x15] = current->Part->GetAppliedAttributeFParam(FromIndex("ZONE_STEP%d", num), 0.01f);
	}
}

void __declspec(naked) ConvertSlotToStateCave()
{
	__asm
	{
		SAVE_REGS_EAX;
		mov ecx, [esp + 0x18];
		call ConvertSlotToState;
		RESTORE_REGS_EAX;
		ret 4;
	}
}

void __declspec(naked) ConvertSlotToRegionCave()
{
	__asm
	{
		SAVE_REGS_EAX;
		mov ecx, [esp + 0x18];
		call ConvertSlotToRegion;
		RESTORE_REGS_EAX;
		ret;
	}
}

void __declspec(naked) FixZoneLoadCave()
{
	static constexpr auto hExit = 0x007C4479;
	static constexpr auto hMult = 100.0f;
	static constexpr auto hAdd = 0.5f;

	__asm
	{
		fmul hMult;
		fadd hAdd;
		jmp hExit;
	}
}

void InitAutosculpt()
{
	injector::WriteMemory(0x009F867C, ConvertSlotToFng);
	injector::MakeJMP(0x00843180, ConvertSlotToRegionCave);
	injector::MakeJMP(0x00846FE0, ConvertSlotToStateCave);

	// Disable chop top values copy
	for (int i = 0; i < 21; i++) Game::CopyPoseValueToFamilyMap[i] = 0x15;

	// Fix loading float zone value that is transformed to int
	injector::MakeJMP(0x007C4473, FixZoneLoadCave);

	injector::MakeCALL(0x0085B3A0, GetActiveZoneNum);

	// Disable region set
	injector::MakeNOP(0x00854725, 3);

	injector::WriteMemory(0x009FAA48, AutosculptSlider_Act);

	injector::MakeCALL(0x0085B3D6, ZoneName);

	injector::MakeCALL(0x0085AFDE, InitAutosculptSlider, true);
}