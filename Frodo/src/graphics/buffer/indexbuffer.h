#pragma once
#include "buffer.h"
#include <fd.h>
#include <graphics/d3dcontext.h>


class FDAPI FDIndexBuffer : public FDBuffer {
private:
	DXGI_FORMAT format;
	unsigned int count;

public:
	FDIndexBuffer(void* data, size_t size, FD_INDEXBUFFER_FORMAT format);
	FDIndexBuffer(unsigned int* data, unsigned int count);
	FDIndexBuffer(int* data, unsigned int count);
	FDIndexBuffer(unsigned short* data, unsigned int count);
	FDIndexBuffer(short* data, unsigned int count);

	void Bind() override;

	__forceinline unsigned int GetCount() const { return count; }
};
