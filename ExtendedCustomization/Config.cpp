#include "Config.h"

PartConfig* SharedConfig::GetPart(Slot slot)
{
	for (auto& partConfig : this->Parts)
	{
		if (partConfig.Slot == slot)
		{
			return &partConfig;
		}
	}

	return NULL;
}

CarConfig* GlobalConfig::GetCarConfig(CarType carId)
{
	auto carName = GetCarTypeName(carId);
	if (carName)
	{
		auto carHash = StringHash(carName);
		for (auto carConfig : this->Cars)
		{
			if (carConfig->NameHash == carHash)
			{
				return carConfig;
			}
		}
	}

	return NULL;
}

State GlobalConfig::GetPopUpHeadLights(CarType carId)
{
	auto carConfig = this->GetCarConfig(carId);
	if (carConfig)
	{
		if (carConfig->PopUpHeadLights != State::Default)
		{
			return carConfig->PopUpHeadLights;
		}
	}

	return g_Config.PopUpHeadLights;
}

int GlobalConfig::GetVersion(CarType carId)
{
	auto carConfig = this->GetCarConfig(carId);
	if (carConfig)
	{
		return carConfig->Version;
	}

	return 0;
}

PartConfig GlobalConfig::GetPart(Slot slot, CarType carId)
{
	PartConfig result;
	auto part = this->SharedConfig::GetPart(slot);
	if (part)
	{
		result = *part;
	}

	auto carConfig = this->GetCarConfig(carId);
	if (carConfig)
	{
		auto part = carConfig->GetPart(slot);
		if (part)
		{
			if (part->State != State::Default)
			{
				result.State = part->State;
			}

			if (part->Camera.size())
			{
				result.Camera = part->Camera;
			}

			if (part->Header != -1)
			{
				result.Header = part->Header;
			}
		}
	}

	return result;
}