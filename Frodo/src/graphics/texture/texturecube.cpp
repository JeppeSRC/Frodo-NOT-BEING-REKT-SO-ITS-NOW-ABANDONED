#include "texturecube.h"
#include <util/string.h>
#include <core/log.h>

void TextureCube::LoadSingleFile(const String& filename) {

	unsigned char* data = FDLoadImage(filename, &width, &height, &bits);

	if (data == nullptr) {
		FD_FATAL("[TextureCube] Could not load file \"%s\"", *filename);
		return;
	}

	size_t faceWidth = width / 3;
	size_t faceHeight = height >> 2;
	size_t stride = bits >> 3;

	unsigned char** newData = new unsigned char*[6];
	for (size_t i = 0; i < 6; i++)
		newData[i] = new unsigned char[faceWidth * faceHeight * stride];

	size_t index = 0;

	for (size_t y = 0; y < 4; y++) {
		for (size_t x = 0; x < 3; x++) {
			if (y == 0 || y == 2 || y == 3) {
				if (x != 1) continue;
			}

			size_t yOffset = faceHeight * y;
			size_t xOffset = faceWidth * x;
			size_t index = 0;
			for (size_t yy = 0; yy < faceHeight; yy++) {
				size_t ya = yOffset + yy ;
				for (size_t xx = 0; xx < faceWidth ; xx++) {
					size_t xa = xOffset + xx;

					newData[index][(xx + yy * faceWidth) * stride + 0] = data[(xa + ya * width) * stride + 0];
					newData[index][(xx + yy * faceWidth) * stride + 1] = data[(xa + ya * width) * stride + 1];
					newData[index][(xx + yy * faceWidth) * stride + 2] = data[(xa + ya * width) * stride + 2];

					if (stride == 4)
						newData[index][(xx + yy * faceWidth) * stride + 3] = data[(xa + ya * width) * stride + 3];
				}
			}

			index++;
		}
	}


	D3D11_TEXTURE2D_DESC d;

	d.Width = faceWidth;
	d.Height = faceHeight;
	d.ArraySize = 6;
	d.SampleDesc.Count = 1;
	d.SampleDesc.Quality = 0;
	d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d.CPUAccessFlags = 0;
	d.Format = bits == 24 ? DXGI_FORMAT_R8G8B8A8_UNORM : bits == 32 ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_UNKNOWN;
	d.MipLevels = 1;
	d.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	d.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SHADER_RESOURCE_VIEW_DESC s;
	ZeroMemory(&s, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	s.Format = d.Format;
	s.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	s.TextureCube.MipLevels = 1;
	s.TextureCube.MostDetailedMip = 0;
	
	D3D11_SUBRESOURCE_DATA r[6];
	ZeroMemory(&r, sizeof(D3D11_SUBRESOURCE_DATA));

	for (size_t i = 0; i < 6; i++) {
		r[i].pSysMem = newData[i];
		r[i].SysMemPitch = faceWidth * stride;
	}

	D3DContext::GetDevice()->CreateTexture2D(&d, r, &resource);

	FD_ASSERT((resource != nullptr && "Resource null"));

	D3DContext::GetDevice()->CreateShaderResourceView((ID3D11Resource*)resource, &s, &resourceView);

	FD_ASSERT((resourceView != nullptr && "Resource view null"));

	for (size_t i = 0; i < 6; i++)
		delete[] newData[i];

	delete[] newData;
	delete[] data;
	
}

void TextureCube::LoadMultipleFiles(const String* filePaths) {
	
	struct FD_IMAGE_FILE {
		unsigned int width;
		unsigned int height;
		unsigned int bits;

		unsigned char* data;
	} files[6];

	unsigned int w = 0;
	unsigned int h = 0;
	unsigned int b = 0;

	for (size_t i = 0; i < 6; i++) {
		FD_IMAGE_FILE& file = files[i];

		file.data = FDLoadImage(*filePaths[i], &file.width, &file.height, &file.bits);

		if (i == 0) {
			w = file.width;
			h = file.height;
			b = file.bits;
			continue;
		}

		if (w != file.width || h != file.height || b != file.bits) {
			FD_FATAL("[TextureCube] All files must be the same format and size!");
			return;
		}
	}

	D3D11_TEXTURE2D_DESC d;
	d.Width = files[0].width;
	d.Height = files[0].height;
	d.Format = files[0].bits == 24 ? DXGI_FORMAT_R8G8B8A8_UNORM : files[0].bits == 32 ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_UNKNOWN;
	d.ArraySize = 6;
	d.CPUAccessFlags = 0;
	d.MipLevels = 1;
	d.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	d.SampleDesc.Quality = 0;
	d.SampleDesc.Count = 1;
	d.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SHADER_RESOURCE_VIEW_DESC s;
	ZeroMemory(&s, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	s.Format = d.Format;
	s.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	s.TextureCube.MipLevels = 1;
	s.TextureCube.MostDetailedMip = 0;

	D3D11_SUBRESOURCE_DATA r[6];

	for (size_t i = 0; i < 6; i++) {
		r[i].pSysMem = files[i].data;
		r[i].SysMemPitch = files[0].width * (bits >> 3);
		r[i].SysMemSlicePitch = 0;
	}

	D3DContext::GetDevice()->CreateTexture2D(&d, r, &resource);

	FD_ASSERT((resource != nullptr && "Resource null"));

	D3DContext::GetDevice()->CreateShaderResourceView((ID3D11Resource*)resource, &s, &resourceView);

	FD_ASSERT((resourceView != nullptr && "Resource view null"));

	for (size_t i = 0; i < 6; i++) {
		delete[] files[i].data;
	}

}

TextureCube::TextureCube(const String* files) {
	if (files[1].IsNull()) {
		LoadSingleFile(**files);
	} else {
		LoadMultipleFiles(files);
	}
}

TextureCube::~TextureCube() {
	DX_FREE(resource);
}

void TextureCube::Bind(unsigned int slot) {
	D3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &resourceView);
}