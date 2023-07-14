#pragma once
#include <vector>
#include "CarRenderInfo.h"
#include "ePoly.h"
#include "eView.h"
#include "Constants.h"
#include "Neon.h"

void InitNeon();

inline void sub_5BC4A0(eView* view, bVector3* out, bVector3* v)
{
	double v3 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.y
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.y
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.y
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.y;
	float v9 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.z
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.z
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.z
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.z;
	float v10 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.w
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.w
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.w
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.w;
	float v5 = v->z * view->PlatInfo->ProjectionZBiasMatrix.v2.x
		+ v->y * view->PlatInfo->ProjectionZBiasMatrix.v1.x
		+ v->x * view->PlatInfo->ProjectionZBiasMatrix.v0.x
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.x;

	float v4 = 1.0 / v10;
	float v6 = v4 * v5;

	out->x = (v6 + 1.0) * (float)*Game::ScreenSizeX * 0.5;
	out->y = (v3 * v4 - 1.0) * (float)*Game::ScreenSizeY * (-0.5);
	out->z = v4 * v9;
}

struct Neon
{
	bMatrix4* Start;
	bMatrix4* End;
};

struct NeonPulse
{
	float Val = 1.0f;
	float Dir = -1.0f;
};

class CarNeon
{
private:
	CarRenderInfo* carRenderInfo;
	bMatrix4* carMatrix;
	std::vector<Neon> neons;
	bMatrix4 mIdentity;
	TextureInfo* neonBlur = NULL;
	NeonPulse pulse;
	inline static NeonPulse pulseBackup;
	bColor color;

public:
	TextureInfo* NeonTexture = NULL;

	CarNeon(CarRenderInfo* carRenderInfo, bMatrix4* carMatrix)
	{
		this->carRenderInfo = carRenderInfo;
		this->carMatrix = carMatrix;
		this->mIdentity.Identity();
		this->neonBlur = GetTextureInfo(Hashes::NEONBLUR, 0, 0);

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
		auto neonPart = this->carRenderInfo->RideInfo->GetPart(Slot_Neon);
		if (neonPart)
		{
			auto textureHash = neonPart->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
			if (textureHash)
			{
				this->NeonTexture = GetTextureInfo(textureHash, 0, 0);
			}
		}
	}

	void Update()
	{
		float pulse = this->carRenderInfo->RideInfo->AutosculptData[ZoneNeon].Zones[2];
		if (pulse)
		{
			this->pulse.Val += *Game::DeltaTime * this->pulse.Dir * pulse * 2.0f;
			if (this->pulse.Val > 1.0f)
			{
				this->pulse.Val = 1.0f;
				this->pulse.Dir *= -1.0f;
			}

			if (this->pulse.Val < 0.3f)
			{
				this->pulse.Val = 0.3f;
				this->pulse.Dir *= -1.0f;
			}
		}
		else
		{
			this->pulse.Val = 1;
		}

		float h = carRenderInfo->RideInfo->AutosculptData[ZoneNeon].Zones[0] * 0.9999f;
		float s = carRenderInfo->RideInfo->AutosculptData[ZoneNeon].Zones[1];

		float r, g, b;
		HSV2RGB(h, 1.0f - s, 1, &r, &g, &b);

		this->color.R = r * 128.0f;
		this->color.G = g * 128.0f;
		this->color.B = b * 128.0f;
		this->color.A = 0xFF * this->pulse.Val;

		pulseBackup = this->pulse;
	}

	bColor GetColor()
	{
		return this->color;
	}

	void FindMarkers()
	{
		auto rideInfo = this->carRenderInfo->RideInfo;
		Slot slots[] = { Slot::FRONT_BUMPER, Slot::REAR_BUMPER, Slot::REAR_BUMPER, Slot::INTERIOR };
		for (Slot slot : slots)
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
						neons.push_back({ (bMatrix4*)start, (bMatrix4*)end });
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	void RenderShadow(int a1, float* a2, float a3, int a4, int a5, int a6)
	{
		if (this->NeonTexture)
		{
			auto backup = this->carRenderInfo->CarShadowTexture;

			this->carRenderInfo->CarShadowTexture = this->NeonTexture;
			this->carRenderInfo->DrawAmbientShadow(a1, a2, a3, a4, a5, a6);

			this->carRenderInfo->CarShadowTexture = backup;
		}
	}

	void RenderMarkers()
	{
		if (this->NeonTexture)
		{
			for (auto neon : neons)
			{
				RenderMarker(neon.Start, neon.End);
			}
		}
	}

	void RenderMarker(bMatrix4* startMatrix, bMatrix4* endMatrix)
	{
		ePoly poly;
		poly.SetColor(this->color);
		auto carPos = this->carMatrix->v3;
		auto carMatrix = *this->carMatrix;
		carMatrix.v3 = { 0,0,0,0 };

		auto view = eView::GetPlayer1();
		auto camera = view->pCamera;

		bVector2 v35;
		bVector3 a2;
		bVector3 a3;
		bVector3 v43;
		bVector3 v239;
		const float size = 0.03;

		float v14 = carPos.x - camera->CurrentKey.Position.x;
		float v16 = carPos.y - camera->CurrentKey.Position.y;
		float v18 = carPos.z - camera->CurrentKey.Position.z;

		a3.x = camera->CurrentKey.Matrix.v0.x;
		a3.y = camera->CurrentKey.Matrix.v1.x;
		a3.z = camera->CurrentKey.Matrix.v2.x;

		v43.x = camera->CurrentKey.Matrix.v0.y;
		v43.y = camera->CurrentKey.Matrix.v1.y;
		v43.z = camera->CurrentKey.Matrix.v2.y;

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
		bVector3 v42 = -v43;

		a3 = { 0, 0, 0 };
		eMulVector(&a3, startMatrix, &a3);
		eMulVector(&a3, &carMatrix, &a3);

		a3.x = a3.x + carPos.x;
		a3.y = a3.y + carPos.y;
		a3.z = a3.z + carPos.z;

		v43 = { 0, 0, 0 };
		eMulVector(&v43, endMatrix, &v43);
		eMulVector(&v43, &carMatrix, &v43);
		v43.x = v43.x + carPos.x;
		v43.y = v43.y + carPos.y;
		v43.z = v43.z + carPos.z;

		sub_5BC4A0(view, &a2, &a3);
		sub_5BC4A0(view, &v239, &v43);
		v35.x = v239.x - a2.x;
		v35.y = v239.y - a2.y;
		bNormalize(&v35, &v35);

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
		poly.Render(this->neonBlur, &this->mIdentity);

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
		poly.Render(this->neonBlur, &this->mIdentity);

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
		poly.Render(this->neonBlur, &this->mIdentity);

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
		poly.Render(this->neonBlur, &this->mIdentity);
	}
};