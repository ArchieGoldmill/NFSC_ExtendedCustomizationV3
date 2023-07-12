#pragma once
#include "CarRenderInfo.h"
#include "ePoly.h"
#include "eView.h"

class CarNeon
{
private:
	CarRenderInfo* carRenderInfo;
	D3D::Matrix* carMatrix;
	float width;

public:
	CarNeon(CarRenderInfo* carRenderInfo, D3D::Matrix* carMatrix)
	{
		this->carRenderInfo = carRenderInfo;
		this->width = 0.05f;
		this->carMatrix = carMatrix;
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

		D3DXVECTOR3 start = { 1, 1, 0 };
		D3DXVECTOR3 end = { 0, 0, 1 };
		auto axis = end - start;

		float lenght = D3DXVec3Length(&axis);

		poly.Vertices[0].x = -this->width;
		poly.Vertices[0].y = 0;
		poly.Vertices[0].z = 0;

		poly.Vertices[1].x = this->width;
		poly.Vertices[1].y = 0;
		poly.Vertices[1].z = 0;

		poly.Vertices[2].x = this->width;
		poly.Vertices[2].y = 0;
		poly.Vertices[2].z = lenght;

		poly.Vertices[3].x = -this->width;
		poly.Vertices[3].y = 0;
		poly.Vertices[3].z = lenght;

		poly.UVs[0].y = 0.5;
		poly.UVs[1].y = 0.5;
		poly.UVs[2].y = 0.5;
		poly.UVs[3].y = 0.5;
		
		auto lookAt = D3D::Matrix::FromLookAt({ 0, 0, 0 }, axis);
		poly.TransformVertices(lookAt);
		D3DXVec3Normalize(&axis, &axis);

		auto view = eView::GetPlayer1();
		auto cameraMatrix = view->pCamera->CurrentKey.Matrix;
		cameraMatrix.Inverse();

		D3D::Matrix localCamera;
		auto invCarmatrix = *carMatrix;
		invCarmatrix.Inverse();
		D3D::Matrix::Multiply(&localCamera, &cameraMatrix, &invCarmatrix);

		auto localCamPos = localCamera.GetW();

		auto poly1 = poly;
		auto m1 = D3D::Matrix::Identity();
		m1.SetW(start);
		poly1.TransformVertices(m1);
		auto polygonPosition = poly1.GetCenter();
		D3DXVECTOR3 cameraToPolygon = polygonPosition - localCamPos;

		D3DXVECTOR3 crossProduct;
		D3DXVec3Cross(&crossProduct, &axis, &cameraToPolygon);
		float angle = atan2(crossProduct.y, crossProduct.x);

		auto rotation = D3D::Matrix::FromRotationAxis(axis, angle);
		rotation.SetW(start);
		poly.TransformVertices(rotation);

		poly.Render(GetTextureInfo(StringHash("NEONBLUR"), 0, 0), carMatrix);

		this->RenderLeftSide(poly, localCamPos);
		this->RenderRightSide(poly, localCamPos);
	}

	void RenderLeftSide(ePoly poly, D3DXVECTOR3 dir)
	{
		auto side = D3DXVECTOR3(poly.Vertices[3] - poly.Vertices[2]);
		D3DXVec3Normalize(&dir, &dir);
		D3DXVec3Normalize(&side, &side);

		D3DXVECTOR3 perp;
		D3DXVec3Cross(&perp, &side, &dir);

		poly.Vertices[0] = poly.Vertices[3] - D3DXVECTOR4(perp * this->width, 0);
		poly.Vertices[1] = poly.Vertices[2] - D3DXVECTOR4(perp * this->width, 0);

		poly.UVs[0].y = 0;
		poly.UVs[1].y = 0;

		poly.Render(GetTextureInfo(StringHash("NEONBLUR"), 0, 0), carMatrix);
	}

	void RenderRightSide(ePoly poly, D3DXVECTOR3 dir)
	{
		auto side = D3DXVECTOR3(poly.Vertices[3] - poly.Vertices[2]);
		D3DXVec3Normalize(&dir, &dir);
		D3DXVec3Normalize(&side, &side);

		D3DXVECTOR3 perp;
		D3DXVec3Cross(&perp, &side, &dir);

		poly.Vertices[3] = poly.Vertices[0] + D3DXVECTOR4(perp * this->width, 0);
		poly.Vertices[2] = poly.Vertices[1] + D3DXVECTOR4(perp * this->width, 0);

		poly.UVs[0].y = 0.5;
		poly.UVs[1].y = 0.5;
		poly.UVs[2].y = 0;
		poly.UVs[3].y = 0;

		poly.Render(GetTextureInfo(StringHash("NEONBLUR"), 0, 0), carMatrix);
	}
};