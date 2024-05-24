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

CarConfig* GlobalConfig::GetCarConfig(Hash carName)
{
	for (auto carConfig : this->Cars)
	{
		if (carConfig->NameHash == carName)
		{
			return carConfig;
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

State GlobalConfig::GetForceLodA(Hash carId)
{
	auto carConfig = this->GetCarConfig(carId);
	if (carConfig)
	{
		if (carConfig->ForceLodA != State::Default)
		{
			return carConfig->ForceLodA;
		}
	}

	return g_Config.ForceLodA;
}

State GlobalConfig::GetExtraDamage(CarType carId)
{
	auto carConfig = this->GetCarConfig(carId);
	if (carConfig)
	{
		if (carConfig->ExtraDamage != State::Default)
		{
			return carConfig->ExtraDamage;
		}
	}

	return g_Config.ExtraDamage;
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
			if (part->AftermarketState != State::Default)
			{
				result.AftermarketState = part->AftermarketState;
			}

			if (part->AutosculptState != State::Default)
			{
				result.AutosculptState = part->AutosculptState;
			}

			if (part->Carbon != State::Default)
			{
				result.Carbon = part->Carbon;
			}

			if (part->Camera.size())
			{
				result.Camera = part->Camera;
			}

			if (part->Header != -1)
			{
				result.Header = part->Header;
			}

			if (part->Animation != Animate::Default)
			{
				result.Animation = part->Animation;
			}
		}
	}

	return result;
}

Slot GlobalConfig::GetSlotByHeader(Hash header)
{
	for (auto& partConfig : this->Parts)
	{
		if (partConfig.Header == header)
		{
			return partConfig.Slot;
		}
	}

	return Slot::INVALID;
}