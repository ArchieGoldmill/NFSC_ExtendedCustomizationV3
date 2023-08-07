#pragma once
#include "Feature.h"
#include "RideInfo.h"
#include "Constants.h"

float __stdcall GetRideHeight(RideInfo* rideInfo)
{
	float rideHeight = rideInfo->RideHeight;
	auto body = rideInfo->GetPart(Slot::BODY);
	if (body)
	{
		float offset = body->GetAppliedAttributeFParam(Hashes::RIDE_HEIGHT_OFFSET, 0);
		rideHeight -= offset;
	}

	return rideHeight;
}

void __declspec(naked) RideHeightCave4()
{
	static constexpr auto cExit = 0x007C1C53;

	_asm
	{
		SAVE_REGS_ECX;
		push eax;
		call GetRideHeight;
		push ecx;
		fstp[esp];
		pop ecx;
		RESTORE_REGS_ECX;

		jmp cExit;
	}
}

void __declspec(naked) RideHeightCave3()
{
	static constexpr auto cExit = 0x007DF76B;

	_asm
	{
		pushad;
		push ecx;
		call GetRideHeight;
		popad;

		jmp cExit;
	}
}


void __declspec(naked) RideHeightCave2()
{
	static constexpr auto cExit = 0x007E1003;

	_asm
	{
		pushad;
		push ecx;
		call GetRideHeight;
		popad;

		jmp cExit;
	}
}

void __declspec(naked) RideHeightCave1()
{
	static constexpr auto cExit = 0x007AD7CF;

	_asm
	{
		SAVE_REGS_EDX;
		push edx;
		call GetRideHeight;
		push edx;
		fstp[esp];
		pop edx;
		RESTORE_REGS_EDX;

		jmp cExit;
	}
}

void InitRideHeight()
{
	// Remove ride height reset if goes to low
	injector::WriteMemory<size_t>(0x007C1FA5, 0x64D8900000013AE9);

	injector::MakeJMP(0x007AD7C9, RideHeightCave1);
	injector::MakeJMP(0x007E0FFD, RideHeightCave2);
	injector::MakeJMP(0x007DF765, RideHeightCave3);
	injector::MakeJMP(0x007C1C4D, RideHeightCave4);
}