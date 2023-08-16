#pragma once
#include "Feature.h"
#include "FeCustomizeParts.h"
#include "Constants.h"
#include "FeCustomizeMain.h"
#include "FrontEndRenderingCar.h"
#include "Config.h"
#include "WheelBrands.h"
#include "FrontEndRenderingCar.h"
#include "AutosculptSelectablePart.h"

void AddMenuOption(FeCustomizeParts* _this, Slot slot, CarType carId)
{
	auto partConfig = g_Config.GetPart(slot, carId);
	if (partConfig.State == State::Enabled)
	{
		_this->AddMenuOption(partConfig.Header, slot, _this->IsAutosculpt, slot == Slot::FRONT_WHEEL ? 17 : 0);
	}
}

void AddWheelBrands(FeCustomizeParts* _this, Slot slot)
{
	_this->Header->SetLanguageHash(Hashes::CUST_MAINMENU_FRONT_RIMS);
	int wheelSize = 17;
	auto part = FrontEndRenderingCar::Get()->RideInfo.GetPart(slot);
	if (part)
	{
		wheelSize = part->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, wheelSize);
	}

	for (auto& wheel : WheelBrands)
	{
		_this->AddMenuOption(wheel.Header, slot, _this->IsAutosculpt, wheelSize);
	}
}

void __stdcall PopulateAllOptions(FeCustomizeParts* _this)
{
	auto carId = FrontEndRenderingCar::GetCarId();

	switch (FeCustomizeMain::SelectedItem)
	{
	case CustomizeMainMenu::AFTERMARKET:
		_this->Header->SetLanguageHash(Hashes::CUST_INSTALL);

		AddMenuOption(_this, Slot::HOOD, carId);
		AddMenuOption(_this, Slot::ROOFSCOOP, carId);
		AddMenuOption(_this, Slot::SPOILER, carId);
		AddMenuOption(_this, Slot::FRONT_BUMPER_BADGING_SET, carId);
		AddMenuOption(_this, Slot::REAR_BUMPER_BADGING_SET, carId);
		AddMenuOption(_this, Slot::LEFT_HEADLIGHT, carId);
		AddMenuOption(_this, Slot::LEFT_BRAKELIGHT, carId);
		AddMenuOption(_this, Slot::LEFT_SIDE_MIRROR, carId);
		AddMenuOption(_this, Slot::DOOR_LEFT, carId);
		break;
	case CustomizeMainMenu::AUTOSCULPT:
		_this->Header->SetLanguageHash(Hashes::CUST_AUTOSCULPT);

		AddMenuOption(_this, Slot::FRONT_BUMPER, carId);
		AddMenuOption(_this, Slot::REAR_BUMPER, carId);
		AddMenuOption(_this, Slot::SKIRT, carId);
		AddMenuOption(_this, Slot::HOOD, carId);
		AddMenuOption(_this, Slot::ROOFSCOOP, carId);
		AddMenuOption(_this, Slot::SPOILER, carId);
		AddMenuOption(_this, Slot::ROOF, carId);
		AddMenuOption(_this, Slot::EXHAUST, carId);
		AddMenuOption(_this, Slot_Stance, carId);
		AddMenuOption(_this, Slot::LICENSE_PLATE, carId);
		AddMenuOption(_this, Slot_Neon, carId);
		AddMenuOption(_this, Slot::FRONT_WHEEL, carId);
		break;
	case CustomizeMainMenu::FRONT_WHEELS:
		AddWheelBrands(_this, Slot::FRONT_WHEEL);
		break;
	case CustomizeMainMenu::REAR_WHEELS:
		AddWheelBrands(_this, Slot::REAR_WHEEL);
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
		AddMenuOption(_this, Slot::INTERIOR, carId);
		AddMenuOption(_this, Slot::DECAL_FRONT_WINDOW_TEX0, carId);
		AddMenuOption(_this, Slot::DECAL_REAR_WINDOW_TEX0, carId);
		AddMenuOption(_this, Slot::VINYL_GENERIC, carId);

		break;
	}
}

Hash __fastcall StandardSelectablePart_GetCategoryHash(StandardSelectablePart* _this)
{
	auto header = g_Config.GetPart(_this->SlotId, FrontEndRenderingCar::GetCarId()).Header;
	if (header != -1)
	{
		return header;
	}

	return _this->GetCategoryHash();
}

Hash __fastcall AutosculptSelectablePart_GetCategoryHash(AutosculptSelectablePart* _this)
{
	auto header = g_Config.GetPart(_this->SlotId, FrontEndRenderingCar::GetCarId()).Header;
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

void InitFeCustomizePartsMenu()
{
	injector::MakeJMP(0x00866074, PopulateAllOptionsCave);

	// Start part menu from first part in list
	injector::WriteMemory<BYTE>(0x0086623F, 0);

	injector::WriteMemory(0x009F9D14, StandardSelectablePart_GetCategoryHash);
	injector::WriteMemory(0x009F9F6C, AutosculptSelectablePart_GetCategoryHash);

	injector::WriteMemory(0x009F9D1C, GetPrice);
	injector::WriteMemory(0x009F9F74, GetPrice);
}