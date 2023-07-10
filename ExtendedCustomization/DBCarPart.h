#pragma once
#include "Hashes.h"
#include "Game.h"
#include "eSolid.h"
#include "RideInfo.h"

class DBCarPart
{
public:
	void* GetAppliedAttributeParam(Hash hash)
	{
		static auto _GetAppliedAttributeParam = (void* (__thiscall*)(DBCarPart*, Hash, int))0x007B00E0;
		return _GetAppliedAttributeParam(this, hash, 0);
	}

	int GetAppliedAttributeIParam(Hash hash, int defaultValue)
	{
		static auto _GetAppliedAttributeIParam = (int(__thiscall*)(DBCarPart*, Hash, int defaultValue))0x007C27C0;
		return _GetAppliedAttributeIParam(this, hash, defaultValue);
	}

	float GetAppliedAttributeFParam(Hash hash, float defaultValue)
	{
		float* ptr = (float*)this->GetAppliedAttributeParam(hash);
		if (ptr)
		{
			return ptr[1];
		}

		return defaultValue;
	}

	Hash GetModelNameHash(int a1, int a2, int a3)
	{
		FUNC(0x007CDCA0, Hash, __thiscall, _GetModelNameHash, DBCarPart*, int, int, int);
		return _GetModelNameHash(this, a1, a2, a3);
	}

	Hash GetTextureName()
	{
		FUNC(0x007C9EB0, Hash, __thiscall, _GetTextureName, DBCarPart*);
		return _GetTextureName(this);
	}

	bool IsAutosculpt()
	{
		return this->GetAppliedAttributeIParam(Hashes::MORPHTARGET_NUM, 0);
	}

	bool IsStock()
	{
		return this->GetAppliedAttributeIParam(Hashes::STOCK, 0);
	}

	bool HasKitW(int kitw)
	{
		for (int i = 0; i < 99; i++)
		{
			char kitStr[16];
			sprintf_s(kitStr, "KITW_%d", i);

			int* kit = (int*)this->GetAppliedAttributeParam(StringHash(kitStr));
			if (!kit)
			{
				if (i == 0 && kitw == 0)
				{
					return true;
				}

				break;
			}

			if (kit[1] == kitw)
			{
				return true;
			}
		}

		return false;
	}

	eSolid* GetSolid()
	{
		auto nameHash = this->GetModelNameHash(0, 1, 0);
		return eSolid::Find(nameHash);
	}

	D3D::Matrix* GetMarker(Hash markerHash)
	{
		auto solid = this->GetSolid();
		if (solid)
		{
			auto marker = solid->GetPostionMarker(markerHash);
			if (marker)
			{
				return &marker->Matrix;
			}
		}

		return NULL;
	}

	D3D::Matrix* GetAttachMarker(RideInfo* rideInfo, D3DXVECTOR3* scale = NULL)
	{
		Slot MarkerSlots[] = { Slot::FRONT_BUMPER, Slot::REAR_BUMPER, Slot::INTERIOR };

		auto markerName = this->GetAppliedAttributeIParam(Hashes::MARKER, 0);
		if (markerName)
		{
			for (auto markerSlot : MarkerSlots)
			{
				auto markerPart = rideInfo->GetPart(markerSlot);
				if (markerPart)
				{
					if (scale)
					{
						*scale = markerPart->GetMarkerScale(markerName);
					}

					return markerPart->GetMarker(markerName);
				}
			}
		}

		return NULL;
	}

	D3DXVECTOR3 GetMarkerScale(Hash marker)
	{
		D3DXVECTOR3 scale;

		scale.x = this->GetAppliedAttributeFParam(StringHash1("_SX", marker), 1.0f);
		scale.y = this->GetAppliedAttributeFParam(StringHash1("_SY", marker), 1.0f);
		scale.z = this->GetAppliedAttributeFParam(StringHash1("_SZ", marker), 1.0f);

		return scale;
	}

	bool HasMarkerName()
	{
		return this->GetAppliedAttributeIParam(Hashes::MARKER, 0) != 0;
	}
};