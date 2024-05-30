#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"

void SetWindowDamage(CarRenderInfo* carRenderInfo, ReplacementTextureIndex index, DamageZone damageZone, Hash from, Hash to = Hashes::WINDOW_FRONT)
{
	if (carRenderInfo->Damage.IsDamaged(damageZone, DamageType_Medium))
	{
		to = Hashes::WINDOW_DAMAGE0;
	}

	if (carRenderInfo->Damage.IsDamaged(damageZone, DamageType_Heavy))
	{
		to = Hashes::WINDOW_DAMAGE1;
	}

	carRenderInfo->GeneralReplacementTextures[(int)index].Update(from, to);
	carRenderInfo->CarbonReplacementTextures[(int)index].Update(from, to);
}

void __fastcall SetPlayerDamage(CarRenderInfo* carRenderInfo, int, CarDamage* pDamage)
{
	auto damage = *pDamage;
	if (carRenderInfo->Damage.Damage == damage.Damage)
	{
		return;
	}

	carRenderInfo->Damage = damage;
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone_Front), Slot::DAMAGE_FRONT, Slot::DAMAGE_FRONT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone_Rear), Slot::DAMAGE_REAR, Slot::DAMAGE_REAR);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone_FrontLeft), Slot::DAMAGE_FRONT_LEFT, Slot::DAMAGE_FRONT_LEFT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone_FrontRight), Slot::DAMAGE_FRONT_RIGHT, Slot::DAMAGE_FRONT_RIGHT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone_RearLeft), Slot::DAMAGE_REAR_LEFT, Slot::DAMAGE_REAR_LEFT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone_RearRight), Slot::DAMAGE_REAR_RIGHT, Slot::DAMAGE_REAR_RIGHT);

	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowFront, DamageZone_Front, Hashes::WINDOW_FRONT);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowFrontLeft, DamageZone_FrontLeft, Hashes::WINDOW_LEFT_FRONT);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowFrontRight, DamageZone_FrontRight, Hashes::WINDOW_RIGHT_FRONT);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowRearLeft, DamageZone_RearLeft, Hashes::WINDOW_LEFT_REAR);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowRearRight, DamageZone_RearRight, Hashes::WINDOW_RIGHT_REAR);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::RearDefroster, DamageZone_Rear, Hashes::REAR_DEFROSTER, Hashes::REAR_DEFROSTER);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowRear, DamageZone_Rear, Hashes::WINDOW_REAR);
}

void __fastcall OnTaskFX(int* a, int, float b)
{
	FUNC(0x006C6380, void, __thiscall, _OnTaskFX, int*, float);
	_OnTaskFX(a, b);

	auto pVehicle = (PVehicle*)(a + 0x2E);
	auto carRenderConn = CarRenderConn::Get(pVehicle);
	if (carRenderConn)
	{
		auto carRenderInfo = carRenderConn->pCarRenderInfo;

		if (carRenderInfo && !carRenderInfo->IsLightOn())
		{
			pVehicle->GlareOff(VehicleFX_HEADLIGHTS);
		}
	}
}

void InitDamage()
{
	injector::MakeCALL(0x007BE162, SetPlayerDamage);
	injector::MakeCALL(0x006D0A97, OnTaskFX);

	if (g_Config.WindowDamageStages)
	{
		injector::WriteMemory<BYTE>(0x006ED843, 3);
		injector::WriteMemory<int>(0x006ED84E, 4);
	}
}