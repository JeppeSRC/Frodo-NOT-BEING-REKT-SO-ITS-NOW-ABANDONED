#include "texture2d.h"
#include <D3D/Include/WICTextureLoader.h>
#include <core/log.h>

using namespace DirectX;

FDTexture2D::FDTexture2D(const String& filename) : FDTexture2D() {
	
	CreateWICTextureFromFile(D3DContext::GetDevice(), D3DContext::GetDeviceContext(), filename.GetWCHAR(), (ID3D11Resource**)&resource, &resourceView);
	
	FD_ASSERT(resource && resourceView);

	D3D11_TEXTURE2D_DESC d;

	resource->GetDesc(&d);

	this->width = d.Width;
	this->height = d.Height;
}

FDTexture2D::FDTexture2D(void* data, unsigned int width, unsigned int height, FD_TEXTURE2D_FORMAT format) : FDTexture2D() {
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
	
	unsigned int size = 0;

	switch (format) {
		case FD_TEXTURE2D_FORMAT_UNKNOWN:
			FD_ASSERT(FD_TEXTURE2D_FORMAT_UNKNOWN);
			break;
		case FD_TEXTURE2D_FORMAT_UINT_8_8_8_8:
			d.Format = DXGI_FORMAT_R8G8B8A8_UINT;
			size = 4;
			break;
		case FD_TEXTURE2D_FORMAT_FLOAT_32_32_32_32:
			d.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			size = 32;
			break;
	}

	D3D11_SUBRESOURCE_DATA s;
	ZeroMemory(&s, sizeof(D3D11_SUBRESOURCE_DATA));

	s.pSysMem = data;
	s.SysMemPitch = width * size;

	D3DContext::GetDevice()->CreateTexture2D(&d, &s, &resource);

	FD_ASSERT(resource);
	
	D3DContext::GetDevice()->CreateShaderResourceView(resource, nullptr, &resourceView);

	FD_ASSERT(resourceView);
}

FDTexture2D::~FDTexture2D() {
	DX_FREE(resource)
	DX_FREE(resourceView)
}

void FDTexture2D::Bind() {
	D3DContext::GetDeviceContext()->PSSetShaderResources(0, 1, &resourceView);
}