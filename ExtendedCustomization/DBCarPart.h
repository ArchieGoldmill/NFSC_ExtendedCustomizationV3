#pragma once
#include "Hashes.h"
#include "Game.h"
#include "eSolid.h"
#include "RideInfo.h"
#include <stdio.h>

struct UpgradeGroup
{
	unsigned char Level;
	unsigned char Part;
};

template<typename T>
struct Attribute
{
	Hash Name;
	T Value;
};

struct DBCarPart
{
	template<typename T>
	Attribute<T>* GetAppliedAttributeParam(Hash hash)
	{
		static auto _GetAppliedAttributeParam = (void* (__thiscall*)(DBCarPart*, Hash, int))0x007B00E0;
		return (Attribute<T>*)_GetAppliedAttributeParam(this, hash, 0);
	}

	int GetAppliedAttributeIParam(Hash hash, int defaultValue)
	{
		static auto _GetAppliedAttributeIParam = (int(__thiscall*)(DBCarPart*, Hash, int defaultValue))0x007C27C0;
		return _GetAppliedAttributeIParam(this, hash, defaultValue);
	}

	float GetAppliedAttributeFParam(Hash hash, float defaultValue)
	{
		auto attr = this->GetAppliedAttributeParam<float>(hash);
		if (attr)
		{
			return attr->Value;
		}

		return defaultValue;
	}

	Hash GetModelNameHash(int a1, int a2, int a3)
	{
		FUNC(0x007CDCA0, Hash, __thiscall, _GetModelNameHash, DBCarPart*, int, int, int);
		return _GetModelNameHash(this, a1, a2, a3);
	}

	Hash GetPartNameHash()
	{
		FUNC(0x007CD930, Hash, __thiscall, _GetPartNameHash, DBCarPart*);
		return _GetPartNameHash(this);
	}

	Hash GetTextureName()
	{
		FUNC(0x007C9EB0, Hash, __thiscall, _GetTextureName, DBCarPart*);
		return _GetTextureName(this);
	}

	int GetKit(int def = 0)
	{
		return this->GetAppliedAttributeIParam(Hashes::KITNUMBER, def);
	}

	bool IsStockByKit()
	{
		return this->GetKit() == 0;
	}

	bool IsAutosculpt()
	{
		return this->GetAppliedAttributeIParam(Hashes::MORPHTARGET_NUM, 0) != 0;
	}

	bool IsCarbon()
	{
		return this->GetAppliedAttributeIParam(Hashes::CARBONFIBRE, 0) != 0;
	}

	bool IsStock()
	{
		return this->GetAppliedAttributeIParam(Hashes::STOCK, 0);
	}

	bool HasKitW(int kitw)
	{
		for (int i = 0; i < 99; i++)
		{
			auto attr = this->GetAppliedAttributeParam<int>(FromIndex("KITW_%d", i));
			if (!attr)
			{
				if (i == 0 && kitw == 0)
				{
					return true;
				}

				break;
			}

			if (attr->Value == kitw)
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

	D3DXMATRIX* GetMarker(Hash markerHash)
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

	D3DXMATRIX* GetAttachMarker(RideInfo* rideInfo, D3DXVECTOR3* scale = NULL)
	{
		auto markerName = this->GetAppliedAttributeIParam(Hashes::MARKER, 0);
		if (markerName)
		{
			for (auto markerSlot : MarkerSlots)
			{
				auto markerPart = rideInfo->GetPart(markerSlot);
				if (markerPart && markerPart->HasMarker(markerName) >= 0)
				{
					auto marker = markerPart->GetMarker(markerName);
					if (marker)
					{
						if (scale)
						{
							*scale = markerPart->GetMarkerScale(markerName);
						}

						return marker;
					}
				}
			}
		}

		return NULL;
	}

	int HasMarker(Hash marker)
	{
		for (int i = 0; i < 99; i++)
		{
			auto attr = this->GetAppliedAttributeParam<Hash>(FromIndex("MARKER_%d", i));
			if (attr)
			{
				if (attr->Value == marker)
				{
					return i;
				}
			}
			else
			{
				break;
			}
		}

		return -1;
	}

	D3DXVECTOR3 GetMarkerScale(Hash marker)
	{
		D3DXVECTOR3 scale = { 1.0f, 1.0f, 1.0f };
		int i = this->HasMarker(marker);
		if (i >= 0)
		{
			marker = FromIndex("MARKER_%d", i);
			float s = this->GetAppliedAttributeFParam(StringHash1("_S", marker), 1.0f);
			scale.x = this->GetAppliedAttributeFParam(StringHash1("_SX", marker), s);
			scale.y = this->GetAppliedAttributeFParam(StringHash1("_SY", marker), s);
			scale.z = this->GetAppliedAttributeFParam(StringHash1("_SZ", marker), s);
		}

		return scale;
	}

	bool HasMarkerName()
	{
		return this->GetAppliedAttributeIParam(Hashes::MARKER, 0) != 0;
	}
};