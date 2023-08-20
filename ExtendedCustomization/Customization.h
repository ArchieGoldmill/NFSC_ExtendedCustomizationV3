#pragma once
#include "Feature.h"
#include "FeGarageMain.h"
#include "CarPartDatabase.h"
#include "StandardSelectablePart.h"
#include "FECarRecord.h"
#include "Constants.h"
#include "Config.h"
#include "LegacyCustomization.h"
#include "CarCustomizeManager.h"
#include "RandomParts.h"

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
	auto part = CarPartDatabase::Instance->GetByKit(slot, rideInfo->CarId, kit);
	if (part)
	{
		InstallPart(rideInfo, record, slot, part);
	}
}

void InstallByKitNumber(Slot from, Slot slot, RideInfo* rideInfo, FECustomizationRecord* record)
{
	auto fromPart = rideInfo->GetPart(from);
	if (fromPart)
	{
		int kit = fromPart->GetKit();
		InstallByKitNumber(slot, rideInfo, record, kit);
	}
}

void InstallBodyPart(RideInfo* rideInfo, FECustomizationRecord* record, Slot slot, int kit)
{
	auto carId = rideInfo->CarId;

	// Check if car has parts with this slot type that have KITW
	auto part = CarPartDatabase::Instance->GetByKitW(slot, carId, kit);
	if (!part)
	{
		// If not then general parts are supported for this kit
		kit = 0;
	}

	auto installedPart = rideInfo->GetPart(slot);

	// Check if current part has matching KITW to bodykit
	if (installedPart && installedPart->HasKitW(kit))
	{
		return;
	}

	if (!part)
	{
		// Get first part that has KITW to install
		part = CarPartDatabase::Instance->GetByKitW(slot, carId, 0);
	}

	InstallPart(rideInfo, record, slot, part);
}

void HandleBrakes(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record)
{
	if (!rideInfo)
	{
		return;
	}

	auto rotor = rideInfo->GetPart(Slot::FRONT_ROTOR);
	auto brake = rideInfo->GetPart(Slot::FRONT_BRAKE);
	if (brake && !rotor)
	{
		if (brake->IsStockByKit())
		{
			InstallByKitNumber(Slot::FRONT_ROTOR, rideInfo, record, 0);
		}
		else
		{
			auto currentPart = CarCustomizeManager::Get()->GetCurrentPart(Slot::FRONT_ROTOR);
			if (currentPart && currentPart->GetKit())
			{
				InstallPart(rideInfo, record, Slot::FRONT_ROTOR, currentPart);
			}
			else
			{
				InstallByKitNumber(Slot::FRONT_ROTOR, rideInfo, record, 1);
			}
		}
	}

	if (!brake && rotor)
	{
		if (rotor->IsStockByKit())
		{
			InstallByKitNumber(Slot::FRONT_BRAKE, rideInfo, record, 0);
		}
		else
		{
			auto currentPart = CarCustomizeManager::Get()->GetCurrentPart(Slot::FRONT_BRAKE);
			if (currentPart && currentPart->GetKit())
			{
				InstallPart(rideInfo, record, Slot::FRONT_BRAKE, currentPart);
			}
			else
			{
				InstallByKitNumber(Slot::FRONT_BRAKE, rideInfo, record, 1);
			}
		}
	}

	InstallByKitNumber(Slot::FRONT_BRAKE, Slot::REAR_BRAKE, rideInfo, record);
	InstallByKitNumber(Slot::FRONT_ROTOR, Slot::REAR_ROTOR, rideInfo, record);
}

void HandleGenericVinyls(RideInfo* rideInfo, FECustomizationRecord* record)
{
	auto vinyl = rideInfo->GetPart(Slot::VINYL_GENERIC);
	if (vinyl && vinyl->IsStock())
	{
		FeGarageMain::UnInstall(rideInfo, record, Slot::VINYL_GENERIC);
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
		int kit = body->GetKit();
		for (auto kiwSlot : KitwSlots)
		{
			InstallBodyPart(rideInfo, record, kiwSlot, kit);
		}
	}

	InstallByKitNumber(Slot::LEFT_SIDE_MIRROR, Slot::RIGHT_SIDE_MIRROR, rideInfo, record);

	auto leftHeadlight = rideInfo->GetPart(Slot::LEFT_HEADLIGHT);
	if (leftHeadlight)
	{
		int kit = leftHeadlight->GetKit();
		InstallByKitNumber(Slot::RIGHT_HEADLIGHT, rideInfo, record, kit);
		InstallByKitNumber(Slot::LEFT_HEADLIGHT_GLASS, rideInfo, record, kit);
		InstallByKitNumber(Slot::RIGHT_HEADLIGHT_GLASS, rideInfo, record, kit);
	}

	auto leftBrakelight = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
	if (leftBrakelight)
	{
		int kit = leftBrakelight->GetKit();
		InstallByKitNumber(Slot::RIGHT_BRAKELIGHT, rideInfo, record, kit);
		InstallByKitNumber(Slot::LEFT_BRAKELIGHT_GLASS, rideInfo, record, kit);
		InstallByKitNumber(Slot::RIGHT_BRAKELIGHT_GLASS, rideInfo, record, kit);
	}

	InstallByKitNumber(Slot::FRONT_WINDOW, Slot::DECAL_FRONT_WINDOW, rideInfo, record);

	InstallByKitNumber(Slot::REAR_WINDOW, Slot::DECAL_REAR_WINDOW, rideInfo, record);

	InstallByKitNumber(Slot::DOOR_LEFT, Slot::DOOR_RIGHT, rideInfo, record);
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
		Legacy::HandleSpecialCustomizationV2(feGarageMain, rideInfo, record);
	}
	else
	{
		feGarageMain->HandleSpecialCustomization(rideInfo, record);
	}

	HandleBrakes(feGarageMain, rideInfo, record);
	HandleGenericVinyls(rideInfo, record);
}

bool __fastcall StandardSelectablePart_Install(StandardSelectablePart* selectablePart, int, FECustomizationRecord* record, bool setOnly)
{
	if (!record)
	{
		record = FECustomizationRecord::Get();
	}

	auto carId = FECarRecord::GetCarType();
	auto slot = selectablePart->SlotId;
	auto part = selectablePart->Part;

	if (slot == Slot::FRONT_BRAKE)
	{
		auto rotorPart = record->GetInstalledPart(carId, Slot::FRONT_ROTOR);
		if (rotorPart && rotorPart->IsStockByKit() != part->IsStockByKit())
		{
			record->UnInstallPart(carId, Slot::FRONT_ROTOR);
		}
	}

	if (slot == Slot::FRONT_ROTOR)
	{
		auto rotorPart = record->GetInstalledPart(carId, Slot::FRONT_BRAKE);
		if (rotorPart && rotorPart->IsStockByKit() != part->IsStockByKit())
		{
			record->UnInstallPart(carId, Slot::FRONT_BRAKE);
		}
	}

	int version = g_Config.GetVersion(carId);
	if (version == 3)
	{
		return record->SetInstalledPart(slot, part, carId, setOnly);
	}

	return selectablePart->Install(record, setOnly);
}

void __fastcall CarCustomizeManager_HandleCart(CarCustomizeManager* customizeManager, int, int* carType, StandardSelectablePart* selectablePart)
{
	auto carId = CarType(carType[0x2F]);
	int version = g_Config.GetVersion(carId);
	if (version != 3)
	{
		customizeManager->HandleCart(carType, selectablePart);
	}
}

void InitCustomization()
{
	InitRandomParts();

	injector::MakeCALL(0x0085EAEC, FeGarageMain_HandleSpecialCustomization);

	injector::WriteMemory(0x009F9CFC, StandardSelectablePart_Install);
	injector::MakeCALL(0x0084324A, StandardSelectablePart_Install);

	injector::MakeCALL(0x00865597, CarCustomizeManager_HandleCart);
}