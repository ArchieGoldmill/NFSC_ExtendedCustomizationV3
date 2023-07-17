#pragma once
#include "Feature.h"
#include "FeCustomizeVisuals.h"
#include "FeCustomizeMain.h"
#include "Hashes.h"
#include "FEButton.h"

void __fastcall FeCustomizeVisuals_Setup(FeCustomizeVisuals* _this)
{
	_this->SetTitleName(Hashes::CUST_MAINMENU_VISUALS_TITLE);

	if (FeCustomizeMain::SelectedItem == CustomizeMainMenu::PAINTS)
	{
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_PAINT, (int)VisualsMenu::BODY_PAINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_HOOD_PAINT, (int)VisualsMenu::HOOD_PAINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_SPOILER_PAINT, (int)VisualsMenu::SPOILER_PAINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_FRONT_RIMS, (int)VisualsMenu::FRONT_RIM_PAINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_FRONT_LIP, (int)VisualsMenu::FRONT_LIP_PAINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_REAR_RIMS, (int)VisualsMenu::REAR_RIM_PAINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_REAR_LIP, (int)VisualsMenu::REAR_LIP_PAINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_CALIPER_PAINT, (int)VisualsMenu::CALIPER_PAINT));
	}

	if (FeCustomizeMain::SelectedItem == CustomizeMainMenu::VISUALS)
	{
		_this->AddOption(TextOption::Create(Hashes::PART_VINYLS, (int)VisualsMenu::VINYLS));
		_this->AddOption(TextOption::Create(Hashes::PART_WINDOW_TINT, (int)VisualsMenu::WINDOW_TINT));
		_this->AddOption(TextOption::Create(Hashes::CUST_MAINMENU_RIDE_HEIGHT, (int)VisualsMenu::RIDE_HEIGHT));
	}

	_this->FinishAddingOptions();
	_this->SetInitialOption(FEButton::GetLastButton(_this->PackageFilename));
	_this->RefreshHeader();
}

VisualsMenu __stdcall ShowVisualsMenu(VisualsMenu menu)
{
	FeCustomizeVisuals::SelectedItem = menu;
	if (menu == VisualsMenu::CALIPER_PAINT || menu == VisualsMenu::FRONT_LIP_PAINT
		|| menu == VisualsMenu::REAR_LIP_PAINT || menu == VisualsMenu::REAR_RIM_PAINT)
	{
		return VisualsMenu::FRONT_RIM_PAINT;
	}

	if (menu == VisualsMenu::HOOD_PAINT || menu == VisualsMenu::SPOILER_PAINT)
	{
		return VisualsMenu::BODY_PAINT;
	}

	return menu;
}

void __declspec(naked) OpenCustomVisualsMenuCave()
{
	static constexpr auto cExit = 0x008684E3;
	__asm
	{
		mov eax, [esp + 0x60];
		SAVE_REGS_EAX;
		push eax;
		call ShowVisualsMenu;
		RESTORE_REGS_EAX;
		cmp eax, 04;
		jmp cExit;
	}
}

void InitFeCustomizeVisualsMenu()
{
	injector::MakeJMP(0x008684DC, OpenCustomVisualsMenuCave);
	injector::MakeCALL(0x0085A565, FeCustomizeVisuals_Setup);

	// Disable stock rims check for paint
	injector::MakeNOP(0x0086850C, 2);
}