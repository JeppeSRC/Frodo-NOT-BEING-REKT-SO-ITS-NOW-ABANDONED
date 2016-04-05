#pragma once
#include "texture.h"
#include <util/string.h>

class FDAPI FDTexture2D : public FDTexture {
private:
	ID3D11ShaderResourceView* resourceView;
	ID3D11Texture2D* resource;

public:
	FDTexture2D(const String& filename);
	FDTexture2D(void* data, size_t size);
	~FDTexture2D();

	void Bind() override;
};
