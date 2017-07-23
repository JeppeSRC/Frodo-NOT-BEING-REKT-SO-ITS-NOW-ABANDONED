#include "asset.h"

#include <core/log.h>

namespace FD {

Asset::Asset(const Asset* asset) {
	name = asset->name;
	folder = asset->folder;
	packageName = asset->packageName;
	type = asset->type;

	size = asset->size;
	data = new byte[size];

	memcpy(data, asset->data, size);
}

String Asset::GetString() const {
	if (type != FD_ASSET_TYPE_STRING) {
		FD_FATAL("[Asset]: Failed to create string, \"%s\" is not of type String!", *name);
		return String("ERROR");
	}
	return String((char*)data);
}

Font* Asset::GetFont(uint32 size, ivec2 dpi, Font::FD_RANGE<>* range, uint32 num_ranges) const {
	if (type != FD_ASSET_TYPE_FONT) {
		FD_FATAL("[Asset]: Failed to create font, \"%s\" is not of type Font!", *name);
		return nullptr;
	}
	return new Font(data, (uint32)this->size, size, dpi, range, num_ranges);
}

Shader* Asset::GetShader() const {
	if (type != FD_ASSET_TYPE_SHADER) {
		FD_FATAL("[Asset]: Failed to create shader, \"%s\" is not of type Shader!", *name);
		return nullptr;
	}

	char* b = (char*)data;

	String v(b);
	b += v.length + 1;

	String p(b);

	return new Shader(v, p, "", true);
}

Texture* Asset::GetTexture() const {

	if (type == FD_ASSET_TYPE_TEXTURE2D) {
		uint32 w = 0;
		uint32 h = 0;
		uint32 b = 0;

		byte* data = Texture::Load(this->data, &w, &h, &b);

		if (b != 32) {
			FD_FATAL("[Asset]: Failed to create texture2d \"%s\", Only 32 bit textues supported atm!", *name);
			return nullptr;
		}

		return new Texture2D(data, w, h, FD_TEXTURE_FORMAT_UINT_8_8_8_8);
	} else if (type == FD_ASSET_TYPE_TEXTURECUBE) {
		//TODO: implement
		FD_WARNING("[Asset]: TODO Implement %s!!!" __FUNCSIG__);
		return nullptr;
	}


	FD_FATAL("[Asset]: Failed to create texture, \"%s\" is not of type Texture2D or TextureCube!", *name);
	return nullptr;
}

}