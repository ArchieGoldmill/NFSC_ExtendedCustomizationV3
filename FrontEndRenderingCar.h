#pragma once
#include "RideInfo.h"
#include "Node.h"

class FrontEndRenderingCar : Node<FrontEndRenderingCar>
{
public:
	RideInfo RideInfo;

	static FrontEndRenderingCar* Get()
	{
		auto list = (FrontEndRenderingCar**)0x00B77970;
		return *list;
	}
};