#pragma once
#include <vector>
#include <cmath>
#include "CarRenderInfo.h"
#include "ePoly.h"
#include "eView.h"
#include "Constants.h"
#include "Neon.h"
#include "DBCarPart.h"
#include "Config.h"

void InitNeon();

struct Neon
{
	D3DXMATRIX* Start;
	D3DXMATRIX* End;
};

struct NeonPulse
{
	float Val = 1.0f;
	float Dir = -1.0f;
	Color color;
};

class CarNeon
{
private:
	CarRenderInfo* carRenderInfo;
	D3DXMATRIX* carMatrix;
	D3DXMATRIX* carMatrixReflection;
	std::vector<Neon> neons;
	D3DXMATRIX identity;
	TextureInfo* neonBlur = NULL;
	TextureInfo* neonBlurInner = NULL;

	NeonPulse pulse;
	inline static NeonPulse pulseBackup;

public:
	TextureInfo* NeonTexture = NULL;

	CarNeon(CarRenderInfo* carRenderInfo, D3DXMATRIX* carMatrix, D3DXMATRIX* carMatrixReflection)
	{
		this->carRenderInfo = carRenderInfo;
		this->carMatrix = carMatrix;
		this->carMatrixReflection = carMatrixReflection;
		this->neonBlur = TextureInfo::Get(Hashes::NEONBLUR, 0, 0);
		this->neonBlurInner = TextureInfo::Get(Hashes::NEONBLUR_INNER, 0, 0);
		D3DXMatrixIdentity(&this->identity);

		if (Game::InFrontEnd())
		{
			this->pulse = pulseBackup;
		}
		else
		{
			this->pulse.Dir = 1.0f;
			this->pulse.Val = (bRandom(101) % 100 + 30) / 100.0f;
		}
	}

	void Init()
	{
		auto neonPart = this->carRenderInfo->pRideInfo->GetPart(Slot_Neon);
		if (neonPart)
		{
			auto textureHash = neonPart->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
			if (textureHash)
			{
				this->NeonTexture = TextureInfo::Get(textureHash, 0, 0);
			}
		}
	}

	void Update()
	{
		auto neonPart = this->carRenderInfo->pRideInfo->GetPart(Slot_Neon);
		if (neonPart)
		{
			float minPulse = 0.3f;
			if (neonPart->IsAutosculpt())
			{
				float h = this->carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneNeon].Zones[0] * 0.9999f;
				float s = 1.0f - this->carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneNeon].Zones[1];
				float br = 1.0f - this->carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneNeon].Zones[2];

				float r, g, b;
				HSV2RGB_255(h, s, br, &r, &g, &b);

				if (neonPart->GetAppliedAttributeIParam(Hashes::MORPHTARGET_NUM, 0) > 4)
				{
					minPulse = 0.0f;

					float h2 = this->carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneNeon].Zones[4] * 0.9999f;
					float s2 = 1.0f - this->carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneNeon].Zones[5];
					float br2 = 1.0f - this->carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneNeon].Zones[6];

					float r2, g2, b2;
					HSV2RGB_255(h2, s2, br2, &r2, &g2, &b2);

					this->SetColor(r, g, b, r2, g2, b2);
				}
				else
				{
					this->SetColor(r, g, b);
				}
			}
			else
			{
				this->UpdateColorFromAttribs();
			}

			this->UpdatePulse(minPulse);
		}
	}

	Color GetColor()
	{
		return this->pulse.color;
	}

	void FindMarkers()
	{
		auto rideInfo = this->carRenderInfo->pRideInfo;
		auto neonPart = rideInfo->GetPart(Slot_Neon);
		if (neonPart && neonPart->GetAppliedAttributeIParam(Hashes::DRAW_MARKERS, 1) == 1)
		{
			for (Slot slot : NeonSlots)
			{
				auto part = rideInfo->GetPart(slot);
				if (part)
				{
					for (int i = 0; i < 99; i++)
					{
						char buff[64];
						sprintf_s(buff, "NEON_START%02d", i);
						auto start = part->GetMarker(StringHash(buff));
						if (start)
						{
							sprintf_s(buff, "NEON_END%02d", i);
							auto end = part->GetMarker(StringHash(buff));
							if (end)
							{
								this->neons.push_back({ start, end });
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}

	void RenderShadow(eView* a1, D3DXVECTOR4* a2, float a3, D3DXMATRIX* a4, D3DXMATRIX* a5, D3DXMATRIX* a6)
	{
		if (this->NeonTexture)
		{
			auto backup = this->carRenderInfo->CarShadowTexture;

			this->carRenderInfo->CarShadowTexture = this->NeonTexture;
			this->carRenderInfo->DrawAmbientShadow(a1, a2, a3, a4, a5, a6);

			this->carRenderInfo->CarShadowTexture = backup;
		}
	}

	void RenderMarkers(bool reflection)
	{
		if (this->NeonTexture)
		{
			auto neonPart = this->carRenderInfo->pRideInfo->GetPart(Slot_Neon);
			for (auto neon : this->neons)
			{
				float size = g_Config.NeonSize;
				if (this->neonBlur && size > 0)
				{
					RenderMarker(neon.Start, neon.End, size, this->pulse.color, this->neonBlur, reflection);
				}

				size = g_Config.NeonInnerSize;
				if (this->neonBlurInner && size > 0)
				{
					Color color = this->pulse.color;
					float h, s, v;
					RGB2HSV(color.Bytes[0], color.Bytes[1], color.Bytes[2], &h, &s, &v);
					s -= 0.5;
					if (s < 0)
					{
						s = 0;
					}

					v *= 0.5f;

					float r, g, b;
					HSV2RGB_255(h, s, v, &r, &g, &b);
					color.Bytes[0] = r;
					color.Bytes[1] = g;
					color.Bytes[2] = b;

					RenderMarker(neon.Start, neon.End, size, color, this->neonBlurInner, reflection);
				}
			}
		}
	}

private:

	void RenderMarker(D3DXMATRIX* startMatrix, D3DXMATRIX* endMatrix, float size, Color color, TextureInfo* texture, bool reflection)
	{
		// This is a direct copy of the code from UG2 decomp, not fully reversed yet.

		ePoly poly;
		poly.SetColor(color);
		auto carMatrix = reflection ? *this->carMatrixReflection : *this->carMatrix;
		auto carPos = GetVector(&carMatrix, 3);

		SetVector(&carMatrix, 3, { 0,0,0,0 });

		auto view = eView::GetPlayer1();
		auto camera = view->pCamera;

		D3DXVECTOR2 v35;
		D3DXVECTOR3 a2;
		D3DXVECTOR4 a3;
		D3DXVECTOR4 v43;
		D3DXVECTOR3 v239;

		float v14 = carPos.x - camera->CurrentKey.Position.x;
		float v16 = carPos.y - camera->CurrentKey.Position.y;
		float v18 = carPos.z - camera->CurrentKey.Position.z;

		a3.x = camera->CurrentKey.Matrix._11;
		a3.y = camera->CurrentKey.Matrix._21;
		a3.z = camera->CurrentKey.Matrix._31;

		v43.x = camera->CurrentKey.Matrix._12;
		v43.y = camera->CurrentKey.Matrix._22;
		v43.z = camera->CurrentKey.Matrix._32;

		float v50 = size * v43.x;
		float v45 = size * v43.y;
		float v46 = size * v43.z;
		a2.z = v46;
		float v146 = v46;
		float v144 = v50;
		float v145 = v45;
		float v40 = size * a3.x;
		float v41 = size * a3.y;
		float v37 = size * a3.z;
		D3DXVECTOR4 v42 = -v43;

		a3 = { 0, 0, 0, 0 };
		D3DXVec3Transform(&a3, (D3DXVECTOR3*)&a3, startMatrix);
		D3DXVec3Transform(&a3, (D3DXVECTOR3*)&a3, &carMatrix);

		a3.x = a3.x + carPos.x;
		a3.y = a3.y + carPos.y;
		a3.z = a3.z + carPos.z;

		v43 = { 0, 0, 0, 0 };
		D3DXVec3Transform(&v43, (D3DXVECTOR3*)&v43, endMatrix);
		D3DXVec3Transform(&v43, (D3DXVECTOR3*)&v43, &carMatrix);
		v43.x = v43.x + carPos.x;
		v43.y = v43.y + carPos.y;
		v43.z = v43.z + carPos.z;

		view->TransformByZBias(&a2, (D3DXVECTOR3*)&a3);
		view->TransformByZBias(&v239, (D3DXVECTOR3*)&v43);
		v35.x = v239.x - a2.x;
		v35.y = v239.y - a2.y;
		D3DXVec2Normalize(&v35, &v35);

		poly.Vertices[0] = a3;
		poly.Vertices[1] = v43;
		float v282 = v37 * v35.y;
		float v207 = v40 * v35.y;
		poly.Vertices[2].x = v43.x - v207;

		float v66 = v42.y * size * v35.x;
		float v310 = v42.z * size * v35.x;
		float v173 = v42.x * size * v35.x;
		poly.Vertices[2].x = poly.Vertices[2].x - v173;
		poly.Vertices[2].y = v43.y - v41 * v35.y - v66;
		poly.Vertices[2].z = v43.z - v282 - v310;

		poly.Vertices[3].x = a3.x - v207 - v173;
		poly.Vertices[3].y = a3.y - v41 * v35.y - v66;
		poly.Vertices[3].z = a3.z - v282 - v310;

		poly.UVs[0].x = 0.5;
		poly.UVs[0].y = 0.5;
		poly.UVs[1].x = 0.5;
		poly.UVs[1].y = 0.5;
		poly.UVs[2].x = 0.5;
		poly.UVs[2].y = 0.0;
		poly.UVs[3].x = 0.5;
		poly.UVs[3].y = 0.0;
		poly.Render(texture, &this->identity);

		float v254 = v37 * v35.y;
		float v77 = v40 * v35.y;
		float v78 = v41 * v35.y;
		float v258 = v42.z * size * v35.x;
		float v56 = v42.x * size * v35.x;
		float v57 = v42.y * size * v35.x;
		poly.Vertices[2].x += v56 * 2 + v77 * 2;
		poly.Vertices[2].y += v57 * 2 + v78 * 2;
		poly.Vertices[2].z += v258 * 2 + v254 * 2;

		poly.Vertices[3].x += v77 * 2 + v56 * 2;
		poly.Vertices[3].y += v78 * 2 + v57 * 2;
		poly.Vertices[3].z += v258 * 2 + v254 * 2;
		poly.Render(texture, &this->identity);

		float v268 = v37 * v35.y;
		float v185 = v40 * v35.y;
		poly.Vertices[0].x = a3.x - v185;
		float v99 = v42.x * size;
		float v100 = v42.y * size;
		float v270 = v42.z * size * v35.x;
		float v189 = v99 * v35.x;
		poly.Vertices[0].x = poly.Vertices[0].x - v189;
		poly.Vertices[0].y = a3.y - v41 * v35.y - v100 * v35.x;
		poly.Vertices[0].z = a3.z - v268 - v270;
		poly.Vertices[1].x = a3.x - v185;
		poly.Vertices[1].y = a3.y - v41 * v35.y;
		poly.Vertices[1].x = poly.Vertices[1].x - v189;
		poly.Vertices[1].y = poly.Vertices[1].y - v100 * v35.x;
		float v298 = v37 * v35.x;
		poly.Vertices[1].x = poly.Vertices[1].x - v40 * v35.x;
		poly.Vertices[1].y = poly.Vertices[1].y - v41 * v35.x;
		poly.Vertices[1].x = poly.Vertices[1].x - v50 * v35.y;
		poly.Vertices[1].y = poly.Vertices[1].y - v45 * v35.y;
		poly.Vertices[1].z = a3.z - v268 - v270 - v298 - v46 * v35.y;
		poly.Vertices[2].x = v185 + a3.x;
		poly.Vertices[2].y = v41 * v35.y + a3.y;
		poly.Vertices[2].x = poly.Vertices[2].x + v35.x * v99;
		poly.Vertices[2].y = v100 * v35.x + poly.Vertices[2].y;
		poly.Vertices[2].x = poly.Vertices[2].x - v35.x * v40;
		poly.Vertices[2].y = poly.Vertices[2].y - v41 * v35.x;
		poly.Vertices[2].x = poly.Vertices[2].x - v35.y * v50;
		poly.Vertices[2].y = poly.Vertices[2].y - v45 * v35.y;
		poly.Vertices[2].z = v268 + a3.z + v270 - v298 - v46 * v35.y;
		poly.Vertices[3].x = a3.x + v35.y * v40;
		poly.Vertices[3].y = v41 * v35.y + a3.y;
		poly.Vertices[3].x = poly.Vertices[3].x + v35.x * v99;
		poly.Vertices[3].y = v100 * v35.x + poly.Vertices[3].y;
		poly.Vertices[3].z = v270 + v268 + a3.z;

		poly.UVs[0].x = 0.5;
		poly.UVs[0].y = 0.0;
		poly.UVs[1].x = 0.0;
		poly.UVs[1].y = 0.0;
		poly.UVs[2].x = 0.0;
		poly.UVs[2].y = 1.0;
		poly.UVs[3].x = 0.5;
		poly.UVs[3].y = 1.0;
		poly.Render(texture, &this->identity);

		float v269 = v37 * v35.y;
		float v179 = v40 * v35.y;

		poly.Vertices[0].x = v43.x - v179;

		float v97 = v42.y * size;
		float v273 = v42.z * size * v35.x;
		float v187 = v42.x * size * v35.x;

		poly.Vertices[0].x = poly.Vertices[0].x - v187;
		poly.Vertices[0].y = v43.y - v41 * v35.y - v97 * v35.x;
		poly.Vertices[0].z = v43.z - v269 - v273;
		poly.Vertices[1].x = v43.x - v179;
		poly.Vertices[1].y = v43.y - v41 * v35.y;
		poly.Vertices[1].x = poly.Vertices[1].x - v187;
		poly.Vertices[1].y = poly.Vertices[1].y - v97 * v35.x;

		float v287 = v37 * v35.x;

		float v199 = v40 * v35.x;
		poly.Vertices[1].x = poly.Vertices[1].x + v199;
		poly.Vertices[1].y = v41 * v35.x + poly.Vertices[1].y;

		float v291 = v46 * v35.y;

		float v203 = v50 * v35.y;

		poly.Vertices[1].x = poly.Vertices[1].x + v203;
		poly.Vertices[1].y = v45 * v35.y + poly.Vertices[1].y;
		poly.Vertices[1].z = v291 + v43.z - v269 - v273 + v287;

		poly.Vertices[2].x = v43.x + v179;
		poly.Vertices[2].y = v41 * v35.y + v43.y;

		poly.Vertices[2].x = poly.Vertices[2].x + v187;
		poly.Vertices[2].y = v97 * v35.x + poly.Vertices[2].y;

		poly.Vertices[2].x = poly.Vertices[2].x + v199;
		poly.Vertices[2].y = v41 * v35.x + poly.Vertices[2].y;
		float v307 = v146 * v35.y;

		float v219 = v144 * v35.y;
		poly.Vertices[2].x = poly.Vertices[2].x + v219;
		poly.Vertices[2].y = v145 * v35.y + poly.Vertices[2].y;
		poly.Vertices[2].z = v307 + v269 + v43.z + v273 + v287;

		poly.UVs[0].x = 0.5;
		poly.UVs[0].y = 0.0;
		poly.UVs[1].x = 1.0;
		poly.UVs[1].y = 0.0;
		poly.UVs[2].x = 1.0;
		poly.UVs[2].y = 1.0;
		poly.Vertices[3].x = v43.x + v179;
		poly.UVs[3].x = 0.5;
		poly.UVs[3].y = 1.0;
		poly.Vertices[3].y = v41 * v35.y + v43.y;

		poly.Vertices[3].x = poly.Vertices[3].x + v187;
		poly.Vertices[3].y = v97 * v35.x + poly.Vertices[3].y;
		poly.Vertices[3].z = v273 + v269 + v43.z;
		poly.Render(texture, &this->identity);
	}

	void UpdatePulse(float minPulse)
	{
		float pulse = 0;
		auto neonPart = this->carRenderInfo->pRideInfo->GetPart(Slot_Neon);

		if (neonPart->IsAutosculpt())
		{
			pulse = this->carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneNeon].Zones[3];
		}
		else if (neonPart->GetAppliedAttributeBParam(Hashes::PULSE, false))
		{
			pulse = 0.5;
		}

		if (pulse)
		{
			this->pulse.Val += Game::DeltaTime() * this->pulse.Dir * pulse * 3.0f;
			if (this->pulse.Val > 1.0f)
			{
				this->pulse.Val = 1.0f;
				this->pulse.Dir *= -1.0f;
			}

			if (this->pulse.Val < minPulse)
			{
				this->pulse.Val = minPulse;
				this->pulse.Dir *= -1.0f;
			}
		}
		else
		{
			this->pulse.Val = 1;
		}

		pulseBackup = this->pulse;
	}

	void UpdateColorFromAttribs()
	{
		auto neonPart = this->carRenderInfo->pRideInfo->GetPart(Slot_Neon);

		BYTE r1 = neonPart->GetAppliedAttributeIParam(Hashes::RED, 0);
		BYTE g1 = neonPart->GetAppliedAttributeIParam(Hashes::GREEN, 0);
		BYTE b1 = neonPart->GetAppliedAttributeIParam(Hashes::BLUE, 0);

		if (neonPart->GetAppliedAttributeParam<int>(Hashes::RED2))
		{
			BYTE r2 = neonPart->GetAppliedAttributeIParam(Hashes::RED2, 0);
			BYTE g2 = neonPart->GetAppliedAttributeIParam(Hashes::GREEN2, 0);
			BYTE b2 = neonPart->GetAppliedAttributeIParam(Hashes::BLUE2, 0);

			this->SetColor(r1, g1, b1, r2, g2, b2);
		}
		else
		{
			this->SetColor(r1, g1, b1);
		}
	}

	void SetColor(float r1, float g1, float b1, float r2, float g2, float b2)
	{
		float lerp = 1.0f - this->pulse.Val;
		this->pulse.color.Bytes[0] = std::lerp(r1, r2, lerp);
		this->pulse.color.Bytes[1] = std::lerp(g1, g2, lerp);
		this->pulse.color.Bytes[2] = std::lerp(b1, b2, lerp);
		this->pulse.color.Bytes[3] = 0xFF;
	}

	void SetColor(float r, float g, float b)
	{
		this->pulse.color.Bytes[0] = r * this->pulse.Val;
		this->pulse.color.Bytes[1] = g * this->pulse.Val;
		this->pulse.color.Bytes[2] = b * this->pulse.Val;
		this->pulse.color.Bytes[3] = 0xFF * this->pulse.Val;
	}
};