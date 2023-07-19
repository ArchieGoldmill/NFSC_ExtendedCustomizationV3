#pragma once
#include "Math.h"
#include "Camera.h"

struct eViewPlatInfo
{
	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;
	D3DXMATRIX ProjectionZBiasMatrix;
	D3DXMATRIX ViewProjectionMatrix;
	D3DXMATRIX ViewProjectionZBiasMatrix;
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