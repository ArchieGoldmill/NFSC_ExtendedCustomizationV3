#pragma once
#include "Feature.h"
#include "FERenderingCar.h"
#include "DBCarPart.h"
#include "CarRenderInfo.h"

namespace Legacy
{
	bool ShowWarning(Slot slot)
	{
		auto rideInfo = FERenderingCar::GetRideInfo();
		if (g_Config.GetVersion(rideInfo->CarId) < 3)
		{
			auto carRenderInfo = rideInfo->pCarRenderInfo;

			if (slot == Slot::FRONT_BUMPER_BADGING_SET || slot == Slot::REAR_BUMPER_BADGING_SET ||
				slot == Slot::FRONT_BUMPER || slot == Slot::REAR_BUMPER || slot == Slot::SKIRT)
			{
				auto body = rideInfo->GetPart(Slot::BODY);
				return body && !body->IsStock();
			}

			if (slot == Slot::EXHAUST)
			{
				return !carRenderInfo->HasExhaustMarker();
			}
		}

		return false;
	}

	int __stdcall PartWarning(Slot slot, int a2)
	{
		if (ShowWarning(slot))
		{
			auto ShowPartWarning = (int(__stdcall*)(Hash, int))0x0085F8F0;
			ShowPartWarning(0x22105094, a2);
			return 1;
		}

		return 0;
	}

	int __stdcall DisableParts(Hash header)
	{
		auto slot = g_Config.GetSlotByHeader(header);
		return ShowWarning(slot);
	}

	void __declspec(naked) DisablePartsCave()
	{
		static constexpr auto cExit = 0x00866638;
		static constexpr auto cExit1 = 0x008665FC;
		__asm
		{
			pushad;
			mov eax, [ecx + 0x2C];
			push eax;
			call DisableParts;
			test eax, eax;
			popad;
			je enableParts;
			jmp cExit1;

		enableParts:
			jmp cExit;
		}
	}

	void __declspec(naked) PartWarningCave()
	{
		static constexpr auto cExit = 0x0085F7BD;
		__asm
		{
			pushad;
			push ebx;
			push eax;
			call PartWarning;
			test eax, eax;
			popad;
			je noWarn;
			pop edi;
			pop esi;
			pop ebx;
			ret 0x0014;

		noWarn:
			jmp cExit;
		}
	}

	void InitMenu()
	{
		injector::MakeJMP(0x0085F74C, PartWarningCave);
		injector::MakeJMP(0x008665DA, DisablePartsCave);
	}
}