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

	float FrontSteerAngle;

	bool DebugRotorGlow;
	bool AllVinylsTransformable;
	bool AllVinylsMirrorable;
	bool SmoothVinylTransform;
	bool ProperDecalMirror;
	bool FixVinylTransformOrder;
	bool FixDecals;
	bool FixDecalsZOrder;
	bool SeparateNosExhaustFX;
	bool FixAutosculptExhaustFX;
	bool ExhaustShake;
	bool RotorGlow;
	bool UnhardcodeBodykitNames;
	bool Camber;
	bool TrackWidth;
	bool Neon;
	bool BrakelightGlow;
	bool CustomPaints;
	bool PartAnimations;
	bool FixAutosculptLods;
	bool IgnoreSpoilerBoundingBox;
	bool LicensePlateText;
	bool TireWidth;
	bool FixClaiperLighting;

	PartConfig GetPart(Slot slot, CarType carId);
	Slot GetSlotByHeader(Hash header);
	CarConfig* GetCarConfig(CarType carId);
	CarConfig* GetCarConfig(Hash carName);
	State GetPopUpHeadLights(CarType carId);
	State GetForceLodA(Hash carName);
	int GetVersion(CarType carId);
};

inline GlobalConfig g_Config;

void InitConfig();