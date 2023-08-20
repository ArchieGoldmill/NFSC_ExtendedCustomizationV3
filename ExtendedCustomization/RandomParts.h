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
	int kitw = body ? body->GetKit() : -1;

	auto frontBumper = SetRandomPart(rideInfo, Slot::FRONT_BUMPER, false, kitw);
	if (frontBumper)
	{
		int kit = frontBumper->GetKit();
		InstallByKitNumber(Slot::REAR_BUMPER, rideInfo, null, kit);
		InstallByKitNumber(Slot::SKIRT, rideInfo, null, kit);
	}

	auto frontFender = SetRandomPart(rideInfo, Slot_FrontFender, false, kitw);
	if (frontFender)
	{
		int kit = frontFender->GetKit();
		InstallByKitNumber(Slot_RearFender, rideInfo, null, kit);
	}

	SetRandomPart(rideInfo, Slot::EXHAUST, false, kitw);
	SetRandomPart(rideInfo, Slot::SPOILER, true, kitw);
	SetRandomPart(rideInfo, Slot::HOOD, true, kitw);
	SetRandomPart(rideInfo, Slot::LEFT_HEADLIGHT, true, kitw);
	SetRandomPart(rideInfo, Slot::LEFT_BRAKELIGHT, false, kitw);
	SetRandomPart(rideInfo, Slot::LEFT_SIDE_MIRROR, false, kitw);
	SetRandomPart(rideInfo, Slot::INTERIOR, false, kitw);

	SetRandomDecals(rideInfo);

	SetRandomPart(rideInfo, Slot::FRONT_WHEEL, true);
	SetRandomTires(rideInfo);

	SetRandomVinyl(rideInfo);
	SetRandomNeon(rideInfo);

	SetRandomPart(rideInfo, Slot::STEERINGWHEEL, false);
	SetRandomPart(rideInfo, Slot::ATTACHMENT14, false);

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
		Legacy::SetRandomPartsV2(rideInfo, hash);
	}
	else
	{
		Legacy::SetRandomParts(rideInfo, hash);
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