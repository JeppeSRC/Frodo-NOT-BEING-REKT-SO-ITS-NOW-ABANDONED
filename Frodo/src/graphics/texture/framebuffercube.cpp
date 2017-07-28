#include "framebuffercube.h"

#include <core/log.h>

namespace FD {

FramebufferCube::FramebufferCube(uint32 w, uint32 h, FD_TEXTURE_FORMAT format, bool createDepth) {
	this->width = w;
	this->height = h;

	D3D11_TEXTURE2D_DESC t2d = { 0 };

	t2d.ArraySize = 6;
	t2d.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	t2d.Width = w;
	t2d.Height = h;
	t2d.MipLevels = 1;
	t2d.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	t2d.SampleDesc.Count = 1;
	t2d.Usage = D3D11_USAGE_DEFAULT;

	switch (format) {
		case FD_TEXTURE_FORMAT_FLOAT_32_32_32_32:
			t2d.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case FD_TEXTURE_FORMAT_FLOAT_D32:
			t2d.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case FD_TEXTURE_FORMAT_UINT_8_8_8_8:
			t2d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case FD_TEXTURE_FORMAT_UINT_8:
			t2d.Format = DXGI_FORMAT_R8_UNORM;
			break;
	}

	D3DContext::GetDevice()->CreateTexture2D(&t2d, 0, (ID3D11Texture2D**)&resource);

	FD_ASSERT(resource == nullptr);

	D3D11_RENDER_TARGET_VIEW_DESC rtv;
	ZeroMemory(&rtv, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	rtv.Format = t2d.Format;
	rtv.Texture2DArray.ArraySize = 6;
	rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;

	D3DContext::GetDevice()->CreateRenderTargetView(resource, &rtv, &targetView);

	FD_ASSERT(targetView == nullptr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv;
	ZeroMemory(&srv, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srv.Format = t2d.Format;
	srv.TextureCube.MipLevels = 1;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

	D3DContext::GetDevice()->CreateShaderResourceView(resource, &srv, &resourceView);

	FD_ASSERT(resourceView == nullptr);

	depthView = nullptr;

	if (!createDepth) return;

	ID3D11Texture2D* tmp = nullptr;

	ZeroMemory(&t2d, sizeof(D3D11_TEXTURE2D_DESC));

	t2d.ArraySize = 6;
	t2d.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	t2d.Width = width;
	t2d.Height = height;
	t2d.SampleDesc.Count = 1;
	t2d.MipLevels = 1;
	t2d.Usage = D3D11_USAGE_DEFAULT;
	t2d.Format = DXGI_FORMAT_D32_FLOAT;

	D3DContext::GetDevice()->CreateTexture2D(&t2d, 0, &tmp);

	D3D11_DEPTH_STENCIL_VIEW_DESC dd;
	ZeroMemory(&dd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dd.Format = t2d.Format;
	dd.Texture2DArray.ArraySize = 6;
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;

	D3DContext::GetDevice()->CreateDepthStencilView(tmp, &dd, &depthView);

	DX_FREE(tmp);

	FD_ASSERT(depthView == nullptr);
}

void FramebufferCube::BindAsRenderTarget() {
	D3DContext::SetRenderTargets(1, &targetView, depthView);
	D3DContext::SetViewPort(0, 0, (float32)width, (float32)height);
}

}