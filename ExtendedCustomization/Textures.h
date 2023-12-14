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

void __fastcall UpdateLightStateTextures(CarRenderInfo* carRenderInfo)
{
	auto rideInfo = carRenderInfo->pRideInfo;

	Hash headlight = InitTexture(rideInfo, Slot::LEFT_HEADLIGHT, "_KIT00_HEADLIGHT");
	Hash brakelight = InitTexture(rideInfo, Slot::LEFT_BRAKELIGHT, "_KIT00_BRAKELIGHT");

	Hash headlightOff = StringHash1("_OFF", headlight);
	Hash headlightGlassOff = StringHash1("_GLASS_OFF", headlight);
	Hash headlightOn = StringHash1("_ON", headlight);
	Hash headlightGlassOn = StringHash1("_GLASS_ON", headlight);
	Hash headlightHash, headlighGlasstHash;
	if (TextureInfo::Get(headlightOff, false, false))
	{
		bool lightsOn = Game::InRace() && carRenderInfo->IsGlareOn(VehicleFX::LIGHT_HEADLIGHTS);
		headlightHash = lightsOn ? headlightOn : headlightOff;
		headlighGlasstHash = lightsOn ? headlightGlassOn : headlightGlassOff;
	}
	else
	{
		headlightHash = headlightOn;
		headlighGlasstHash = headlightGlassOn;
	}

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightLeft].Update(Hashes::HEADLIGHT_LEFT, headlightHash);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightRight].Update(Hashes::HEADLIGHT_RIGHT, headlightHash);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightGlassLeft].Update(Hashes::HEADLIGHT_GLASS_LEFT, headlighGlasstHash);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::HeadlightGlassRight].Update(Hashes::HEADLIGHT_GLASS_RIGHT, headlighGlasstHash);

	Hash brakelightHash, brakelightGlassHash;
	Hash brakelightOnf = StringHash1("_ONF", brakelight);
	Hash brakelightOn = StringHash1("_ON", brakelight);
	Hash brakelightGlassOn = StringHash1("_GLASS_ON", brakelight);
	Hash brakelightGlassOff = StringHash1("_GLASS_OFF", brakelight);
	Hash brakelightGlassOnf = StringHash1("_GLASS_ONF", brakelight);
	Hash brakelightOff = StringHash1("_OFF", brakelight);

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightCenter].Update(Hashes::BRAKELIGHT_CENTRE, carRenderInfo->IsLeftBrakelightOn() ? brakelightOn : brakelightOff);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightGlassCenter].Update(Hashes::BRAKELIGHT_GLASS_CENTRE, carRenderInfo->IsLeftBrakelightOn() ? brakelightGlassOn : brakelightGlassOff);

	if (TextureInfo::Get(brakelightOnf, false, false) && Game::InRace())
	{
		brakelightOff = brakelightOnf;
	}

	if (TextureInfo::Get(brakelightGlassOnf, false, false) && Game::InRace())
	{
		brakelightGlassOff = brakelightGlassOnf;
	}

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightLeft].Update(Hashes::BRAKELIGHT_LEFT, carRenderInfo->IsLeftBrakelightOn() ? brakelightOn : brakelightOff);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightRight].Update(Hashes::BRAKELIGHT_RIGHT, carRenderInfo->IsRightBrakelightOn() ? brakelightOn : brakelightOff);

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightGlassLeft].Update(Hashes::BRAKELIGHT_GLASS_LEFT, carRenderInfo->IsLeftBrakelightOn() ? brakelightGlassOn : brakelightGlassOff);
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::BrakelightGlassRight].Update(Hashes::BRAKELIGHT_GLASS_RIGHT, carRenderInfo->IsRightBrakelightOn() ? brakelightGlassOn : brakelightGlassOff);

	// Reverse
	Hash carHash = StringHash(rideInfo->GetCarTypeName());
	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::Reverse].Update(Hashes::REVERSE, carRenderInfo->IsReverseOn() ? StringHash1("_REVERSE_ON", carHash) : StringHash1("_REVERSE_OFF", carHash));

	// Interior
	Hash interiorHash = GetInteriorBaseTexture(rideInfo);
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

	Hash brakelightOnf = StringHash1("_KIT00_BRAKELIGHT_ONF", carHash);
	Hash brakelightGlassOnf = StringHash1("_KIT00_BRAKELIGHT_GLASS_ONF", carHash);
	auto leftBrakelight = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
	if (leftBrakelight)
	{
		auto texture = leftBrakelight->GetTextureName();
		if (texture)
		{
			brakelightOnf = StringHash1("_ONF", texture);
			brakelightGlassOnf = StringHash1("_GLASS_ONF", texture);
		}
	}

	usedCarTextureInfo->AddToPerm(brakelightOnf);
	usedCarTextureInfo->AddToPerm(brakelightGlassOnf);

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

	carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::GlobalSkin].Update(Hashes::GLOBAL_SKIN, Hashes::METAL_SWATCH);

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
	carRenderInfo->CarbonReplacementTextures[(int)ReplacementTextureIndex::GlobalSkin].Update(Hashes::GLOBAL_SKIN, Hashes::CARBONFIBRE);
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