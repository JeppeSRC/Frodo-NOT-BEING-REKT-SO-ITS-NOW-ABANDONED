#pragma once
#include "texture.h"
#include <util/string.h>

enum FD_TEXTURE2D_FORMAT {
	UNKNOWN,
	FD_TEXTURE2D_FORMAT_UINT_8_8_8_8,
	FD_TEXTURE2D_FORMAT_FLOAT_32_32_32_32
};

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
