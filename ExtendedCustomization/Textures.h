#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Game.h"
#include "eLightContext.h"
#include "ReplacementTextureEntry.h"
#include "CarRenderConn.h"
#include "DBCarPart.h"
#include "ReplacementTextures.h"

void __fastcall UpdateLightStateTextures(CarRenderInfo* carRenderInfo)
{
	if (carRenderInfo->RideInfo->CarId.Id == 0x11)
	{
		auto entries = carRenderInfo->Get<ReplacementTextureEntry>(0x588);
		auto rideInfo = carRenderInfo->RideInfo;

		auto leftHeadlightPart = rideInfo->GetPart(Slot::LEFT_HEADLIGHT);
		if (leftHeadlightPart)
		{
			auto leftHeadlight = entries + 0x1B;
			auto rightHeadlight = entries + 0x1C;
			auto leftHeadlightGlass = entries + 0x20;
			auto rightHeadlightGlass = entries + 0x21;

			bool lightsOn = Game::InRace() && carRenderInfo->IsGlareOn();
			auto texture = leftHeadlightPart->GetTextureName();

			auto newName = StringHash1(lightsOn ? "_ON" : "_OFF", texture);
			leftHeadlight->Update(lightsOn ? Hashes::HEADLIGHT_LEFT_ON : Hashes::HEADLIGHT_LEFT_OFF, newName);
			rightHeadlight->Update(lightsOn ? Hashes::HEADLIGHT_RIGHT_ON : Hashes::HEADLIGHT_RIGHT_OFF, newName);

			newName = StringHash1(lightsOn ? "_GLASS_ON" : "_GLASS_OFF", texture);
			leftHeadlightGlass->Update(Hashes::HEADLIGHT_GLASS_LEFT, newName);
			rightHeadlightGlass->Update(Hashes::HEADLIGHT_GLASS_RIGHT, newName);
		}

		auto leftBrakelightPart = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
		if (leftBrakelightPart)
		{
			auto leftBrakelight = entries + 0x1D;
			auto rightBrakelight = entries + 0x1E;
			auto leftBrakelightGlass = entries + 0x1F;
			auto rightBrakelightGlass = entries + 0x22;
			auto centerBrakelight = entries + 0x23;

			auto texture = leftBrakelightPart->GetTextureName();

			auto newName = StringHash1(Game::InRace() ? (carRenderInfo->IsLeftBrakelightOn() ? "_ON" : "_ONF") : "_OFF", texture);
			leftBrakelight->Update(Game::InRace() ? Hashes::BRAKELIGHT_LEFT_ON : Hashes::BRAKELIGHT_LEFT_OFF, newName);
			rightBrakelight->Update(Game::InRace() ? Hashes::BRAKELIGHT_RIGHT_ON : Hashes::BRAKELIGHT_RIGHT_OFF, newName);
			centerBrakelight->Update(Game::InRace() ? Hashes::BRAKELIGHT_CENTRE_ON : Hashes::BRAKELIGHT_CENTRE_OFF, newName);

			newName = StringHash1(Game::InRace() ? "_GLASS_ON" : "_GLASS_OFF", texture);
			leftBrakelightGlass->Update(Hashes::BRAKELIGHT_GLASS_LEFT, newName);
			rightBrakelightGlass->Update(Hashes::BRAKELIGHT_GLASS_RIGHT, newName);
		}

		Hash carHash = StringHash(rideInfo->GetCarTypeName());
		bool isReverseOn = carRenderInfo->IsReverseOn();
		auto newName = isReverseOn ? StringHash1("_REVERSE_ON", carHash) : StringHash1("_REVERSE_OFF", carHash);
		auto reverse = entries + 0x24;

		reverse->Update(carRenderInfo->IsReverseOn() ? Hashes::REVERSE_ON : Hashes::REVERSE_OFF, newName);
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

void __stdcall GetUsedCarTextureInfo(Hash* texPtr, RideInfo* rideInfo)
{
	auto leftBrakelight = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
	if (leftBrakelight)
	{
		auto texture = leftBrakelight->GetTextureName();
		SetTextureHash(texPtr, StringHash1("_ONF", texture));
	}

	Hash carHash = StringHash(rideInfo->GetCarTypeName());
	SetTextureHash(texPtr, StringHash1("_REVERSE_ON", carHash));
	SetTextureHash(texPtr, StringHash1("_REVERSE_OFF", carHash));
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

void InitTextures()
{
	InitReplacementTextures();

	injector::WriteMemory(0x007CECC7, ((int)Slot::LEFT_HEADLIGHT + 0x15) * 4);
	injector::WriteMemory(0x007CECCD, ((int)Slot::LEFT_BRAKELIGHT + 0x15) * 4);
	injector::MakeJMP(0x007CF764, GetUsedCarTextureInfoCave);

	injector::MakeCALL(0x007DE789, UpdateLightStateTextures);
}