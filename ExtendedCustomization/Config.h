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

enum class Animate
{
	None = 0,
	Headlights = 1,
	Hood = 2,
	LeftDoor = 3,
	RightDoor = 4,
	Doors = 5,
	Trunk = 6,
	Default = -1
};

struct PartConfig
{
	State AftermarketState = State::Default;
	State AutosculptState = State::Default;
	Hash Header = -1;
	Slot Slot = Slot::INVALID;
	std::string Camera;
	Animate Animation;

	bool IsEnabled()
	{
		return AftermarketState == State::Enabled || AutosculptState == State::Enabled;
	}

	State GetState(bool isAutosculpt)
	{
		return isAutosculpt ? AutosculptState : AftermarketState;
	}
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

	float NeonSize;
	float NeonInnerSize;

	bool DebugRotorGlow;
	unsigned int DebugAICar;
	bool DebugDamage;

	int PartPreviewDelay;

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
	bool BodykitNames;
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
	bool RideHeight;
	bool FixClaiperLighting;
	bool FixShadowClipping;
	bool AllowStockWheelsPaint;
	bool RearWheels;
	bool FixTireSkids;
	bool AllWheelsRotated;
	bool RemoveWheelMasking;
	bool FixWheelLighting;
	bool BrakeScale;
	bool ExhaustSmoke;
	bool WindowDamageStages;

	bool HK_Enabled;
	int HK_ToggleHood;
	int HK_ToggleLeftDoor;
	int HK_ToggleRightDoor;
	int HK_ToggleTrunk;
	int HK_ToggleHeadlights;
	int HK_ToggleLights;

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