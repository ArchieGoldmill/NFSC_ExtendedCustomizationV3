#include <filesystem>
#include "../ThirdParty/IniReader/IniReader.h"
#include "Config.h"
#include "ConfigParts.h"
#include "Game.h"

namespace fs = std::filesystem;

State InitState(CIniReader& iniReader, const char* section, const char* key)
{
	int value = iniReader.ReadInteger(section, key, -1);
	if (value < -1 || value > 1)
	{
		return State::Default;
	}

	return (State)value;
}

void InitPart(CIniReader& iniReader, SharedConfig* shared, Slot slot, const char* iniSection)
{
	PartConfig partConfig;

	partConfig.Slot = slot;
	partConfig.State = InitState(iniReader, iniSection, "Enabled");
	partConfig.Camera = iniReader.ReadString(iniSection, "Camera", "");
	partConfig.Animation = (Animate)iniReader.ReadInteger(iniSection, "Animate", -1);

	auto header = iniReader.ReadString(iniSection, "Header", "");
	if (header.size())
	{
		partConfig.Header = StringHash(header.c_str());
	}

	shared->Parts.push_back(partConfig);
}

void InitParts(CIniReader& iniReader, SharedConfig* shared)
{
	for (auto part : ConfigParts)
	{
		InitPart(iniReader, shared, part.first, part.second);
	}

	for (int i = 0; i < 16; i++)
	{
		char buffer[32];
		sprintf(buffer, "PART_ATTACHMENT%02d", i);
		InitPart(iniReader, shared, (Slot)((int)Slot::ATTACHMENT0 + i), buffer);
	}
}

void InitShared(CIniReader& iniReader, SharedConfig* shared)
{
	InitParts(iniReader, shared);

	shared->PopUpHeadLights = InitState(iniReader, "GENERAL", "PopUpHeadLights");
	shared->ForceLodA = InitState(iniReader, "GENERAL", "ForceLodA");
}

void InitCars()
{
	std::string path = "ExtendedCustomizationCars";
	fs::directory_iterator iterator;
	try
	{
		iterator = fs::directory_iterator(path);
	}
	catch (...)
	{
		return;
	}

	for (const auto& entry : iterator)
	{
		auto path = entry.path();
		CIniReader iniReader(path.string().c_str());

		auto carConfig = new CarConfig();
		carConfig->Name = path.filename().string();
		carConfig->NameHash = StringHash(carConfig->Name.c_str());
		carConfig->Version = iniReader.ReadInteger("GENERAL", "Version", 2);
		InitShared(iniReader, carConfig);

		g_Config.Cars.push_back(carConfig);
	}
}

CarConfig* GetCarByName(const char* name)
{
	for (auto car : g_Config.Cars)
	{
		if (car->Name == name)
		{
			return car;
		}
	}

	return NULL;
}

void InitPopupCar(const char* name)
{
	if (!GetCarByName(name))
	{
		auto carConfig = new CarConfig();
		carConfig->Name = name;
		carConfig->NameHash = StringHash(name);
		carConfig->PopUpHeadLights = State::Enabled;

		g_Config.Cars.push_back(carConfig);
	}
}

void InitDefaultCars()
{
	InitPopupCar("RX7");
	InitPopupCar("CAMARO");
	InitPopupCar("240SX");
	InitPopupCar("MR2");
	InitPopupCar("COROLLA");
	InitPopupCar("CHARGER69");
}

void InitConfig()
{
	CIniReader iniReader("ExtendedCustomization.ini");

	InitShared(iniReader, &g_Config);

	g_Config.FrontSteerAngle = iniReader.ReadFloat("GENERAL", "FrontSteerAngle", 0);
	g_Config.PartPreviewDelay = iniReader.ReadInteger("GENERAL", "PartPreviewDelay", 2400);
	g_Config.NeonSize = iniReader.ReadFloat("GENERAL", "NeonSize", 0);
	g_Config.NeonInnerSize = iniReader.ReadFloat("GENERAL", "NeonInnerSize", 0);

	g_Config.DebugRotorGlow = iniReader.ReadInteger("DEBUG", "RotorGlow", 0) == 1;
	g_Config.DebugAICar = StringHash32(iniReader.ReadString("DEBUG", "AICar", "").c_str());

	g_Config.AllVinylsTransformable = iniReader.ReadInteger("MODS", "AllVinylsTransformable", 0) == 1;
	g_Config.AllVinylsMirrorable = iniReader.ReadInteger("MODS", "AllVinylsMirrorable", 0) == 1;
	g_Config.SmoothVinylTransform = iniReader.ReadInteger("MODS", "SmoothVinylTransform", 0) == 1;
	g_Config.ProperDecalMirror = iniReader.ReadInteger("MODS", "ProperDecalMirror", 0) == 1;
	g_Config.FixVinylTransformOrder = iniReader.ReadInteger("MODS", "FixVinylTransformOrder", 0) == 1;
	g_Config.FixDecals = iniReader.ReadInteger("MODS", "FixDecals", 0) == 1;
	g_Config.SeparateNosExhaustFX = iniReader.ReadInteger("MODS", "SeparateNosExhaustFX", 0) == 1;
	g_Config.FixAutosculptExhaustFX = iniReader.ReadInteger("MODS", "FixAutosculptExhaustFX", 0) == 1;
	g_Config.ExhaustShake = iniReader.ReadInteger("MODS", "ExhaustShake", 0) == 1;
	g_Config.RotorGlow = iniReader.ReadInteger("MODS", "RotorGlow", 0) == 1;
	g_Config.BodykitNames = iniReader.ReadInteger("MODS", "BodykitNames", 0) == 1;
	g_Config.Camber = iniReader.ReadInteger("MODS", "Camber", 0) == 1;
	g_Config.TrackWidth = iniReader.ReadInteger("MODS", "TrackWidth", 0) == 1;
	g_Config.BrakelightGlow = iniReader.ReadInteger("MODS", "BrakelightGlow", 0) == 1;
	g_Config.Neon = iniReader.ReadInteger("MODS", "Neon", 0) == 1;
	g_Config.CustomPaints = iniReader.ReadInteger("MODS", "CustomPaints", 0) == 1;
	g_Config.PartAnimations = iniReader.ReadInteger("MODS", "PartAnimations", 0) == 1;
	g_Config.FixAutosculptLods = iniReader.ReadInteger("MODS", "FixAutosculptLods", 0) == 1;
	g_Config.IgnoreSpoilerBoundingBox = iniReader.ReadInteger("MODS", "IgnoreSpoilerBoundingBox", 0) == 1;
	g_Config.LicensePlateText = iniReader.ReadInteger("MODS", "LicensePlateText", 0) == 1;
	g_Config.TireWidth = iniReader.ReadInteger("MODS", "TireWidth", 0) == 1;
	g_Config.FixClaiperLighting = iniReader.ReadInteger("MODS", "FixClaiperLighting", 0) == 1;
	g_Config.FixShadowClipping = iniReader.ReadInteger("MODS", "FixShadowClipping", 0) == 1;
	g_Config.AllowStockWheelsPaint = iniReader.ReadInteger("MODS", "AllowStockWheelsPaint", 0) == 1;
	g_Config.RideHeight = iniReader.ReadInteger("MODS", "RideHeight", 0) == 1;
	g_Config.RearWheels = iniReader.ReadInteger("MODS", "RearWheels", 0) == 1;
	g_Config.AllWheelsRotated = iniReader.ReadInteger("MODS", "AllWheelsRotated", 0) == 1;
	g_Config.RemoveWheelMasking = iniReader.ReadInteger("MODS", "RemoveWheelMasking", 0) == 1;
	g_Config.FixTireSkids = iniReader.ReadInteger("MODS", "FixTireSkids", 0) == 1;
	g_Config.FixWheelLighting = iniReader.ReadInteger("MODS", "FixWheelLighting", 0) == 1;
	g_Config.BrakeScale = iniReader.ReadInteger("MODS", "BrakeScale", 0) == 1;
	g_Config.ExhaustSmoke = iniReader.ReadInteger("MODS", "ExhaustSmoke", 0) == 1;

	g_Config.HK_Enabled = iniReader.ReadInteger("HOT_KEYS", "Enabled", 0) == 1;
	g_Config.HK_ToggleHeadlights = iniReader.ReadInteger("HOT_KEYS", "ToggleHeadlights", 0);
	g_Config.HK_ToggleHood = iniReader.ReadInteger("HOT_KEYS", "ToggleHood", 0);
	g_Config.HK_ToggleLeftDoor = iniReader.ReadInteger("HOT_KEYS", "ToggleLeftDoor", 0);
	g_Config.HK_ToggleRightDoor = iniReader.ReadInteger("HOT_KEYS", "ToggleRightDoor", 0);
	g_Config.HK_ToggleTrunk = iniReader.ReadInteger("HOT_KEYS", "ToggleTrunk", 0);

	InitCars();

	InitDefaultCars();
}