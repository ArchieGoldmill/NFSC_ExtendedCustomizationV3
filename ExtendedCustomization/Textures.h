#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Game.h"
#include "ReplacementTextureEntry.h"
#include "CarRenderConn.h"
#include "DBCarPart.h"
#include "ReplacementTextures.h"
#include "EmissiveTextures.h"

Hash InitTexture(RideInfo* rideInfo, Slot slot, const char* def)
{
	auto part = rideInfo->GetPart(slot);
	if (part)
	{
		auto tex = part->GetTextureName();
		if (tex)
		{
			return tex;
		}
	}

	auto carHash = rideInfo->GetCarTypeHash();
	return StringHash1(def, carHash);
}

Hash GetInteriorBaseTexture(RideInfo* rideInfo)
{
	return InitTexture(rideInfo, Slot::INTERIOR, "_INTERIOR");
}

void UpdateHeadlightTextures(CarRenderInfo* carRenderInfo)
{
	Hash headlightLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_ON;
	Hash headlightRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_ON;

	if (TextureInfo::Get(carRenderInfo->UsedTextureInfo.HEADLIGHT_OFF, false, false))
	{
		headlightLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_OFF;
		headlightRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_OFF;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_LHEAD) && carRenderInfo->Extras->Animations->IsLeftHeadlightOpen())
	{
		headlightLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_ON;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_RHEAD) && carRenderInfo->Extras->Animations->IsRightHeadlightOpen())
	{
		headlightRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_ON;
	}

	if (TextureInfo::Get(carRenderInfo->UsedTextureInfo.HEADLIGHT_DAMAGE0, false, false))
	{
		if (carRenderInfo->Damage.IsHeadlightLeftDamaged())
		{
			headlightLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_DAMAGE0;
		}

		if (carRenderInfo->Damage.IsHeadlightRightDamaged())
		{
			headlightRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_DAMAGE0;
		}
	}

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightLeft].Update(Hashes::HEADLIGHT_LEFT, headlightLeft);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightRight].Update(Hashes::HEADLIGHT_RIGHT, headlightRight);
}

void UpdateHeadlightGlassTextures(CarRenderInfo* carRenderInfo)
{
	Hash headlightGlassLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_ON;
	Hash headlightGlassRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_ON;

	if (TextureInfo::Get(carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_OFF, false, false))
	{
		headlightGlassLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_OFF;
		headlightGlassRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_OFF;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_LHEAD) && carRenderInfo->Extras->Animations->IsLeftHeadlightOpen())
	{
		headlightGlassLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_ON;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_RHEAD) && carRenderInfo->Extras->Animations->IsRightHeadlightOpen())
	{
		headlightGlassRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_ON;
	}

	if (TextureInfo::Get(carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_DAMAGE0, false, false))
	{
		if (carRenderInfo->Damage.IsHeadlightGlassLeftDamaged())
		{
			headlightGlassLeft = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_DAMAGE0;
		}

		if (carRenderInfo->Damage.IsHeadlightGlassRightDamaged())
		{
			headlightGlassRight = carRenderInfo->UsedTextureInfo.HEADLIGHT_GLASS_DAMAGE0;
		}
	}

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightGlassLeft].Update(Hashes::HEADLIGHT_GLASS_LEFT, headlightGlassLeft);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightGlassRight].Update(Hashes::HEADLIGHT_GLASS_RIGHT, headlightGlassRight);
}

void UpdateBrakelightTextures(CarRenderInfo* carRenderInfo)
{
	Hash brakelightLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_OFF;
	Hash brakelightRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_OFF;
	Hash brakelightCentre = carRenderInfo->UsedTextureInfo.BRAKELIGHT_OFF;

	if (Game::InRace() && TextureInfo::Get(carRenderInfo->UsedTextureInfo.BRAKELIGHT_ONF, false, false))
	{
		brakelightLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_ONF;
		brakelightRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_ONF;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_LBRAKE))
	{
		brakelightLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_ON;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_RBRAKE))
	{
		brakelightRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_ON;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_CBRAKE))
	{
		brakelightCentre = carRenderInfo->UsedTextureInfo.BRAKELIGHT_ON;
	}

	if (TextureInfo::Get(carRenderInfo->UsedTextureInfo.BRAKELIGHT_DAMAGE0, false, false))
	{
		if (carRenderInfo->Damage.IsBrakelightLeftDamaged())
		{
			brakelightLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_DAMAGE0;
		}

		if (carRenderInfo->Damage.IsBrakelightRightDamaged())
		{
			brakelightRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_DAMAGE0;
		}

		if (carRenderInfo->Damage.IsBrakelightCentreDamaged())
		{
			brakelightCentre = carRenderInfo->UsedTextureInfo.BRAKELIGHT_DAMAGE0;
		}
	}

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightLeft].Update(Hashes::BRAKELIGHT_LEFT, brakelightLeft);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightRight].Update(Hashes::BRAKELIGHT_RIGHT, brakelightRight);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightCenter].Update(Hashes::BRAKELIGHT_CENTRE, brakelightCentre);
}

void UpdateBrakelightGlassTextures(CarRenderInfo* carRenderInfo)
{
	Hash brakelightGlassLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_OFF;
	Hash brakelightGlassRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_OFF;
	Hash brakelightGlassCentre = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_OFF;

	if (Game::InRace() && TextureInfo::Get(carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_ONF, false, false))
	{
		brakelightGlassLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_ONF;
		brakelightGlassRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_ONF;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_LBRAKE))
	{
		brakelightGlassLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_ON;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_RBRAKE))
	{
		brakelightGlassRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_ON;
	}

	if (carRenderInfo->IsGlareOn(VehicleFX_CBRAKE))
	{
		brakelightGlassCentre = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_ON;
	}

	if (TextureInfo::Get(carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_DAMAGE0, false, false))
	{
		if (carRenderInfo->Damage.IsBrakelightGlassLeftDamaged())
		{
			brakelightGlassLeft = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_DAMAGE0;
		}

		if (carRenderInfo->Damage.IsBrakelightGlassRightDamaged())
		{
			brakelightGlassRight = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_DAMAGE0;
		}

		if (carRenderInfo->Damage.IsBrakelightCentreDamaged())
		{
			brakelightGlassCentre = carRenderInfo->UsedTextureInfo.BRAKELIGHT_GLASS_DAMAGE0;
		}
	}

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightGlassLeft].Update(Hashes::BRAKELIGHT_GLASS_LEFT, brakelightGlassLeft);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightGlassRight].Update(Hashes::BRAKELIGHT_GLASS_RIGHT, brakelightGlassRight);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightGlassCenter].Update(Hashes::BRAKELIGHT_GLASS_CENTRE, brakelightGlassCentre);
}

void UpdateReverseTexture(CarRenderInfo* carRenderInfo)
{
	Hash carHash = StringHash(carRenderInfo->pRideInfo->GetCarTypeName());
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::Reverse].Update(Hashes::REVERSE, 
		carRenderInfo->IsGlareOn(VehicleFX_REVERSE) ? StringHash1("_REVERSE_ON", carHash) : StringHash1("_REVERSE_OFF", carHash));
}

void UpdateInteriorGlowTexture(CarRenderInfo* carRenderInfo)
{
	Hash interiorHash = GetInteriorBaseTexture(carRenderInfo->pRideInfo);
	Hash interiorOff = StringHash1("_OFF", interiorHash);
	Hash interiorOn = StringHash1("_ON", interiorHash);
	if (TextureInfo::Get(interiorOff, false, false))
	{
		interiorHash = Game::InRace() ? interiorOn : interiorOff;
	}
	else
	{
		interiorHash = interiorOn;
	}

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::InteriorGlow].Update(Hashes::INTERIOR_GLOW, interiorHash);

}

void __fastcall UpdateLightStateTextures(CarRenderInfo* carRenderInfo)
{
	UpdateHeadlightTextures(carRenderInfo);
	UpdateHeadlightGlassTextures(carRenderInfo);
	UpdateBrakelightTextures(carRenderInfo);
	UpdateBrakelightGlassTextures(carRenderInfo);
	UpdateReverseTexture(carRenderInfo);
	UpdateInteriorGlowTexture(carRenderInfo);

	carRenderInfo->CopyCarbonReplacementTable();
}

void AddWheelTexture(UsedCarTextureInfo* usedCarTextureInfo, RideInfo* rideInfo, Slot slot)
{
	auto wheel = rideInfo->GetPart(slot);
	if (wheel)
	{
		auto textureName = wheel->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
		if (textureName)
		{
			usedCarTextureInfo->AddToPerm(StringHash1("_WHEEL", textureName));
		}
	}
}

void __stdcall GetUsedCarTextureInfo(UsedCarTextureInfo* usedCarTextureInfo, RideInfo* rideInfo)
{
	Hash carHash = StringHash(rideInfo->GetCarTypeName());

	AddWheelTexture(usedCarTextureInfo, rideInfo, Slot::FRONT_WHEEL);
	AddWheelTexture(usedCarTextureInfo, rideInfo, Slot::REAR_WHEEL);

	usedCarTextureInfo->BRAKELIGHT_ONF = StringHash1("_KIT00_BRAKELIGHT_ONF", carHash);
	usedCarTextureInfo->BRAKELIGHT_GLASS_ONF = StringHash1("_KIT00_BRAKELIGHT_GLASS_ONF", carHash);

	auto leftBrakelight = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
	if (leftBrakelight)
	{
		auto texture = leftBrakelight->GetTextureName();
		if (texture)
		{
			usedCarTextureInfo->BRAKELIGHT_ONF = StringHash1("_ONF", texture);
			usedCarTextureInfo->BRAKELIGHT_GLASS_ONF = StringHash1("_GLASS_ONF", texture);
		}
	}

	usedCarTextureInfo->AddToPerm(usedCarTextureInfo->HEADLIGHT_DAMAGE0);
	usedCarTextureInfo->AddToPerm(usedCarTextureInfo->HEADLIGHT_GLASS_DAMAGE0);

	usedCarTextureInfo->AddToPerm(usedCarTextureInfo->BRAKELIGHT_ONF);
	usedCarTextureInfo->AddToPerm(usedCarTextureInfo->BRAKELIGHT_GLASS_ONF);

	usedCarTextureInfo->AddToPerm(usedCarTextureInfo->BRAKELIGHT_DAMAGE0);
	usedCarTextureInfo->AddToPerm(usedCarTextureInfo->BRAKELIGHT_GLASS_DAMAGE0);

	usedCarTextureInfo->AddToPerm(StringHash1("_REVERSE_ON", carHash));
	usedCarTextureInfo->AddToPerm(StringHash1("_REVERSE_OFF", carHash));

	Hash interiorHash = GetInteriorBaseTexture(rideInfo);
	usedCarTextureInfo->AddToPerm(StringHash1("_ON", interiorHash));
	usedCarTextureInfo->AddToPerm(StringHash1("_OFF", interiorHash));
	usedCarTextureInfo->AddToPerm(StringHash1("_N", interiorHash));
	usedCarTextureInfo->AddToPerm(interiorHash);
}

void __stdcall HandleTextureReplacements(CarRenderInfo* carRenderInfo)
{
	// General
	memset(carRenderInfo->GeneralReplacementTextures + 2, 0, 72 * 12);

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::SpolierSkin1].Update(Hashes::SPOILER_SKIN1, Hashes::METAL_SWATCH);

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::WindowFront].Update(Hashes::WINDOW_FRONT, Hashes::WINDOW_FRONT);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::WindowFrontLeft].Update(Hashes::WINDOW_LEFT_FRONT, Hashes::WINDOW_FRONT);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::WindowFrontRight].Update(Hashes::WINDOW_RIGHT_FRONT, Hashes::WINDOW_FRONT);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::WindowRearLeft].Update(Hashes::WINDOW_LEFT_REAR, Hashes::WINDOW_FRONT);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::WindowRearRight].Update(Hashes::WINDOW_RIGHT_REAR, Hashes::WINDOW_FRONT);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::WindowRear].Update(Hashes::WINDOW_REAR, Hashes::WINDOW_FRONT);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::RearDefroster].Update(Hashes::REAR_DEFROSTER, Hashes::REAR_DEFROSTER);

	Hash interiorHash = GetInteriorBaseTexture(carRenderInfo->pRideInfo);
	if (interiorHash)
	{
		if (!TextureInfo::Get(interiorHash, false, false))
		{
			interiorHash = StringHash1("_OFF", interiorHash);
		}

		carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::Interior].Update(Hashes::INTERIOR, interiorHash);
	}

	// Carbon
	carRenderInfo->CopyCarbonReplacementTable();
	carRenderInfo->CarbonReplacementTextures[(int)ReplacementTextureIndex::CarSkin].Update(carRenderInfo->UsedTextureInfo.SKIN1, Hashes::CARBONFIBRE);
	carRenderInfo->CarbonReplacementTextures[(int)ReplacementTextureIndex::SpolierSkin1].Update(Hashes::SPOILER_SKIN1, Hashes::CARBONFIBRE);
}

void __declspec(naked) GetUsedCarTextureInfoCave()
{
	__asm
	{
		pushad;
		push[esp + 0x94];
		push esi;
		call GetUsedCarTextureInfo;
		popad;

		pop esi;
		pop ebp;
		pop ebx;
		add esp, 0x60;
		ret;
	}
}

void __declspec(naked) HandleTextureReplacementsCave()
{
	__asm
	{
		pushad;
		push esi;
		call HandleTextureReplacements;
		popad;
		ret;
	}
}

void InitTextures()
{
	InitReplacementTextures();
	InitEmissiveTextures();

	injector::WriteMemory(0x007CECC7, ((int)Slot::LEFT_HEADLIGHT + 0x15) * 4);
	injector::WriteMemory(0x007CECCD, ((int)Slot::LEFT_BRAKELIGHT + 0x15) * 4);
	injector::MakeJMP(0x007CF764, GetUsedCarTextureInfoCave);

	injector::MakeCALL(0x007DE789, UpdateLightStateTextures);

	injector::MakeJMP(0x007D9E14, HandleTextureReplacementsCave);
}