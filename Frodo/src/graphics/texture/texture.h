#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>

enum FD_TEXTURE_FORMAT {
	FD_TEXTURE_FORMAT_UNKNOWN,
	FD_TEXTURE_FORMAT_UINT_8_8_8_8,
	FD_TEXTURE_FORMAT_FLOAT_32_32_32_32,
	FD_TEXTURE_FORMAT_FLOAT_D32,
	FD_TEXTURE_FORMAT_UINT_8
};


class FDAPI Texture {
protected:
	unsigned int width;
	unsigned int height;
	unsigned int bits;

	ID3D11ShaderResourceView* resourceView = nullptr;

public:
	Texture() { resourceView = nullptr; }
	virtual ~Texture() { DX_FREE(resourceView); }

	virtual void Bind(unsigned int slot = 0) = 0;

	inline unsigned int GetWidth() const { return width; }
	inline unsigned int GetHeight() const { return height; }

	inline ID3D11ShaderResourceView* GetResourceView() const { return resourceView; }
};

extern unsigned char* FDLoadImage(const String& filename, unsigned int* width, unsigned int* height, unsigned int* bits);