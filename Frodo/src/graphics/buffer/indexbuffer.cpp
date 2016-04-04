#include "indexbuffer.h"
#include <core/log.h>

inline static DXGI_FORMAT get_buffer_format(FD_INDEXBUFFER_FORMAT format) {
	switch (format) {
		case UNKNOWN:
			FD_ASSERT(UNKNOWN, "Unknown indexbuffer format");
			break;
		case FD_INDEXBUFFER_FORMAT_UINT32:
			return DXGI_FORMAT_R32_UINT;
		case FD_INDEXBUFFER_FORMAT_INT32:
			return DXGI_FORMAT_R32_SINT;
		case FD_INDEXBUFFER_FORMAT_UINT16:
			return DXGI_FORMAT_R16_UINT;
		case FD_INDEXBUFFER_FORMAT_INT16:
			return DXGI_FORMAT_R16_SINT;
	}

	return DXGI_FORMAT_UNKNOWN;
}

FDIndexBuffer::FDIndexBuffer(void* data, size_t size, FD_INDEXBUFFER_FORMAT format) : FDBuffer() {

	this->format = get_buffer_format(format);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = size;
	bd.CPUAccessFlags = 0;
	bd.StructureByteStride = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;

	this->size = size;

	D3D11_SUBRESOURCE_DATA srd;

	srd.pSysMem = data;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	D3DContext::GetDevice()->CreateBuffer(&bd, &srd, &buffer);
	FD_ASSERT(buffer, "indexbuffer creation failed");
}

FDIndexBuffer::FDIndexBuffer(unsigned int* data, size_t count) : FDIndexBuffer(data, count * sizeof(unsigned int), FD_INDEXBUFFER_FORMAT_UINT32) {}

FDIndexBuffer::FDIndexBuffer(int* data, size_t count) : FDIndexBuffer(data, count * sizeof(int), FD_INDEXBUFFER_FORMAT_INT32) {}

FDIndexBuffer::FDIndexBuffer(unsigned short* data, size_t count) : FDIndexBuffer(data, count * sizeof(unsigned short), FD_INDEXBUFFER_FORMAT_UINT16) {}

FDIndexBuffer::FDIndexBuffer(short* data, size_t count) : FDIndexBuffer(data, count * sizeof(short), FD_INDEXBUFFER_FORMAT_INT16) {}


void FDIndexBuffer::Bind() {
	D3DContext::GetDeviceContext()->IASetIndexBuffer(buffer, format, 0);
}