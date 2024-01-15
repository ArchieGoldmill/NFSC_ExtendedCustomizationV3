#pragma once
#include "Feature.h"

struct fixup
{
	void* replacement_texture_table;
	std::uint16_t replacement_textures_count;
	std::uint16_t reference_count;
};

fixup ReplacementTextureTableFixes[700];

void InitReplacementTextureTableFixes()
{
	if (g_Config.ReplacementTextureTableFixes)
	{
		injector::WriteMemory(0x00558771, ReplacementTextureTableFixes);
		injector::WriteMemory(0x0055879E, ReplacementTextureTableFixes);
		injector::WriteMemory(0x005587D5, ReplacementTextureTableFixes);
		injector::WriteMemory(0x00558820, ReplacementTextureTableFixes);
		injector::WriteMemory(0x00558884, ReplacementTextureTableFixes);

		injector::WriteMemory(0x0055878F, std::end(ReplacementTextureTableFixes));
		injector::WriteMemory(0x005587E8, std::end(ReplacementTextureTableFixes));
	}
}