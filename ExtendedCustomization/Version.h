#pragma once
#include "Feature.h"

void InitVersion()
{
	auto text = (char*)"1.4\n\nExtended Customization 3.3.4\nby ARCHIE";
	auto textPtr = (char**)0x00857811;
	*textPtr = text;
	injector::MakeNOP(0x00591072, 7, true);
}