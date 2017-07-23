#pragma once

#include "texture.h"

namespace FD {

class FDAPI ShadowMap2D : public Texture {
protected:
	ID3D11Texture2D* resource;
	ID3D11DepthStencilView* depthView;

public:
	ShadowMap2D(uint32 width, uint32 height);
	~ShadowMap2D();

	void Bind(uint32 slot = 0) override;
	void BindAsRenderTarget();

	inline ID3D11Resource* GetResource() const { return resource; }
	inline ID3D11DepthStencilView* GetDepthView() const { return depthView; }
};

}
