#pragma once
#include "Feature.h"
#include "RideInfo.h"
#include "DBCarPart.h"
#include "CarRenderInfo.h"
#include "Constants.h"

float GetPartValue(RideInfo* rideInfo, Slot slot, Hash hash)
{
	float val = 0;

	if (rideInfo) {
		auto part = rideInfo->GetPart(slot);
		if (part) {
			val = part->GetAppliedAttributeFParam(hash, 0);
		}
	}

	return val;
}

float __stdcall GetTrackWidth(CarRenderInfo* carRenderInfo, int isRear, int original)
{
	auto rideInfo = carRenderInfo->RideInfo;

	float res = (float)original * 0.001f;
	auto bodyPart = rideInfo->GetPart(Slot::BODY);
	if (bodyPart)
	{
		float offset = bodyPart->GetAppliedAttributeFParam(isRear ? Hashes::REAR_TIRE_OFFSET : Hashes::FRONT_TIRE_OFFSET, 0);
		if (offset)
		{
			res = offset;
		}
	}

	res += GetPartValue(rideInfo, isRear ? Slot::REAR_BUMPER_BADGING_SET : Slot::FRONT_BUMPER_BADGING_SET, Hashes::TIRE_OFFSET);
	res += rideInfo->AutosculptData[ZoneStance].Zones[isRear ? 3 : 2] / 7.0f;

	return res;
}

void __declspec(naked) TrackWidthCave()
{
	static constexpr auto Exit = 0x007CB869;

	__asm
	{
		pushad;

		mov ecx, [esp + 0x38];
		push ecx;
		push ebp;
		push esi;
		call GetTrackWidth;

		popad;

		jmp Exit;
	}
}

void InitTrackWidth()
{
	if (g_Config.TrackWidth)
	{
		injector::MakeJMP(0x007CB85F, TrackWidthCave, true);
	}
}