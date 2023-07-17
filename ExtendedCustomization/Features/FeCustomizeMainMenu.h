#pragma once
#include "Feature.h"
#include "FeCustomizeMain.h"
#include "FEButton.h"

bool AnyAttachmentsEnabled()
{
	auto carId = FECarRecord::GetCarType();
	for (int i = 0; i < 13; i++)
	{
		auto part = g_Config.GetPart((Slot)((int)Slot::ATTACHMENT0 + i), carId);
		if (part.State == State::Enabled)
		{
			return true;
		}
	}

	return false;
}

void __fastcall FeCustomizeMain_Setup(FeCustomizeMain* _this)
{
	_this->SetTitleName(Hashes::CUST_MAINMENU_TITLE);

	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_PERFORMANCE, (int)CustomizeMainMenu::PERFORMANCE));
	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_PARTS, (int)CustomizeMainMenu::AFTERMARKET));
	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_AUTOSCULPT, (int)CustomizeMainMenu::AUTOSCULPT));
	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_FRONT_RIMS, (int)CustomizeMainMenu::FRONT_RIMS));
	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_REAR_RIMS, (int)CustomizeMainMenu::REAR_RIMS));
	if (AnyAttachmentsEnabled())
	{
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_ATTACHMENTS, (int)CustomizeMainMenu::ATTACHMENTS));
	}

	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_SPECIALTIES, (int)CustomizeMainMenu::SPECIALTIES));
	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_PAINTS, (int)CustomizeMainMenu::PAINTS));
	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_VISUALS, (int)CustomizeMainMenu::VISUALS));
	_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_INSTALL, (int)CustomizeMainMenu::CART));

	_this->FinishAddingOptions();
	_this->SetInitialOption(FEButton::GetLastButton(_this->PackageFilename));
	_this->RefreshHeader();
}

CustomizeMainMenu __stdcall ShowMenu(CustomizeMainMenu menu)
{
	FeCustomizeMain::SelectedItem = menu;
	if (menu == CustomizeMainMenu::FRONT_RIMS || menu == CustomizeMainMenu::REAR_RIMS)
	{
		return CustomizeMainMenu::AUTOSCULPT;
	}

	if (menu == CustomizeMainMenu::SPECIALTIES || menu == CustomizeMainMenu::ATTACHMENTS)
	{
		return CustomizeMainMenu::AFTERMARKET;
	}

	if (menu == CustomizeMainMenu::PAINTS)
	{
		return CustomizeMainMenu::VISUALS;
	}

	return menu;
}

void __declspec(naked) OpenCustomMenuCave()
{
	static constexpr auto Exit = 0x008683CF;
	__asm
	{
		mov eax, [esp + 0x60];
		SAVE_REGS_EAX;
		push eax;
		call ShowMenu;
		RESTORE_REGS_EAX;
		test eax, eax;
		jmp Exit;
	}
}

void InitFeCustomizeMainMenu()
{
	injector::MakeCALL(0x0085A48F, FeCustomizeMain_Setup, true);
	injector::MakeJMP(0x008683C9, OpenCustomMenuCave, true);
}