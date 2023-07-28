#pragma once
#include "Feature.h"
#include "RideInfo.h"
#include "LegacyRandomParts.h"

void __fastcall SetRandomParts(RideInfo* rideInfo, int, int hash)
{
	int version = g_Config.GetVersion(rideInfo->CarId);
	if (version == 3)
	{

	}
	else if (version == 2)
	{
		Legacy::SetRandomParts(rideInfo, hash);
	}
	else
	{
		rideInfo->SetRandomParts(hash);
	}
}

int __fastcall CreateVehicle(int a1, int, int vltHash, int a3, int a4)
{
	FUNC(0x006298C0, int, __thiscall, GRacerInfo_CreateVehicle, int, int, int, int);
	return GRacerInfo_CreateVehicle(a1, g_Config.DebugAICar, a3, a4);
}

void InitRandomParts()
{
	injector::MakeCALL(0x00629D69, SetRandomParts);

	if (g_Config.DebugAICar)
	{
		injector::MakeCALL(0x0066985C, CreateVehicle);
	}
}