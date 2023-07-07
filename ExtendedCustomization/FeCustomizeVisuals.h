#pragma once
#include "VectorScrollerMenu.h"

enum class VisualsMenu :int
{
	BODY_PAINT = 0,
	FRONT_RIM_PAINT = 1,
	VINYLS = 2,
	WINDOW_TINT = 3,
	RIDE_HEIGHT = 4,
	CALIPER_PAINT = 5,
	FRONT_LIP_PAINT = 6,
	REAR_LIP_PAINT = 7,
	REAR_RIM_PAINT = 8,
	HOOD_PAINT = 9,
	SPOILER_PAINT = 10
};

class FeCustomizeVisuals : public VectorScrollerMenu
{
public:
	inline static VisualsMenu SelectedItem;
};