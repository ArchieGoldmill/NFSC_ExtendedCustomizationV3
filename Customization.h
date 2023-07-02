#pragma once
#include "Feature.h"
#include "FeGarageMain.h"
#include "CarPartDatabase.h"
#include "StandardSelectablePart.h"
#include "FECarRecord.h"
#include "Constants.h"

void InstallPart(RideInfo* rideInfo, FECustomizationRecord* record, Slot slot, DBCarPart* part)
{
	rideInfo->SetPart(slot, part);
	if (record)
	{
		record->SetInstalledPart(slot, part, rideInfo->CarId, true);
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

void HandleSpecialCustomization(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record)
{
	if (!rideInfo)
	{
		return;
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
	if (rideInfo->CarId == 0x11)
	{
		HandleSpecialCustomization(feGarageMain, rideInfo, record);
	}
	else
	{
		feGarageMain->HandleSpecialCustomization(rideInfo, record);
	}
}

void InstallBodyPart(FECustomizationRecord* record, Slot slot, int carId, int kit)
{
	auto installedPart = record->GetInstalledPart(carId, slot);
	if (installedPart && installedPart->HasKitW(kit))
	{
		return;
	}

	auto carPartDatabase = CarPartDatabase::Get();
	if (carPartDatabase)
	{
		auto part = carPartDatabase->GetByKitW(slot, carId, kit);
		if (part)
		{
			record->SetInstalledPart(slot, part, carId, false);
		}
	}
}

bool __fastcall StandardSelectablePart_Install(StandardSelectablePart* selectablePart, int, FECustomizationRecord* record, bool setOnly)
{
	if (!record)
	{
		record = FECustomizationRecord::Get();
	}

	int carId = -1;
	auto slot = selectablePart->SlotId;
	auto part = selectablePart->Part;
	auto feCarRecord = FECarRecord::Get();
	if (feCarRecord)
	{
		carId = feCarRecord->GetType();
	}

	if (slot == Slot::BODY)
	{
		int kit = part->GetAppliedAttributeIParam(Hashes::KITNUMBER, 0);
		InstallBodyPart(record, Slot::FRONT_BUMPER, carId, kit);
		InstallBodyPart(record, Slot::REAR_BUMPER, carId, kit);
		InstallBodyPart(record, Slot::SKIRT, carId, kit);
		InstallBodyPart(record, Slot_FrontFender, carId, kit);
		InstallBodyPart(record, Slot_RearFender, carId, kit);
		InstallBodyPart(record, Slot::HOOD, carId, kit);
		InstallBodyPart(record, Slot_Trunk, carId, kit);
	}

	return record->SetInstalledPart(slot, part, carId, setOnly);
}

int __fastcall IsSetHeadlightOn(FECarRecord* feCarRecord)
{
	int carId = feCarRecord->GetType();

	bool popupsEnabled = false;
	if (popupsEnabled)
	{
		return 5;
	}

	return 0;
}

void InitCustomization()
{
	injector::MakeCALL(0x0085EAEC, FeGarageMain_HandleSpecialCustomization, true);

	injector::WriteMemory(0x009F9CFC, StandardSelectablePart_Install, true);
	injector::MakeCALL(0x0084324A, StandardSelectablePart_Install, true);

	injector::MakeCALL(0x00865278, IsSetHeadlightOn, true);
	injector::MakeNOP(0x00865280, 60, true);
}