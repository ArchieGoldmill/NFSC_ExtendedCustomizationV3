#pragma once

void InitVersion()
{
	auto text = (char*)"1.4\n\nExtended Customization 3.0.0\nby ARCHIE";
	char** textPtr = (char**)(0x00857811);
	*textPtr = text;
	injector::MakeNOP(0x00591072, 7, true);
}