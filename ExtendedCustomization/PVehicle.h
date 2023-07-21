#pragma once

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

	bool IsGlareOn(int num)
	{
		FUNC(0x006D8080, bool, __thiscall, _IsGlareOn, PVehicle*, int);
		return _IsGlareOn(this, num);
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
};