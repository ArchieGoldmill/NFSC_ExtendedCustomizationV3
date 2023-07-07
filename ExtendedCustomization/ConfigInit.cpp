#include <filesystem>
#include "../ThirdParty/IniReader/IniReader.h"
#include "Config.h"
#include "ConfigParts.h"
#include "Game.h"

namespace fs = std::filesystem;

State InitState(int value)
{
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
	partConfig.State = InitState(iniReader.ReadInteger(iniSection, "Enabled", -1));
	partConfig.Camera = iniReader.ReadString(iniSection, "Camera", "");

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
}

void InitShared(CIniReader& iniReader, SharedConfig* shared)
{
	InitParts(iniReader, shared);
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
		InitShared(iniReader, carConfig);

		g_Config.Cars.push_back(carConfig);
	}
}

void InitConfig()
{
	CIniReader iniReader("ExtendedCustomization.ini");

	InitShared(iniReader, &g_Config);

	InitCars();
}