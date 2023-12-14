#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"

void __fastcall SetPlayerDamage(CarRenderInfo* carRenderInfo, int, unsigned int* pDamage)
{
	int damage = *pDamage;
	if (carRenderInfo->Damage == damage)
	{
		return;
	}

	carRenderInfo->Damage = damage;

	carRenderInfo->SetCarDamageState((damage & 7) != 0, Slot::DAMAGE_FRONT, Slot::DAMAGE_FRONT);
	carRenderInfo->SetCarDamageState((damage & 0x38u) >= 8, Slot::DAMAGE_REAR, Slot::DAMAGE_REAR);
	carRenderInfo->SetCarDamageState((damage & 0x7000u) >= 0x1000, Slot::DAMAGE_FRONT_LEFT, Slot::DAMAGE_FRONT_LEFT);
	carRenderInfo->SetCarDamageState((damage & 0x38000u) >= 0x8000, Slot::DAMAGE_FRONT_RIGHT, Slot::DAMAGE_FRONT_RIGHT);
	carRenderInfo->SetCarDamageState((damage & 0x1C0000u) >= 0x40000, Slot::DAMAGE_REAR_LEFT, Slot::DAMAGE_REAR_LEFT);
	carRenderInfo->SetCarDamageState((damage & 0xE00000u) >= 0x200000, Slot::DAMAGE_REAR_RIGHT, Slot::DAMAGE_REAR_RIGHT);


}

void InitDamage()
{
	injector::MakeCALL(0x007BE162, SetPlayerDamage);
	injector::WriteMemory<BYTE>(0x006ED843, 3);
	injector::WriteMemory<int>(0x006ED84E, 3);
}