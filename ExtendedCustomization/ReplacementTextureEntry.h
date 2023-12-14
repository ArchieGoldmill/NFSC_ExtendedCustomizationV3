#pragma once
#include "Hashes.h"
#include "TextureInfo.h"
#include "Game.h"

struct ReplacementTextureEntry
{
	Hash OldName = 0;
	Hash NewName = 0;
	TextureInfo* Texture = 0;

	void Update(Hash oldName, Hash newName)
	{
		if (this->NewName != newName)
		{
			this->OldName = oldName;
			this->NewName = newName;
			this->Texture = TextureInfo::Invalid;
		}
	}

	void Set(Hash oldName, Hash newName)
	{
		auto textureInfo = TextureInfo::Get(newName, false, false);
		if (textureInfo)
		{
			this->OldName = oldName;
			this->NewName = newName;
			this->Texture = textureInfo;
		}
		else
		{
			this->OldName = 0;
			this->NewName = 0;
			this->Texture = TextureInfo::Invalid;
		}
	}

	void Clear()
	{
		this->OldName = 0;
		this->NewName = 0;
		this->Texture = TextureInfo::Invalid;
	}
};