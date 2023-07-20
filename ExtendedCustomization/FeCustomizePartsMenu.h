#pragma once
#include "Feature.h"
#include "FeCustomizeParts.h"
#include "Constants.h"
#include "FeCustomizeMain.h"
#include "FrontEndRenderingCar.h"
#include "Config.h"

void AddMenuOption(FeCustomizeParts* _this, Slot slot, CarType carId)
{
	auto partConfig = g_Config.GetPart(slot, carId);
	if (partConfig.State == State::Enabled)
	{
		_this->AddMenuOption(partConfig.Header, slot, _this->IsAutosculpt, 0);
	}
}

void __stdcall PopulateAllOptions(FeCustomizeParts* _this)
{
	auto carId = FrontEndRenderingCar::Get()->RideInfo.CarId;

	switch (FeCustomizeMain::SelectedItem)
	{
	case CustomizeMainMenu::AFTERMARKET:
		_this->Header->SetLanguageHash(Hashes::CUST_INSTALL);

		AddMenuOption(_this, Slot::BODY, carId);
		AddMenuOption(_this, Slot::HOOD, carId);
		AddMenuOption(_this, Slot::ROOFSCOOP, carId);
		AddMenuOption(_this, Slot::SPOILER, carId);
		AddMenuOption(_this, Slot::FRONT_BUMPER_BADGING_SET, carId);
		AddMenuOption(_this, Slot::REAR_BUMPER_BADGING_SET, carId);
		AddMenuOption(_this, Slot::LEFT_HEADLIGHT, carId);
		AddMenuOption(_this, Slot::LEFT_BRAKELIGHT, carId);
		AddMenuOption(_this, Slot::LEFT_SIDE_MIRROR, carId);
		AddMenuOption(_this, Slot::DOOR_LEFT, carId);
		_this->AddMenuOption(0xB47AB2E, Slot::FRONT_WHEEL, _this->IsAutosculpt, 17);
		break;
	case CustomizeMainMenu::AUTOSCULPT:
		_this->Header->SetLanguageHash(Hashes::CUST_AUTOSCULPT);

		AddMenuOption(_this, Slot::FRONT_BUMPER, carId);
		AddMenuOption(_this, Slot::REAR_BUMPER, carId);
		AddMenuOption(_this, Slot::SKIRT, carId);
		AddMenuOption(_this, Slot::HOOD, carId);
		AddMenuOption(_this, Slot::ROOFSCOOP, carId);
		AddMenuOption(_this, Slot::SPOILER, carId);
		AddMenuOption(_this, Slot::EXHAUST, carId);
		AddMenuOption(_this, Slot_Stance, carId);
		AddMenuOption(_this, Slot::LICENSE_PLATE, carId);
		AddMenuOption(_this, Slot_Neon, carId);
		break;
	case CustomizeMainMenu::FRONT_RIMS:
		_this->Header->SetLanguageHash(Hashes::CUST_AUTOSCULPT);
		_this->AddMenuOption(0xB47AB2E, Slot::FRONT_WHEEL, _this->IsAutosculpt, 17);
		break;
	case CustomizeMainMenu::REAR_RIMS:
		_this->Header->SetLanguageHash(Hashes::CUST_AUTOSCULPT);
		_this->AddMenuOption(0xB47AB2E, Slot::REAR_WHEEL, _this->IsAutosculpt, 17);
		break;
	case CustomizeMainMenu::ATTACHMENTS:
		_this->Header->SetLanguageHash(Hashes::CUST_INSTALL);
		for (int i = 0; i < 13; i++)
		{
			AddMenuOption(_this, (Slot)((int)Slot::ATTACHMENT0 + i), carId);
		}

		break;
	case CustomizeMainMenu::SPECIALTIES:
		_this->Header->SetLanguageHash(Hashes::CUST_INSTALL);

		AddMenuOption(_this, Slot::FRONT_BRAKE, carId);
		AddMenuOption(_this, Slot::FRONT_ROTOR, carId);
		AddMenuOption(_this, Slot::INTERIOR, carId);
		AddMenuOption(_this, Slot::STEERINGWHEEL, carId);
		AddMenuOption(_this, Slot::ATTACHMENT14, carId); // Left seat
		AddMenuOption(_this, Slot::ATTACHMENT15, carId); // Right seat
		AddMenuOption(_this, Slot::DECAL_FRONT_WINDOW_TEX0, carId);
		AddMenuOption(_this, Slot::DECAL_REAR_WINDOW_TEX0, carId);
		AddMenuOption(_this, Slot::VINYL_GENERIC, carId);

		break;
	}
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

void InitFeCustomizePartsMenu()
{
	injector::MakeJMP(0x00866074, PopulateAllOptionsCave);

	// Start part menu from first part in list
	injector::WriteMemory<BYTE>(0x0086623F, 0);
}