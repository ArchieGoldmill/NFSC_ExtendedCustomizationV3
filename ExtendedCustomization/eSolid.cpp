#include "eModel.h"

void eSolid::GetBoundingBox(D3DXVECTOR3* a, D3DXVECTOR3* b)
{
	eModel model; // Fake model just to call method
	model.Solid = this;
	model.GetBoundingBox(a, b);
}
