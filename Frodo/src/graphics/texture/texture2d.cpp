#include "texture2d.h"
#include <core/log.h>
#include <util/vfs/vfs.h>

namespace FD {

Texture2D::Texture2D(const String& filename)  {

	//TODO: only supports 32bit
	uint32 bits = 0;
	byte* data = Texture::Load(filename, &width, &height, &bits, false);

	if (bits != 32) FD_WARNING("[Texture2D] Only supports 32 bit images atm!");

	D3D11_TEXTURE2D_DESC d;
	d.ArraySize = 1;
	d.Width = width;
	d.Height = height;
	d.MipLevels = 1;
	d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d.CPUAccessFlags = 0;
	d.MiscFlags = 0;
	d.Usage = D3D11_USAGE_DEFAULT;
	d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d.SampleDesc.Count = 1;
	d.SampleDesc.Quality = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC s;
	ZeroMemory(&s, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	s.Format = d.Format;
	s.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	s.Texture2D.MipLevels = 1;
	s.Texture2D.MostDetailedMip = 0;

	D3D11_SUBRESOURCE_DATA r;
	r.pSysMem = data;
	r.SysMemPitch = width * (bits / 8);
	r.SysMemSlicePitch = 0;

	D3DContext::GetDevice()->CreateTexture2D(&d, &r, (ID3D11Texture2D**)&resource);

	FD_ASSERT(resource == nullptr);

	D3DContext::GetDevice()->CreateShaderResourceView(resource, &s, &resourceView);

	FD_ASSERT(resourceView == nullptr);
}

Texture2D::Texture2D(void* data, uint32 width, uint32 height, FD_TEXTURE_FORMAT format) {
	this->width = width;
	this->height = height;
	D3D11_TEXTURE2D_DESC d;
	ZeroMemory(&d, sizeof(D3D11_TEXTURE2D_DESC));

	d.ArraySize = 1;
	d.MipLevels = 1;
	d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d.Width = width;
	d.Height = height;
	d.SampleDesc.Count = 1;
	d.Usage = D3D11_USAGE_DEFAULT;

	uint32 size = 0;

	switch (format) {
		case FD_TEXTURE_FORMAT_UNKNOWN:
			FD_ASSERT(format == FD_TEXTURE_FORMAT_UNKNOWN);
			break;
		case FD_TEXTURE_FORMAT_UINT_8_8_8_8:
			d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			size = 4;
			break;
		case FD_TEXTURE_FORMAT_FLOAT_32_32_32_32:
			d.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			size = 16;
			break;
		case FD_TEXTURE_FORMAT_UINT_8:
			d.Format = DXGI_FORMAT_R8_UNORM;
			size = 1;
	}



	D3D11_SUBRESOURCE_DATA s;
	ZeroMemory(&s, sizeof(D3D11_SUBRESOURCE_DATA));

	s.pSysMem = data;
	s.SysMemPitch = width * size;

	D3DContext::GetDevice()->CreateTexture2D(&d, &s, (ID3D11Texture2D**)&resource);

	FD_ASSERT(resource == nullptr);

	D3DContext::GetDevice()->CreateShaderResourceView(resource, nullptr, &resourceView);

	FD_ASSERT(resourceView == nullptr);
}

Texture2D::~Texture2D() {
	DX_FREE(resource);
}

void Texture2D::Bind(uint32 slot) const {
	D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &resourceView);
}

}