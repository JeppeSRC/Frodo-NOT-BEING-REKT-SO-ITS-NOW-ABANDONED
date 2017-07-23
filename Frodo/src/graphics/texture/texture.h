#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>
#include <util/string.h>

namespace FD {

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

	ID3D11Resource* resource;
	ID3D11ShaderResourceView* resourceView = nullptr;

public:
	Texture() { resourceView = nullptr; }
	virtual ~Texture() { DX_FREE(resourceView); DX_FREE(resource); }

	virtual void Bind(uint32 slot = 0) = 0;

	inline uint32 GetWidth() const { return width; }
	inline uint32 GetHeight() const { return height; }

	inline ID3D11ShaderResourceView* GetResourceView() const { return resourceView; }
	inline ID3D11Resource* GetResource() const { return resource; }

	static byte* Load(const String& filename, uint32* width, uint32* height, uint32* bits, bool flipY = false);
	static byte* Load(void* memory, uint32* width, uint32* height, uint32* bits, bool flipY = false);
};

}

