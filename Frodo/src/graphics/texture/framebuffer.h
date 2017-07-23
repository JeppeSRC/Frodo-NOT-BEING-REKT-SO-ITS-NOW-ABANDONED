#pragma once

#include "texture.h"

namespace FD {

class FDAPI Framebuffer : public Texture {
protected:
	ID3D11RenderTargetView* targetView;
	ID3D11DepthStencilView* depthView;

	Framebuffer() { targetView = nullptr; depthView = nullptr; }
public:
	~Framebuffer() { DX_FREE(targetView); DX_FREE(depthView); }


	void Bind(uint32 slot = 0);
	virtual void BindAsRenderTarget() = 0;

	inline ID3D11DepthStencilView* GetDepthView() const { return depthView; }
	inline ID3D11RenderTargetView* GetRenderTargetView() const { return targetView; }
};

}
