#pragma once
#include "texture.h"
#include <util/string.h>

class FDAPI Texture2D : public Texture {
private:
	ID3D11Texture2D* resource;

	Texture2D() { resource = nullptr;resourceView = nullptr; }

public:
	Texture2D(const String& filename);
	Texture2D(void* data, unsigned int width, unsigned int height, FD_TEXTURE_FORMAT format);
	~Texture2D();

	void Bind(unsigned int slot = 0) override;
};
