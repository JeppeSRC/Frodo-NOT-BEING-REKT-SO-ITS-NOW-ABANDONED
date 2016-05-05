#include <graphics/texture/framebuffer2d.h>
#include <core/log.h>

Framebuffer2D::Framebuffer2D(unsigned int width, unsigned int height, FD_TEXTURE_FORMAT format) {
	this->width = width;
	this->height = height;

	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));

	td.ArraySize = 1;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	
	switch (format) {
		case FD_TEXTURE_FORMAT_UNKNOWN:
			FD_ASSERT(FD_TEXTURE_FORMAT_UNKNOWN);
		case FD_TEXTURE_FORMAT_FLOAT_D32:
			td.Format = DXGI_FORMAT_D32_FLOAT;
			break;
		case FD_TEXTURE_FORMAT_FLOAT_32_32_32_32:
			td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
	}

	D3DContext::GetDevice()->CreateTexture2D(&td, 0, &resource);

	FD_ASSERT(resource);

	D3D11_SHADER_RESOURCE_VIEW_DESC vd;
	ZeroMemory(&vd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	vd.Format = td.Format;
	vd.Texture2D.MipLevels = 1;
	vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	D3DContext::GetDevice()->CreateShaderResourceView(resource, nullptr, &resourceView);

	FD_ASSERT(resourceView);

}

Framebuffer2D::~Framebuffer2D() {
	DX_FREE(resource);
}

void Framebuffer2D::Bind(unsigned int slot) {
	D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &resourceView);
}

void Framebuffer2D::BindAsRenderTarget() {
	D3DContext::SetRenderTarget(renderTargetView);
	D3DContext::SetViewPort(0, 0, width, height);
}