#pragma once
#include "Feature.h"
#include "FeCustomizeParts.h"
#include "Constants.h"
#include "FeCustomizeMain.h"
#include "FERenderingCar.h"
#include "Config.h"
#include "WheelBrands.h"
#include "AutosculptSelectablePart.h"

void AddMenuOption(FeCustomizeParts* _this, Slot slot, CarType carId, bool autosculpt = false)
{
	auto partConfig = g_Config.GetPart(slot, carId);

	auto state = partConfig.GetState(autosculpt);

	if (state == State::Default && slot == Slot::ROOF && autosculpt)
	{
		int* racingClassPtr = (int*)Attrib_GetAttributePointer(0x00BBABB0, 0x247E0956, 0);
		if (racingClassPtr && *racingClassPtr == 2)
		{
			state = State::Enabled;
		}
	}

	if (state == State::Enabled)
	{
		_this->AddMenuOption(partConfig.Header, slot, _this->IsAutosculpt, slot == Slot::FRONT_WHEEL ? 17 : 0);
	}
}

int AddWheelBrands(FeCustomizeParts* _this, Slot slot)
{
	_this->Header->SetLanguageHash(Hashes::CUST_MAINMENU_FRONT_RIMS);

	int wheelSize = 17;
	Hash currentBrand = 0;
	int initialOption = 0;

	auto part = FERenderingCar::GetRideInfo()->GetPart(slot);
	if (part)
	{
		wheelSize = part->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, wheelSize);
		currentBrand = part->GetAppliedAttributeIParam(Hashes::BRAND_NAME, 0);
	}

	int i = 0;
	for (auto& wheel : WheelBrands)
	{
		if (currentBrand)
		{
			if (wheel.Brand == currentBrand)
			{
				initialOption = i;
			}

			i++;
		}

		_this->AddMenuOption(wheel.Header, slot, _this->IsAutosculpt, wheelSize);
	}

	return initialOption;
}

void __stdcall PopulateAllOptions(FeCustomizeParts* _this)
{
	auto carId = FERenderingCar::GetCarId();
	int initialOption = 0;

	switch (FeCustomizeMain::SelectedItem)
	{
	case CustomizeMainMenu::AFTERMARKET:
		_this->Header->SetLanguageHash(Hashes::CUST_INSTALL);

		AddMenuOption(_this, Slot::FRONT_BUMPER, carId);
		AddMenuOption(_this, Slot::REAR_BUMPER, carId);
		AddMenuOption(_this, Slot::SKIRT, carId);
		AddMenuOption(_this, Slot::FRONT_BUMPER_BADGING_SET, carId);
		AddMenuOption(_this, Slot::REAR_BUMPER_BADGING_SET, carId);
		AddMenuOption(_this, Slot::HOOD, carId);
		AddMenuOption(_this, Slot::ROOF, carId);
		AddMenuOption(_this, Slot::ROOFSCOOP, carId);
		AddMenuOption(_this, Slot::SPOILER, carId);
		AddMenuOption(_this, Slot::EXHAUST, carId);
		AddMenuOption(_this, Slot::HEADLIGHT, carId);
		AddMenuOption(_this, Slot::LEFT_HEADLIGHT, carId);
		AddMenuOption(_this, Slot::LEFT_BRAKELIGHT, carId);
		AddMenuOption(_this, Slot::LEFT_SIDE_MIRROR, carId);
		AddMenuOption(_this, Slot::DOOR_LEFT, carId);
		AddMenuOption(_this, Slot_Trunk, carId);
		AddMenuOption(_this, Slot::INTERIOR, carId);
		break;
	case CustomizeMainMenu::AUTOSCULPT:
		_this->Header->SetLanguageHash(Hashes::CUST_AUTOSCULPT);

		AddMenuOption(_this, Slot::FRONT_BUMPER, carId, true);
		AddMenuOption(_this, Slot::REAR_BUMPER, carId, true);
		AddMenuOption(_this, Slot::SKIRT, carId, true);
		AddMenuOption(_this, Slot::HOOD, carId, true);
		AddMenuOption(_this, Slot::ROOFSCOOP, carId, true);
		AddMenuOption(_this, Slot::SPOILER, carId, true);
		AddMenuOption(_this, Slot::ROOF, carId, true);
		AddMenuOption(_this, Slot::EXHAUST, carId, true);
		AddMenuOption(_this, Slot_Stance, carId, true);
		AddMenuOption(_this, Slot::LICENSE_PLATE, carId, true);
		AddMenuOption(_this, Slot_Neon, carId, true);
		AddMenuOption(_this, Slot::FRONT_WHEEL, carId, true);
		AddMenuOption(_this, Slot::INTERIOR, carId);
		break;
	case CustomizeMainMenu::FRONT_WHEELS:
		initialOption = AddWheelBrands(_this, Slot::FRONT_WHEEL);
		break;
	case CustomizeMainMenu::REAR_WHEELS:
		initialOption = AddWheelBrands(_this, Slot::REAR_WHEEL);
		break;
	case CustomizeMainMenu::ATTACHMENTS:
		_this->Header->SetLanguageHash(Hashes::CUST_INSTALL);
		for (auto slot : AttachSlots)
		{
			AddMenuOption(_this, slot, carId);
		}

		break;
	case CustomizeMainMenu::SPECIALTIES:
		_this->Header->SetLanguageHash(Hashes::CUST_INSTALL);

		AddMenuOption(_this, Slot::BODY, carId);
		AddMenuOption(_this, Slot::FRONT_BRAKE, carId);
		AddMenuOption(_this, Slot::FRONT_ROTOR, carId);
		AddMenuOption(_this, Slot_Tires, carId);
		AddMenuOption(_this, Slot::DECAL_FRONT_WINDOW_TEX0, carId);
		AddMenuOption(_this, Slot::DECAL_REAR_WINDOW_TEX0, carId);
		AddMenuOption(_this, Slot::VINYL_GENERIC, carId);

		break;
	}

	_this->FinishAddingOptions();
	_this->SetInitialOption(initialOption);
}

Hash __fastcall StandardSelectablePart_GetCategoryHash(StandardSelectablePart* _this)
{
	auto header = g_Config.GetPart(_this->SlotId, FERenderingCar::GetCarId()).Header;
	if (header != -1)
	{
		return header;
	}

	return _this->GetCategoryHash();
}

Hash __fastcall AutosculptSelectablePart_GetCategoryHash(AutosculptSelectablePart* _this)
{
	auto header = g_Config.GetPart(_this->SlotId, FERenderingCar::GetCarId()).Header;
	if (header != -1)
	{
		return header;
	}

	return _this->GetCategoryHash();
}

void __declspec(naked) PopulateAllOptionsCave()
{
	static constexpr auto cExit = 0x00866235;
	__asm
	{
		pushad;
		push esi;
		call PopulateAllOptions;
		popad;
		jmp cExit;
	}
}

int __fastcall GetPrice(StandardSelectablePart* selectablePart)
{
	auto part = selectablePart->Part;
	auto priceAttr = part->GetAppliedAttributeParam<int>(Hashes::PRICE);
	if (priceAttr)
	{
		return priceAttr->Value;
	}

	return selectablePart->GetPrice();
}

Hash __fastcall GetPartNameForSelected3(StandardSelectablePart* selectablePart)
{
	return selectablePart->Part->GetPartNameHash();
}

Hash __fastcall GetPartNameForSelected2(DBCarPart* part, int, Hash, int)
{
	return part->GetPartNameHash();
}

Hash __fastcall GetPartNameForSelected1(CarCustomizeManager* carCustomizeManager, int, FECustomizationRecord* record, Slot slot)
{
	auto carType = FECarRecord::GetCarType();
	auto part = record->GetInstalledPart(carType, slot);
	if (part)
	{
		return part->GetPartNameHash();
	}

	return Hashes::CUST_PART_MISSINGSTRING;
}

void InitFeCustomizePartsMenu()
{
	injector::MakeJMP(0x00866074, PopulateAllOptionsCave);

	// Start part menu from first part in list
	injector::MakeNOP(0x00866239, 12);

	injector::WriteMemory(0x009F9D14, StandardSelectablePart_GetCategoryHash);
	injector::WriteMemory(0x009F9F6C, AutosculptSelectablePart_GetCategoryHash);

	injector::WriteMemory(0x009F9D1C, GetPrice);
	injector::WriteMemory(0x009F9F74, GetPrice);

	// Make selected part search by part name rather then languagehash
	injector::MakeCALL(0x0085FA93, GetPartNameForSelected1);
	injector::MakeCALL(0x0085FC1A, GetPartNameForSelected2);
	injector::WriteMemory<unsigned short>(0x0085FA61, 0xC88B);
	injector::MakeCALL(0x0085FA63, GetPartNameForSelected3);
	injector::WriteMemory<unsigned short>(0x0085FC2F, 0xCE8B);
	injector::MakeCALL(0x0085FC31, GetPartNameForSelected3);
}