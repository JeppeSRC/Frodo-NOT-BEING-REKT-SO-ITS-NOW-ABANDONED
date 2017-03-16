#pragma once

#include <fd.h>

#include <util/string.h>
#include <graphics/font/font.h>
#include <graphics/shader/shader.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/texture2d.h>

namespace FD {

enum FD_ASSET_TYPE {
	FD_ASSET_TYPE_RAW,
	FD_ASSET_TYPE_STRING,
	FD_ASSET_TYPE_FONT,
	FD_ASSET_TYPE_SHADER,

	FD_ASSET_TYPE_TEXTURE2D,
	FD_ASSET_TYPE_TEXTURECUBE,
};

class FDAPI Asset {
public:
	String name;
	String folder;
	uint64 size;
	FD_ASSET_TYPE type;
	String packageName;

	void* data;

	Asset() { size = 0; data = nullptr; }
	Asset(const Asset* asset);

	~Asset() { delete[] data; }

	String GetString() const;
	Font* GetFont(uint32 size, ivec2 dpi) const;
	Shader* GetShader() const;
	Texture* GetTexture() const;
};

}