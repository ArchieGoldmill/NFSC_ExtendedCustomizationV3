#pragma once
#include "Feature.h"
#include "RideInfo.h"
#include "DBCarPart.h"
#include "CarRenderInfo.h"
#include "Constants.h"

void SetPartValue(RideInfo* rideInfo, Slot slot, Hash hash, float& val)
{
	if (rideInfo) {
		auto part = rideInfo->GetPart(slot);
		if (part) {
			auto attr = part->GetAppliedAttributeParam<float>(hash);
			if (attr)
			{
				val = attr->Value;
			}
		}
	}
}

float __stdcall GetTrackWidth(CarRenderInfo* carRenderInfo, int isRear, int original)
{
	auto rideInfo = carRenderInfo->RideInfo;

	float res = (float)original * 0.001f;

	SetPartValue(rideInfo, Slot::BODY, isRear ? Hashes::REAR_TIRE_OFFSET : Hashes::FRONT_TIRE_OFFSET, res);
	SetPartValue(rideInfo, isRear ? Slot::REAR_BUMPER_BADGING_SET : Slot::FRONT_BUMPER_BADGING_SET, Hashes::TIRE_OFFSET, res);

	res += rideInfo->AutoSculptRegions[ZoneStance].Zones[isRear ? 3 : 2] / 7.0f;
	res += rideInfo->AutoSculptRegions[ZoneStance].Zones[isRear ? 1 : 0] / 20.0f; // Camber

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