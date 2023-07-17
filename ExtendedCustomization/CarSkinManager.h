#pragma once

enum eCarSkinTask
{
	eCST_Finished = 0,
	eCST_Started = 1,
	eCST_CanvasBasePaintFill = 2,
	eCST_LowerLayersBasePaintFill = 3,
	eCST_LowerLayersInitialize = 4,
	eCST_CanvasInitialize = 5,
	eCST_CanvasInitializeCompressed = 6,
	eCST_CompressLowerLayers = 7,
	eCST_StepRasterizer = 8,
	eCST_Finalize = 9,
	eCST_CopyLowerLayersToCanvas = 0xA,
	eCST_CopyLowerLayersToCanvasCompressed = 0xB,
	eCST_CopyToDestination = 0xC,
	eCST_CopyToDestinationNoCompression = 0xD
};

struct CarSkinManager
{
	char unk_0[0x20];
	bool RasterizeLowerLayers;
	char unk_1[0x3];
	int NumLowerLayers;
	int NumLayers;
	char unk_2[0x8];
	eCarSkinTask CurrentTask;
	int CurrentLayer;
	char unk_3[0x4];

	static CarSkinManager* Get()
	{
		return (CarSkinManager*)0x00B76860;
	}
};