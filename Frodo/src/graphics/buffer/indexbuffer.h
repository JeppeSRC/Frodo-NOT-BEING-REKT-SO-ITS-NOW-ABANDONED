#pragma once
#include "buffer.h"
#include <fd.h>
#include <graphics/d3dcontext.h>

enum FD_INDEXBUFFER_FORMAT {
	FD_INDEXBUFFER_FORMAT_UNKNOWN,
	FD_INDEXBUFFER_FORMAT_UINT32,
	FD_INDEXBUFFER_FORMAT_INT32,
	FD_INDEXBUFFER_FORMAT_UINT16,
	FD_INDEXBUFFER_FORMAT_INT16,
};

class FDAPI IndexBuffer : public Buffer {
private:
	DXGI_FORMAT format;
	unsigned int count;

public:
	IndexBuffer(void* data, size_t size, FD_INDEXBUFFER_FORMAT format);
	IndexBuffer(unsigned int* data, unsigned int count);
	IndexBuffer(int* data, unsigned int count);
	IndexBuffer(unsigned short* data, unsigned int count);
	IndexBuffer(short* data, unsigned int count);

	void Bind() override;

	__forceinline unsigned int GetCount() const { return count; }
};
