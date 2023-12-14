#pragma once
#include "CarDamage.h"

enum class VehicleFX
{
	LIGHT_NONE = 0,
	LIGHT_LHEAD = 1,
	LIGHT_RHEAD = 2,
	LIGHT_CHEAD = 4,
	LIGHT_HEADLIGHTS = 7,
	LIGHT_LBRAKE = 8,
	LIGHT_RBRAKE = 0x10,
	LIGHT_CBRAKE = 0x20,
	LIGHT_BRAKELIGHTS = 0x38,
	LIGHT_LREVERSE = 0x40,
	LIGHT_RREVERSE = 0x80,
	LIGHT_REVERSE = 0xC0,
	LIGHT_LRSIGNAL = 0x100,
	LIGHT_RRSIGNAL = 0x200,
	LIGHT_LFSIGNAL = 0x400,
	LIGHT_LSIGNAL = 0x5000,
	LIGHT_RFSIGNAL = 0x800,
	LIGHT_RSIGNAL = 0xA00,
	LIGHT_COPRED = 0x1000,
	LIGHT_COPBLUE = 0x2000,
	LIGHT_COPWHITE = 0x4000,
	LIGHT_COPS = 0x7000
};

class AIVehicle
{
public:
	float GetAccelerating()
	{
		auto f_this = (float*)this;
		return f_this[0x32];
	}

	float GetBraking()
	{
		auto f_this = (float*)this;
		return f_this[0x33];
	}
};

class PVehicle
{
public:
	AIVehicle* GetAIVehicle()
	{
		static auto _GetAIVehicle = (AIVehicle * (__thiscall*)(PVehicle*))0x006D8110;
		return _GetAIVehicle(this);
	}

	bool IsGlareOn(VehicleFX fx)
	{
		FUNC(0x006D8080, bool, __thiscall, _IsGlareOn, PVehicle*, VehicleFX);
		return _IsGlareOn(this, fx);
	}

	void GlareOn(VehicleFX fx)
	{
		FUNC(0x006C0BA0, void, __thiscall, _GlareOn, PVehicle*, VehicleFX);
		_GlareOn(this, fx);
	}

	void GlareOff(VehicleFX fx)
	{
		FUNC(0x006C0BC0, void, __thiscall, _GlareOff, PVehicle*, VehicleFX);
		_GlareOff(this, fx);
	}

	float GetSpeed()
	{
		static auto _GetSpeed = (float(__thiscall*)(PVehicle*))0x006D8070;
		return _GetSpeed(this);
	}

	float GetBraking()
	{
		auto aiVehicle = this->GetAIVehicle();
		if (aiVehicle)
		{
			return aiVehicle->GetBraking();
		}

		return 0.0f;
	}

	float GetAccelerating()
	{
		auto aiVehicle = this->GetAIVehicle();
		if (aiVehicle)
		{
			return aiVehicle->GetAccelerating();
		}

		return 0.0f;
	}

	CarDamage GetDamage()
	{
		CarDamage result;
		result.Damage = DamageZone_None;

		int** ithis = (int**)this;
		auto simObject = ithis[0x11];
		if (simObject)
		{
			result.Damage = (DamageZone)simObject[0x11];
		}

		return result;
	}
};