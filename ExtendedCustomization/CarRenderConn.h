#pragma once
#include "Func.h"
#include "PVehicle.h"
#include "Effect.h"

class CarRenderInfo;
class CarRenderConn
{
public:
	int** data[0x10];
	CarRenderInfo* pCarRenderInfo;
	BYTE unk[0x140];
	float LeftWheelRotation;
	float RightWheelRotation;
	bTList<Effect> ExhaustEffects;

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
			if (list[i]->pCarRenderInfo == carRenderInfo)
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

	void UpdateEffects(int a1, int a2)
	{
		FUNC(0x007D5DF0, void, __thiscall, _UpdateEffects, CarRenderConn*, int, int);
		_UpdateEffects(this, a1, a2);
	}
};