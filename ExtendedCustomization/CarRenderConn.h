#pragma once
#include "PVehicle.h"

class CarRenderInfo;
class CarRenderConn
{
public:
	int** data[0x10];
	CarRenderInfo* carRenderInfo;

	static int GetListCount()
	{
		static int* ListCount = (int*)0x00B77DBC;
		return *ListCount;
	}

	static CarRenderConn** GetList()
	{
		static CarRenderConn*** List = (CarRenderConn***)0x00B77DB4;
		return *List;
	}

	static CarRenderConn* Get(CarRenderInfo* carRenderInfo)
	{
		int count = CarRenderConn::GetListCount();
		auto list = CarRenderConn::GetList();
		for (int i = 0; i < count; i++)
		{
			if (list[i]->carRenderInfo == carRenderInfo)
			{
				return list[i];
			}
		}

		return nullptr;
	}

	PVehicle* GetPVehicle()
	{
		return (PVehicle*)(this->data[2][0x21]);
	}
};