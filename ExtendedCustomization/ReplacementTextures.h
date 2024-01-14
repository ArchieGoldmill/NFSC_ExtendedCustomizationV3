#pragma once
#include "Feature.h"
#include "eModel.h"
#include "CarRenderInfo.h"
#include "ReplacementTextureEntry.h"
#include "CarRenderInfoExtras.h"

void __fastcall AttachReplacementTextureTable(eModel* model, int, CarRenderInfo* carRenderInfo, Slot slot, ReplacementTextureEntry* nodes, int count, int)
{
	if (carRenderInfo->Extras->LicensePlateText && slot == Slot::LICENSE_PLATE)
	{
		model->AttachReplacementTextureTable(carRenderInfo->Extras->LicensePlateText->TextureTable, 11);
	}
	else
	{
		auto part = carRenderInfo->pRideInfo->GetPart(slot);
		if (part && part->IsCarbonSkin())
		{
			model->AttachReplacementTextureTable(carRenderInfo->CarbonReplacementTextures, count);
		}
		else
		{
			model->AttachReplacementTextureTable(nodes, count);
		}
	}
}

void __declspec(naked) AttachReplacementTextureTableCave()
{
	static constexpr auto hExit = 0x007D5748;

	__asm
	{
		push[esp + 0x20];
		push ebx;
		call AttachReplacementTextureTable;

		jmp hExit;
	}
}

void InitReplacementTextures()
{
	if (g_Config.LicensePlateText)
	{
		injector::MakeJMP(0x007D5743, AttachReplacementTextureTableCave, true);
	}

	// Make hood use generic AttachReplacementTextureTable 
	injector::WriteMemory<BYTE>(0x007D5694, 0xFF);
}