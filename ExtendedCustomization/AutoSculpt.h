#pragma once
#include "Func.h"
#include "Math.h"

struct RideInfo;

struct AutoSculptRegion
{
	float Zones[11];
	int Modified;

	int GetInt(int zone)
	{
		return ToInt(this->Zones[zone] * 100.0f);
	}
};

struct AutoSculpt
{
	int IsInitialized;
	AutoSculptRegion Regions[0x15];

	static int GetActiveZoneNum(int a1)
	{
		FUNC(0x007B9AE0, int, __stdcall, _GetActiveZoneNum, int);
		return _GetActiveZoneNum(a1);
	}

	void Pose(Hash* hashes, int count, RideInfo* rideInfo)
	{
		static auto _AutoSculptPose = (void(__thiscall*)(AutoSculpt*, Hash*, int, RideInfo*))0x007DC7F0;
		_AutoSculptPose(this, hashes, count, rideInfo);
	}
};