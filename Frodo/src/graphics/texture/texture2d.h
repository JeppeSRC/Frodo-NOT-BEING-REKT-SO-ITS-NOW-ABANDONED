#pragma once
#include "texture.h"
#include <util/string.h>

namespace FD {

class FDAPI Texture2D : public Texture {
private:
	ID3D11Texture2D* resource;

	Texture2D() { resource = nullptr; resourceView = nullptr; }

public:
	Texture2D(const String& filename);
	Texture2D(void* data, uint32 width, uint32 height, FD_TEXTURE_FORMAT format);
	~Texture2D();

	void Bind(uint32 slot = 0) override;
};

}