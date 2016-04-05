#include "vertexbuffer.h"
#include <core/log.h>

FDVertexBuffer::FDVertexBuffer(void* data, size_t size, unsigned int stride) : FDBuffer(), stride(stride) {

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = size;
	bd.CPUAccessFlags = 0;
	bd.StructureByteStride = stride;
	bd.Usage = D3D11_USAGE_DEFAULT;

	this->size = size;
	this->stride = stride;

	D3D11_SUBRESOURCE_DATA srd;

	srd.pSysMem = data;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	D3DContext::GetDevice()->CreateBuffer(&bd, &srd, &buffer);
	FD_ASSERT(buffer);
}

void FDVertexBuffer::Bind() {
	unsigned int offset = 0;
	D3DContext::GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}