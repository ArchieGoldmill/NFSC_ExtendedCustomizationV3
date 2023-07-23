#pragma once
#include "Feature.h"
#include "FeGarageMain.h"
#include "FECarRecord.h"
#include "Config.h"

namespace Legacy
{
	//void HandleDoors(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record, char* carName)
	//{
	//	if (Config::GetPartState(rideInfo->CarId, DBPart::LeftDoor) == Config::EnabledState)
	//	{
	//		int* leftDoor = Game::GetPart(rideInfo, DBPart::LeftDoor);
	//		if (leftDoor)
	//		{
	//			int kit = Game::GetAppliedAttributeIParam1(leftDoor, Game::StringHash((char*)"KITNUMBER"), 0);
	//			if (kit)
	//			{
	//				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftDoor, 1, "%s_KIT%02d_DOOR_RIGHT", carName, kit);
	//			}
	//			else
	//			{
	//				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightDoor, 1, "%s_DOOR_RIGHT", carName);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		int* body = Game::GetPart(rideInfo, DBPart::Body);
	//		if (body)
	//		{
	//			int kit = Game::GetAppliedAttributeIParam1(body, Game::StringHash((char*)"KITNUMBER"), 0);
	//			if (kit)
	//			{
	//				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftDoor, 1, "%s_KITW%02d_DOOR_LEFT", carName, kit);
	//				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightDoor, 1, "%s_KITW%02d_DOOR_RIGHT", carName, kit);
	//			}
	//			else
	//			{
	//				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::LeftDoor, 1, "%s_DOOR_LEFT", carName);
	//				Game::InstallPart(_this, rideInfo, FECustomizationRecord, DBPart::RightDoor, 1, "%s_DOOR_RIGHT", carName);
	//			}
	//		}
	//	}
	//}

	void HandleSpecialCustomizationV2(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record)
	{
		if (!rideInfo)
		{
			return;
		}

		char* carName = GetCarTypeName(rideInfo->CarId);
		//HandleDoors(feGarageMain, record, rideInfo, carName);
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