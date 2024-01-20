#pragma once
#include <D3dx9math.h>
#include "Slots.h"

const Slot Slot_Trunk = Slot::BRAKELIGHT;
const Slot Slot_FrontFender = Slot::FRONT_BUMPER_BADGING_SET;
const Slot Slot_RearFender = Slot::REAR_BUMPER_BADGING_SET;
const Slot Slot_Stance = Slot::MISC;
const Slot Slot_Neon = Slot::CV;
const Slot Slot_LeftSeat = Slot::ATTACHMENT14;
const Slot Slot_RightSeat = Slot::ATTACHMENT15;
const Slot Slot_Tires = Slot::CUSTOM_HUD;

const int ZoneFrontBumper = 0;
const int ZoneRearBumper = 1;
const int ZoneSkirt = 2;
const int ZoneFrontWheel = 3;
const int ZoneHood = 4;
const int ZoneSpoiler = 5;
const int ZoneRoofScoop = 6;
const int ZoneRoof = 7;
const int ZoneExhaust = 8;
const int ZoneRearWheel = 9;
const int ZoneColor1 = 10;
const int ZoneColor2 = 11;
const int ZoneColor3 = 12;
const int ZoneStance = 13;
const int ZoneNeon = 14;
const int ZoneLicenseplate = 15;

enum class ReplacementTextureIndex : int
{
	CarSkin,
	SpolierSkin1,

	Interior,
	InteriorGlow,

	WindowFront,
	WindowFrontLeft,
	WindowFrontRight,
	WindowRear,
	RearDefroster,
	WindowRearLeft,
	WindowRearRight,

	HeadlightLeft,
	HeadlightRight,
	HeadlightGlassLeft,
	HeadlightGlassRight,
	BrakelightLeft,
	BrakelightRight,
	BrakelightCenter,
	BrakelightGlassLeft,
	BrakelightGlassRight,
	BrakelightGlassCenter,

	Reverse,

	FrontDecal,
	RearDecal,

	LicensePlate,
	LicensePlateFrame,
	LicensePlate0,
	LicensePlate1,
	LicensePlate2,
	LicensePlate3,
	LicensePlate4,
	LicensePlate5,
	LicensePlate6,
	LicensePlate7,
	LicensePlate8
};

inline Slot AttachSlots[] = {
	Slot::ATTACHMENT0, Slot::ATTACHMENT1, Slot::ATTACHMENT2, Slot::ATTACHMENT3, Slot::ATTACHMENT4, Slot::ATTACHMENT5, Slot::ATTACHMENT6, Slot::ATTACHMENT7,
	Slot::ATTACHMENT8, Slot::ATTACHMENT9, Slot::ATTACHMENT10, Slot::ATTACHMENT11, Slot::ATTACHMENT12, Slot::ATTACHMENT13, Slot::ATTACHMENT14,Slot::ATTACHMENT15,
	Slot::STEERINGWHEEL
};

inline Slot KitwSlots[] = {
	Slot::HOOD, Slot::FRONT_BUMPER, Slot::REAR_BUMPER, Slot::SKIRT, Slot_FrontFender, Slot_RearFender, Slot_Trunk, Slot::HEADLIGHT, Slot::LEFT_HEADLIGHT, Slot::LEFT_BRAKELIGHT,
	Slot::LEFT_SIDE_MIRROR, Slot::DOOR_LEFT, Slot::EXHAUST, Slot::SPOILER, Slot::FRONT_WINDOW, Slot::REAR_WINDOW, Slot::REAR_LEFT_WINDOW, Slot::REAR_RIGHT_WINDOW,
	Slot::INTERIOR, Slot::ROOF, Slot::FRONT_LEFT_WINDOW, Slot::FRONT_RIGHT_WINDOW
};

inline Slot MarkerSlots[] = { Slot::FRONT_BUMPER, Slot::REAR_BUMPER, Slot::INTERIOR, Slot::SKIRT, Slot::BODY, Slot::ROOF, Slot_Trunk, Slot::HOOD };

inline Slot NeonSlots[] = { Slot::FRONT_BUMPER, Slot::REAR_BUMPER, Slot::SKIRT, Slot::INTERIOR, Slot::ATTACHMENT0, Slot::ATTACHMENT1, Slot::ATTACHMENT2,
	Slot::ATTACHMENT3, Slot::ATTACHMENT4, Slot::ATTACHMENT5, Slot::ATTACHMENT6, Slot::ATTACHMENT7, Slot::ATTACHMENT8, Slot::ATTACHMENT9, Slot::ATTACHMENT10,
	Slot::ATTACHMENT11, Slot::ATTACHMENT12, Slot::ATTACHMENT13, Slot::ATTACHMENT14,Slot::ATTACHMENT15 };

inline Slot CustomTextureSlots[] = { Slot::FRONT_BUMPER, Slot::REAR_BUMPER, Slot::SKIRT, Slot::BODY, Slot_FrontFender, Slot_RearFender, Slot_Trunk, Slot::SPOILER,
	Slot::ATTACHMENT0, Slot::ATTACHMENT1, Slot::ATTACHMENT2, Slot::ATTACHMENT3, Slot::ATTACHMENT4, Slot::ATTACHMENT5, Slot::ATTACHMENT6, Slot::ATTACHMENT7, 
	Slot::ATTACHMENT8, Slot::ATTACHMENT9, Slot::ATTACHMENT10, Slot::ATTACHMENT11, Slot::ATTACHMENT12, Slot::ATTACHMENT13, Slot::ATTACHMENT14,Slot::ATTACHMENT15 };

inline D3DXMATRIX CenterMarker;