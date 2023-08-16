#pragma once
#include "Feature.h"
#include "RideInfo.h"
#include "LegacyRandomParts.h"
#include "Constants.h"
#include <vector>

void HandleSpecialCustomizationV3(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record);
void InstallByKitNumber(Slot slot, RideInfo* rideInfo, FECustomizationRecord* record, int kit);

void SetRandomPartsV3(RideInfo* rideInfo)
{
	auto carId = rideInfo->CarId;

	auto body = SetRandomPart(rideInfo, Slot::BODY, false);
	int kit = body->GetKit();

	auto frontBumper = SetRandomPart(rideInfo, Slot::FRONT_BUMPER, false, kit);
	if (frontBumper)
	{
		kit = frontBumper->GetKit();
		InstallByKitNumber(Slot::REAR_BUMPER, rideInfo, null, kit);
		InstallByKitNumber(Slot::SKIRT, rideInfo, null, kit);

		auto frontFender = SetRandomPart(rideInfo, Slot_FrontFender, false, kit);
		if (frontFender)
		{
			kit = frontFender->GetKit();
			InstallByKitNumber(Slot_RearFender, rideInfo, null, kit);
		}
	}

	SetRandomPart(rideInfo, Slot::EXHAUST, false, kit);
	SetRandomPart(rideInfo, Slot::SPOILER, true, kit);
	SetRandomPart(rideInfo, Slot::HOOD, true, kit);
	SetRandomPart(rideInfo, Slot::LEFT_HEADLIGHT, true, kit);
	SetRandomPart(rideInfo, Slot::LEFT_BRAKELIGHT, false, kit);
	SetRandomPart(rideInfo, Slot::LEFT_SIDE_MIRROR, false, kit);

	if (bRandom(2))
	{
		SetRandomPart(rideInfo, Slot::DECAL_FRONT_WINDOW_TEX0, false);
		SetRandomPart(rideInfo, Slot::DECAL_REAR_WINDOW_TEX0, false);
	}

	SetRandomPart(rideInfo, Slot::FRONT_WHEEL, true);
	SetRandomPart(rideInfo, Slot_Tires, false);

	if (bRandom(2))
	{
		auto neon = SetRandomPart(rideInfo, Slot_Neon, false);
		rideInfo->AutoSculptRegions[ZoneNeon].Zones[0] = (float)bRandom(99) / 100.0f;
		if (neon->GetAppliedAttributeIParam(Hashes::MORPHTARGET_NUM, 0) > 2)
		{
			rideInfo->AutoSculptRegions[ZoneNeon].Zones[2] = (float)bRandom(99) / 100.0f;
		}
	}

	SetRandomPart(rideInfo, Slot::STEERINGWHEEL, false);
	SetRandomPart(rideInfo, Slot::ATTACHMENT14, false);

	if (bRandom(2))
	{
		SetRandomPart(rideInfo, Slot::VINYL_GENERIC, false);
	}

	rideInfo->RideHeight = 1.0f;

	HandleSpecialCustomizationV3(null, rideInfo, null);
}

void __fastcall SetRandomParts(RideInfo* rideInfo, int, int hash)
{
	int version = g_Config.GetVersion(rideInfo->CarId);
	if (version == 3)
	{
		SetRandomPartsV3(rideInfo);
	}
	else if (version == 2)
	{
		Legacy::SetRandomParts(rideInfo, hash);
	}
	else
	{
		rideInfo->SetRandomParts(hash);
	}
}

int __fastcall CreateVehicle(int a1, int, int vltHash, int a3, int a4)
{
	FUNC(0x006298C0, int, __thiscall, GRacerInfo_CreateVehicle, int, int, int, int);
	return GRacerInfo_CreateVehicle(a1, g_Config.DebugAICar, a3, a4);
}

void InitRandomParts()
{
	injector::MakeCALL(0x00629D69, SetRandomParts);

	if (g_Config.DebugAICar)
	{
		injector::MakeCALL(0x0066985C, CreateVehicle);
	}
}