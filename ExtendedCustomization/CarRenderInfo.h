#pragma once

#include "TextureInfo.h"
#include "RideInfo.h"
#include "PositionMarker.h"
#include "eLightMaterial.h"
#include "eModel.h"
#include "CarRenderConn.h"

struct PositionMarkers
{
	PositionMarker* Spoiler;
	PositionMarker* Spoiler2;
	PositionMarker* RoofScoop;
	PositionMarker* DriverPosition;
	PositionMarker* HandPosition;
	PositionMarker* CenterExhaust;
	PositionMarker* RightExhaust;
	PositionMarker* LeftExhaust;
	PositionMarker* FrontLicensePlate;
	PositionMarker* RearLicensePlate;
};

struct Materials
{
	eLightMaterial* Body;
	eLightMaterial* Hood;
	eLightMaterial* WindowTint;
	eLightMaterial* FrontRims;
	eLightMaterial* Calipers;
	eLightMaterial* Spoiler;
	eLightMaterial* RoofScoop;
	eLightMaterial* Unknown;
};

struct WheelData
{
	float YRenderOffset[4];
	float Widths[2];
	float Radius[2];
	float WidthScales[4];
	float RadiusScales[4];
	float BrakeMarkerY[2];
};

class CarRenderInfoExtras;
class CarRenderInfo
{
public:
	char unk_1[0x370];
	D3DXVECTOR3 BoundingBoxMin;
	CarRenderInfoExtras* Extras;
	D3DXVECTOR4 BoundingBoxMax;
	char unk_2[0x5C];
	int IsHoodCarbon;
	RideInfo* RideInfo;
	int* CarTypeInfo;
	TextureInfo* CarShadowTexture;
	TextureInfo* CarShadowRampTexture;
	Materials Materials;
	PositionMarkers Markers;
	char unk_3[0x18];
	WheelData WheelData;
	char unk_4[0x818];
	eModel* PartModel[0x59][5];

	void UpdateLightStateTextures()
	{
		FUNC(0x007ADC90, void, __thiscall, _UpdateLightStateTextures, CarRenderInfo*);
		_UpdateLightStateTextures(this);
	}

	template <typename T>
	T* Get(int offset)
	{
		auto ptr = (BYTE*)this;
		return (T*)(ptr + offset);
	}

	D3DXMATRIX* GetMatrix()
	{
		return *this->Get<D3DXMATRIX*>(0x2CB4);
	}

	bool IsLeftBrakelightOn()
	{
		return this->IsBrakelightOn(8);
	}

	bool IsRightBrakelightOn()
	{
		return this->IsBrakelightOn(0x10);
	}

	bool IsCentreBrakelightOn()
	{
		return this->IsBrakelightOn(0x20);
	}

	bool IsReverseOn()
	{
		return this->IsBrakelightOn(0xC0);
	}

	bool IsGlareOn()
	{
		auto carRenderConn = CarRenderConn::Get(this);
		if (carRenderConn)
		{
			auto pVehicle = carRenderConn->GetPVehicle();
			if (pVehicle)
			{
				return pVehicle->IsGlareOn(7);
			}
		}

		return false;
	}

	bool IsPlayer()
	{
		return this->RideInfo->RenderUsage == 0;
	}

	double DrawAmbientShadow(int a1, float* a2, float a3, int a4, int a5, int a6)
	{
		FUNC(0x007BE4C0, double, __thiscall, _DrawAmbientShadow, CarRenderInfo*, int, float*, float, int, int, int);
		return _DrawAmbientShadow(this, a1, a2, a3, a4, a5, a6);
	}

	void UpdateWheelYRenderOffset()
	{
		FUNC(0x007CB6F0, void, __thiscall, _UpdateWheelYRenderOffset, CarRenderInfo*);
		_UpdateWheelYRenderOffset(this);
	}

	bool HasExhaustMerker()
	{
		return this->Markers.LeftExhaust || this->Markers.RightExhaust || this->Markers.CenterExhaust;
	}

private:
	bool IsBrakelightOn(int b)
	{
		auto a1 = this->Get<BYTE>(0x30C4);
		auto a2 = this->Get<BYTE>(0x30C0);

		return (*a1 & b) == 0 && (*a2 & b) != 0;
	}
};