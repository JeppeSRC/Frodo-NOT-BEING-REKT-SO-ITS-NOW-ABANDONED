#pragma once
#include <graphics/d3dcontext.h>
#include <graphics/texture/texture.h>
#include <fd.h>

class FDAPI Framebuffer2D : Texture {
private:
	ID3D11Texture2D* resource;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;

public:

	Framebuffer2D(unsigned int width, unsigned int height, FD_TEXTURE_FORMAT format, bool createDepthStencil = true);
	~Framebuffer2D();

	void Bind(unsigned int slot = 0) override;
	void BindAsRenderTarget();
};
