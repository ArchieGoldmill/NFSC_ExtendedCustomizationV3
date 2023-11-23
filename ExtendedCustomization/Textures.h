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
	auto entries = carRenderInfo->Get<ReplacementTextureEntry>(0x6CC);
	auto rideInfo = carRenderInfo->pRideInfo;

	Hash headlight = InitTexture(rideInfo, Slot::LEFT_HEADLIGHT, "_KIT00_HEADLIGHT");
	Hash brakelight = InitTexture(rideInfo, Slot::LEFT_BRAKELIGHT, "_KIT00_BRAKELIGHT");

	auto headlightLeft = entries++;
	auto headlightRight = entries++;
	auto headlightGlassLeft = entries++;
	auto headlightGlassRight = entries++;

	Hash headlightOff = StringHash1("_OFF", headlight);
	Hash headlightGlassOff = StringHash1("_GLASS_OFF", headlight);
	Hash headlightOn = StringHash1("_ON", headlight);
	Hash headlightGlassOn = StringHash1("_GLASS_ON", headlight);
	Hash headlightHash, headlighGlasstHash;
	if (TextureInfo::Get(headlightOff, false, false))
	{
		bool lightsOn = Game::InRace() && carRenderInfo->IsGlareOn();
		headlightHash = lightsOn ? headlightOn : headlightOff;
		headlighGlasstHash = lightsOn ? headlightGlassOn : headlightGlassOff;
	}
	else
	{
		headlightHash = headlightOn;
		headlighGlasstHash = headlightGlassOn;
	}

	headlightLeft->Update(Hashes::HEADLIGHT_LEFT, headlightHash);
	headlightRight->Update(Hashes::HEADLIGHT_RIGHT, headlightHash);
	headlightGlassLeft->Update(Hashes::HEADLIGHT_GLASS_LEFT, headlighGlasstHash);
	headlightGlassRight->Update(Hashes::HEADLIGHT_GLASS_RIGHT, headlighGlasstHash);

	auto brakelightLeft = entries++;
	auto brakelightRight = entries++;
	auto brakelightGlassLeft = entries++;
	auto brakelightGlassRight = entries++;
	auto brakelightCentre = entries++;
	auto brakelightGlassCentre = entries++;

	Hash brakelightHash, brakelightGlassHash;
	Hash brakelightOnf = StringHash1("_ONF", brakelight);
	Hash brakelightOn = StringHash1("_ON", brakelight);
	Hash brakelightGlassOn = StringHash1("_GLASS_ON", brakelight);
	Hash brakelightGlassOff = StringHash1("_GLASS_OFF", brakelight);
	Hash brakelightGlassOnf = StringHash1("_GLASS_ONF", brakelight);
	Hash brakelightOff = StringHash1("_OFF", brakelight);

	brakelightCentre->Update(Hashes::BRAKELIGHT_CENTRE, carRenderInfo->IsLeftBrakelightOn() ? brakelightOn : brakelightOff);
	brakelightGlassCentre->Update(Hashes::BRAKELIGHT_GLASS_CENTRE, carRenderInfo->IsLeftBrakelightOn() ? brakelightGlassOn : brakelightGlassOff);

	if (TextureInfo::Get(brakelightOnf, false, false) && Game::InRace())
	{
		brakelightOff = brakelightOnf;
	}

	if (TextureInfo::Get(brakelightGlassOnf, false, false) && Game::InRace())
	{
		brakelightGlassOff = brakelightGlassOnf;
	}

	brakelightLeft->Update(Hashes::BRAKELIGHT_LEFT, carRenderInfo->IsLeftBrakelightOn() ? brakelightOn : brakelightOff);
	brakelightRight->Update(Hashes::BRAKELIGHT_RIGHT, carRenderInfo->IsRightBrakelightOn() ? brakelightOn : brakelightOff);

	brakelightGlassLeft->Update(Hashes::BRAKELIGHT_GLASS_LEFT, carRenderInfo->IsLeftBrakelightOn() ? brakelightGlassOn : brakelightGlassOff);
	brakelightGlassRight->Update(Hashes::BRAKELIGHT_GLASS_RIGHT, carRenderInfo->IsRightBrakelightOn() ? brakelightGlassOn : brakelightGlassOff);

	// Reverse
	auto reverse = entries++;
	Hash carHash = StringHash(rideInfo->GetCarTypeName());
	reverse->Update(Hashes::REVERSE, carRenderInfo->IsReverseOn() ? StringHash1("_REVERSE_ON", carHash) : StringHash1("_REVERSE_OFF", carHash));

	// Interior
	auto interior = entries++;
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

	interior->Update(Hashes::INTERIOR_GLOW, interiorHash);
}

void SetTextureHash(Hash* texPtr, Hash hash)
{
	for (int i = 0; i < 100; i++)
	{
		// We already have this texture
		if (texPtr[i] == hash)
		{
			return;
		}

		if (texPtr[i] == 0)
		{
			texPtr[i] = hash;
			texPtr[0xD0]++;
			return;
		}
	}

	MessageBoxA(NULL, "Unable to set texture hash! Please report.", "Extended Customization", MB_ICONERROR);
}

void SetWheelTexture(Hash* texPtr, RideInfo* rideInfo, Slot slot)
{
	auto wheel = rideInfo->GetPart(slot);
	if (wheel)
	{
		auto textureName = wheel->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
		if (textureName)
		{
			SetTextureHash(texPtr, StringHash1("_WHEEL", textureName));
		}
	}
}

void __stdcall GetUsedCarTextureInfo(Hash* texPtr, RideInfo* rideInfo)
{
	Hash carHash = StringHash(rideInfo->GetCarTypeName());

	SetWheelTexture(texPtr, rideInfo, Slot::FRONT_WHEEL);
	SetWheelTexture(texPtr, rideInfo, Slot::REAR_WHEEL);

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

	SetTextureHash(texPtr, brakelightOnf);
	SetTextureHash(texPtr, brakelightGlassOnf);

	SetTextureHash(texPtr, StringHash1("_REVERSE_ON", carHash));
	SetTextureHash(texPtr, StringHash1("_REVERSE_OFF", carHash));

	Hash interiorHash = GetInteriorBaseTexture(rideInfo);
	SetTextureHash(texPtr, StringHash1("_ON", interiorHash));
	SetTextureHash(texPtr, StringHash1("_OFF", interiorHash));
	SetTextureHash(texPtr, StringHash1("_N", interiorHash));
	SetTextureHash(texPtr, interiorHash);
}

void __stdcall HandleTextureReplacements(CarRenderInfo* carRenderInfo)
{
	Hash interiorHash = GetInteriorBaseTexture(carRenderInfo->pRideInfo);
	if (interiorHash)
	{
		if (!TextureInfo::Get(interiorHash, false, false))
		{
			interiorHash = StringHash1("_OFF", interiorHash);
		}

		auto badgingEntry = carRenderInfo->Get<ReplacementTextureEntry>(0x5C4);
		badgingEntry->Set(Hashes::INTERIOR, interiorHash);
	}
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