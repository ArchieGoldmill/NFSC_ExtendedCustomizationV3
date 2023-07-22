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

struct SharedConfig
{
	std::vector<PartConfig> Parts;
	State PopUpHeadLights = State::Default;
	State ForceLodA = State::Default;

	PartConfig* GetPart(Slot slot);
};

struct CarConfig : public SharedConfig
{
	std::string Name;
	Hash NameHash;
	int Version = 0;
};

struct GlobalConfig : public SharedConfig
{
	std::vector<CarConfig*> Cars;

	bool DebugRotorGlow;
	bool AllVinylsTransformable;
	bool AllVinylsMirrorable;
	bool SmoothVinylTransform;
	bool ProperDecalMirror;
	bool FixVinylTransformOrder;
	bool FixDecals;
	bool SeparateNosExhaustFX;
	bool FixAutosculptExhaustFX;
	bool ExhaustShake;
	bool RotorGlow;
	bool UnhardcodeBodykitNames;

	PartConfig GetPart(Slot slot, CarType carId);
	CarConfig* GetCarConfig(CarType carId);
	CarConfig* GetCarConfig(Hash carName);
	State GetPopUpHeadLights(CarType carId);
	State GetForceLodA(Hash carName);
	int GetVersion(CarType carId);
};

inline GlobalConfig g_Config;

void InitConfig();