#pragma once

#include "TextureInfo.h"
#include "RideInfo.h"
#include "PositionMarker.h"
#include "eLightMaterial.h"
#include "eModel.h"
#include "CarRenderConn.h"
#include "eView.h"
#include "DBCarPart.h"
#include "eLightFlare.h"
#include "eDynamicLightPack.h"
#include "CarDamage.h"

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

struct CarMaterials
{
	eLightMaterial* Body;
	eLightMaterial* Hood;
	eLightMaterial* WindowTint;
	eLightMaterial* FrontWheel;
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

struct CarEmitterPosition : bSNode<CarEmitterPosition>
{
	D3DVECTOR Position;
	PositionMarker* Marker;
};

struct AutoSculptRender
{
	eModel Models[0x15][12];
	int HighlightRegion;
	int HighlightZone;
	bool HighlightEnabled;
	bool HighlightRenderAllEnabled;
	bool Initialized;
	CarRenderInfo* RenderInfo;
	TextureInfo* HighLightTexture;
	eView* SelectionView;
};

struct CarPartInfo
{
	D3DXVECTOR4 Position;
	int NumCulledNotVisible;
	int NumCulledVisible;
	bool IsVisible;
	char pad[7];
};

struct CarPartCuller
{
	CarPartInfo CarPartInfoTable[11];
};

struct UsedCarTextureInfo
{
	Hash TexturesToLoadPerm[104];
	Hash TexturesToLoadTemp[104];
	int NumTexturesToLoadPerm;
	int NumTexturesToLoadTemp;
	Hash SKIN1;
	Hash SKIN1B;
	Hash GLOBAL_SKIN1;
	Hash WHEEL;
	Hash SPINNER_TEXTURE;
	Hash BADGING;
	Hash SPOILER_SKIN1;
	Hash ROOF_SKIN;
	Hash _unk1;
	Hash HEADLIGHT_LEFT;
	Hash HEADLIGHT_RIGHT;
	Hash BRAKELIGHT_LEFT;
	Hash BRAKELIGHT_RIGHT;
	Hash BRAKELIGHT_CENTRE;
	Hash HEADLIGHT_GLASS_LEFT;
	Hash HEADLIGHT_GLASS_RIGHT;
	Hash BRAKELIGHT_GLASS_LEFT;
	Hash BRAKELIGHT_GLASS_RIGHT;
	Hash BRAKELIGHT_GLASS_CENTRE;
	Hash _unk2;
	Hash TIRE;
	Hash RIM;
	Hash RIM_BLUR;
	Hash LICENSE_PLATE;
	Hash PrecompileSkin;
	Hash _unk3;
	Hash PrecompileSkin4;
	Hash CompositeSkinHash;
	Hash CompositeWheelHash;
	Hash PrecompileSkin2;
	Hash PrecompileSkin3;
	Hash _unk4;
	Hash HEADLIGHT_OFF;
	Hash HEADLIGHT_ON;
	Hash HEADLIGHT_DAMAGE0;
	Hash HEADLIGHT_GLASS_OFF;
	Hash HEADLIGHT_GLASS_ON;
	Hash HEADLIGHT_GLASS_DAMAGE0;
	Hash BRAKELIGHT_OFF;
	Hash BRAKELIGHT_ON;
	Hash BRAKELIGHT_DAMAGE0;
	Hash BRAKELIGHT_GLASS_OFF;
	Hash BRAKELIGHT_GLASS_ON;
	Hash BRAKELIGHT_GLASS_DAMAGE0;
	Hash BRAKELIGHT_ONF;
	Hash BRAKELIGHT_GLASS_ONF;
	Hash _unk7;
	Hash SHADOW;

	void AddToPerm(Hash hash)
	{
		FUNC(0x007B1120, int, __cdecl, _AddToPerm, Hash*, int, int, Hash);
		this->NumTexturesToLoadPerm += _AddToPerm(this->TexturesToLoadPerm, this->NumTexturesToLoadPerm, 104, hash);
	}
};

class CarRenderInfoExtras;
struct CarRenderInfo
{
	D3DXMATRIX ExhaustLocalWorldCopy[3];
	D3DXMATRIX TireLocalWorldCopy[4];
	D3DXMATRIX SpoilerLocalWorldCopy;
	D3DXMATRIX RoofScoopLocalWorldCopy;
	D3DXMATRIX LastFewMatrices[3];
	D3DXVECTOR4 LastFewPositions[3];
	short MatrixIndex;
	short TailDrag;
	float AnimTime;
	int CarFrame;
	float CarTimebaseStart;
	D3DXVECTOR4 Velocity;
	D3DXVECTOR4 AngularVelocity;
	D3DXVECTOR4 Acceleration;
	D3DXVECTOR3 BoundingBoxMin;
	CarRenderInfoExtras* Extras;
	D3DXVECTOR4 BoundingBoxMax;
	char unk_2[0x5C];
	int IsHoodCarbon;
	RideInfo* pRideInfo;
	void* CarTypeInfo;
	TextureInfo* CarShadowTexture;
	TextureInfo* CarShadowRampTexture;
	CarMaterials Materials;
	PositionMarkers Markers;
	int unk_4[4];
	bTList<eLightFlare> LightFlares;
	WheelData WheelData;
	bSlist<CarEmitterPosition> EmitterPositions[28];
	ReplacementTextureEntry GeneralReplacementTextures[74];
	ReplacementTextureEntry CarbonReplacementTextures[74];
	ReplacementTextureEntry DecalReplacementTextures[2];
	ReplacementTextureEntry CaliperReplacementTextures[2][2];
	eModel* PartModel[0x5A][5];
	int unk_5[6];
	eDynamicLightPack DynamicLightPack;
	eDynamicLight DynamicLightsTable[2];
	int unk_6[5];
	int KitNumber;
	AutoSculptRender AutosculptRender;
	D3DXMATRIX* Matrix;
	UsedCarTextureInfo UsedTextureInfo;
	int LightsState1;
	int LightsState2;
	int MinLodLevel;
	int MaxLodLevel;
	int MinReflectionLodLevel;
	float DeltaTime;
	float Radius;
	int unk_7;
	CarPartCuller PartCuller;
	int unk_8;
	CarDamage Damage;

	void Ctor(RideInfo* rideInfo)
	{
		FUNC(0x007E55E0, void, __thiscall, _Ctor, CarRenderInfo*, RideInfo*);
		_Ctor(this, rideInfo);
	}

	void UpdateLightStateTextures()
	{
		FUNC(0x007ADC90, void, __thiscall, _UpdateLightStateTextures, CarRenderInfo*);
		_UpdateLightStateTextures(this);
	}

	PVehicle* GetPVehicle()
	{
		if (Game::InRace())
		{
			auto carRenderConn = CarRenderConn::Get(this);
			if (carRenderConn)
			{
				return carRenderConn->GetPVehicle();
			}
		}

		return null;
	}

	bool IsGlareOn(VehicleFX vehicleFX)
	{
		return (this->LightsState1 & (int)vehicleFX) != 0;
	}

	bool IsPlayer()
	{
		return this->pRideInfo->RenderUsage == 0;
	}

	double DrawAmbientShadow(eView* a1, D3DXVECTOR4* a2, float a3, D3DXMATRIX* a4, D3DXMATRIX* a5, D3DXMATRIX* a6)
	{
		FUNC(0x007BE4C0, double, __thiscall, _DrawAmbientShadow, CarRenderInfo*, eView*, D3DXVECTOR4*, float, D3DXMATRIX*, D3DXMATRIX*, D3DXMATRIX*);
		return _DrawAmbientShadow(this, a1, a2, a3, a4, a5, a6);
	}

	void UpdateWheelYRenderOffset()
	{
		FUNC(0x007CB6F0, void, __thiscall, _UpdateWheelYRenderOffset, CarRenderInfo*);
		_UpdateWheelYRenderOffset(this);
	}

	bool HasExhaustMarker()
	{
		return this->Markers.LeftExhaust || this->Markers.RightExhaust || this->Markers.CenterExhaust;
	}

	int SetPlayerDamage(int* damage)
	{
		FUNC(0x007BDEA0, int, __thiscall, _SetPlayerDamage, CarRenderInfo*, int*);
		return _SetPlayerDamage(this, damage);
	}

	bool IsBrakelightOn(int b)
	{
		return (this->LightsState2 & b) == 0 && (this->LightsState1 & b) != 0;
	}

	eLightMaterial* ResolveMaterial(Slot slot)
	{
		if (slot == Slot::SPOILER && this->Materials.Spoiler)
		{
			return this->Materials.Spoiler;
		}

		if (slot == Slot::ROOFSCOOP && this->Materials.RoofScoop)
		{
			return this->Materials.RoofScoop;
		}

		if (slot == Slot::HOOD && this->Materials.Hood)
		{
			return this->Materials.Hood;
		}

		return this->Materials.Body;
	}

	void SetCarDamageState(bool state, Slot fromSlot, Slot toSlot)
	{
		FUNC(0x007ADA60, void, __thiscall, _SetCarDamageState, CarRenderInfo*, bool, Slot, Slot);
		_SetCarDamageState(this, state, fromSlot, toSlot);
	}

	void CopyCarbonReplacementTable()
	{
		memcpy(this->CarbonReplacementTextures + 2, this->GeneralReplacementTextures + 2, 72 * 12);
	}

	void SetWindowDamage(ReplacementTextureIndex index, DamageZone damageZone, Hash from, Hash to);
};