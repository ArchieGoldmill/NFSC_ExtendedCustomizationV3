#pragma once
#include <vector>
#include "CarRenderInfo.h"
#include "ePoly.h"
#include "eView.h"

inline float screenWidth = 1920;
inline float screenHeight = 1080;

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

class CarNeon
{
private:
	CarRenderInfo* carRenderInfo;
	bMatrix4* carMatrix;
	std::vector<Neon> neons;
	bMatrix4 mIdentity;
	TextureInfo* neonBlur;

public:
	CarNeon(CarRenderInfo* carRenderInfo, bMatrix4* carMatrix)
	{
		this->carRenderInfo = carRenderInfo;
		this->carMatrix = carMatrix;
		this->mIdentity.Identity();
		this->neonBlur = GetTextureInfo(Hashes::NEONBLUR, 0, 0);
	}

	void Update()
	{

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
					sprintf_s(buff, "NEON_START%d", i);
					auto start = part->GetMarker(StringHash(buff));
					if (start)
					{
						sprintf_s(buff, "NEON_END%d", i);
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

	void RenderMarkers()
	{
		for (auto neon : neons)
		{
			RenderMarker(neon.Start, neon.End);
		}
	}

	void RenderMarker(bMatrix4* startMatrix, bMatrix4* endMatrix)
	{
		ePoly poly;
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

		poly.Vertices[0].y = a3.y;
		poly.Vertices[0].x = a3.x;
		poly.Vertices[0].z = a3.z;
		poly.Vertices[1].y = v43.y;
		poly.Vertices[1].x = v43.x;
		poly.Vertices[1].z = v43.z;
		float v282 = v37 * v35.y;
		float v207 = v40 * v35.y;
		poly.Vertices[2].x = v43.x - v207;

		float v66 = v42.y * size * v35.x;
		float v310 = v42.z * size * v35.x;
		float v173 = v42.x * size * v35.x;
		poly.Vertices[2].x = poly.Vertices[2].x - v173;
		poly.Vertices[2].y = v43.y - v41 * v35.y - v66;
		poly.Vertices[2].z = v43.z - v282 - v310;
		poly.Vertices[3].x = a3.x - v207;
		poly.Vertices[3].y = a3.y - v41 * v35.y;
		poly.Vertices[3].x = poly.Vertices[3].x - v173;
		poly.Vertices[3].y = poly.Vertices[3].y - v66;
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
		float v318 = v254 + v254;
		float v256 = v77 + v77;
		poly.Vertices[2].x = v256 + poly.Vertices[2].x;
		poly.Vertices[2].y = v78 * 2 + poly.Vertices[2].y;
		float v258 = v42.z * size * v35.x;
		float v56 = v42.x * size * v35.x;
		float v57 = v42.y * size * v35.x;
		poly.Vertices[2].x = v56 * 2 + poly.Vertices[2].x;
		poly.Vertices[2].y = v57 * 2 + poly.Vertices[2].y;
		poly.Vertices[2].z = v258 * 2 + v318 + poly.Vertices[2].z;
		float v290 = v254;
		float v87 = v77;
		float v262 = v290 * 2;
		float v240 = v87 + v87;
		poly.Vertices[3].x = v240 + poly.Vertices[3].x;
		poly.Vertices[3].y = v78 * 2 + poly.Vertices[3].y;
		float v292 = v258;
		float v93 = v56;
		float v266 = v292 * 2;
		float v225 = v93 + v93;
		poly.Vertices[3].x = v225 + poly.Vertices[3].x;
		poly.Vertices[3].y = v57 * 2 + poly.Vertices[3].y;
		poly.Vertices[3].z = v266 + v262 + poly.Vertices[3].z;
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