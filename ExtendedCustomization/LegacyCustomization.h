#pragma once
#include "Feature.h"
#include "FeGarageMain.h"
#include "FECarRecord.h"

namespace Legacy
{
	void UninstallPart(RideInfo* rideInfo, FECustomizationRecord* record, Slot slot)
	{
		if (record)
		{
			record->UnInstallPart(rideInfo->CarId, slot);
		}

		if (rideInfo)
		{
			rideInfo->SetPart(slot, 0);
		}
	}

	void HandleExhaust(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	{
		auto rearBumper = rideInfo->GetPart(Slot::REAR_BUMPER);
		if (rearBumper)
		{
			if (rearBumper->IsStock())
			{
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::EXHAUST, 1, "%s_KIT00_EXHAUST", carName);
			}
			else
			{
				auto exhaust = rideInfo->GetPart(Slot::EXHAUST);
				if (!exhaust || exhaust->IsStock())
				{
					FeGarageMain::InstallPart(_this, rideInfo, record, Slot::EXHAUST, 1, "EXHAUST_STYLE17_LEVEL1");
				}
			}
		}
		else
		{
			UninstallPart(rideInfo, record, Slot::EXHAUST);
		}
	}

	void HandleBadging(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, const char* position, Slot slot, char* carName)
	{
		auto body = rideInfo->GetPart(Slot::BODY);
		if (body && !body->IsStock())
		{
			UninstallPart(rideInfo, record, Slot::FRONT_BUMPER_BADGING_SET);
			UninstallPart(rideInfo, record, Slot::REAR_BUMPER_BADGING_SET);
			return;
		}

		if (g_Config.GetPart(slot, rideInfo->CarId).State == State::Enabled)
		{
			auto badgingPart = rideInfo->GetPart(slot);
			if (!badgingPart)
			{
				if (!body || body->IsStock())
				{
					FeGarageMain::InstallPart(_this, rideInfo, record, slot, 1, "%s_KIT00_%s_BUMPER_BADGING_SET", carName, position);
				}
			}
		}
		else
		{
			auto bumperPart = rideInfo->GetPart((Slot)((int)slot - 1));
			if (bumperPart)
			{
				int kit = bumperPart->GetKit();
				FeGarageMain::InstallPart(_this, rideInfo, record, slot, 1, "%s_KIT%02d_%s_BUMPER_BADGING_SET", carName, kit, position);
			}
		}
	}

	void HandleDoors(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	{
		if (g_Config.GetPart(Slot::DOOR_LEFT, rideInfo->CarId).State == State::Enabled)
		{
			auto leftDoor = rideInfo->GetPart(Slot::DOOR_LEFT);
			if (leftDoor)
			{
				int kit = leftDoor->GetKit();
				if (kit)
				{
					FeGarageMain::InstallPart(_this, rideInfo, record, Slot::DOOR_RIGHT, 1, "%s_KIT%02d_DOOR_RIGHT", carName, kit);
				}
				else
				{
					FeGarageMain::InstallPart(_this, rideInfo, record, Slot::DOOR_RIGHT, 1, "%s_DOOR_RIGHT", carName);
				}
			}
		}
		else
		{
			auto body = rideInfo->GetPart(Slot::BODY);
			if (body)
			{
				int kit = body->GetKit();
				if (kit)
				{
					FeGarageMain::InstallPart(_this, rideInfo, record, Slot::DOOR_LEFT, 1, "%s_KITW%02d_DOOR_LEFT", carName, kit);
					FeGarageMain::InstallPart(_this, rideInfo, record, Slot::DOOR_RIGHT, 1, "%s_KITW%02d_DOOR_RIGHT", carName, kit);
				}
				else
				{
					FeGarageMain::InstallPart(_this, rideInfo, record, Slot::DOOR_LEFT, 1, "%s_DOOR_LEFT", carName);
					FeGarageMain::InstallPart(_this, rideInfo, record, Slot::DOOR_RIGHT, 1, "%s_DOOR_RIGHT", carName);
				}
			}
		}
	}

	void HandleSpecialCustomizationV2(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record)
	{
		if (!rideInfo)
		{
			return;
		}

		char* carName = GetCarTypeName(rideInfo->CarId);

		HandleBadging(feGarageMain, rideInfo, record, "FRONT", Slot::FRONT_BUMPER_BADGING_SET, carName);
		HandleBadging(feGarageMain, rideInfo, record, "REAR", Slot::REAR_BUMPER_BADGING_SET, carName);
		HandleDoors(feGarageMain, rideInfo, record, carName);
		HandleExhaust(feGarageMain, rideInfo, record, carName);
	}

	int __fastcall IsSetHeadlightOn(FECarRecord* feCarRecord)
	{
		auto carId = feCarRecord->GetType();

		if (g_Config.GetPopUpHeadLights(carId) == State::Enabled)
		{
			return 5;
		}

		return 0;
	}

	int __stdcall CarHasPopups(CarType carId)
	{
		return g_Config.GetPopUpHeadLights(carId) == State::Enabled;
	}

	void __declspec(naked) CarHasPopupsCave()
	{
		static constexpr auto cExit = 0x0085980E;
		__asm
		{
			mov eax, [esi];
			pushad;
			push eax;
			call CarHasPopups;
			cmp eax, 1;
			popad;

			jmp cExit;
		}
	}

	void InitCustomization()
	{
		injector::MakeCALL(0x00865278, IsSetHeadlightOn);
		injector::MakeNOP(0x00865280, 60);

		injector::MakeJMP(0x00859809, CarHasPopupsCave);
		injector::MakeNOP(0x00859819, 23);
	}
}