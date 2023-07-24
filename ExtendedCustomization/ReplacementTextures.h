#pragma once
#include "Feature.h"
#include "eModel.h"
#include "CarRenderInfo.h"
#include "ReplacementTextureEntry.h"
#include "CarRenderInfoExtras.h"

void __fastcall AttachReplacementTextureTable(eModel* model, int, CarRenderInfo* carRenderInfo, ReplacementTextureEntry* nodes, int count, int)
{
	if (strstr(model->Solid->Name, "LICENSE_PLATE"))
	{
		model->AttachReplacementTextureTable(carRenderInfo->Extras->Textures->LicensePlate, 11);
	}
	else
	{
		model->AttachReplacementTextureTable(nodes, count);
	}
}

void __declspec(naked) AttachReplacementTextureTableCave()
{
	static constexpr auto hExit = 0x007D5748;

	__asm
	{
		push ebx;
		call AttachReplacementTextureTable;

		jmp hExit;
	}
}

void InitReplacementTextures()
{
	injector::MakeJMP(0x007D5743, AttachReplacementTextureTableCave, true);
}