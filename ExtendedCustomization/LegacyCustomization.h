#pragma once
#include "Feature.h"
#include "FeGarageMain.h"
#include "FECarRecord.h"

namespace Legacy
{
	void SetHeadlightsOff(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	{
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::LEFT_HEADLIGHT, 1, "%s_LEFT_HEADLIGHT_OFF", carName);
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_HEADLIGHT, 1, "%s_RIGHT_HEADLIGHT_OFF", carName);
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_HEADLIGHT_GLASS, 1, "%s_RIGHT_HEADLIGHT_GLASS_OFF", carName);
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::LEFT_HEADLIGHT_GLASS, 1, "%s_LEFT_HEADLIGHT_GLASS_OFF", carName);
	}

	void SetHeadlightsOn(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	{
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::LEFT_HEADLIGHT, 1, "%s_LEFT_HEADLIGHT", carName);
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_HEADLIGHT, 1, "%s_RIGHT_HEADLIGHT", carName);
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_HEADLIGHT_GLASS, 1, "%s_RIGHT_HEADLIGHT_GLASS", carName);
		FeGarageMain::InstallPart(_this, rideInfo, record, Slot::LEFT_HEADLIGHT_GLASS, 1, "%s_LEFT_HEADLIGHT_GLASS", carName);
	}

	void SetHeadlights(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	{
		auto leftHeadlight = rideInfo->GetPart(Slot::LEFT_HEADLIGHT);
		if (leftHeadlight)
		{
			int kit = leftHeadlight->GetKit();
			if (kit)
			{
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_HEADLIGHT, 1, "%s_KIT%02d_RIGHT_HEADLIGHT", carName, kit);
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_HEADLIGHT_GLASS, 1, "%s_KIT%02d_RIGHT_HEADLIGHT_GLASS", carName, kit);
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::LEFT_HEADLIGHT_GLASS, 1, "%s_KIT%02d_LEFT_HEADLIGHT_GLASS", carName, kit);
			}
			else
			{
				SetHeadlightsOn(_this, rideInfo, record, carName);
			}
		}
	}

	void HandleHeadlights(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName, bool bot = false)
	{
		if (!bot && g_Config.GetPopUpHeadLights(rideInfo->CarId) == State::Enabled)
		{
			SetHeadlightsOff(_this, rideInfo, record, carName);
			return;
		}

		if (g_Config.GetPart(Slot::LEFT_HEADLIGHT, rideInfo->CarId).IsEnabled())
		{
			SetHeadlights(_this, rideInfo, record, carName);
			return;
		}


		SetHeadlightsOn(_this, rideInfo, record, carName);
	}

	void HandleBrakelights(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	{
		auto leftBrakelight = rideInfo->GetPart(Slot::LEFT_BRAKELIGHT);
		if (leftBrakelight)
		{
			int kit = leftBrakelight->GetKit();
			if (kit)
			{
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_BRAKELIGHT, 1, "%s_KIT%02d_RIGHT_BRAKELIGHT", carName, kit);
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::LEFT_BRAKELIGHT_GLASS, 1, "%s_KIT%02d_LEFT_BRAKELIGHT_GLASS", carName, kit);
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_BRAKELIGHT_GLASS, 1, "%s_KIT%02d_RIGHT_BRAKELIGHT_GLASS", carName, kit);
			}
			else
			{
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_BRAKELIGHT, 1, "%s_RIGHT_BRAKELIGHT", carName);
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::LEFT_BRAKELIGHT_GLASS, 1, "%s_LEFT_BRAKELIGHT_GLASS", carName);
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_BRAKELIGHT_GLASS, 1, "%s_RIGHT_BRAKELIGHT_GLASS", carName);
			}
		}
	}

	void HandleSideMirrors(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	{
		auto leftMirror = rideInfo->GetPart(Slot::LEFT_SIDE_MIRROR);
		if (leftMirror)
		{
			int kit = leftMirror->GetKit();
			if (kit)
			{
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_SIDE_MIRROR, 1, "%s_KIT%02d_RIGHT_SIDE_MIRROR", carName, kit);
			}
			else
			{
				FeGarageMain::InstallPart(_this, rideInfo, record, Slot::RIGHT_SIDE_MIRROR, 1, "%s_RIGHT_SIDE_MIRROR", carName);
			}
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
			FeGarageMain::UnInstall(rideInfo, record, Slot::EXHAUST);
		}
	}

	void HandleBadging(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record)
	{
		auto body = rideInfo->GetPart(Slot::BODY);
		if (body && !body->IsStock())
		{
			FeGarageMain::UnInstall(rideInfo, record, Slot::FRONT_BUMPER_BADGING_SET);
			FeGarageMain::UnInstall(rideInfo, record, Slot::REAR_BUMPER_BADGING_SET);
			return;
		}
	}

	void HandleBadging(FeGarageMain* _this, RideInfo* rideInfo, FECustomizationRecord* record, const char* position, Slot slot, char* carName)
	{
		auto body = rideInfo->GetPart(Slot::BODY);
		if (g_Config.GetPart(slot, rideInfo->CarId).IsEnabled())
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
		if (g_Config.GetPart(Slot::DOOR_LEFT, rideInfo->CarId).IsEnabled())
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

	void HandleBody(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record)
	{
		auto body = rideInfo->GetPart(Slot::BODY);
		if (body && !body->IsStock())
		{
			FeGarageMain::UnInstall(rideInfo, record, Slot::FRONT_BUMPER);
			FeGarageMain::UnInstall(rideInfo, record, Slot::REAR_BUMPER);
			FeGarageMain::UnInstall(rideInfo, record, Slot::SKIRT);
		}
	}

	void HandleSpecialCustomizationV2(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record, bool bot = false)
	{
		if (!rideInfo)
		{
			return;
		}

		char* carName = GetCarTypeName(rideInfo->CarId);

		HandleBody(feGarageMain, rideInfo, record);
		HandleBadging(feGarageMain, rideInfo, record);
		HandleBadging(feGarageMain, rideInfo, record, "FRONT", Slot::FRONT_BUMPER_BADGING_SET, carName);
		HandleBadging(feGarageMain, rideInfo, record, "REAR", Slot::REAR_BUMPER_BADGING_SET, carName);
		HandleDoors(feGarageMain, rideInfo, record, carName);
		HandleExhaust(feGarageMain, rideInfo, record, carName);
		HandleSideMirrors(feGarageMain, rideInfo, record, carName);
		HandleBrakelights(feGarageMain, rideInfo, record, carName);
		HandleHeadlights(feGarageMain, rideInfo, record, carName, bot);
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