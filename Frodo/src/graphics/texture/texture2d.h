#pragma once
#include "texture.h"
#include <util/string.h>

class FDAPI FDTexture2D : public FDTexture {
private:
	ID3D11ShaderResourceView* resourceView;
	ID3D11Texture2D* resource;

	FDTexture2D() { resource = nullptr;resourceView = nullptr; }

public:
	FDTexture2D(const String& filename);
	FDTexture2D(void* data, unsigned int width, unsigned int height, FD_TEXTURE2D_FORMAT format);
	~FDTexture2D();

	void Bind() override;
};
