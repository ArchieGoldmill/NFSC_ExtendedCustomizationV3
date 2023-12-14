#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"

void SetWindowDamage(CarRenderInfo* carRenderInfo, ReplacementTextureIndex index, DamageZone damageZone, Hash from)
{
	auto damage = carRenderInfo->Damage;
	Hash newHash = Hashes::WINDOW_FRONT;
	if (damage.IsDamaged(damageZone, DamageType::Medium))
	{
		newHash = Hashes::WINDOW_DAMAGE0;
	}

	if (damage.IsDamaged(damageZone, DamageType::Heavy))
	{
		newHash = Hashes::WINDOW_DAMAGE1;
	}

	carRenderInfo->GeneralReplacementTextures[(int)index].Update(from, newHash);
}

void __fastcall SetPlayerDamage(CarRenderInfo* carRenderInfo, int, CarDamage* pDamage)
{
	auto damage = *pDamage;
	if (carRenderInfo->Damage == damage)
	{
		return;
	}

	carRenderInfo->Damage = damage;
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone::Front), Slot::DAMAGE_FRONT, Slot::DAMAGE_FRONT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone::Rear), Slot::DAMAGE_REAR, Slot::DAMAGE_REAR);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone::FrontLeft), Slot::DAMAGE_FRONT_LEFT, Slot::DAMAGE_FRONT_LEFT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone::FrontRight), Slot::DAMAGE_FRONT_RIGHT, Slot::DAMAGE_FRONT_RIGHT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone::RearLeft), Slot::DAMAGE_REAR_LEFT, Slot::DAMAGE_REAR_LEFT);
	carRenderInfo->SetCarDamageState(damage.IsDamaged(DamageZone::RearRight), Slot::DAMAGE_REAR_RIGHT, Slot::DAMAGE_REAR_RIGHT);

	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowFront, DamageZone::Front, Hashes::WINDOW_FRONT);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowFrontLeft, DamageZone::FrontLeft, Hashes::WINDOW_LEFT_FRONT);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowFrontRight, DamageZone::FrontRight, Hashes::WINDOW_RIGHT_FRONT);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowRearLeft, DamageZone::RearLeft, Hashes::WINDOW_LEFT_REAR);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowRearRight, DamageZone::RearRight, Hashes::WINDOW_RIGHT_REAR);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::RearDefroster, DamageZone::Rear, Hashes::REAR_DEFROSTER);
	SetWindowDamage(carRenderInfo, ReplacementTextureIndex::WindowRear, DamageZone::Rear, Hashes::WINDOW_REAR);
}

void InitDamage()
{
	injector::MakeCALL(0x007BE162, SetPlayerDamage);
	if (g_Config.WindowDamageStages)
	{
		injector::WriteMemory<BYTE>(0x006ED843, 3);
		injector::WriteMemory<int>(0x006ED84E, 4);
	}
}