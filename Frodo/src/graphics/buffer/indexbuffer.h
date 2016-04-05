#pragma once
#include "buffer.h"
#include <fd.h>
#include <graphics/d3dcontext.h>


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
