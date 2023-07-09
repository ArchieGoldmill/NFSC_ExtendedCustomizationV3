#pragma once
#include "Feature.h"
#include "FeGarageMain.h"
#include "CarPartDatabase.h"
#include "StandardSelectablePart.h"
#include "FECarRecord.h"
#include "Constants.h"
#include "Config.h"
#include "LegacyCustomization.h"

void InstallPart(RideInfo* rideInfo, FECustomizationRecord* record, Slot slot, DBCarPart* part)
{
	if (part)
	{
		rideInfo->SetPart(slot, part);
		if (record)
		{
			record->SetInstalledPart(slot, part, rideInfo->CarId, true);
		}
	}
}

void InstallByKitNumber(Slot slot, RideInfo* rideInfo, FECustomizationRecord* record, int kit)
{
	auto carPartDatabase = CarPartDatabase::Get();
	if (carPartDatabase)
	{
		auto part = carPartDatabase->GetByKitNumber(slot, rideInfo->CarId, kit);
		if (part)
		{
			InstallPart(rideInfo, record, slot, part);
		}
	}
}

void InstallBodyPart(RideInfo* rideInfo, FECustomizationRecord* record, Slot slot, int kit)
{
	auto carId = rideInfo->CarId;

	auto installedPart = rideInfo->GetPart(slot);
	if (installedPart && installedPart->HasKitW(kit))
	{
		return;
	}

	auto carPartDatabase = CarPartDatabase::Get();
	if (carPartDatabase)
	{
		auto part = carPartDatabase->GetByKitW(slot, carId, kit);
		if (!part)
		{
			part = carPartDatabase->GetByKitW(slot, carId, 0);
		}

		InstallPart(rideInfo, record, slot, part); 
	}
}

void HandleSpecialCustomizationV3(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record)
{
	if (!rideInfo)
	{
		return;
	}

	auto body = rideInfo->GetPart(Slot::BODY);
	if (body)
	{
		int kit = body->GetAppliedAttributeIParam(Hashes::KITNUMBER, 0);
		InstallBodyPart(rideInfo, record, Slot::FRONT_BUMPER, kit);
		InstallBodyPart(rideInfo, record, Slot::REAR_BUMPER, kit);
		InstallBodyPart(rideInfo, record, Slot::SKIRT, kit);
		InstallBodyPart(rideInfo, record, Slot_FrontFender, kit);
		InstallBodyPart(rideInfo, record, Slot_RearFender, kit);
		InstallBodyPart(rideInfo, record, Slot::HOOD, kit);
		InstallBodyPart(rideInfo, record, Slot::LEFT_HEADLIGHT, kit);
		InstallBodyPart(rideInfo, record, Slot::LEFT_SIDE_MIRROR, kit);
		InstallBodyPart(rideInfo, record, Slot_Trunk, kit);
	}

	auto leftMirror = rideInfo->GetPart(Slot::LEFT_SIDE_MIRROR);
	if (leftMirror)
	{
		int kit = leftMirror->GetAppliedAttributeIParam(Hashes::KITNUMBER, 0);
		InstallByKitNumber(Slot::RIGHT_SIDE_MIRROR, rideInfo, record, kit);
	}

	auto leftHeadlight = rideInfo->GetPart(Slot::LEFT_HEADLIGHT);
	if (leftHeadlight)
	{
		int kit = leftHeadlight->GetAppliedAttributeIParam(Hashes::KITNUMBER, 0);
		InstallByKitNumber(Slot::RIGHT_HEADLIGHT, rideInfo, record, kit);
		InstallByKitNumber(Slot::LEFT_HEADLIGHT_GLASS, rideInfo, record, kit);
		InstallByKitNumber(Slot::RIGHT_HEADLIGHT_GLASS, rideInfo, record, kit);
	}

	auto leftBrakelight = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
	if (leftBrakelight)
	{
		int kit = leftBrakelight->GetAppliedAttributeIParam(Hashes::KITNUMBER, 0);
		InstallByKitNumber(Slot::RIGHT_BRAKELIGHT, rideInfo, record, kit);
		InstallByKitNumber(Slot::LEFT_BRAKELIGHT_GLASS, rideInfo, record, kit);
		InstallByKitNumber(Slot::RIGHT_BRAKELIGHT_GLASS, rideInfo, record, kit);
	}
}

void __fastcall FeGarageMain_HandleSpecialCustomization(FeGarageMain* feGarageMain, int, RideInfo* rideInfo, FECustomizationRecord* record)
{
	int version = g_Config.GetVersion(rideInfo->CarId);
	if (version == 3)
	{
		HandleSpecialCustomizationV3(feGarageMain, rideInfo, record);
	}
	else if (version == 2)
	{
		HandleSpecialCustomizationV2(feGarageMain, rideInfo, record);
	}
	else
	{
		feGarageMain->HandleSpecialCustomization(rideInfo, record);
	}
}

bool __fastcall StandardSelectablePart_Install(StandardSelectablePart* selectablePart, int, FECustomizationRecord* record, bool setOnly)
{
	if (!record)
	{
		record = FECustomizationRecord::Get();
	}

	auto carId = FECarRecord::GetCarType();

	return record->SetInstalledPart(selectablePart->SlotId, selectablePart->Part, carId, setOnly);
}

void InitCustomization()
{
	InitLegacyCustomization();

	injector::MakeCALL(0x0085EAEC, FeGarageMain_HandleSpecialCustomization, true);

	injector::WriteMemory(0x009F9CFC, StandardSelectablePart_Install, true);
	injector::MakeCALL(0x0084324A, StandardSelectablePart_Install, true);
}