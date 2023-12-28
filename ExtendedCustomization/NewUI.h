#pragma once
#include "Feature.h"
#include "VectorScrollerMenu.h"
#include "FEGroup.h"
#include "../ThirdParty/IniReader/IniReader.h"

void* __fastcall VectorScrollerMenu_AddOption(VectorScrollerMenu* _this, int, TextOption* textOption)
{
	auto result = _this->AddOption(textOption);

	if (textOption->Image)
	{
		auto hash = StringHash1("_ICON", textOption->NameHash);
		textOption->Image->Handle = TextureInfo::Get(hash, 0, 0) ? hash : Hashes::CUST_PARTS_GENERIC_ATTACHMENT_ICON;
	}

	return result;
}

void AddCurrentOptions(ScreenConstructorData* data)
{
	auto group = (FEGroup*)data->pPackage->FindObjectByHash(Hashes::OPTION_1);

	auto str = (FEString*)group->Children.Head->Next;
	auto newStr = str->Clone();

	newStr->Prev = group;
	newStr->Next = group->Next;
	group->Next = newStr;

	newStr->NameHash = Hashes::CURRENT_OPTION;
	newStr->pData->Position.X = 300;
	newStr->pData->Position.Y = -82;
	newStr->pData->Size.X = 1;
	newStr->pData->Size.Y = 1;
	newStr->pData->Color.A = 255;
	newStr->pData->Color.R = 255;
	newStr->pData->Color.G = 255;
	newStr->pData->Color.B = 255;
	newStr->Format = 2;
}

VectorScrollerMenu* __cdecl CreateCustomizePerformance(ScreenConstructorData* data)
{
	static auto CreateCustomizePerformance = (VectorScrollerMenu * (__cdecl*)(ScreenConstructorData*))0x00574900;

	auto result = CreateCustomizePerformance(data);
	AddCurrentOptions(data);

	return result;
}

VectorScrollerMenu* __cdecl CreateCustomizeParts(ScreenConstructorData* data)
{
	static auto CreateCustomizeParts = (VectorScrollerMenu * (__cdecl*)(ScreenConstructorData*))0x005748A0;

	auto result = CreateCustomizeParts(data);
	AddCurrentOptions(data);

	return result;
}

VectorScrollerMenu* __cdecl CreateFeVinylSelection(ScreenConstructorData* data)
{
	static auto CreateFeVinylSelection = (VectorScrollerMenu * (__cdecl*)(ScreenConstructorData*))0x00574F00;

	auto result = CreateFeVinylSelection(data);
	AddCurrentOptions(data);

	return result;
}

void __fastcall VectorScrollerMenu_RefreshHeader(VectorScrollerMenu* menu)
{
	menu->RefreshHeader();
	auto str = (FEString*)menu->ConstructData.pPackage->FindObjectByHash(Hashes::CURRENT_OPTION);
	if (str)
	{
		str->SetLanguageHash(menu->Options.Current->NameHash);
	}
}

void UpdateUnlimiterConfig()
{
	CIniReader fngFixes("UnlimiterData\\_FNGFixes.ini");
	int count = fngFixes.ReadInteger("FNGFixes", "NumberOfFNGFixes", 0);
	if (count)
	{
		for (int i = 1; i <= count; i++)
		{
			char section[64];
			sprintf(section, "FNG%d", i);
			auto fngName = fngFixes.ReadString(section, "FNGName", "");
			auto objectPrefix = fngFixes.ReadString(section, "ObjectPrefix", "");
			if (fngName == "FeCustomizeParts.fng" && objectPrefix == "OPTION_")
			{
				fngFixes.WriteString(section, "Child1", "OPTION_IMAGE_");
				break;
			}
		}
	}
}

void InitNewUI()
{
	if (g_Config.NewUI)
	{
		UpdateUnlimiterConfig();

		injector::MakeNOP(0x005A2C17, 6, true); // Always show new part icon

		injector::MakeCALL(0x0085FE9E, VectorScrollerMenu_AddOption, true); // Parts
		injector::MakeCALL(0x0085A95A, VectorScrollerMenu_AddOption, true); // Perfromance
		injector::MakeCALL(0x0085BBE3, VectorScrollerMenu_AddOption, true); // Vinyls

		injector::WriteMemory(0x00A5E324, CreateCustomizeParts, true);
		injector::WriteMemory(0x00A5E32C, CreateCustomizePerformance, true);
		injector::WriteMemory(0x00A5E3B4, CreateFeVinylSelection, true);

		injector::MakeCALL(0x005A4EE5, VectorScrollerMenu_RefreshHeader, true);
	}
}