#pragma once
#include "Math.h"
#include "Camera.h"
#include "Game.h"

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

	void TransformByZBias(D3DXVECTOR3* out, D3DXVECTOR3* v)
	{
		D3DXVECTOR4 t;
		D3DXVec3Transform(&t, v, &this->PlatInfo->ProjectionZBiasMatrix);

		float v4 = 1.0 / t.w;
		float v6 = v4 * t.x;

		out->x = (v6 + 1.0) * (float)*Game::ScreenSizeX * 0.5;
		out->y = (t.y * v4 - 1.0) * (float)*Game::ScreenSizeY * (-0.5);
		out->z = v4 * t.z;
	}
};