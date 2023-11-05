#pragma once
#include "TextureInfo.h"
#include "eSolid.h"
#include "eLightMaterial.h"
#include "LightingContext.h"
#include "Func.h"

struct RenderingModel
{
	inline static RenderingModel* List = (RenderingModel*)0x00AB2780;
	inline static int* ListCount = (int*)0x00AB0BF0;

	static RenderingModel* GetLast()
	{
		return List + (*ListCount - 1);
	}

	static void Create(void* meshEntry, eSolid* solid, int flags, void* effect, TextureInfo** textures, D3DXMATRIX* trs, DynamicLightingContext* context, eLightMaterial* material, void* blend_trs, void* pca)
	{
		FUNC(0x00727930, void, __cdecl, _Create, void*, eSolid*, int, void*, TextureInfo**, D3DXMATRIX*, DynamicLightingContext*, eLightMaterial*, void*, void*);
		_Create(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
	}

	RenderState render_bits;
	TextureInfo* base_texture_info;
	IDirect3DTexture9* d3d9_diffuse_texture;
	IDirect3DTexture9* d3d9_normal_texture;
	IDirect3DTexture9* d3d9_height_texture;
	IDirect3DTexture9* d3d9_specular_texture;
	IDirect3DTexture9* d3d9_opacity_texture;
	void* mesh_entry;
	bool is_tri_stripped;
	eSolid* solid;
	int flags;
	void* effect;
	DynamicLightingContext* light_context;
	eLightMaterial* light_material;
	D3DXMATRIX* local_to_world;
	D3DXMATRIX* blending_matrices;
	TextureInfo* diffuse_texture_info;
	TextureInfo* normal_texture_info;
	TextureInfo* height_texture_info;
	TextureInfo* specular_texture_info;
	TextureInfo* opacity_texture_info;
	int sort_flags;
	void* _null;
	float negative_one;
	void* blend_data;
	bool use_low_lod;
};