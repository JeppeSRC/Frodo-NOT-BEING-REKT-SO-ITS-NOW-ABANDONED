#pragma once
#include <fd.h>
#include <graphics/d3dcontext.h>
#include <common.h>

static unsigned int numBuffers = 0;

class FDAPI FDBuffer {
protected:
	ID3D11Buffer* buffer;
	size_t size;

	FDBuffer() { buffer = nullptr; }

public:
	virtual ~FDBuffer() { DX_FREE(buffer) }

	virtual void Bind() = 0;

	inline size_t GetSize() const { return size; }
	inline ID3D11Buffer* GetBuffer() const { return buffer; }
};
