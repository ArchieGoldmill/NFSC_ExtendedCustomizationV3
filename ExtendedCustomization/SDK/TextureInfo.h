#pragma once
#include "Node.h"

class TextureInfoPlatInfo : public bNode<TextureInfoPlatInfo>
{
public:
	int type;
	int format;
	void* pD3DTexture;
};

class TextureInfoPlatInterface
{
public:
	TextureInfoPlatInfo* PlatInfo;
};

class TextureInfo : public TextureInfoPlatInterface
{
public:
	inline static TextureInfo* Invalid = (TextureInfo*)-1;

};