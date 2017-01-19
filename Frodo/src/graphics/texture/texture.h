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

	uint32 width;
	uint32 height;
	uint32 bits;

	ID3D11ShaderResourceView* resourceView = nullptr;

public:
	Texture() { resourceView = nullptr; }
	virtual ~Texture() { DX_FREE(resourceView); }

	virtual void Bind(uint32 slot = 0) = 0;

	inline uint32 GetWidth() const { return width; }
	inline uint32 GetHeight() const { return height; }

	inline ID3D11ShaderResourceView* GetResourceView() const { return resourceView; }

	static byte* Load(const String& filename, uint32* width, uint32* height, uint32* bits);
};
