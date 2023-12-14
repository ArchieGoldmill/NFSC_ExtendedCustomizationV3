#include "PartMarker.h"
#include "CarRenderInfoExtras.h"

D3DXMATRIX* PartMarker::Get(D3DXMATRIX* matrix)
{
	D3DXMATRIX scale;
	auto part = this->carRenderInfo->pRideInfo->GetPart(this->slot);
	if (part && part->IsStock())
	{
		D3DXMatrixIdentity(&scale);
	}
	else
	{
		D3DXMatrixScaling(&scale, this->scale.x, this->scale.y, this->scale.z);
	}

	D3DXMatrixMultiply(&this->state, &scale, this->marker);
	auto animation = this->carRenderInfo->Extras->Animations->GetPartAnimation(this->attachTo);
	if (animation)
	{
		auto attachAnim = animation->Calculate();
		D3DXMatrixMultiply(&this->state, &this->state, &attachAnim);
	}

	D3DXMatrixMultiply(&this->state, &this->state, matrix);

	return &this->state;
}