#pragma once
#include "D3DWrapper.h"
#include "Camera.h"

struct eViewPlatInfo
{
	bMatrix4 ViewMatrix;
	bMatrix4 ProjectionMatrix;
	bMatrix4 ProjectionZBiasMatrix;
	bMatrix4 ViewProjectionMatrix;
	bMatrix4 ViewProjectionZBiasMatrix;
};

class eViewPlatInterface
{
public:
	eViewPlatInfo* PlatInfo;
};

class __declspec(align(4)) eView : public eViewPlatInterface
{
public:
	char* Name;
	int Id;
	bool Active;
	char LetterBox;
	float H;
	float NearZ;
	float FarZ;
	float FovBias;
	float FovDegrees;
	float FovAngle;
	bool BlackAndWhiteMode;
	int PixelMinSize;
	D3DXVECTOR4 ViewDirection;
	Camera* pCamera;

	static eView* GetPlayer1()
	{
		return (eView*)0x00B4AF90;
	}
};