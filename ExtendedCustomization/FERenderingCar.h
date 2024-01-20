#pragma once
#include "RideInfo.h"
#include "Node.h"

struct FERenderingCar : bNode<FERenderingCar>
{
	RideInfo mRideInfo;

	static FERenderingCar* Instance()
	{
		auto list = (FERenderingCar**)0x00B77970;
		return *list;
	}

	static CarType GetCarId()
	{
		return Instance()->mRideInfo.CarId;
	}

	static RideInfo* GetRideInfo()
	{
		return &Instance()->mRideInfo;
	}
};