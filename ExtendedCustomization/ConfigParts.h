#pragma once
#include "Slots.h"

std::pair<Slot, const char*> ConfigParts[] = { 
	{ Slot::BODY, "PART_BODY" },
	{ Slot::FRONT_BUMPER, "PART_FRONT_BUMPER" },
	{ Slot::REAR_BUMPER, "PART_REAR_BUMPER" },
	{ Slot::SKIRT, "PART_SKIRT" },
	{ Slot::HOOD, "PART_HOOD" },
	{ Slot::ROOFSCOOP, "PART_ROOF_SCOOP" },
	{ Slot::SPOILER, "PART_SPOILER" },
	{ Slot::EXHAUST, "PART_EXHAUST" },
	{ Slot::LICENSE_PLATE, "PART_LICENSE_PLATE" },
	{ Slot::FRONT_BRAKE, "PART_FRONT_BRAKE" },
	{ Slot::FRONT_ROTOR, "PART_FRONT_ROTOR" },
	{ Slot::DECAL_FRONT_WINDOW_TEX0, "PART_FRONT_DECAL" },
	{ Slot::DECAL_REAR_WINDOW_TEX0, "PART_REAR_DECAL" },
	{ Slot::LEFT_HEADLIGHT, "PART_LEFT_HEADLIGHT" },
	{ Slot::LEFT_BRAKELIGHT, "PART_LEFT_BRAKELIGHT" },
	{ Slot::FRONT_BUMPER_BADGING_SET, "PART_FRONT_BUMPER_BADGING" },
	{ Slot::REAR_BUMPER_BADGING_SET, "PART_REAR_BUMPER_BADGING" },
	{ Slot::INTERIOR, "PART_INTERIOR" },
	{ Slot::VINYL_GENERIC, "PART_VINYL_GENERIC" },
	{ Slot::CV, "PART_CV" }
};