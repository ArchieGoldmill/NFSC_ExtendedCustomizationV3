#pragma once
#include <vector>
#include <string>
#include "Hashes.h"
#include "Slots.h"
#include "Game.h"

enum class State
{
	Enabled = 1,
	Disabled = 0,
	Default = -1,
};

struct PartConfig
{
	State State = State::Default;
	Hash Header = -1;
	Slot Slot = Slot::INVALID;
	std::string Camera;
};

class SharedConfig
{
public:
	std::vector<PartConfig> Parts;
	State PopUpHeadLights = State::Default;
	State ForceLodA = State::Default;

	PartConfig* GetPart(Slot slot);
};

class CarConfig : public SharedConfig
{
public:
	std::string Name;
	Hash NameHash;
	int Version = 0;
};

class GlobalConfig : public SharedConfig
{
public:
	std::vector<CarConfig*> Cars;

	PartConfig GetPart(Slot slot, CarType carId);
	CarConfig* GetCarConfig(CarType carId);
	CarConfig* GetCarConfig(Hash carName);
	State GetPopUpHeadLights(CarType carId);
	State GetForceLodA(Hash carName);
	int GetVersion(CarType carId);
};

inline GlobalConfig g_Config;

void InitConfig();