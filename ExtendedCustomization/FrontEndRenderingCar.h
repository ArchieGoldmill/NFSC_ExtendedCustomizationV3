#pragma once
#include "RideInfo.h"
#include "Node.h"

class FrontEndRenderingCar : bNode<FrontEndRenderingCar>
{
public:
	RideInfo RideInfo;

	static FrontEndRenderingCar* Get()
	{
		auto list = (FrontEndRenderingCar**)0x00B77970;
		return *list;
	}

	static CarType GetCarId()
	{
		return Get()->RideInfo.CarId;
	}
};