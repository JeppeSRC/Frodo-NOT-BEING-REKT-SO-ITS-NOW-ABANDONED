#include "shadowmap2d.h"
#include <core/log.h>

namespace FD {

ShadowMap2D::ShadowMap2D(uint32 width, uint32 height) {
	this->width = width;
	this->height = height;
	this->bits = 32;

	D3D11_TEXTURE2D_DESC t2d = { 0 };

	t2d.ArraySize = 1;
	t2d.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	t2d.Format = DXGI_FORMAT_R24G8_TYPELESS;
	t2d.Width = width;
	t2d.Height = height;
	t2d.MipLevels = 1;
	t2d.SampleDesc.Count = 1;
	t2d.Usage = D3D11_USAGE_DEFAULT;

	D3DContext::GetDevice()->CreateTexture2D(&t2d, 0, &resource);

	FD_ASSERT(resource == nullptr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv;
	ZeroMemory(&srv, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srv.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srv.Texture2D.MipLevels = 1;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	D3DContext::GetDevice()->CreateShaderResourceView(resource, &srv, &resourceView);

	FD_ASSERT(resourceView == nullptr);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv;
	ZeroMemory(&dsv, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	D3DContext::GetDevice()->CreateDepthStencilView(resource, &dsv, &depthView);

	FD_ASSERT(depthView == nullptr);
}

ShadowMap2D::~ShadowMap2D() {
	DX_FREE(resource);
	DX_FREE(depthView);
}

void ShadowMap2D::Bind(uint32 slot) {
	D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &resourceView);
}

void ShadowMap2D::BindAsRenderTarget() {
	D3DContext::GetDeviceContext()->OMSetRenderTargets(0, nullptr, depthView);
}

}