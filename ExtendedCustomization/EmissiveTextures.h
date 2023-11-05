#pragma once
#include "RenderingModel.h"

DynamicLightingContext Prelit;
bool PrelitInit = false;

void InitPrelit()
{
	if (!PrelitInit)
	{
		memset(&Prelit, 0, sizeof(DynamicLightingContext));
		for (int i = 0; i < 10; i++)
		{
			Prelit.harmonics[i] = { 0.7f, 0.7f, 0.7f, 0.7f };
		}

		PrelitInit = true;
	}
}

void __cdecl CreateRenderingModel(void* meshEntry, eSolid* solid, int flags, void* effect, TextureInfo** textures, D3DXMATRIX* trs, DynamicLightingContext* context, eLightMaterial* material, void* blend_trs, void* pca)
{
	InitPrelit();

	RenderingModel::Create(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
	auto model = RenderingModel::GetLast();

	auto key = model->diffuse_texture_info->key;
	key = StringHash1("_E", key);
	auto emissive = TextureInfo::Get(key, false, false);
	if (emissive)
	{
		textures[0] = emissive;
		material = eLightMaterial::Get(StringHash("DEFAULT"), 0);
		context = &Prelit;
		RenderingModel::Create(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
	}
}

void InitEmissiveTextures()
{
	injector::MakeCALL(0x007295EE, CreateRenderingModel);
}