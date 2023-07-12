#pragma once
#include "CarRenderInfo.h"
#include "ePoly.h"
#include "eView.h"

class CarNeon
{
private:
	CarRenderInfo* carRenderInfo;
	D3D::Matrix matrix;

public:
	CarNeon(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	void Update()
	{

	}

	void FindMarkers()
	{

	}

	void RenderMarkers()
	{
		ePoly poly;

		D3DXVECTOR3 start = { 0.3, 0, 0.3 };
		D3DXVECTOR3 end = { 1, 1, 1 };
		auto axis = end - start;

		float lenght = D3DXVec3Length(&axis);

		poly.Vertices[0].x = -0.1;
		poly.Vertices[0].y = 0;
		poly.Vertices[0].z = 0;

		poly.Vertices[1].x = 0.1;
		poly.Vertices[1].y = 0;
		poly.Vertices[1].z = 0;

		poly.Vertices[2].x = 0.1;
		poly.Vertices[2].y = 0;
		poly.Vertices[2].z = lenght;

		poly.Vertices[3].x = -0.1;
		poly.Vertices[3].y = 0;
		poly.Vertices[3].z = lenght;

		poly.UVs[0].y = 0.5;
		poly.UVs[1].y = 0.5;
		poly.UVs[2].y = 0.5;
		poly.UVs[3].y = 0.5;

		auto carMatrix = this->carRenderInfo->GetMatrix();

		auto lookAt = D3D::Matrix::FromLookAt({ 0,0,0 }, axis);
		poly.TransformVertices(lookAt);

		D3DXVec3Normalize(&axis, &axis);
		auto view = eView::GetPlayer1();
		auto dir = D3DXVECTOR3(view->pCamera->CurrentKey.Position) - carMatrix->GetW();
		//D3DXVec3Normalize(&dir, &dir);
		auto poly1 = poly;
		auto m1 = D3D::Matrix::Identity();
		m1.SetW(start);
		poly1.TransformVertices(m1);
		auto polygonPosition = poly1.GetCenter();
		D3DXVECTOR3 cameraToPolygon = polygonPosition - dir;

		D3DXVECTOR3 crossProduct;
		D3DXVec3Cross(&crossProduct, &axis, &cameraToPolygon);
		float angle = atan2(crossProduct.y, crossProduct.x);

		auto rotation = D3D::Matrix::FromRotationAxis(axis, angle);
		rotation.SetW(start);
		poly.TransformVertices(rotation);

		poly.Render(GetTextureInfo(StringHash("NEONBLUR"), 0, 0), carMatrix);
	}
};