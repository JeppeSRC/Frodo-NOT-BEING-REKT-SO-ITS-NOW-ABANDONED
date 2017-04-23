#include "vertexbuffer.h"
#include <core/log.h>

namespace FD {

VertexBuffer::VertexBuffer(uint32 structSize, uint32 num) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = size = structSize * num;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.StructureByteStride = stride = structSize;
	bd.Usage = D3D11_USAGE_DYNAMIC;

	D3DContext::GetDevice()->CreateBuffer(&bd, 0, &buffer);
	FD_ASSERT(buffer == nullptr);
}

VertexBuffer::VertexBuffer(void* data, uint_t size, uint32 stride, bool dynamic) : Buffer(), stride(stride) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = (uint32)size;
	bd.CPUAccessFlags = dynamic ? D3D10_CPU_ACCESS_WRITE : 0;
	bd.StructureByteStride = stride;
	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;

	this->size = (uint32)size;
	this->stride = stride;

	D3D11_SUBRESOURCE_DATA srd;

	srd.pSysMem = data;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	D3DContext::GetDevice()->CreateBuffer(&bd, &srd, &buffer);
	FD_ASSERT(buffer == nullptr);
}

void* VertexBuffer::Map(FD_MAP_FLAG flag) {
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DContext::GetDeviceContext()->Map((ID3D11Resource*)buffer, 0, (D3D11_MAP)flag, 0, &map);

	FD_ASSERT(map.pData == nullptr);

	return map.pData;
}

void VertexBuffer::Unmap() {
	D3DContext::GetDeviceContext()->Unmap((ID3D11Resource*)buffer, 0);
}

void VertexBuffer::Bind() {
	uint32 offset = 0;
	D3DContext::GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

void VertexBuffer::Bind(uint32 slot) {
	uint32 offset = 0;
	D3DContext::GetDeviceContext()->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
}

}