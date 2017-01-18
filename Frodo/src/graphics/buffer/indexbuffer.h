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
	uint32 count;

public:
	IndexBuffer(void* data, uint_t size, FD_INDEXBUFFER_FORMAT format);
	IndexBuffer(uint32* data, uint32 count);
	IndexBuffer(int32* data, uint32 count);
	IndexBuffer(uint16* data, uint32 count);
	IndexBuffer(int16* data, uint32 count);

	void Bind() override;

	__forceinline uint32 GetCount() const { return count; }
};
