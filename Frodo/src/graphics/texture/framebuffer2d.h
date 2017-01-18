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

	Framebuffer2D(uint32 width, uint32 height, FD_TEXTURE_FORMAT format, bool createDepthStencil = true);
	~Framebuffer2D();

	void Bind(uint32 slot = 0) override;
	void BindAsRenderTarget();

	inline ID3D11Texture2D* GetResource() const { return  resource; }
	inline ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView; }
	inline ID3D11DepthStencilView* GetDepthView() const { return depthStencilView; }
};


template<uint32 C>
class FramebufferMRT {
private:
	uint32 numRenderTargets;
	bool initialized = false;

	Framebuffer2D* renderTargets[C];

public:
	FramebufferMRT() { }
	FramebufferMRT(uint32 width, uint32 height, FD_TEXTURE_FORMAT format, bool createDepthStencil = true) {
		Init(width, height, format, createDepthStencil);
	}


	~FramebufferMRT() {
		for (uint32 i = 0; i < numRenderTargets; i++) {
			delete renderTargets[i];
		}
	}

	inline void Init(uint32 width, uint32 height, FD_TEXTURE_FORMAT format, bool createDepthStencil = true) {
		if (initialized) {
			for (uint32 i = 0; i < numRenderTargets; i++)
				delete renderTargets[i];
		}
		numRenderTargets = C;

		renderTargets[0] = new Framebuffer2D(width, height, format, createDepthStencil);

		for (uint32 i = 1; i < numRenderTargets; i++) {
			renderTargets[i] = new Framebuffer2D(width, height, format, false);
		}

		initialized = true;
	}

	inline void BindAsRenderTarget() const {
		ID3D11RenderTargetView* targets[C];

		for (uint32 i = 0; i < numRenderTargets; i++) {
			targets[i] = renderTargets[i]->GetRenderTargetView();
		}

		ID3D11DepthStencilView* depthView = renderTargets[0]->GetDepthView();

		D3DContext::GetContext()->SetRenderTargets(numRenderTargets, targets, depthView == nullptr ? D3DContext::GetDefaultDepthStencilView() : depthView);
	}

	inline Framebuffer2D** GetFramebuffers() const { return  renderTargets; }

	inline Framebuffer2D* operator[](uint32 index) const { return renderTargets[index]; }
};