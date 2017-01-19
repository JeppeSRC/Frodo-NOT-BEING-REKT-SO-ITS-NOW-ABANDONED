#pragma once
#include "buffer.h"

enum FD_MAP_FLAG {
	FD_MAP_WRITE = D3D11_MAP_WRITE,
	FD_MAP_WRITE_NO_OVERWRITE = D3D11_MAP_WRITE_NO_OVERWRITE,
	FD_MAP_WRITE_DISCARD = D3D11_MAP_WRITE_DISCARD,
	FD_MAP_READ = D3D11_MAP_READ,
	FD_MAP_READ_WRITE = D3D11_MAP_READ_WRITE
};


class FDAPI VertexBuffer : public Buffer {
private:
	uint32 stride;
	uint32 size;
	
public:
	VertexBuffer(uint32 structSize, uint32 num);
	VertexBuffer(void* data, uint_t size, uint32 stride, bool dynamic = false);

	void* Map(FD_MAP_FLAG flag);
	void Unmap();

	void Bind() override;
	void Bind(uint32 slot);
};
