#pragma once
#include <vector>
#include "CarRenderInfo.h"
#include "ePoly.h"
#include "eView.h"

inline void __cdecl bNormalize2(bVector2* a1, bVector2* a2)
{
	long double v3; // st7
	double v5; // st7
	double v6; // st6
	float v7; // [esp+8h] [ebp+8h]

	v3 = sqrt(a2->y * a2->y + a2->x * a2->x);
	v7 = v3;
	if (v3 == 0)
	{
		a1->x = 1.0;
		a1->y = 0.0;
	}
	else
	{
		v5 = a2->y;
		v6 = 1.0 / v7;
		a1->x = v6 * a2->x;
		a1->y = v6 * v5;
	}
}

inline void __cdecl eMulVector(bVector3* a1, bMatrix4* a2, bVector3* a3)
{
	double v3; // st6
	double v4; // st7

	v3 = a2->v2.y * a3->z + a2->v0.y * a3->x + a2->v1.y * a3->y + a2->v3.y;
	v4 = a2->v2.z * a3->z + a2->v0.z * a3->x + a2->v1.z * a3->y + a2->v3.z;
	a1->x = a2->v2.x * a3->z + a2->v1.x * a3->y + a2->v0.x * a3->x + a2->v3.x;
	a1->y = v3;
	a1->z = v4;
}

inline float screenWidth = 1920;
inline float screenHeight = 1080;

inline void sub_5BC4A0(eView* view, bVector3* a2, bVector3* a3)
{
	double v3; // st6
	float v4; // [esp+Ch] [ebp-24h]
	float v5; // [esp+10h] [ebp-20h]
	float v6; // [esp+10h] [ebp-20h]
	float v7; // [esp+14h] [ebp-1Ch]
	float v8; // [esp+18h] [ebp-18h]
	float v9; // [esp+28h] [ebp-8h]
	float v10; // [esp+2Ch] [ebp-4h]

	v7 = a3->y;
	v8 = a3->z;
	v3 = v8 * view->PlatInfo->ProjectionZBiasMatrix.v2.y
		+ a3->x * view->PlatInfo->ProjectionZBiasMatrix.v0.y
		+ v7 * view->PlatInfo->ProjectionZBiasMatrix.v1.y
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.y;
	v9 = v8 * view->PlatInfo->ProjectionZBiasMatrix.v2.z
		+ a3->x * view->PlatInfo->ProjectionZBiasMatrix.v0.z
		+ v7 * view->PlatInfo->ProjectionZBiasMatrix.v1.z
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.z;
	v10 = v8 * view->PlatInfo->ProjectionZBiasMatrix.v2.w
		+ a3->x * view->PlatInfo->ProjectionZBiasMatrix.v0.w
		+ v7 * view->PlatInfo->ProjectionZBiasMatrix.v1.w
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.w;
	v5 = v8 * view->PlatInfo->ProjectionZBiasMatrix.v2.x
		+ v7 * view->PlatInfo->ProjectionZBiasMatrix.v1.x
		+ a3->x * view->PlatInfo->ProjectionZBiasMatrix.v0.x
		+ view->PlatInfo->ProjectionZBiasMatrix.v3.x;
	v4 = 1.0 / v10;
	v6 = v4 * v5;
	a2->x = (v6 + 1.0) * (double)1920 * 0.5;
	a2->y = (v3 * v4 - 1.0) * (double)1080 * (-0.5);
	a2->z = v4 * v9;
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
	D3D::Matrix* carMatrix;
	float width;
	std::vector<Neon> neons;
	D3D::Matrix Identity;

public:
	CarNeon(CarRenderInfo* carRenderInfo, D3D::Matrix* carMatrix)
	{
		this->carRenderInfo = carRenderInfo;
		this->width = 0.05f;
		this->carMatrix = carMatrix;
		Identity = D3D::Matrix::Identity();
	}

	void Update()
	{

	}

	void FindMarkers()
	{
		auto rideInfo = this->carRenderInfo->RideInfo;
		Slot slots[] = { Slot::FRONT_BUMPER, Slot::REAR_BUMPER };
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
		auto carPos1 = this->carMatrix->GetW();
		auto carPos = &carPos1;
		auto carMatrix1 = (bMatrix4*)this->carMatrix;
		auto carMatrix = *carMatrix1;
		carMatrix.v3.x = 0;
		carMatrix.v3.y = 0;
		carMatrix.v3.z = 0;
		carMatrix.v3.x = 0;

		poly.UVs[0].x = 0.5;
		poly.UVs[1].x = 0.5;
		poly.UVs[2].x = 0.5;
		poly.UVs[3].x = 0.5;

		auto view = eView::GetPlayer1();
		auto camera = view->pCamera;

		//
		bVector2 v35;
		bVector2 v83;
		bVector3 a2;
		bVector3 a3;
		bVector3 v43;
		bVector3 v239;
		float a8 = 0.07;

		float v12 = carPos->x;
		float v14 = v12 - camera->CurrentKey.Position.x;
		float v15 = camera->CurrentKey.Matrix.v1.x;
		float v16 = carPos->y - camera->CurrentKey.Position.y;
		float v17 = camera->CurrentKey.Position.z;
		a3.x = camera->CurrentKey.Matrix.v0.x;
		float v18 = carPos->z - v17;
		float v19 = camera->CurrentKey.Matrix.v2.x;
		a3.y = v15;
		float v20 = camera->CurrentKey.Matrix.v0.y;
		a3.z = v19;
		float v21 = camera->CurrentKey.Matrix.v1.y;
		v43.x = v20;
		float v22 = camera->CurrentKey.Matrix.v2.y;
		v43.y = v21;
		v43.z = v22;
		float v45 = 1.0;
		float v39 = sqrt(v18 * v18 + v14 * v14 + v16 * v16);

		float v36 = v45 * a8;

		float v50 = v36 * v43.x;
		v45 = v36 * v43.y;
		float v46 = v36 * v43.z;
		a2.z = v46;
		float v146 = v46;
		float v144 = v50;
		float v145 = v45;
		float v40 = v36 * a3.x;
		float v41 = v36 * a3.y;
		float v37 = v36 * a3.z;
		float v42 = -v43.x;
		float v38 = -v43.y;
		v39 = -v43.z;

		{
			a3.x = 0.0;
			a3.y = 0.0;
			a3.z = 0.0;
			eMulVector(&a3, startMatrix, &a3);
			eMulVector(&a3, &carMatrix, &a3);
			a3.x = a3.x + carPos->x;
			a3.y = a3.y + carPos->y;
			v43.x = 0.0;
			v43.y = 0.0;
			float v28 = a3.z + carPos->z;
			v43.z = 0.0;
			a3.z = v28;
			eMulVector(&v43, endMatrix, &v43);
			eMulVector(&v43, &carMatrix, &v43);
			v43.x = v43.x + carPos->x;
			v43.y = v43.y + carPos->y;
			v43.z = v43.z + carPos->z;
			float v162 = v43.x - a3.x;
			float v163 = v43.y - a3.y;
			float v164 = v43.z - a3.z;
			sub_5BC4A0(view, &a2, &a3);
			sub_5BC4A0(view, &v239, &v43);
			v35.x = v239.x - a2.x;
			v35.y = v239.y - a2.y;
			bNormalize2(&v83, &v35);
			v35 = v83;
			float v141 = v40;
			poly.Vertices[0].y = a3.y;
			float v142 = v41;
			poly.Vertices[0].x = a3.x;
			poly.Vertices[0].z = a3.z;
			poly.Vertices[1].y = v43.y;
			poly.Vertices[1].x = v43.x;
			poly.Vertices[1].z = v43.z;
			float v143 = v37;
			float v282 = v37 * v35.y;
			float v244 = v37;
			float v207 = v40 * v35.y;
			float v208 = v282;
			float v237 = v38;
			poly.Vertices[2].x = v43.x - v207;
			float v238 = v39;
			float v248 = v37;
			float v246 = v39 * v36;
			float v67 = v246;
			float v65 = v42 * v36;
			float v119 = v37;
			float v66 = v38 * v36;
			float v236 = v39;
			float v310 = v246 * v35.x;
			float v174 = v310;
			float v173 = v65 * v35.x;
			float v235 = v38;
			poly.Vertices[2].x = poly.Vertices[2].x - v173;
			poly.Vertices[2].y = v43.y - v41 * v35.y - v66 * v35.x;
			poly.Vertices[2].z = v43.z - v282 - v310;
			float v117 = v40;
			float v118 = v41;
			float v284 = v282;
			float v250 = v207;
			poly.Vertices[3].x = a3.x - v207;
			poly.Vertices[3].y = a3.y - v41 * v35.y;
			float v302 = v246;
			float v73 = v246;
			float v71 = v65;
			float v72 = v66;
			float v252 = v310;
			float v232 = v310;
			float v231 = v173;
			float v30 = 0;
			poly.Vertices[3].x = poly.Vertices[3].x - v173;
			poly.Vertices[3].y = poly.Vertices[3].y - v66 * v35.x;
			poly.Vertices[3].z = a3.z - v282 - v310;
		}
		
		//

		poly.Render(GetTextureInfo(StringHash("NEONBLUR"), 0, 0), &this->Identity);
	}

	//D3DXVECTOR3 GetLocalCameraPosition()
	//{
	//	auto view = eView::GetPlayer1();
	//	auto cameraMatrix = view->pCamera->CurrentKey.Matrix;
	//	cameraMatrix.Inverse();

	//	D3D::Matrix localCamera;
	//	auto invCarmatrix = *this->carMatrix;
	//	invCarmatrix.Inverse();
	//	D3D::Matrix::Multiply(&localCamera, &cameraMatrix, &invCarmatrix);
	//	return localCamera.GetW();
	//}

	//void RenderLeftSide(ePoly poly, D3DXVECTOR3 dir)
	//{
	//	auto side = poly.Vertices[3].V - poly.Vertices[2].V;
	//	D3DXVec3Normalize(&dir, &dir);
	//	D3DXVec3Normalize(&side, &side);

	//	D3DXVECTOR3 perp;
	//	D3DXVec3Cross(&perp, &side, &dir);

	//	poly.Vertices[0].V = poly.Vertices[3].V - perp * this->width, 0;
	//	poly.Vertices[1].V = poly.Vertices[2].V - perp * this->width, 0;

	//	poly.UVs[0].y = 0;
	//	poly.UVs[1].y = 0;

	//	poly.Render(GetTextureInfo(StringHash("NEONBLUR"), 0, 0), carMatrix);
	//}

	//void RenderRightSide(ePoly poly, D3DXVECTOR3 dir)
	//{
	//	auto side = poly.Vertices[3].V - poly.Vertices[2].V;
	//	D3DXVec3Normalize(&dir, &dir);
	//	D3DXVec3Normalize(&side, &side);

	//	D3DXVECTOR3 perp;
	//	D3DXVec3Cross(&perp, &side, &dir);

	//	poly.Vertices[3].V = poly.Vertices[0].V + perp * this->width, 0;
	//	poly.Vertices[2].V = poly.Vertices[1].V + perp * this->width, 0;

	//	poly.UVs[0].y = 0.5;
	//	poly.UVs[1].y = 0.5;
	//	poly.UVs[2].y = 0;
	//	poly.UVs[3].y = 0;

	//	poly.Render(GetTextureInfo(StringHash("NEONBLUR"), 0, 0), carMatrix);
	//}
};