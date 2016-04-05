#include "texture2d.h"
#include <D3D/Include/WICTextureLoader.h>

using namespace DirectX;

FDTexture2D::FDTexture2D(const String& filename) {
	
	CreateWICTextureFromFile(D3DContext::GetDevice(), D3DContext::GetDeviceContext(), filename.GetWCHAR(), (ID3D11Resource**)&resource, &resourceView);
	
	D3D11_TEXTURE2D_DESC d;

	resource->GetDesc(&d);

	this->width = d.Width;
	this->height = d.Height;
}

FDTexture2D::~FDTexture2D() {
	resource->Release();
	resourceView->Release();
}

void FDTexture2D::Bind() {
	D3DContext::GetDeviceContext()->PSSetShaderResources(0, 1, &resourceView);
}