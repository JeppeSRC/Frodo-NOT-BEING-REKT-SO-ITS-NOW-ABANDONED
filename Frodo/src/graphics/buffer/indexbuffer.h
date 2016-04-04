#pragma once
#include "buffer.h"
#include <fd.h>
#include <graphics/d3dcontext.h>

enum FD_INDEXBUFFER_FORMAT {
	UNKNOWN,
	FD_INDEXBUFFER_FORMAT_UINT32,
	FD_INDEXBUFFER_FORMAT_INT32,
	FD_INDEXBUFFER_FORMAT_UINT16,
	FD_INDEXBUFFER_FORMAT_INT16,
};

class FDAPI FDIndexBuffer : public FDBuffer {
private:
	DXGI_FORMAT format;

public:
	FDIndexBuffer(void* data, size_t size, FD_INDEXBUFFER_FORMAT format);
	FDIndexBuffer(unsigned int* data, size_t count);
	FDIndexBuffer(int* data, size_t count);
	FDIndexBuffer(unsigned short* data, size_t count);
	FDIndexBuffer(short* data, size_t count);

	void Bind() override;
};
