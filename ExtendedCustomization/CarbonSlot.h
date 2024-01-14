#pragma once
#include "Feature.h"

bool __stdcall IsCarbonSlot(FeCustomizeParts* feCustomizeParts, Hash header)
{
	if (header != Hashes::CUST_PARTS_HOODS && header != Hashes::CUST_PARTS_SPOILERS)
	{
		auto textOption = (TextOption*)feCustomizeParts->Options.Current;

		if (textOption)
		{
			auto slot = (Slot)textOption->DescHash;
			auto carId = FECarRecord::GetCarType();
			auto configPart = g_Config.GetPart(slot, carId);

			return configPart.Carbon == State::Enabled;
		}
	}

	return false;
}

void __declspec(naked) CarbonSlotCave1()
{
	static constexpr auto cExit = 0x00866428;

	__asm
	{
		pushad;
		push edi;
		push esi;
		call IsCarbonSlot;
		cmp al, 1;
		popad;

		jmp cExit;
	}
}

void __declspec(naked) CarbonSlotCave2()
{
	static constexpr auto cExit = 0x0086655C;

	__asm
	{
		pushad;
		push edi;
		push esi;
		call IsCarbonSlot;
		cmp al, 1;
		popad;

		jmp cExit;
	}
}

bool __stdcall IsWheelSlot(FeCustomizeParts* feCustomizeParts)
{
	auto textOption = (TextOption*)feCustomizeParts->Options.Current;

	if (textOption)
	{
		auto slot = (Slot)textOption->DescHash;
		return slot == Slot::FRONT_WHEEL || slot == Slot::REAR_WHEEL;
	}

	return false;
}

void __declspec(naked) WheelSizeCave1()
{
	static constexpr auto cExit = 0x008663F3;

	__asm
	{
		pushad;
		push esi;
		call IsWheelSlot;
		cmp al, 1;
		popad;

		jmp cExit;
	}
}

void __declspec(naked) WheelSizeCave2()
{
	static constexpr auto cExit = 0x00866542;

	__asm
	{
		pushad;
		push esi;
		call IsWheelSlot;
		cmp al, 1;
		popad;

		jmp cExit;
	}
}

void __fastcall ShowProperHelpBar(FeCustomizeParts* feCustomizeParts)
{
	bool isStock = false;
	auto carPartTextOption = (CarPartTextOption*)feCustomizeParts->SubOptionsScroller.Current;
	if (carPartTextOption && carPartTextOption->SelectablePart)
	{
		auto part = carPartTextOption->SelectablePart->Part;
		if (part)
		{
			isStock = part->IsStock();
		}
	}

	auto textOption = (TextOption*)feCustomizeParts->Options.Current;
	Hash message = Hashes::NORMAL_HELP;

	if (!isStock && textOption)
	{
		auto slot = (Slot)textOption->DescHash;
		auto carId = FECarRecord::GetCarType();
		auto configPart = g_Config.GetPart(slot, carId);

		if (configPart.Carbon == State::Enabled)
		{
			message = Hashes::CARBON_HELP;
		}

		if (!feCustomizeParts->IsAutosculpt && (slot == Slot::FRONT_WHEEL || slot == Slot::REAR_WHEEL))
		{
			message = Hashes::CARBON_HELP;
		}
	}

	cFEng::Instance()->QueuePackageMessage(message, feCustomizeParts->PackageFilename, 0);
}

Slot SetPromptText_LastSlot = Slot::INVALID;
void __fastcall SetPromptText(FeCustomizeParts* feCustomizeParts)
{
	auto textOption = (TextOption*)feCustomizeParts->Options.Current;

	if (textOption)
	{
		auto slot = (Slot)textOption->DescHash;
		auto carId = FECarRecord::GetCarType();
		auto configPart = g_Config.GetPart(slot, carId);

		if (slot == Slot::FRONT_WHEEL || slot == Slot::REAR_WHEEL)
		{
			FE_String_Printf(feCustomizeParts->PackageFilename, 0xEFB07160, "%d\"", feCustomizeParts->WheelFilter);
			FEString::SetLanguageHash(feCustomizeParts->PackageFilename, 0x25379D08, Hashes::CUST_CHANGESIZE);
		}
		else if (configPart.Carbon == State::Enabled)
		{
			if (SetPromptText_LastSlot != slot)
			{
				feCustomizeParts->RoofScoopCarbonFilter = CarCustomizeManager::Instance()->IsInstalledPartCarbon(slot);
			}
			SetPromptText_LastSlot = slot;

			bool isCarbon = feCustomizeParts->RoofScoopCarbonFilter;
			if (slot == Slot::HOOD)
			{
				isCarbon = feCustomizeParts->HoodCarbonFilter;
			}
			if (slot == Slot::SPOILER)
			{
				isCarbon = feCustomizeParts->SpoilerCarbonFilter;
			}

			FEString::SetLanguageHash(feCustomizeParts->PackageFilename, 0xEFB07160, isCarbon ? Hashes::CUST_PARTS_CARBON : Hashes::CUST_PARTS_NORMAL);
			FEString::SetLanguageHash(feCustomizeParts->PackageFilename, 0x25379D08, Hashes::CUST_MATERIAL);
		}
	}
}

void __fastcall IncrementCurrentOption(FeCustomizeParts* feCustomizeParts, int, Hash header, int initialOption)
{
	SetPromptText(feCustomizeParts);

	auto textOption = (TextOption*)feCustomizeParts->Options.Current;

	if (textOption)
	{
		auto slot = (Slot)textOption->DescHash;
		auto carId = FECarRecord::GetCarType();
		auto configPart = g_Config.GetPart(slot, carId);
		bool inc = false;
		bool isCarbon = false;

		if (slot == Slot::FRONT_WHEEL || slot == Slot::REAR_WHEEL)
		{
			feCustomizeParts->WheelFilter++;
			if (feCustomizeParts->WheelFilter > 21)
			{
				feCustomizeParts->WheelFilter = 17;
			}

			inc = true;
		}
		else
		{
			if (slot == Slot::HOOD)
			{
				isCarbon = feCustomizeParts->HoodCarbonFilter = !feCustomizeParts->HoodCarbonFilter;
				inc = true;
			}

			if (slot == Slot::SPOILER)
			{
				isCarbon = feCustomizeParts->SpoilerCarbonFilter = !feCustomizeParts->SpoilerCarbonFilter;
				inc = true;
			}

			if (slot == Slot::ROOFSCOOP || configPart.Carbon == State::Enabled)
			{
				isCarbon = feCustomizeParts->RoofScoopCarbonFilter = !feCustomizeParts->RoofScoopCarbonFilter;
				inc = true;
			}
		}

		if (inc)
		{
			feCustomizeParts->SubOptionsScroller.RemoveAll();
			feCustomizeParts->PopulateSubOptions(feCustomizeParts->Options.Current, slot, isCarbon, feCustomizeParts->IsAutosculpt,
				feCustomizeParts->WheelFilter, initialOption);

			feCustomizeParts->ResetSubOptions(feCustomizeParts->Options.Current, true);
			feCustomizeParts->RefreshHeader();
		}
	}
}

void InitCarbonSlot()
{
	injector::MakeJMP(0x008401F0, ShowProperHelpBar);
	injector::MakeJMP(0x00840340, SetPromptText);
	injector::MakeCALL(0x00866579, IncrementCurrentOption);

	injector::MakeJMP(0x00866422, CarbonSlotCave1);
	injector::MakeJMP(0x00866556, CarbonSlotCave2);

	injector::MakeJMP(0x008663EC, WheelSizeCave1);
	injector::MakeJMP(0x0086653C, WheelSizeCave2);
}