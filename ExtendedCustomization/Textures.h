#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Game.h"
#include "ReplacementTextureEntry.h"
#include "CarRenderConn.h"
#include "DBCarPart.h"
#include "ReplacementTextures.h"
#include "EmissiveTextures.h"

void __fastcall UpdateLightStateTextures(CarRenderInfo* carRenderInfo)
{
	auto rideInfo = carRenderInfo->pRideInfo;
	int version = g_Config.GetVersion(rideInfo->CarId);
	if (version == 3)
	{
		auto entries = carRenderInfo->Get<ReplacementTextureEntry>(0x6CC);

		auto leftHeadlightPart = rideInfo->GetPart(Slot::LEFT_HEADLIGHT);
		if (leftHeadlightPart)
		{
			auto headlights = entries++;
			auto headlightsOnOff = entries++;
			auto headlightsGlass = entries++;

			bool lightsOn = Game::InRace() && carRenderInfo->IsGlareOn();
			auto texture = leftHeadlightPart->GetTextureName();

			headlights->Update(Hashes::HEADLIGHTS, texture);

			auto newName = StringHash1(lightsOn ? "_ON" : "_OFF", texture);
			headlightsOnOff->Update(lightsOn ? Hashes::HEADLIGHTS_ON : Hashes::HEADLIGHTS_OFF, newName);

			newName = StringHash1(lightsOn ? "_GLASS_ON" : "_GLASS_OFF", texture);
			headlightsGlass->Update(Hashes::HEADLIGHTS_GLASS, newName);
		}

		auto leftBrakelightPart = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
		if (leftBrakelightPart)
		{
			auto brakelights = entries++;
			auto brakelightsOnOff = entries++;
			auto brakelightsGlass = entries++;

			auto texture = leftBrakelightPart->GetTextureName();

			brakelights->Update(Hashes::BRAKELIGHTS, texture);

			auto newName = StringHash1(Game::InRace() ? (carRenderInfo->IsLeftBrakelightOn() ? "_ON" : "_ONF") : "_OFF", texture);
			brakelightsOnOff->Update(Game::InRace() ? Hashes::BRAKELIGHTS_ON : Hashes::BRAKELIGHTS_OFF, newName);

			newName = StringHash1(Game::InRace() ? "_GLASS_ON" : "_GLASS_OFF", texture);
			brakelightsGlass->Update(Hashes::BRAKELIGHTS_GLASS, newName);
		}

		Hash carHash = StringHash(rideInfo->GetCarTypeName());
		bool isReverseOn = carRenderInfo->IsReverseOn();
		auto newName = isReverseOn ? StringHash1("_REVERSE_ON", carHash) : StringHash1("_REVERSE_OFF", carHash);
		auto reverse = entries++;
		reverse->Update(isReverseOn ? Hashes::REVERSE_ON : Hashes::REVERSE_OFF, newName);

		newName = Game::InRace() ? StringHash1("_INTERIOR_ON", carHash) : StringHash1("_INTERIOR_OFF", carHash);
		auto interiorOn = entries++;
		interiorOn->Update(Game::InRace() ? Hashes::INTERIOR_ON : Hashes::INTERIOR_OFF, newName);
	}
	else
	{
		carRenderInfo->UpdateLightStateTextures();
	}
}

void SetTextureHash(Hash* texPtr, Hash hash)
{
	for (int i = 0; i < 100; i++)
	{
		// We already have this texture
		if (texPtr[i] == hash)
		{
			break;
		}

		if (texPtr[i] == 0)
		{
			texPtr[i] = hash;
			break;
		}
	}

	texPtr[0xD0]++;
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
	SetWheelTexture(texPtr, rideInfo, Slot::FRONT_WHEEL);
	SetWheelTexture(texPtr, rideInfo, Slot::REAR_WHEEL);

	auto leftBrakelight = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
	if (leftBrakelight)
	{
		auto texture = leftBrakelight->GetTextureName();
		SetTextureHash(texPtr, texture);
		SetTextureHash(texPtr, StringHash1("_ONF", texture));
	}

	auto leftHeadlight = rideInfo->GetPart(Slot::LEFT_HEADLIGHT);
	if (leftHeadlight)
	{
		auto texture = leftHeadlight->GetTextureName();
		SetTextureHash(texPtr, texture);
	}

	Hash carHash = StringHash(rideInfo->GetCarTypeName());
	SetTextureHash(texPtr, StringHash1("_REVERSE_ON", carHash));
	SetTextureHash(texPtr, StringHash1("_REVERSE_OFF", carHash));
	SetTextureHash(texPtr, StringHash1("_INTERIOR_ON", carHash));

	auto interior = rideInfo->GetPart(Slot::INTERIOR);
	if (interior)
	{
		auto textureName = interior->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
		if (textureName)
		{
			SetTextureHash(texPtr, textureName);
		}
	}

	// Emissive
	SetTextureHash(texPtr, StringHash1("_INTERIOR_E", carHash));
}

void __stdcall HandleTextureReplacements(CarRenderInfo* carRenderInfo)
{
	auto interior = carRenderInfo->pRideInfo->GetPart(Slot::INTERIOR);
	if (interior)
	{
		auto textureName = interior->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
		if (textureName)
		{
			auto badgingEntry = carRenderInfo->Get< ReplacementTextureEntry>(0x5C4);
			badgingEntry->Set(Hashes::INTERIOR, textureName);
		}
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