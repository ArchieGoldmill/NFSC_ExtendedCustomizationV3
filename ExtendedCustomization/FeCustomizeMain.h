#pragma once
#include "VectorScrollerMenu.h"

enum class CustomizeMainMenu :int
{
	AFTERMARKET = 0,
	AUTOSCULPT = 1,
	PERFORMANCE = 2,
	VISUALS = 3,
	CART = 4,
	FRONT_WHEELS = 5,
	REAR_WHEELS = 6,
	PAINTS = 7,
	SPECIALTIES = 8,
	ATTACHMENTS = 9
};

class FeCustomizeMain : public VectorScrollerMenu
{
public:
	inline static CustomizeMainMenu SelectedItem;
};