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
	DBCarPart* body;
	if (bRandom(2))
	{
		body = SetRandomPart(rideInfo, Slot::BODY, 0);
	}
	else
	{
		body = rideInfo->GetPart(Slot::BODY);
	}

	int kitw = body ? body->GetKit() : -1;

	auto frontBumper = SetRandomPart(rideInfo, Slot::FRONT_BUMPER, 0, kitw);
	if (frontBumper)
	{
		int kit = frontBumper->GetKit();
		InstallByKitNumber(Slot::REAR_BUMPER, rideInfo, null, kit);
		InstallByKitNumber(Slot::SKIRT, rideInfo, null, kit);
	}

	auto frontFender = SetRandomPart(rideInfo, Slot_FrontFender, 0, kitw);
	if (frontFender)
	{
		int kit = frontFender->GetKit();
		InstallByKitNumber(Slot_RearFender, rideInfo, null, kit);
	}

	SetRandomPart(rideInfo, Slot::EXHAUST, 0, kitw);
	SetRandomPart(rideInfo, Slot::SPOILER, 1, kitw);
	SetRandomPart(rideInfo, Slot::HOOD, 1, kitw);
	SetRandomPart(rideInfo, Slot::LEFT_HEADLIGHT, 0, kitw);
	SetRandomPart(rideInfo, Slot::LEFT_BRAKELIGHT, 0, kitw);
	SetRandomPart(rideInfo, Slot::LEFT_SIDE_MIRROR, 0, kitw);
	SetRandomPart(rideInfo, Slot::INTERIOR, 0, kitw);

	SetRandomDecals(rideInfo);

	SetRandomPart(rideInfo, Slot::FRONT_WHEEL, 1);
	SetRandomTires(rideInfo);

	SetRandomVinyl(rideInfo);
	SetRandomNeon(rideInfo);

	SetRandomPart(rideInfo, Slot::STEERINGWHEEL, 0);
	SetRandomPart(rideInfo, Slot::ATTACHMENT14, 0);

	rideInfo->RideHeight = 1.0f;

	HandleSpecialCustomizationV3(null, rideInfo, null);
}

void SetRandomRearWheels(RideInfo* rideInfo)
{
	auto carId = rideInfo->CarId;
	DBCarPart* part = null;
	while (true)
	{
		part = CarPartDatabase::Instance->GetCarPart(Slot::REAR_WHEEL, carId, part);
		if (!part)
		{
			break;
		}

		if (part->GetAppliedAttributeBParam(Hashes::DEFAULT, false))
		{
			rideInfo->SetPart(Slot::REAR_WHEEL, part, false);

			break;
		}
	}
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

	SetRandomRearWheels(rideInfo);
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