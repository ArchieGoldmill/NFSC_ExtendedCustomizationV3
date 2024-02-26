#pragma once
#include "CarDamage.h"

enum VehicleFX
{
	VehicleFX_NONE = 0,
	VehicleFX_LHEAD = 1,
	VehicleFX_RHEAD = 2,
	VehicleFX_CHEAD = 4,
	VehicleFX_HEADLIGHTS = 7,
	VehicleFX_LBRAKE = 8,
	VehicleFX_RBRAKE = 0x10,
	VehicleFX_CBRAKE = 0x20,
	VehicleFX_BRAKELIGHTS = 0x38,
	VehicleFX_LIGHTS = 0x3F,
	VehicleFX_LREVERSE = 0x40,
	VehicleFX_RREVERSE = 0x80,
	VehicleFX_REVERSE = 0xC0,
	VehicleFX_LRSIGNAL = 0x100,
	VehicleFX_RRSIGNAL = 0x200,
	VehicleFX_LFSIGNAL = 0x400,
	VehicleFX_LSIGNAL = 0x5000,
	VehicleFX_RFSIGNAL = 0x800,
	VehicleFX_RSIGNAL = 0xA00,
	VehicleFX_COPRED = 0x1000,
	VehicleFX_COPBLUE = 0x2000,
	VehicleFX_COPWHITE = 0x4000,
	VehicleFX_COPS = 0x7000
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

	float GetSpeedKmh()
	{
		return this->GetSpeed() * 3.5999999f;
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