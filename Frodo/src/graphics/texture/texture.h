#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>
#include <common.h>

class FDAPI Texture {
protected:
	unsigned int width;
	unsigned int height;

	ID3D11ShaderResourceView* resourceView;

public:

	virtual ~Texture() { DX_FREE(resourceView); }

	virtual void Bind() = 0;

	inline unsigned int GetWidth() const { return width; }
	inline unsigned int GetHeight() const { return height; }

	inline ID3D11ShaderResourceView* GetResourceView() const { return resourceView; }
};
