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

	inline ID3D11Texture2D* GetResource() const { return  resource; }
	inline ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView; }
	inline ID3D11DepthStencilView* GetDepthView() const { return depthStencilView; }
};


template<unsigned int C>
class FramebufferMRT {
private:
	unsigned int numRenderTargets;

	Framebuffer2D* renderTargets[C];

public:
	FramebufferMRT(unsigned int width, unsigned int height, FD_TEXTURE_FORMAT format, bool createDepthStencil = true) {
		numRenderTargets = C;

		renderTargets[0] = new Framebuffer2D(width, height, format, createDepthStencil);

		for (unsigned int i = 1; i < numRenderTargets; i++) {
			renderTargets[i] = new Framebuffer2D(width, height, format, false);
		}
	}


	~FramebufferMRT() {
		for (unsigned int i = 0; i < numRenderTargets; i++) {
			delete renderTargets[i];
		}
	}

	inline void BindAsRenderTarget() const {
		ID3D11RenderTargetView* targets[C];

		for (unsigned int i = 0; i < numRenderTargets; i++) {
			targets[i] = renderTargets[i]->GetRenderTargetView();
		}

		ID3D11DepthStencilView* depthView = renderTargets[0]->GetDepthView();

		D3DContext::GetContext()->SetRenderTargets(numRenderTargets, targets, depthView == nullptr ? D3DContext::GetDefaultDepthStencilView() : depthView);
	}

	inline Framebuffer2D** GetFramebuffers() const { return  renderTargets; }

	inline Framebuffer2D* operator[](unsigned int index) const { return renderTargets[index]; }
};